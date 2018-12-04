#!/usr/bin/env bash
function rmfileifpresent {
   rm -f -- $1
}
function rmdir {
    rm -rf ./$1
    mkdir ./$1
    cd ./$1
    touch .gitkeep
    cd ./../
}
# removing build outputs
rmdir cmake-build-debug

# removing project deps
rmdir libs

# removing compiled unit testing file
rmfileifpresent test

