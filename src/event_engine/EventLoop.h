//
// Created by marc on 29/12/18.
//

#ifndef NALP_EVENTLOOP_H
#define NALP_EVENTLOOP_H

#include <thread>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <sstream>

#include "../util/AssociativeArray.hpp"

#include "action.h"
#include "Reactor.h"
#include "ThreadGroup.h"
#include "../util/IDKParser.hpp"
#include "Profiler.h"

using namespace std;

namespace ipolitic {
    class EventLoop {
        //map<string, map >
        vector<Reactor*> reactors;
        thread rThread;
        vector<action> actions;
        AssociativeArray<vec_action_stats> actionStats;

        static const int nb_reactor = 4; // max count of reactor
        int nnb_reactor = 0;
        mutex action_mutex;
    public:
        const int FORCE_THREAD_CREATION = 999;
        Profiler profiler;
        bool shouldStop = false;
        AssociativeArray<vec_action_stats> getAssocArrCpy();
        int getMinReactor(action currentAction);
        void reactorThread();
        void insertAction(string UID, function<void()> callback);
        EventLoop(int n);
        void run();
        void stop();
    };
}

#endif //NALP_EVENTLOOP_H
