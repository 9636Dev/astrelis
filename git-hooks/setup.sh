#!/bin/sh

cd $(dirname $0)

files=$(find . -type f -not -name 'setup.sh')
git_hooks_dir=$(git rev-parse --show-toplevel)/.git/hooks

echo "Setting up hooks in $git_hooks_dir"

for file in $files; do
    chmod +x "$file"
    newname=$(basename $file)
    if [[ $newname == *.sh ]]; then
        newname=${newname%.sh}
    fi
    ln -sf "$file" "$git_hooks_dir/$newname"
    echo "Successfully linked $file to $git_hooks_dir/$newname"
done

echo "Done"
