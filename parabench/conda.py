import json
from pathlib import Path
import subprocess
import os

def get_conda_environments():
    args = 'conda info --json'.split()
    try:
        json_str = subprocess.check_output(args)
    except OSError:
        raise OSError("Command 'conda info --json' failed, " \
                      "is conda installed and in PATH?")
    json_data = json.loads(json_str.decode())
    env_paths = {Path(p).name: Path(p) for p in json_data['envs']}
    active_env_path = Path(json_data['default_prefix'])
    return env_paths, active_env_path

def activate_environment(name):
    env_paths, activate_env_path = get_conda_environments()
    paths = os.getenv('PATH').split(':')
    new_path = env_paths[name] / 'bin'
    paths.insert(0, str(new_path))
    os.environ['PATH'] = ':'.join(paths)
