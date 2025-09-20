#!/usr/bin/env python3
"""
Plugin Manager for Jarvis Phase 3.0
Handles dynamic plugin discovery, loading, and command registration.
"""
import os
import sys
import importlib
import importlib.util
from typing import Dict, List, Any, Optional, Callable
from pathlib import Path

class PluginInfo:
    """Metadata for a loaded plugin."""
    def __init__(self, name: str, path: str, module: Any, commands: Dict[str, Callable]):
        self.name = name
        self.path = path
        self.module = module
        self.commands = commands
        self.load_time = os.path.getmtime(path)

class PluginManager:
    """Manages dynamic plugin loading and command registration."""
    
    def __init__(self, plugins_dir: str = "plugins", dev_mode: bool = False):
        self.plugins_dir = Path(plugins_dir)
        self.dev_mode = dev_mode
        self.plugins: Dict[str, PluginInfo] = {}
        self.command_registry: Dict[str, Callable] = {}
        self._setup_plugins_dir()
    
    def _setup_plugins_dir(self) -> None:
        """Create plugins directory if it doesn't exist."""
        self.plugins_dir.mkdir(exist_ok=True)
        # Create __init__.py if missing
        init_file = self.plugins_dir / "__init__.py"
        if not init_file.exists():
            init_file.write_text("# Jarvis Plugins\n")
    
    def discover_plugins(self) -> List[str]:
        """Discover available plugin files in plugins directory."""
        if not self.plugins_dir.exists():
            return []
        
        plugins = []
        for file_path in sorted(self.plugins_dir.glob("*.py")):
            if file_path.name != "__init__.py":
                plugins.append(str(file_path))
        return plugins
    
    def load_plugin(self, plugin_path: str) -> Optional[PluginInfo]:
        """Load a single plugin and extract its commands."""
        try:
            # Create module spec with unique name
            plugin_name = Path(plugin_path).stem
            module_name = f"jarvis_plugin_{plugin_name}_{id(plugin_path)}"
            
            spec = importlib.util.spec_from_file_location(module_name, plugin_path)
            if not spec or not spec.loader:
                return None
            
            # Load module
            module = importlib.util.module_from_spec(spec)
            sys.modules[module_name] = module
            spec.loader.exec_module(module)
            
            # Extract commands (functions that start with 'cmd_')
            commands = {}
            for attr_name in dir(module):
                if attr_name.startswith('cmd_'):
                    func = getattr(module, attr_name)
                    if callable(func):
                        command_name = attr_name[4:]  # Remove 'cmd_' prefix
                        commands[command_name] = func
            
            plugin_info = PluginInfo(plugin_name, plugin_path, module, commands)
            return plugin_info
            
        except Exception as e:
            print(f"[plugin] Failed to load {plugin_path}: {e}")
            return None
    
    def reload_plugin(self, plugin_name: str) -> bool:
        """Reload a specific plugin (for hot-reload in dev mode)."""
        if plugin_name not in self.plugins:
            return False
        
        plugin_info = self.plugins[plugin_name]
        
        # Remove old commands
        for cmd_name in plugin_info.commands:
            self.command_registry.pop(cmd_name, None)
        
        # Reload by re-loading the file with a new module name
        try:
            # Remove old module from sys.modules to force reload
            old_modules = [name for name in sys.modules.keys() if name.startswith(f"jarvis_plugin_{plugin_name}_")]
            for module_name in old_modules:
                del sys.modules[module_name]
            
            # Re-load the plugin file
            new_plugin_info = self.load_plugin(plugin_info.path)
            if new_plugin_info:
                # Update the plugin info
                plugin_info.commands = new_plugin_info.commands
                plugin_info.module = new_plugin_info.module
                plugin_info.load_time = new_plugin_info.load_time
                
                # Re-register commands
                for cmd_name, func in plugin_info.commands.items():
                    self.command_registry[cmd_name] = func
                
                return True
            return False
            
        except Exception as e:
            print(f"[plugin] Failed to reload {plugin_name}: {e}")
            return False
    
    def load_all_plugins(self) -> int:
        """Load all discovered plugins."""
        plugin_paths = self.discover_plugins()
        loaded_count = 0
        
        for plugin_path in plugin_paths:
            plugin_info = self.load_plugin(plugin_path)
            if plugin_info:
                self.plugins[plugin_info.name] = plugin_info
                
                # Register commands (first plugin wins)
                for cmd_name, func in plugin_info.commands.items():
                    if cmd_name not in self.command_registry:
                        self.command_registry[cmd_name] = func
                    else:
                        print(f"[plugin] Warning: Command '{cmd_name}' already exists, skipping")
                
                loaded_count += 1
        
        return loaded_count
    
    def get_command(self, command_name: str) -> Optional[Callable]:
        """Get a command function by name."""
        return self.command_registry.get(command_name)
    
    def list_commands(self) -> List[str]:
        """List all available commands from plugins."""
        return list(self.command_registry.keys())
    
    def list_plugins(self) -> List[str]:
        """List all loaded plugins."""
        return list(self.plugins.keys())
    
    def check_for_updates(self) -> List[str]:
        """Check for plugin file updates (for hot-reload)."""
        if not self.dev_mode:
            return []
        
        updated = []
        for plugin_name, plugin_info in self.plugins.items():
            try:
                current_mtime = os.path.getmtime(plugin_info.path)
                if current_mtime > plugin_info.load_time:
                    updated.append(plugin_name)
            except OSError:
                pass  # File might be deleted
        
        return updated
    
    def hot_reload_updated(self) -> int:
        """Reload all updated plugins."""
        updated_plugins = self.check_for_updates()
        reloaded_count = 0
        
        for plugin_name in updated_plugins:
            if self.reload_plugin(plugin_name):
                reloaded_count += 1
        
        return reloaded_count
