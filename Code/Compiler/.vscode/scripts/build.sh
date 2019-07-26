#!/bin/bash

WORKSPACE=$1
PROJECT_NAME=$2
BUILD_TYPE=$3
FILE=$4

echo -e "\e[32m------ Build Started: Project \"$PROJECT_NAME\" : $BUILD_TYPE ------\e[0m"

OUTPUT_DIR="$WORKSPACE/../bin/$BUILD_TYPE-Linux-x86/Compiler/"
CPP_FILES=$(find -regex '.*/.*\.\(cpp\)$')

if [ ! -d "$OUTPUT_DIR" ]; then
    echo "Creating build output directory..."
    mkdir -p "$OUTPUT_DIR"
fi

if [ -d "$OUTPUT_DIR" ]; then
    if [ $BUILD_TYPE == "Debug" ]; then
        g++ -o "$OUTPUT_DIR$PROJECT_NAME" $CPP_FILES -I . -Wall
    else
        g++ -o "$OUTPUT_DIR$PROJECT_NAME" $CPP_FILES -I .
    fi
    echo "$PROJECT_NAME -> $OUTPUT_DIR$PROJECT_NAME"
else
    echo -e "\e[1;31m====== Build failed for project \"$PROJECT_NAME\" ======\e[0m"
fi

echo -e "\e[1;32m====== Build succeeded for project \"$PROJECT_NAME\" ======\e[0m"