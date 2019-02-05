//
// Created by marcfsr on 24/01/2019.
//



#include "./Profiler.h"

namespace ipolitic {
    void Profiler::profiler_thread() {
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
                cout << "[PROCESSING : "<< currentAction.action_name << " ]" << currentAction.duration << endl;
                // we append given stats to the average
                char out = ThreadGroup::MsToChar(currentAction.duration);
                switch (out) {
                    case 'A':
                        f(
                                &actionStats.operator[](currentAction.action_name).A.avg,
                                &actionStats.operator[](currentAction.action_name).A.count,
                                &currentAction.duration
                        );
                        break;
                    case 'B':
                        f(
                                &actionStats.operator[](currentAction.action_name).B.avg,
                                &actionStats.operator[](currentAction.action_name).B.count,
                                &currentAction.duration
                        );
                        break;
                    case 'C':
                        f(
                                &actionStats.operator[](currentAction.action_name).C.avg,
                                &actionStats.operator[](currentAction.action_name).C.count,
                                &currentAction.duration
                        );
                        break;
                    case 'D':
                        f(
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
            //cout << "PROHILER WAITING " << endl;
            //this_thread::sleep_for(chrono::seconds(2));

        }

        this->idk.save(actionStats);
    }

    void Profiler::run(bool * shouldExit) {
        actionStats = idk.loadFromFile();
        this->shouldExit = shouldExit;
        actionStats = idk.loadFromFile();
        this->rThread = thread([this]() -> void {
            this->profiler_thread();
        });
    }

    void Profiler::insertDuration(string name, int durationMs) {
        innerMutex.lock();
        innerActions.push_back(*new profiler_item{name, durationMs});
        innerMutex.unlock();
    }
}


