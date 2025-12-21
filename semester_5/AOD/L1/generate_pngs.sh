#!/bin/bash

# Check if a directory path is provided as an argument
if [ -z "$1" ]; then
    echo "Usage: $0 <path_to_directory>"
    exit 1
fi

TARGET_DIR="$1"

# Check if the provided path is a valid directory
if [ ! -d "$TARGET_DIR" ]; then
    echo "Error: '$TARGET_DIR' is not a valid directory."
    exit 1
fi

echo "Searching for .dot files in '$TARGET_DIR'..."

# Find all .dot files in the specified directory and its subdirectories
for dotfile in $(find "$TARGET_DIR" -name "*.dot"); do
    # Define the output filename by replacing .dot with .png
    pngfile="${dotfile%.dot}.png"
    
    # Generate the PNG file using the dot command
    echo "Generating $pngfile from $dotfile"
    dot -Tpng "$dotfile" -o "$pngfile"
done

echo "PNG generation complete."