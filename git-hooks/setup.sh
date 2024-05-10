#!/bin/sh

# This file will setup the git hooks for the project

# We need to chdir to the directory of the script
cd $(dirname $0)

files=$(find . -type f -not -name "setup.sh")
git_hooks_dir=$(git rev-parse --show-toplevel)/.git/hooks

echo "Setting up git hooks..."

for file in $files; do
    chmod +x "$file"
    newname=$(basename "$file")
    # If ends with .sh, remove it
    if [[ $newname == *.sh ]]; then
        newname=${newname%.sh}
    fi
    ln -sf "$(readlink -f "$file")" "$git_hooks_dir/$newname"
done

echo "Finished setting up git hooks"
