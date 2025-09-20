#!/usr/bin/env python3
"""
Example plugin for Jarvis Phase 3.0
Demonstrates plugin command structure and capabilities.
"""

def cmd_hello(input_text: str) -> str:
    """Say hello with optional name parameter."""
    parts = input_text.split()
    if len(parts) > 1:
        name = parts[1]
        return f"Hello, {name}! How can I help you today?"
    return "Hello! I'm Jarvis. How can I assist you?"

def cmd_time(input_text: str) -> str:
    """Get current time."""
    import datetime
    now = datetime.datetime.now()
    return f"Current time: {now.strftime('%Y-%m-%d %H:%M:%S')}"

def cmd_echo(input_text: str) -> str:
    """Echo back the input."""
    parts = input_text.split()
    if len(parts) > 1:
        return " ".join(parts[1:])
    return "Echo: (empty input)"

def cmd_help_plugins(input_text: str) -> str:
    """Show available plugin commands."""
    return """Available plugin commands:
- hello [name]: Say hello
- time: Show current time
- echo <text>: Echo back text
- help_plugins: Show this help"""
