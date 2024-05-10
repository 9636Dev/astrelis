import os
from pathlib import Path

CURRENT_DIR = Path(__file__).resolve().parent
PROJECT_DIR = CURRENT_DIR.parent

run_dir = os.path.join(PROJECT_DIR, 'run')
if not os.path.exists(run_dir):
    os.mkdir(run_dir)
    os.mkdir(os.path.join(run_dir, 'lib'))
