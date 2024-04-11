import json
import os
import sys
import shutil
import fnmatch
import subprocess
from pathlib import Path

class ConfigType:
    def __init__(self, name: str, description: str, validation, from_string):
        self.name = name
        self.description = description
        self.validation = validation
        self.from_string_fn = from_string

    def validate(self, value) -> bool:
        return self.validation(value)

    def from_string(self, value) -> tuple:
        try:
            return (self.from_string_fn(value), None)
        except Exception as e:
            return (False, e)

class ConfigValue:
    def __init__(self, name: str, description: str, type: str, default_value):
        self.name = name
        self.description = description
        self.type = type
        self.default_value = default_value


# =================================================================================================
# Modify the code below to suit your project
# =================================================================================================

EXCLUDED_CLEAN_FILES = []
CMAKE_BINARY = "cmake"
CURRENT_DIR = Path(__file__).resolve().parent
PROJECT_DIR = CURRENT_DIR.parent
BUILD_DIR = os.path.join(PROJECT_DIR, 'build')
ORIGINAL_BUILD_DIR = BUILD_DIR # Copy the original build directory
BINARY_DIR = os.path.join(BUILD_DIR, 'bin')
CONFIG_TYPES = {
    "string": ConfigType("string", "A string value", lambda _: True, lambda x: x),
    "generator": ConfigType("generator", "A CMake generator", lambda x: x in ["Ninja", "Unix Makefiles", "Visual Studio 17 2022", "Xcode"], lambda x: x),
    "build_type": ConfigType("build_type", "A build type", lambda x: x in ["Debug", "Release", "RelWithDebugInfo"], lambda x: x),
    "boolean": ConfigType("boolean", "A boolean value", lambda x: x in ["true", "false", True, False], lambda x: x == "true" or x == True),
    "path": ConfigType("path", "A path", lambda x: os.path.exists(x), lambda x: x),
}
CONFIG_VALUES = {
    "cmake_generator": ConfigValue("cmake_generator", "The CMake generator to use", "generator", "Ninja"),
    "build_type": ConfigValue("build_type", "The build type to use", "build_type", "Debug"),
    "compile_commands": ConfigValue("compile_commands", "Generate compile_commands.json", "boolean", "true"),
    "cmake_args": [ConfigValue("cmake_args", "The arguments to pass to CMake", "string", "")],
}

RUN_CONFIG_VALUES = {
    "binary": ConfigValue("binary", "The binary to run", "string", "bin/main"),
    "args": [ConfigValue("arguments", "The arguments to pass to the binary", "string", "")],
}

GLOBAL_CONFIG_VALUES = {
    "working_dir": ConfigValue("working_dir", "The relative path to the working directory", "path", "run"),
}

# =================================================================================================
# Only Modify the code below if you know what you are doing
# =================================================================================================

def safe_get_input(prompt: str, default_value: str) -> tuple[str, bool]:
    try:
        value = input(f'{prompt} [{default_value}]: ').strip()
    except KeyboardInterrupt:
        print('Input cancelled. ')
        return (default_value, False)
    if value == '':
        return (default_value, True)
    return (value, True)

print("Building... (Directory: {})".format(CURRENT_DIR))

# Create the configuration file if it does not exist
config_file = os.path.join(CURRENT_DIR, 'config.json')
if not os.path.exists(config_file):
    print('Configuration file not found. Creating a new one.')
    name,success = safe_get_input('Enter profile name', 'Default')
    if not success:
        exit(1)
    prof = {}

    for key, value in CONFIG_VALUES.items():
        if type(value) == list:
            prof[key] = []
            while True:
                inp, success = safe_get_input(f'Enter {value[0].description} (or ctrl+c to finish)', value[0].default_value)
                if not success:
                    break
                prof[key].append(inp)
            continue

        value, success = safe_get_input(f'Enter {value.description}', value.default_value)
        if not success:
            exit(1) # Exit if the user cancels the input
        prof[key] = value

    config = {
        "profiles": {
            name: prof
        },
        "run_configurations": {}
    }

    print('Writing configuration file...')
    try:
        json.dump(config, open(config_file, 'w'), indent=4)
    except Exception as e:
        print(f'Error writing configuration file: {e}')
        exit(1)

# Read the configuration file
with open(config_file) as f:
    try:
        config = json.load(f)
    except Exception as e:
        print(f'Error reading configuration file: {e}')
        exit(1)

# We make sure that the configuration file is not empty
if not config:
    print('Configuration file is empty. Exiting...')
    exit(1)

