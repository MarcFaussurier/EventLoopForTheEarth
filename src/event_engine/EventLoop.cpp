//
// Created by marc on 29/12/18.
//

#include <string>
#include <math.h>
#include <sstream>


#include "EventLoop.h"

#include "IDKParser.h"
#include "ThreadGroup.h"

using namespace std;

namespace ipolitic {
    ofstream history;

    EventLoop::EventLoop(int n) {
        nnb_reactor = n > 0 ? n : nb_reactor;
        cout << "Starting event loop with " << nnb_reactor << " reactors. " << endl;
        // creating threads groups using ThreadGroup.h
        const char startChar    = 'A';
        const char lastChar     = 'D';

        AssociativeArray<float> threadsPerGroup = ThreadGroup::GetThreadsPerGroup(nnb_reactor);

        for (char i = startChar; i <= lastChar; i = (char) ( (int) i + 1) ) {
            std::string s;
            stringstream ss;
            ss << i;
            s = ss.str();
            int nbThread = (int) round(threadsPerGroup.operator[](s));
            for (int k = 0; k < nbThread; k += 1 ) {
                cout << "Starting reactor #" << reactors.size() << " in group n° " << i << endl;
                reactors.push_back(new Reactor(&actionStats));
                reactors[reactors.size() - 1]->run();
            }
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
        IDKParser idk;
        idk.initHistory();
        idk.loadFromFile();

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