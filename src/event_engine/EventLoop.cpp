//
// Created by marc on 29/12/18.
//
#include "EventLoop.h"
#include "ThreadGroup.h"

using namespace std;

namespace ipolitic {
    ofstream history;
    EventLoop::EventLoop(int n) {
        Lmgr.f(3,5);
        nnb_reactor = n > 0 ? n : nb_reactor;
        cout << "Starting event loop with " << nnb_reactor << " reactors. " << endl;
        // creating threads groups using ThreadGroup.h
        const char startChar = 'A';
        const char lastChar = 'D';

        AssociativeArray<float> threadsPerGroup = ThreadGroup::GetThreadsPerGroup(nnb_reactor);

        for (char i = startChar; i <= lastChar; i = (char) ((int) i + 1)) {
            std::string s;
            stringstream ss;
            ss << i;
            s = ss.str();
            int nbThread = (int) round(threadsPerGroup.operator[](s));
            for (int k = 0; k < nbThread; k += 1) {

                cout << "Starting reactor #" << reactors.size() << " in group n° " << i << endl;
                reactors.push_back(new Reactor(&actionStats));
                reactors[reactors.size() - 1]->group = i;
                reactors[reactors.size() - 1]->run();
                reactors[reactors.size() - 1]->pro = &profiler;
            }
        }
    }

    AssociativeArray<vec_action_stats> EventLoop::getAssocArrCpy() {
        return this->actionStats;
    }

    // todo : rewrite bellow using profiler
    int EventLoop::getMinReactor(action currentAction) {
        char estimatedGroup = this->profiler.getAverageWaitGroup(currentAction.UID);
        // todo : create thread if wait time is too important
        for (int i = 0; i < reactors.size(); i++) {
            float waitTime = reactors[i]->getWaitTime();

            if ((waitTime < 20000 && waitTime >= ThreadGroup::D) && (reactors[i]->group == 'D')) {
                return i;
            }

            if ((waitTime < ThreadGroup::D) && (reactors[i]->group == 'C')) {
                return i;
            }


            if ((waitTime < ThreadGroup::C) && (reactors[i]->group == 'B')) {
                return i;
            }

            if ((waitTime < ThreadGroup::B) && (reactors[i]->group == 'A')) {
                return i;
            }
        }
        return FORCE_THREAD_CREATION;
    }

    void EventLoop::reactorThread() {
        vector<Reactor> rec;
        while (!shouldStop) {
            if (!actions.empty()) {
                action_mutex.lock();
                action currentAction = actions.at(0);
                int minIndex = getMinReactor(currentAction);
                // for really big execution times we create a thread
                if (minIndex == FORCE_THREAD_CREATION) {
                    Reactor * reactor = new Reactor(&this->actionStats);
                    reactor->run();
                    reactor->insertAction(currentAction, true);
                    cout << "New thread created for action {" << currentAction.UID << "} (thread pool is busy - or - action was profiled as too long)" << endl;
                } else {
                    actions.erase(actions.begin());
                    cout << "action #" << actions.size() << " (" << currentAction.UID << ") sent to thread " << minIndex
                         << " (" << reactors[minIndex]->actions.size() << " length) " << endl;
                    action_mutex.unlock();
                    reactors[minIndex]->insertAction(currentAction);
                }
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
        profiler.run(&shouldStop);
        this->rThread = thread([this]() -> void {
            this->reactorThread();
        });
    }

    void EventLoop::stop() {
        for (int i = 0; i < nnb_reactor; i++) {
            cout << "Stopping reactor  #" << i << endl;
            reactors[i]->shouldStop = true;
            reactors[i]->rThread.join();
        }
        shouldStop = true;
        rThread.join();
        profiler.rThread.join();
        Lmgr.close();
    }
}