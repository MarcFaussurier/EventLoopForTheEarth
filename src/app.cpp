#include <iostream>
#include <csignal>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <linux/limits.h>

#include "rang.hpp"
#include "dotenv.h"

using namespace std;
using namespace dotenv;

void stopHandler(int s){
    cout << rang::style::reset << endl;
    cout <<  rang::fg::red << "Stopping your aweasome application..." << rang::fg::reset << endl;
}

char pathToRoot[] = "./../configs/.env";


void loadEnv() {
    char resolved_path[PATH_MAX];
    realpath(pathToRoot, resolved_path);
    string str = string(resolved_path);

    auto& dotenv = env;
    dotenv.config(str);
    dotenv.instance();
    cout << "Hello, " << dotenv["USER"] << std::endl;
}

int main(int argc, char* argv[])
{
    // register the CTRL + C to a custom handler
    struct sigaction sigIntHandler;
    sigIntHandler.sa_handler = stopHandler;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;
    sigaction(SIGINT, &sigIntHandler, NULL);

    // start message
    cout << rang::style::reset << endl;
    cout <<  rang::fg::green << "Starting your aweasome application..." << rang::fg::reset << endl;

    // load the configuration file
    loadEnv();

    // wait for a key to exit
    int x; cin >> x;
    return 0;
}


