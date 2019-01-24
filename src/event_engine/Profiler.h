//
// Created by marcfsr on 24/01/2019.
//

#ifndef NALP_PROFILER_H
#define NALP_PROFILER_H

#include <string>

using namespace std;

const int MAX_HISTORIES = 50;

struct item {
    int     ms_duration;
    int     times;
};

struct action_history {
    string  key;
    item histories[MAX_HISTORIES];
};

class Profiler {

};


#endif //NALP_PROFILER_H
