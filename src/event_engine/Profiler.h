//
// Created by marcfsr on 24/01/2019.
//

#ifndef NALP_PROFILER_H
#define NALP_PROFILER_H

#include <string>
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds
#include <vector>
#include "./../util/AssociativeArray.h"
#include "../util/IDKParser.hpp"
#include "./ThreadGroup.h"

using namespace std;

namespace ipolitic {
    class Profiler {
    public:
        struct profiler_item {
            string action_name;
            int duration;
        };
        vector<profiler_item> innerActions;
        AssociativeArray<IDKParser::actions_data> actionStats;
        mutex innerMutex;
        IDKParser idk;
        thread rThread;
        bool * shouldExit;

        void profiler_thread();

        void run(bool * shouldExit);

        void insertDuration(string name, int durationMs);

        float getAverageWaitTime(string name);

        char getAverageWaitGroup(string name);
    };
}

#endif //NALP_PROFILER_H
