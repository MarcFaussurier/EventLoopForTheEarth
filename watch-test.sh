#!/usr/bin/env bash

source async.sh
source utils.sh

# Configuration
ico=$(pwd)/images/test.png; # Icon of the image used for notifications
success=$(pwd)/images/success.png; # Icon of the image used for notifications
fail=$(pwd)/images/fail.png; # Icon of the image used for notifications
dirs="./src/ ./plugins/ ./tests/"

# Function called at each folder source code update
function buildAndRun() {
    notify-send -t 1000 -i "${ico}" "Tests source code updated!" "File update detected in one of these folders ${dirs}, now re-compiling the tests..."
    g++ ./tests/nstr.cpp -o ./test &&
    notify-send -t 1000 -i "${ico}" "Compilation done!" "Now running the tests ..."
    ./test
    if [[ $? == "0" ]]; then
        notify-send -i "${success}" "TEST PASSED !!" "Congratulation, sir!"
    else
        notify-send -i "${fail}" "TESTS FAILED !!" "Please fix your code before pushing it, sir!"
    fi;
    echo "RET : $?";
};

# Start watcher
watch "${dirs}" buildAndRun