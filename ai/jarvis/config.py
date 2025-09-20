#!/usr/bin/env python3
"""
Configuration management for Jarvis Phase 3.0
Handles YAML config files and environment variable overrides.
"""
import os
import yaml
from typing import Dict, Any, Optional
from pathlib import Path

class JarvisConfig:
    """Centralized configuration management."""
    
    def __init__(self, config_path: str = "jarvis_config.yml"):
        self.config_path = Path(config_path)
        self.config: Dict[str, Any] = {}
        self._load_config()
    
    def _load_config(self) -> None:
        """Load configuration from file and environment variables."""
        # Default configuration
        self.config = {
            "ollama": {
                "url": "http://localhost:11434/api/generate",
                "model": "llama3.1",
                "timeout": 30
            },
            "session": {
                "capacity": 10,
                "persist_path": ""
            },
            "plugins": {
                "directory": "plugins",
                "dev_mode": False,
                "hot_reload": False
            },
            "security": {
                "safe_mode": False,
                "command_whitelist": [],
                "audit_log": "logs/jarvis_audit.log"
            }
        }
        
        # Load from YAML file if exists
        if self.config_path.exists():
            try:
                with open(self.config_path, 'r') as f:
                    file_config = yaml.safe_load(f) or {}
                    self._merge_config(self.config, file_config)
            except Exception as e:
                print(f"[config] Warning: Failed to load {self.config_path}: {e}")
        
        # Override with environment variables
        self._apply_env_overrides()
    
    def _merge_config(self, base: Dict[str, Any], override: Dict[str, Any]) -> None:
        """Recursively merge configuration dictionaries."""
        for key, value in override.items():
            if key in base and isinstance(base[key], dict) and isinstance(value, dict):
                self._merge_config(base[key], value)
            else:
                base[key] = value
    
    def _apply_env_overrides(self) -> None:
        """Apply environment variable overrides."""
        # Ollama settings
        if os.getenv("OLLAMA_URL"):
            self.config["ollama"]["url"] = os.getenv("OLLAMA_URL")
        if os.getenv("OLLAMA_MODEL"):
            self.config["ollama"]["model"] = os.getenv("OLLAMA_MODEL")
        if os.getenv("OLLAMA_TIMEOUT_S"):
            self.config["ollama"]["timeout"] = float(os.getenv("OLLAMA_TIMEOUT_S"))
        
        # Session settings
        if os.getenv("JARVIS_SESSION_PATH"):
            self.config["session"]["persist_path"] = os.getenv("JARVIS_SESSION_PATH")
        if os.getenv("JARVIS_SESSION_CAPACITY"):
            self.config["session"]["capacity"] = int(os.getenv("JARVIS_SESSION_CAPACITY"))
        
        # Plugin settings
        if os.getenv("JARVIS_PLUGINS_DIR"):
            self.config["plugins"]["directory"] = os.getenv("JARVIS_PLUGINS_DIR")
        if os.getenv("JARVIS_DEV_MODE"):
            self.config["plugins"]["dev_mode"] = os.getenv("JARVIS_DEV_MODE").lower() == "true"
        if os.getenv("JARVIS_HOT_RELOAD"):
            self.config["plugins"]["hot_reload"] = os.getenv("JARVIS_HOT_RELOAD").lower() == "true"
        
        # Security settings
        if os.getenv("JARVIS_SAFE_MODE"):
            self.config["security"]["safe_mode"] = os.getenv("JARVIS_SAFE_MODE").lower() == "true"
    
    def get(self, key_path: str, default: Any = None) -> Any:
        """Get configuration value using dot notation (e.g., 'ollama.url')."""
        keys = key_path.split('.')
        value = self.config
        
        for key in keys:
            if isinstance(value, dict) and key in value:
                value = value[key]
            else:
                return default
        
        return value
    
    def set(self, key_path: str, value: Any) -> None:
        """Set configuration value using dot notation."""
        keys = key_path.split('.')
        config = self.config
        
        for key in keys[:-1]:
            if key not in config:
                config[key] = {}
            config = config[key]
        
        config[keys[-1]] = value
    
    def save(self) -> bool:
        """Save current configuration to file."""
        try:
            # Ensure directory exists
            self.config_path.parent.mkdir(parents=True, exist_ok=True)
            
            with open(self.config_path, 'w') as f:
                yaml.dump(self.config, f, default_flow_style=False, indent=2)
            return True
        except Exception as e:
            print(f"[config] Failed to save {self.config_path}: {e}")
            return False
    
    def reload(self) -> None:
        """Reload configuration from file."""
        self._load_config()
