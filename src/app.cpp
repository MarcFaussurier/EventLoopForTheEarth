// promise example

#include <iostream>       // cout
#include <functional>     // ref
#include <thread>         // thread
#include <any>
#include <string>
#include <chrono>
#include <unistd.h>
#include <mutex>
// #include <bits/stdc++.h>
#include <promise.hpp>

#include "event_engine/action.h"
#include "util/AssociativeArray.hpp"
#include "event_engine/Reactor.h"
#include "event_engine/bpromise.h"
#include "event_engine/EventLoop.h"
#include "http_server/web.h"
#include "util/SCFParser.hpp"
#include "util/Files.hpp"

using namespace std;
using namespace ipolitic;
using namespace bpromise;



const int CORE_THREADS_CNT = 8;
const int SUB_THREAD_CNT = 2 * CORE_THREADS_CNT;
const int SEND_TO_SUB_THREAD_AFTER_X_MS = 10;

const auto maxActiveReactorTime = std::chrono::milliseconds(SEND_TO_SUB_THREAD_AFTER_X_MS);    // after 10 ms, action will be sent to a secondary reactor thread

const int WAIT_TIME_BEFORE_EXIT = 5;

EventLoop * EVL;


void eventLoopHandler(int s){
    printf("Stopping server with code: %d\n",s);
    EVL->stop();
    cout << "Sleeping for " << WAIT_TIME_BEFORE_EXIT << " seconds before exit ... " << endl;
    sleep(WAIT_TIME_BEFORE_EXIT);
    exit(1);
}

/***
 *
 *
 *
 * @return
 */

int main ()
{
    cout << "Fetching all scf files ... " << endl;
    vector<string> scfFiles = glob("./../www/", ".*.scf", true);
    cout << "OUTPUT : " << endl;
    for(unsigned long i = 0; i < scfFiles.size(); i +=1) {
        cout << scfFiles.at(i) << endl;
    }

    exit(0);


    SCFParser scfParser;
    // registring shell signals
    struct sigaction sigIntHandler;
    sigIntHandler.sa_handler = eventLoopHandler;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;
    sigaction(SIGINT, &sigIntHandler, NULL);

    /*
    auto onRunHook = []() -> void {
        // run the lua script
        LuaManager Lmgr;
        Lmgr.f(3,5);
        Lmgr.close();
    };

    auto onExitHook = []() -> void {
        // run the lua script
        LuaManager Lmgr;
        Lmgr.f(3,5);
        Lmgr.close();
    };
     */

    // run the event loop
    auto el = new EventLoop(CORE_THREADS_CNT);
    EVL = el;
    el->run();
    runServer(el);
    // exit if server stopped
    sleep(3);
    el->stop();
    return 0;
}