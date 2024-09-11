#!/bin/bash


# With this script, we'll want to add '#ifndef FILENAME_H \n #define FILENAME_H ... #endif'
# to all files in /definitions/.
# This wasn't done.

SEARCH_PATH=$(pwd)/definitions

# Get all files in SEARCH_PATH

for filename in $SEARCH_PATH/*.h; do
  FILE_CONTENT=$(cat "$filename")
  HEADER_NAME=$(echo "$(basename $filename)" | tr '[:lower:]' '[:upper:]')
  HEADER_NAME=$(echo $HEADER_NAME | tr '.' '_')
  printf "\n\n\n#ifndef STRIDE_LANGUAGE_$HEADER_NAME\n#define STRIDE_LANGUAGE_$HEADER_NAME\n\n%s\n\n#endif" "$FILE_CONTENT" > "$filename"
done