#!/usr/bin/env bash

source async.sh
source utils.sh

# Configuration
ico=$(pwd)/images/cpp.png # Icon of the image used for notifications
dirs="./src/"

# Function called at each folder source code update
function buildAndRun() {
    notify-send -t 1000 -i "${ico}" "Source code updated!" "File update detected in one of these folders ${dirs}, now re-compiling ..."
    ./make.sh;
    notify-send -t 1000 -i "${ico}" "Compilation done!" "Now running the software ..."
    ./run.sh;
};

# Start watcher
watch "${dirs}" buildAndRun

