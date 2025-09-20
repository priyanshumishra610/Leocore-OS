# Phase 3.0: Plugin System

## Overview
Phase 3.0 introduces a dynamic plugin system that allows extending Jarvis functionality without modifying core code. Plugins can register commands, handle system interactions, and be hot-reloaded during development.

## Features Implemented

### Plugin Discovery & Loading
- **Auto-discovery**: Scans `plugins/` directory for `.py` files
- **Dynamic loading**: Loads plugins at runtime without restart
- **Command extraction**: Automatically finds functions prefixed with `cmd_`
- **Error handling**: Graceful failure for malformed plugins

### Dynamic Command Registration
- **No core modification**: Add commands without editing bridge code
- **Conflict resolution**: First plugin wins for duplicate commands
- **Command registry**: Centralized command lookup and execution
- **Plugin isolation**: Each plugin runs in its own namespace

### Hot-Reload Support
- **Dev mode**: Enable with `JARVIS_DEV_MODE=true`
- **File watching**: Monitors plugin files for changes
- **Live updates**: Reloads modified plugins without restart
- **Module cleanup**: Handles Python module caching issues

### Configuration Management
- **YAML config**: `jarvis_config.yml` for all settings
- **Environment overrides**: `JARVIS_*` variables take precedence
- **Plugin settings**: Configurable plugin directory and dev mode
- **Session settings**: Configurable memory capacity and persistence

## File Structure
```
ai/jarvis/
├── plugin_manager.py    # Plugin discovery, loading, hot-reload
├── config.py           # YAML + env config management
├── bridge.py           # Updated with plugin integration
├── session.py          # Enhanced with persistence
└── commands.py         # Core system commands

plugins/
├── __init__.py         # Plugin package
└── example.py          # Sample plugin with commands

jarvis_config.yml       # Main configuration file
```

## Usage

### Basic Plugin
Create a file in `plugins/` directory:
```python
def cmd_hello(input_text: str) -> str:
    return f"Hello from plugin! You said: {input_text}"

def cmd_time(input_text: str) -> str:
    import datetime
    return f"Current time: {datetime.datetime.now()}"
```

### Configuration
```yaml
# jarvis_config.yml
plugins:
  directory: "plugins"
  dev_mode: true
  hot_reload: true

ollama:
  url: "http://localhost:11434/api/generate"
  model: "llama3.1"
```

### Environment Variables
```bash
export JARVIS_DEV_MODE=true
export JARVIS_PLUGINS_DIR="custom_plugins"
export OLLAMA_MODEL="llama3.1"
```

## Testing
- **18 tests passing** with **78% coverage**
- Plugin loading, command registration, hot-reload
- Configuration loading and environment overrides
- Bridge integration with plugin system
- Error handling and edge cases

## Known Limitations
- **Hot-reload**: May not work consistently due to Python module caching
- **Command conflicts**: No priority system beyond first-loaded wins
- **Plugin dependencies**: No dependency management between plugins

## Next Steps (Phase 3.1)
- Voice interface integration
- Microphone input and TTS output
- Audio pipeline with fallback to text mode
- Voice command recognition and processing
