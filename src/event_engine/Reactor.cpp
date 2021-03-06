//
// Created by marc on 29/12/18.
//

#include "Reactor.h"

namespace ipolitic {
    Reactor::Reactor(AssociativeArray<vec_action_stats> * actionStats) {
        this->actionStats = actionStats;
    }
    void Reactor::reactorThread() {
        while(!shouldStop) {
            if(!actions.empty()) {
                baction_mutex.lock();
                action currentAction = actions.at(0);
                actions.erase(actions.begin());
                baction_mutex.unlock();
                auto pointA = chrono::system_clock::time_point::clock::now();
                currentAction.callback();
                auto pointB = chrono::system_clock::time_point::clock::now();
                auto duration = chrono::system_clock::duration(pointB - pointA);
                auto int_ms = std::chrono::duration_cast<std::chrono::milliseconds>(duration);

                cout << "Execution of " << currentAction.UID << " took : " << int_ms.count() << " ms " << endl;
                pro->insertDuration(currentAction.UID, int_ms.count());
                action_plurial_stats aps{pointA, duration};
                if (actionStats->IsItem(currentAction.UID)) {
                    actionStats->operator[](currentAction.UID).executionTimes.push_back(aps);
                } else {
                    vec_action_stats vec;
                    vec.executionTimes.push_back(aps);
                    actionStats->AddItem(currentAction.UID, vec);
                }
            }
        }
    }

    void Reactor::insertAction(action action, bool stopAfter) {
        baction_mutex.lock();
        if (stopAfter) {
            function<void()> cb = action.callback;
            action.callback = [&]() -> void {
                sleep(1);
                cb();
                sleep(10);
                this->shouldStop = true;
            };
        }
        this->actions.push_back(action);
        baction_mutex.unlock();
    };

    void Reactor::run() {
        this->rThread = thread([this]() -> void {
            this->reactorThread();
        });
    }

    float Reactor::getWaitTime() {
        int total = 0;
        int count = 0;

        for (int i = 0; i < this->actions.size(); i++) {
            total = total + this->pro->getAverageWaitTime(this->actions[i].UID);
            count++;
        }

        return
        (count > 0) ?
            (total / count)
        :
            0;
    }
};

