from __future__ import annotations
from typing import List, Tuple
import os
import json

class Session:
	"""Bounded conversation memory for Jarvis with optional persistence.

	Set JARVIS_SESSION_PATH to a file path to enable save/load.
	"""

	def __init__(self, capacity: int = 10) -> None:
		self.capacity = max(1, capacity)
		self.messages: List[Tuple[str, str]] = []
		self.path = os.environ.get("JARVIS_SESSION_PATH", "")
		if self.path:
			self._load_silent()

	def add_exchange(self, user_text: str, assistant_text: str) -> None:
		self.messages.append((user_text, assistant_text))
		if len(self.messages) > self.capacity:
			self.messages = self.messages[-self.capacity:]

	def render_prompt(self, new_user_text: str) -> str:
		"""Render a compact prompt from recent context.

		This is a simple concatenation; can be improved later.
		"""
		buf: List[str] = ["You are Jarvis, a concise helpful OS assistant."]
		for u, a in self.messages[-self.capacity:]:
			buf.append(f"User: {u}")
			buf.append(f"Jarvis: {a}")
		buf.append(f"User: {new_user_text}")
		buf.append("Jarvis:")
		return "\n".join(buf)

	def save_if_configured(self) -> None:
		if not self.path:
			return
		data = {"capacity": self.capacity, "messages": self.messages}
		with open(self.path, "w", encoding="utf-8") as f:
			json.dump(data, f)

	def _load_silent(self) -> None:
		try:
			if not os.path.isfile(self.path):
				return
			with open(self.path, "r", encoding="utf-8") as f:
				data = json.load(f)
			if isinstance(data, dict) and "messages" in data:
				msgs = data.get("messages", [])
				if isinstance(msgs, list):
					self.messages = [tuple(x) for x in msgs][-self.capacity:]
		except Exception:
			pass
