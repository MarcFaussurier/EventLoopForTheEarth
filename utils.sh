#!/usr/bin/env bash

GREEN='\e[32m'; CYAN='\e[36m'; NC='\e[0m';
job1=0; job2=0;

function watchFolders() {
    while inotifywait -e modify ${1}; do
        killJob ${job1};
        job1=async buildAndRun;
    done;
};

function watch() {
    async $2;
    job1=${RETURN};
    async watchFolders "$1";
    job2=${RETURN};
    awaitAll;
};

# deps manager function
function install {
    # then perform a simple git clone / update
    printf "________________________\n";
    # if given repo does not exist, we clone it
    if [[ ! -d $1 ]]; then
        printf "${GREEN} Installing${NC} => ${CYAN}$1${NC} (@$1) ... \n";
        mkdir -p ${1}
        printf "Cloning from $3 \n";
        if  [[ $b == "master" ]]; then
            git clone ${3} ./$1;
        else
            git clone ${3} -b $2 ./${1} ;
        fi;
    # else we update it
    else
        printf "${GREEN} Updating${NC} => ${CYAN}$1${NC} (@$1) ... \n";
        cd ./$1;
        git pull origin $2
        cd ./../;
    fi;
};
