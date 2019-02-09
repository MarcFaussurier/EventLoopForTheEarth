//
// Created by marcfsr on 24/01/2019.
//



#include "./Profiler.h"

namespace ipolitic {
    void Profiler::profiler_thread() {
        auto f = [](float * avg, int * cnt, int * t) -> void {
            int x;
            *avg = (
                           *avg   *  *cnt + *t
                   ) / ((x = ++*cnt) > 0 ? x : 999999);
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
                cout << "___________" << endl;
                cout << "ms : " << currentAction.duration << " CHAR : " << out << endl << endl;
                switch (out) {
                    case 'A':
                        f(
                                &(actionStats.operator[](currentAction.action_name).A.avg),
                                &(actionStats.operator[](currentAction.action_name).A.count),
                                &currentAction.duration
                        );
                        break;
                    case 'B':
                        f(
                                &(actionStats.operator[](currentAction.action_name).B.avg),
                                &(actionStats.operator[](currentAction.action_name).B.count),
                                &currentAction.duration
                        );
                        break;
                    case 'C':
                        f(
                                &(actionStats.operator[](currentAction.action_name).C.avg),
                                &actionStats.operator[](currentAction.action_name).C.count,
                                &currentAction.duration
                        );
                        break;
                    case 'D':
                        f(
                                &(actionStats.operator[](currentAction.action_name).D.avg),
                                &(actionStats.operator[](currentAction.action_name).D.count),
                                &currentAction.duration
                        );
                        break;
                    default:
                        break;
                }
                innerActions.erase(innerActions.begin());
                innerMutex.unlock();

                if (rand() % 10 + 1 <= 1) {
                    this->idk.save(actionStats);
                    cout << "file saved" << endl;
                }
            } else {
                this_thread::sleep_for(chrono::milliseconds(100));
            }
            //cout << "PROHILER WAITING " << endl;

        }

        this->idk.save(actionStats);
    }

    void Profiler::run(bool * shouldExit) {
        actionStats = idk.loadFromFile();
        this->shouldExit = shouldExit;
        this->rThread = thread([this]() -> void {
            this->profiler_thread();
        });
    }

    void Profiler::insertDuration(string name, int durationMs) {
        innerMutex.lock();
        innerActions.push_back(*new profiler_item{name, durationMs});
        innerMutex.unlock();
    }

    float Profiler::getAverageWaitTime(string name) {
        float x;
        return (
                    actionStats.operator[](name).A.avg * actionStats.operator[](name).A.count
                +   actionStats.operator[](name).B.avg * actionStats.operator[](name).B.count
                +   actionStats.operator[](name).C.avg * actionStats.operator[](name).C.count
                +   actionStats.operator[](name).D.avg * actionStats.operator[](name).D.count

                )
                /
                ( x = (   actionStats.operator[](name).A.count
                +   actionStats.operator[](name).B.count
                +   actionStats.operator[](name).C.count
                +   actionStats.operator[](name).D.count) > 0 ? x : 99999999) ;
    }

    char Profiler::getAverageWaitGroup(string name) {
        return ThreadGroup::MsToChar(this->getAverageWaitTime(name));
    }
}


