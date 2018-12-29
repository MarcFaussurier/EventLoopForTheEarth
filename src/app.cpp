// promise example
#include <iostream>       // cout
#include <functional>     // ref
#include <thread>         // thread
#include <any>
#include <string>
#include <chrono>
#include <unistd.h>
#include <mutex>
#include <promise.hpp>
#include "./AssociativeArray.h"
using namespace std;
using namespace promise;

#include <bits/stdc++.h>

struct action {
    string UID;                 // the unique identifier so that we can profile routine and optimise thread amount
    function<void()> callback;  // the callback that will be ran later
};

struct action_plurial_stats {
    std::chrono::system_clock::time_point start;
    std::chrono::system_clock::duration duration;
};

struct vec_action_stats {
    vector<action_plurial_stats> executionTimes;
};

const auto maxActiveReactorTime = std::chrono::milliseconds(10);    // after 10 ms, action will be sent to an other reactor

class Reactor {
    AssocArray<vec_action_stats> * actionStats;
public:
    mutex baction_mutex;
    Reactor(AssocArray<vec_action_stats> * actionStats) {
        this->actionStats = actionStats;
    }
    thread rThread;
    vector<action> actions;
    bool shouldStop = false;
    void reactorThread() {
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

    void insertAction(action action) {
        baction_mutex.lock();
        this->actions.push_back(action);
        baction_mutex.unlock();
    };

    void run() {
        this->rThread = thread([this]() -> void {
            this->reactorThread();
        });
    }
};
class EventLoop {
    //map<string, map >
    bool shouldStop = false;
    vector<Reactor*> reactors;
    thread rThread;
    vector<action> actions;
    AssocArray<vec_action_stats> actionStats;

    static const int nb_reactor = 4; // max count of reactor
    int nnb_reactor = 0;
    mutex action_mutex;
public:
    AssocArray<vec_action_stats> getAssocArrCpy() {
        return this->actionStats;
    }

    int getMinReactor() {
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
    void reactorThread() {
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

    void insertAction(string UID, function<void()> callback) {
        action insertedAction{UID, callback};
        action_mutex.lock();
        this->actions.push_back(insertedAction);
        action_mutex.unlock();
    }
    EventLoop(int n) {
        nnb_reactor = n > 0 ? n : nb_reactor;
        for (int i = 0; i < nnb_reactor; i++) {
            cout << "Starting reactor #" << i << endl;
            reactors.push_back(new Reactor(&actionStats));
            reactors[i]->run();
        }
    }
    void run() {
        this->rThread = thread([this]() -> void {
            this->reactorThread();
        });
    }
    void stop() {
        for(int i = 0; i < nnb_reactor; i++) {
            cout << "Stopping reactor  #" << i << endl;
            reactors[i]->shouldStop = true;
            reactors[i]->rThread.join();
        }
        shouldStop = true;
        rThread.join();
    }
};

/* Create new promise object */
template <typename FUNC>
inline Defer bNewPromise(EventLoop * el, string UID, FUNC func) {
    Defer promise = newHeadPromise();
    el->insertAction(UID, [promise, func]() -> void {
        promise->run(func, promise);
    });
    return promise;
}

/***
 *
 *
 *
 * @return
 */

int main ()
{
    auto int_ms = std::chrono::duration_cast<std::chrono::milliseconds>(maxActiveReactorTime);
    cout << int_ms.count() << endl;
    cout << "ms endl" << endl;
    auto el = new EventLoop( 5);
    el->run();

    Defer nextTest = bNewPromise(el,"someTest", [](Defer d) -> void {
        cout << "Promise before resolve" << endl;
        d.resolve();
    }).then([]() -> void {
       cout << "After resolve" << endl;
    });


    for(int i = 0; i < 10; i++) {
        // TODO :: ADD PARAMETERS SO THAT WE CAN PREDICT FUNCTION DURATION AND OPTIMISE THREADS REPARTITIONS

        el->insertAction("testOne", [i, &el]() -> void {
            cout << "parent action no ° " << i << endl;
            usleep(10);

            el->insertAction("testTwo", []() -> void {
                cout << "CHILD" << endl;
                usleep(10);
            });
        });
    }
    sleep(5);
    el->insertAction("testTree", [&el]() -> void {
        cout << "first action " << endl;
        usleep(10);

        el->insertAction("testFour", []() -> void {
            cout << "CHILD" << endl;
            usleep(10);
        });
    });


    el->insertAction("testTree", [&el]() -> void {
        cout << "first action " << endl;
        usleep(100);
    });

    el->insertAction("testTree", [&el]() -> void {
        cout << "first action " << endl;
        usleep(1000);
    });

    sleep(3);


    cout << "==========================================" << endl;
    cout << "Benchmarking results  : " << endl;

    AssocArray<vec_action_stats> cpy = el->getAssocArrCpy();
    long amountOfActionName = cpy.Size();
    for (long i = 0 ; i < amountOfActionName; i += 1) {
        cout << "i="<<i<<" name="<<cpy.GetItemName(i) << " times="<<cpy.operator[](i).executionTimes.size() << endl;
    }

    cout << "Stopping ... " << endl;
    el->stop();
    cout << "Shutdown" << endl;
    return 0;
}