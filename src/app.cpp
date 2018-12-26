// promise example
#include <iostream>       // cout
#include <functional>     // ref
#include <thread>         // thread
#include <future>         // promise, future
#include <any>
#include <string>
#include <chrono>
#include <unistd.h>
#include <mutex>

using namespace std;

class Reactor {
public:
    mutex baction_mutex;
    Reactor() = default;
    thread rThread;
    vector<function<void()>> actions;
    bool shouldStop = false;
    void reactorThread() {
        while(!shouldStop) {
            if(!actions.empty()) {
                baction_mutex.lock();
                function<void()> func = actions.at(0);
                actions.erase(actions.begin());
                baction_mutex.unlock();
                func();
            }
        }
    }

    void insertAction(function<void()> action) {
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
    bool shouldStop = false;
    vector<Reactor*> reactors;
    thread rThread;
    vector<function<void()>> actions;
    static const int nb_reactor = 4; // max count of reactor
    int nnb_reactor = 0;
    mutex * action_mutex;
public:
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
                action_mutex->lock();
                int minIndex = getMinReactor();
                function<void()> func = actions.at(0);
                actions.erase(actions.begin());
                cout << "action #" << actions.size() << " sent to thread " << minIndex << " (" << reactors[minIndex]->actions.size()  <<" length) " << endl;
                action_mutex->unlock();
                reactors[minIndex]->insertAction(func);
            }
        }
    }

    void insertAction(function<void()> action) {
        action_mutex->lock();
        this->actions.push_back(action);
        action_mutex->unlock();
    }
    EventLoop(mutex * action_mutex, int n) {
        this->action_mutex = action_mutex;
        nnb_reactor = n > 0 ? n : nb_reactor;
        for (int i = 0; i < nnb_reactor; i++) {
            cout << "Starting reactor #" << i << endl;
            reactors.push_back(new Reactor());
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
int main ()
{
    mutex action_mutex;

    auto el = new EventLoop(&action_mutex, 1);
    el->run();

    for(int i = 0; i < 10; i++) {
        // TODO :: ADD PARAMETERS SO THAT WE CAN PREDICT FUNCTION DURATION AND OPTIMISE THREADS REPARTITIONS

        el->insertAction([i, &el]() -> void {
            cout << "parent action no ° " << i << endl;
            usleep(100000);

            el->insertAction([]() -> void {
                cout << "CHILD" << endl;
                usleep(100000);
            });
        });
    }

    sleep(5);

    el->insertAction([&el]() -> void {
        cout << "first action " << endl;
        usleep(100000);

        el->insertAction([]() -> void {
            cout << "CHILD" << endl;
            usleep(100000);
        });
    });
    sleep(3);

    cout << "Stopping ... " << endl;
    el->stop();
    cout << "Shutdown" << endl;
    return 0;
}