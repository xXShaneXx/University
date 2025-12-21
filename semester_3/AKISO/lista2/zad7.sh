#!/bin/bash

DIRECTORY="."

for file in "$DIRECTORY"/*; do
    if [[ -f "$file" ]]; then
        lowercase_file=$(echo "$file" | tr '[:upper:]' '[:lower:]')
        
        if [[ "$file" != "$lowercase_file" ]]; then
            mv "$file" "$lowercase_file"
            echo "Renamed: $file -> $lowercase_file"
        fi
    fi
done
