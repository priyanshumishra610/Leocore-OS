import os
import tempfile
from types import SimpleNamespace
from ai.jarvis.session import Session
from ai.jarvis.commands import is_system_command, handle_system_command
from ai.jarvis.bridge import process_line


def test_session_capacity_and_prompt():
	ss = Session(capacity=2)
	ss.add_exchange("hi", "hello")
	ss.add_exchange("how are you?", "great")
	ss.add_exchange("what's up?", "sky")
	p = ss.render_prompt("final")
	assert "how are you?" in p
	assert "what's up?" in p
	assert "hi" not in p


def test_session_persistence(tmp_path, monkeypatch):
	path = tmp_path / "sess.json"
	monkeypatch.setenv("JARVIS_SESSION_PATH", str(path))
	ss = Session(capacity=3)
	ss.add_exchange("a", "b")
	ss.save_if_configured()
	ss2 = Session(capacity=3)
	assert ss2.messages


def test_system_commands_detection_and_handler(monkeypatch):
	assert is_system_command("uptime")
	assert is_system_command("list tasks")
	assert is_system_command("shutdown")
	monkeypatch.setenv("JARVIS_UPTIME", "uptime: 1234ms")
	assert handle_system_command("uptime") == "uptime: 1234ms"
	assert handle_system_command("list tasks").startswith("tasks:")
	assert "requested" in handle_system_command("shutdown")


def test_fs_ls_command(tmp_path):
	f = tmp_path / "a.txt"
	f.write_text("x")
	out = handle_system_command(f"fs ls {tmp_path}")
	assert "a.txt" in out


def test_remind_and_stats(monkeypatch):
	assert handle_system_command("remind buy milk").startswith("reminder set:")
	monkeypatch.setenv("JARVIS_CPU", "cpu: test")
	monkeypatch.setenv("JARVIS_MEM", "mem: 1MB")
	assert handle_system_command("cpu") == "cpu: test"
	assert handle_system_command("mem") == "mem: 1MB"


def test_bridge_ignores_non_prefixed_lines():
	from ai.jarvis.plugin_manager import PluginManager
	ss = Session()
	pm = PluginManager()
	out = process_line("hello", ss, pm)
	assert out == ""


def test_bridge_processes_prefixed_jarvis_system(monkeypatch):
	from ai.jarvis.plugin_manager import PluginManager
	ss = Session()
	pm = PluginManager()
	monkeypatch.setenv("JARVIS_UPTIME", "uptime: 42ms")
	out = process_line("JARVIS: jarvis uptime", ss, pm)
	assert out == "uptime: 42ms"


def test_bridge_without_requests(monkeypatch):
	# simulate no requests installed
	import ai.jarvis.bridge as bridge
	from ai.jarvis.plugin_manager import PluginManager
	monkeypatch.setattr(bridge, "requests", None)
	ss = Session()
	pm = PluginManager()
	out = process_line("JARVIS: jarvis what is life?", ss, pm)
	assert "requests not installed" in out


def test_bridge_with_requests_ok(monkeypatch):
	import ai.jarvis.bridge as bridge
	from ai.jarvis.plugin_manager import PluginManager
	class FakeResp:
		def raise_for_status(self):
			return None
		def json(self):
			return {"response": "OK"}
	class FakeRequests:
		@staticmethod
		def post(url, json=None, timeout=None):  # type: ignore
			return FakeResp()
	monkeypatch.setattr(bridge, "requests", FakeRequests)
	ss = Session()
	pm = PluginManager()
	out = process_line("JARVIS: jarvis hello", ss, pm)
	assert out == "OK"


def test_bridge_with_requests_bad_json(monkeypatch):
	import ai.jarvis.bridge as bridge
	from ai.jarvis.plugin_manager import PluginManager
	class FakeResp:
		def raise_for_status(self):
			return None
		def json(self):
			raise ValueError("bad json")
	class FakeRequests:
		@staticmethod
		def post(url, json=None, timeout=None):  # type: ignore
			return FakeResp()
	monkeypatch.setattr(bridge, "requests", FakeRequests)
	ss = Session()
	pm = PluginManager()
	out = process_line("JARVIS: jarvis hello", ss, pm)
	assert "invalid JSON" in out or "error contacting" in out
