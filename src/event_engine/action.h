//
// Created by marc on 29/12/18.
//

#ifndef NALP_ACTION_H
#define NALP_ACTION_H

#include <string>
#include <functional>
#include <chrono>
#include <vector>

using namespace std;

namespace ipolitic {
    struct action {
        string UID;                 // the unique identifier so that we can profile routine and optimise thread amount
        function<void()> callback;  // the callback that will be ran later
    };

    struct action_plurial_stats {
        chrono::system_clock::time_point start;
        chrono::system_clock::duration duration;
    };

    struct vec_action_stats {
        vector<action_plurial_stats> executionTimes;
    };
}

#endif //NALP_ACTION_H
