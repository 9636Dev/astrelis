#!/bin/sh
#
# There should be a prefix '[XXX] ' in the commit message
# Where XXX is:
# - BUG: A bug fix
# - FEAT: A new feature
# - DOC: Documentation only changes
# - STYLE: Changes that do not affect the meaning of the code (white-space, formatting, missing semi-colons, etc)
# - REFACTOR: A code change that neither fixes a bug nor adds a feature
# - TEST: Adding missing tests or correcting existing tests
# - WIP: Work in progress
# - UPDATE: Updating the code due to external dependencies
# - SUBMODULE: Updating the submodule
# - CI: Changes to our CI configuration files and scripts (example CircleCI)
# - CHORE: Other changes that don't modify src or test files
# - REWRITE: Rewriting the codebase


COMMIT_MSG_FILE="$1"
COMMIT_MSG=$(cat "$COMMIT_MSG_FILE")

# Regular expression to check the prefix of the commit message
PREFIX_REGEX='^\[(BUG|FEAT|DOC|STYLE|REFACTOR|TEST|WIP|UPDATE|SUBMODULE|CI|CHORE|REWRITE)\] '

# Check if the commit message starts with a valid prefix
if [[ ! $COMMIT_MSG =~ $PREFIX_REGEX ]]; then
    echo "ERROR: Commit message does not start with a valid prefix."
    echo "Valid prefixes include [BUG], [FEAT], [DOC], [STYLE], [REFACTOR], [TEST], [WIP], [UPDATE], [SUBMODULE], [CI], [CHORE], [REWRITE]."
    exit 1
fi

# Check if the commit message has at least 10 characters
if [[ ${#COMMIT_MSG} -lt 10 ]]; then
    echo "ERROR: Commit message is too short. It must be at least 10 characters."
    exit 1
fi

# If all checks pass, allow the commit
exit 0
