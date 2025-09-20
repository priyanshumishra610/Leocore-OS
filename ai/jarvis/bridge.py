#!/usr/bin/env python3
import json
import sys
import os
import time
from typing import Dict, List, Optional

from .session import Session
from .commands import handle_system_command, is_system_command
from .plugin_manager import PluginManager
from .config import JarvisConfig

# Load configuration
config = JarvisConfig()
OLLAMA_URL = config.get("ollama.url", "http://localhost:11434/api/generate")
MODEL = config.get("ollama.model", "llama3.1")
TIMEOUT_S = config.get("ollama.timeout", 30)

try:
	import requests  # type: ignore
except Exception:  # pragma: no cover - optional dependency info message
	requests = None  # type: ignore

PROMPT_PREFIX = "JARVIS:"


def _ollama_generate(prompt: str, session: Session) -> str:
	if requests is None:
		return "[jarvis] requests not installed; cannot reach Ollama."
	payload = {
		"model": MODEL,
		"prompt": session.render_prompt(prompt),
		"stream": False,
	}
	try:
		resp = requests.post(OLLAMA_URL, json=payload, timeout=TIMEOUT_S)
		resp.raise_for_status()
		try:
			data = resp.json()
		except Exception as je:  # pragma: no cover - network edge
			return f"[jarvis] invalid JSON from Ollama: {je}"
		if not isinstance(data, dict):
			return "[jarvis] unexpected response type from Ollama"
		text = data.get("response")
		if not text:
			return "[jarvis] empty response"
		return str(text)
	except Exception as e:  # pragma: no cover - network
		return f"[jarvis] error contacting Ollama: {e}"


def process_line(line: str, session: Session, plugin_manager: PluginManager) -> str:
	line = line.strip()
	if not line:
		return ""
	if not line.startswith(PROMPT_PREFIX):
		# Not for us; ignore quietly
		return ""
	payload = line[len(PROMPT_PREFIX):].strip()
	# If line starts with 'jarvis' strip it
	if payload.startswith("jarvis"):
		payload = payload[len("jarvis"):].strip()
	if not payload:
		return "[jarvis] Ready. Type your query."

	# Check for plugin commands first
	cmd_parts = payload.split()
	if cmd_parts:
		plugin_cmd = plugin_manager.get_command(cmd_parts[0])
		if plugin_cmd:
			try:
				return plugin_cmd(payload)
			except Exception as e:
				return f"[jarvis] Plugin error: {e}"

	# System commands short-circuit
	if is_system_command(payload):
		return handle_system_command(payload)

	# AI query
	response = _ollama_generate(payload, session)
	session.add_exchange(payload, response)
	# Best-effort persistence
	try:
		session.save_if_configured()
	except Exception:
		pass
	return response


def main() -> int:
	# Initialize plugin manager
	plugins_dir = config.get("plugins.directory", "plugins")
	dev_mode = config.get("plugins.dev_mode", False)
	plugin_manager = PluginManager(plugins_dir, dev_mode)
	
	# Load all plugins
	loaded_count = plugin_manager.load_all_plugins()
	print(f"[jarvis] Loaded {loaded_count} plugins", file=sys.stderr)
	
	# Initialize session with config
	session_capacity = config.get("session.capacity", 10)
	session_path = config.get("session.persist_path", "")
	session = Session(capacity=session_capacity)
	if session_path:
		session.path = session_path
	
	# Main loop with hot-reload support
	last_reload_check = time.time()
	reload_interval = 1.0  # Check every second in dev mode
	
	# Read stdin line-by-line (serial stdio)
	for raw in sys.stdin:
		# Hot-reload check in dev mode
		if dev_mode and time.time() - last_reload_check > reload_interval:
			reloaded = plugin_manager.hot_reload_updated()
			if reloaded > 0:
				print(f"[jarvis] Reloaded {reloaded} plugins", file=sys.stderr)
			last_reload_check = time.time()
		
		out = process_line(raw, session, plugin_manager)
		if out:
			sys.stdout.write(out + "\n")
			sys.stdout.flush()
	return 0


if __name__ == "__main__":
	raise SystemExit(main())
