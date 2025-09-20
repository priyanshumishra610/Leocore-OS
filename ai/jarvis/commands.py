from __future__ import annotations
from typing import Callable, List
import os
import time

SYSTEM_COMMANDS = {
	"uptime": "uptime",
	"list tasks": "list tasks",
	"shutdown": "shutdown",
	"fs ls": "fs ls",
	"cpu": "cpu",
	"mem": "mem",
	"remind": "remind",
}


def is_system_command(text: str) -> bool:
	return text in SYSTEM_COMMANDS or text.startswith("remind ") or text.startswith("fs ls ")


def handle_system_command(text: str) -> str:
	if text == "uptime":
		return os.environ.get("JARVIS_UPTIME", "uptime: unknown")
	elif text == "list tasks":
		return "tasks: idle"
	elif text == "shutdown":
		return "shutdown: requested"
	elif text.startswith("fs ls ") or text == "fs ls":
		path = text[6:].strip() or "."
		try:
			entries: List[str] = sorted(os.listdir(path))
			return "\n".join(entries)
		except Exception as e:
			return f"fs ls: {e}"
	elif text == "cpu":
		return os.environ.get("JARVIS_CPU", "cpu: x86 (sim)")
	elif text == "mem":
		return os.environ.get("JARVIS_MEM", "mem: unknown")
	elif text.startswith("remind "):
		# very simple reminders (non-persistent)
		msg = text[len("remind "):].strip()
		if not msg:
			return "remind: empty"
		return f"reminder set: {msg}"
	return "unknown command"