# Profile configurations
for profile_name, prof in config['profiles'].items():
    for key, value in CONFIG_VALUES.items():
        if key not in prof:
            print(f'Profile {profile_name} is missing key {key}. \n{value[0].description if type(value) == list else value.description}. \nExiting..')
            exit(1)
        if type(value) == list:
            for i, val in enumerate(prof[key]):
                if not CONFIG_TYPES[value[0].type].validate(val):
                    print(f'Profile {profile_name} has an invalid value for key {key}. \n{value[0].description}. \nExiting...')
                    exit(1)
            continue
        if not CONFIG_TYPES[value.type].validate(prof[key]):
            print(f'Profile {profile_name} has an invalid value for key {key}. \n{value.description}. \nExiting...')
            exit(1)

# Run configurations
for rconf_name, rconf in config['run_configurations'].items():
    for key, value in RUN_CONFIG_VALUES.items():
        if key not in rconf:
            print(f'Run configuration {rconf_name} is missing key {key}. \n{value[0].description if type(value) == list else value.description}. \nExiting..')
            exit(1)
        if type(value) == list:
            for i, val in enumerate(rconf[key]):
                if not CONFIG_TYPES[value[0].type].validate(val):
                    print(f'Run configuration {rconf_name} has an invalid value for key {key}. \n{value[0].description}. \nExiting...')
                    exit(1)
            continue
        if not CONFIG_TYPES[value.type].validate(rconf[key]):
            print(f'Run configuration {rconf_name} has an invalid value for key {key}. \n{value.description}. \nExiting...')
            exit(1)

# Global configurations
for key, value in GLOBAL_CONFIG_VALUES.items():
    if key not in config['global_configs']:
        print(f'Global configuration is missing key {key}. \n{value[0].description if type(value) == list else value.description}. \nExiting..')
        exit(1)
    if type(value) == list:
        for i, val in enumerate(config['global_configs'][key]):
            if not CONFIG_TYPES[value[0].type].validate(val):
                print(f'Global configuration has an invalid value for key {key}. \n{value[0].description}. \nExiting...')
                exit(1)
        continue
    if not CONFIG_TYPES[value.type].validate(config['global_configs'][key]):
        print(f'Global configuration has an invalid value for key {key}. \n{value.description}. \nExiting...')
        exit(1)

# If the arguments are not empty, we use them to select the profile
if len(sys.argv) > 1:
    profile = sys.argv[1]
    if profile not in config['profiles']:
        print('Invalid profile. Exiting...')
        exit(1)
else:
    profiles = list(config['profiles'].keys())
    print('Select a profile to continue...')
    for i, profile in enumerate(profiles):
        print(f'{i+1}. {profile}')

    # Check if the last profile is in the environment
    profile, success = safe_get_input('Select a profile', profiles[0])
    if not success:
        print('Invalid profile. Exiting...')
        exit(1)
    try:
        prof = int(profile)
        if prof < 1 or prof > len(profiles):
            print('Invalid profile. Exiting...')
            exit(1)
        profile = profiles[prof-1]
    except ValueError:
        pass
    if profile not in profiles:
        inp, success = safe_get_input('Invalid profile. Create profile?', "Y")
        if not success:
            print('Invalid Input. Exiting...')
            exit(1)
        if inp.lower().startswith('y'):
            profile, success = safe_get_input('Enter profile name', profile)
            if not success:
                exit(1)
            config['profiles'][profile] = {}
            for key, value in CONFIG_VALUES.items():
                value, success = safe_get_input(f'Enter {value.description}', value.default_value)
                if not success:
                    exit(1)
                config['profiles'][profile][key] = value

ACTIONS = {
    "build": "Build the project",
    "run": "Run the project",
    "clean": "Clean the build directory",
    "configure": "Configure the project",
    "exit": "Exit the program",
    "exec": "Open a shell in the build directory",

}

if len(sys.argv) > 2:
    action = sys.argv[2]
    if action not in ACTIONS:
        print('Invalid action. Exiting...')
        exit(1)
else:
    for i, (key, value) in enumerate(ACTIONS.items()):
        print(f'{i+1}. {value}')

    try:
        action,success = safe_get_input('Select an action', 'build')

        if not success:
            print('Invalid action. Exiting...')
            exit(1)

        try:
            action = int(action)
            if action < 1 or action > len(ACTIONS):
                print('Invalid action. Exiting...')
                exit(1)
            action = list(ACTIONS.keys())[action-1]
        except ValueError:
            pass

        if action not in ACTIONS:
            print('Invalid action. Exiting...')
            exit(1)

    except KeyboardInterrupt:
        print('Exiting...')
        exit(0)

if action == 'exit':
    print('Exiting...')
    exit(0)


# We set the build dir to append the profile name as a subdirectory
BUILD_DIR = os.path.join(BUILD_DIR, profile)

