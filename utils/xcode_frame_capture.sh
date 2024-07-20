#!/bin/bash

OUTPUT_FILE=frame.gputrace

# Param should be executable
if [ -z "$1" ]; then
    echo "Usage: $0 <executable>"
    exit 1
fi

METAL_CAPTURE_ENABLED=1 MVK_CONFIG_AUTO_GPU_CAPTURE_SCOPE=1 MVK_CONFIG_AUTO_GPU_CAPTURE_OUTPUT_FILE=$OUTPUT_FILE $1
echo "Return value: $?"
exit 0
