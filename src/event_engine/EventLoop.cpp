//
// Created by marc on 29/12/18.
//

#include "EventLoop.h"

namespace ipolitic {
    EventLoop::EventLoop(int n) {
        nnb_reactor = n > 0 ? n : nb_reactor;
        for (int i = 0; i < nnb_reactor; i++) {
            cout << "Starting reactor #" << i << endl;
            reactors.push_back(new Reactor(&actionStats));
            reactors[i]->run();
        }
    }

    AssociativeArray<vec_action_stats> EventLoop::getAssocArrCpy() {
        return this->actionStats;
    }

    int EventLoop::getMinReactor() {
        int minIndex = 0;
        int minVal = (int) reactors[minIndex]->actions.size();
        for (int i = 0; i < nnb_reactor; i++) {
            if (reactors[i]->actions.size() < minVal) {
                minIndex = i;
                minVal = (int) reactors[i]->actions.size();
            }
        }
        return minIndex;
    }
    void EventLoop::reactorThread() {
        while(!shouldStop) {
            if(!actions.empty()) {
                action_mutex.lock();
                int minIndex = getMinReactor();
                action currentAction = actions.at(0);
                actions.erase(actions.begin());
                cout << "action #" << actions.size() << " ("<<currentAction.UID<<") sent to thread " << minIndex << " (" << reactors[minIndex]->actions.size()  <<" length) " << endl;
                action_mutex.unlock();
                reactors[minIndex]->insertAction(currentAction);
            }
        }
    }

    void EventLoop::insertAction(string UID, function<void()> callback) {
        action insertedAction{UID, callback};
        action_mutex.lock();
        this->actions.push_back(insertedAction);
        action_mutex.unlock();
    }

    void EventLoop::run() {
        this->rThread = thread([this]() -> void {
            this->reactorThread();
        });
    }
    void EventLoop::stop() {
        for(int i = 0; i < nnb_reactor; i++) {
            cout << "Stopping reactor  #" << i << endl;
            reactors[i]->shouldStop = true;
            reactors[i]->rThread.join();
        }
        shouldStop = true;
        rThread.join();
    }
}