# Create the build directory if it does not exist
if not os.path.exists(BUILD_DIR):
    os.makedirs(BUILD_DIR)

if action == 'clean':
    print('Cleaning the build directory...')

    for file in os.listdir(BUILD_DIR):
        exclude = False
        for pattern in EXCLUDED_CLEAN_FILES:
            if fnmatch.fnmatch(file, pattern):
                exclude = True
                break
        if exclude:
            continue

        file_path = os.path.join(BUILD_DIR, file)
        try:
            if os.path.isdir(file_path):
                print(f'Removing directory {file_path}...', end=' ')
                shutil.rmtree(file_path)
                print('Done')
            else:
                print(f'Removing file {file_path}...', end=' ')
                os.remove(file_path)
                print('Done')
        except Exception as e:
            print(f'Error: {e}')
    print('Done')
    exit(0)

def get_profile_config(profile: str, key: str):
    if (type(CONFIG_VALUES[key]) == list):
        ctype = CONFIG_TYPES[CONFIG_VALUES[key][0].type]
        res = [ctype.from_string(cfg) for cfg in config['profiles'][profile][key]]
        for cfg,ex in res:
            if ex:
                print(f'Error: {ex}')
                exit(1)
        return [cfg for cfg,_ in res]
    cfg,ex= CONFIG_TYPES[CONFIG_VALUES[key].type].from_string(config['profiles'][profile][key])
    if ex:
        print(f'Error: {ex}')
        exit(1)
    return cfg

if action == 'configure':
    print('Configuring the project...')
    generator = get_profile_config(profile, 'cmake_generator')
    build_type = get_profile_config(profile, 'build_type')
    compile_commands = "ON" if get_profile_config(profile, 'compile_commands') else "OFF"

    print('Generating with types:')
    for key, value in CONFIG_VALUES.items():
        if type(value) == list:
            continue
        print(f'{value.name}: {config["profiles"][profile][key]}')

    arguments = [
        f'-DCMAKE_BUILD_TYPE={build_type}',
        f'-DCMAKE_EXPORT_COMPILE_COMMANDS={compile_commands}',
    ]

    cmake_args = get_profile_config(profile, 'cmake_args')
    if cmake_args:
        arguments.extend(cmake_args)

    cmd = f'{CMAKE_BINARY} -G "{generator}" {' '.join(arguments)} -S {PROJECT_DIR} -B {BUILD_DIR}'
    print(f'Running command: {cmd}')
    os.system(cmd)

    if compile_commands == "ON":
        print('Copying compile_commands.json to project root...', end=' ')
        try:
            shutil.copy(os.path.join(BUILD_DIR, 'compile_commands.json'), os.path.join(PROJECT_DIR, 'compile_commands.json'))
            print('Done')
        except Exception as e:
            print(f'Error: {e}')

    exit(0)

if action == 'build':
    print('Building the project...')
    os.system(f'{CMAKE_BINARY} --build {BUILD_DIR} --parallel {os.cpu_count()}')
    exit(0)

if action == 'run':
    cwd = config['global_configs']['working_dir']

    EXECUTABLE_PREFIX = sys.platform == 'win32' and '' or './'
    EXECUTABLE_SUFFIX = sys.platform == 'win32' and '.exe' or ''

    for i, (key, value) in enumerate(config['run_configurations'].items()):
        print(f'{i+1}. {key}')

    inp,res = safe_get_input('Select a run configuration', '1')
    if not res:
        print('Invalid selection. Exiting...')
        exit(1)

    index = int(inp) - 1
    if index < 0 or index >= len(config['run_configurations']):
        print('Invalid selection. Exiting...')
        exit(1)

    name = list(config['run_configurations'].keys())[index]
    conf = config['run_configurations'][name]

    binary = EXECUTABLE_PREFIX + conf['binary'] + EXECUTABLE_SUFFIX
    binary = os.path.join(BUILD_DIR, binary)
    if not os.path.exists(binary):
        print(f'Error: Binary {binary} not found. Exiting...')
        exit(1)

    arguments = conf['args']

    print(f'Running {name} ({binary})...')
    cmd = [binary, *arguments]

    try:
        result = subprocess.run(cmd,
                        cwd=cwd,  # Set your working directory
                        stdin=None,                  # Use the parent's stdin
                        text=True)
        print("Exit code:", result.returncode)
    except KeyboardInterrupt:
        print('Exiting...')
    except Exception as e:
        print(f'Error: {e}')

    exit(0)

if action == 'exec':
    # Open shell in project build directory
    os.system(f'cd {BUILD_DIR} && $SHELL')
