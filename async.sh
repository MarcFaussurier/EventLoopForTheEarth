#!/usr/bin/env bash
#                                               #
#   ASYNC UTILITY FOR SHELL SCRIPTS @LYON1      #
#                                               #
RETURN="";#######################################
KILLED_ID=();

# Run the passed callback asynchronous and return its id
function async() {
    # run the given function using its param
    "$@" <&0 &
    RETURN=$!
    echo "Generated pid : ${RETURN}";
};

# Pause the current thread until the given callback id finish
function await() {
    FAIL=0
    if [[ ! " ${KILLED_ID[@]} " =~ " ${1} " ]]; then
        echo "Now awaiting ${job} ..."
        wait $1 || let "FAIL+=1"
    else
        echo "Not awaiting ${job} as it was killed already."
    fi;
    if [ "$FAIL" == "0" ]; then
        RETURN=0;
    else
        RETURN=1;
    fi
};

# Wait for all jobs to finish
function awaitAll() {
    FAIL=0
    for job in `jobs -p`; do
        if [[ ! " ${KILLED_ID[@]} " =~ " ${job} " ]]; then
            echo "Now awaiting ${job} ..."
            wait $job || let "FAIL+=1"
        else
            echo "Not awaiting ${job} as it was killed already."
        fi;
    done
    if [ "$FAIL" == "0" ]; then
        RETURN=0;
    else
        RETURN=1;
    fi
}

# Kill a job using its id
function killJob() {
    FAIL=0;
    echo "Now killing ${1} ...";
    kill $1 || let "FAIL+=1"
    KILLED_ID=(${1} ${KILLED_ID})
    if [ "$FAIL" == "0" ]; then
        RETURN=0;
    else
        RETURN=1;
    fi;
}

# Kill all the jobs
function killAll() {
    FAIL=0
    for job in `jobs -p`; do
        echo "Now killing ${job} ..."
        kill $job || let "FAIL+=1"
        KILLED_ID=(${job} ${KILLED_ID})
    done;
    if [ "$FAIL" == "0" ]; then
        RETURN=0;
    else
        RETURN=1;
    fi;
}