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
#include "IDKParser.h"
#include "ThreadGroup.h"

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

        int writtenActions = 0;
        mutex innerMutex;
        IDKParser idk;
        thread rThread;
        bool * shouldExit;
        void profiler_thread() {
            auto f = [](int * avg, int * cnt, int * t) -> void {
                *avg = (
                               *avg   *  *cnt + *t
                       ) / ++*cnt;
            };


            while(!*shouldExit) {
                if (!innerActions.empty()) {
                    innerMutex.lock();
                    profiler_item currentAction = innerActions.at(0);
                    // if action is new, we create it
                    if (!actionStats.IsItem(currentAction.action_name)) {
                        actionStats.AddItem(currentAction.action_name, *new IDKParser::actions_data);
                    }
                    // we append given stats to the average
                    char out = ThreadGroup::MsToChar(currentAction.duration);
                    switch (out) {
                        case 'A':
                            f   (
                                    &actionStats.operator[](currentAction.action_name).A.avg,
                                    &actionStats.operator[](currentAction.action_name).A.count,
                                    &currentAction.duration
                            );
                            break;
                        case 'B':
                            f   (
                                    &actionStats.operator[](currentAction.action_name).B.avg,
                                    &actionStats.operator[](currentAction.action_name).B.count,
                                    &currentAction.duration
                            );
                            break;
                        case 'C':
                            f   (
                                    &actionStats.operator[](currentAction.action_name).C.avg,
                                    &actionStats.operator[](currentAction.action_name).C.count,
                                    &currentAction.duration
                            );
                            break;
                        case 'D':
                            f   (
                                    &actionStats.operator[](currentAction.action_name).D.avg,
                                    &actionStats.operator[](currentAction.action_name).D.count,
                                    &currentAction.duration
                            );
                            break;
                        default:
                            break;
                    }
                    innerActions.erase(innerActions.begin());
                    innerMutex.unlock();
                }
                this_thread::sleep_for(chrono::seconds(2));
            }
        }

        void run(bool * shouldExit) {
            actionStats = idk.loadFromFile();
            this->shouldExit = shouldExit;
            actionStats = idk.loadFromFile();
            this->rThread = thread([this]() -> void {
                this->profiler_thread();
            });
        }

        void insertDuration(string name, int durationMs) {
            innerMutex.lock();
            innerActions.push_back(*new profiler_item{name, durationMs});
            innerMutex.unlock();
        }
    };
}


#endif //NALP_PROFILER_H
