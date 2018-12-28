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

using namespace std;
using namespace promise;

#define output_func_name() do{ printf("in function %s, line %d\n", __func__, __LINE__); } while(0)


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
    mutex action_mutex;
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
                action_mutex.lock();
                int minIndex = getMinReactor();
                function<void()> func = actions.at(0);
                actions.erase(actions.begin());
                cout << "action #" << actions.size() << " sent to thread " << minIndex << " (" << reactors[minIndex]->actions.size()  <<" length) " << endl;
                action_mutex.unlock();
                reactors[minIndex]->insertAction(func);
            }
        }
    }

    void insertAction(function<void()> action) {
        action_mutex.lock();
        this->actions.push_back(action);
        action_mutex.unlock();
    }
    EventLoop(int n) {
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

void test1() {
    output_func_name();
}

int test2() {
    output_func_name();
    return 5;
}

void test3(int n) {
    output_func_name();
    printf("n = %d\n", n);
}

Defer run(Defer &next){

    return newPromise([](Defer d){
        output_func_name();
        d.resolve(3, 5, 6);
    }).then([](const int &a, int b, int c) {
                printf("%d %d %d\n", a, b, c);
                output_func_name();
            }).then([](){
                output_func_name();
            }).then([&next](){
                output_func_name();
                next = newPromise([](Defer d) {
                    output_func_name();
                });
                //Will call next.resole() or next.reject() later
                return next;
            }).then([](int n, char c) {
                output_func_name();
                printf("n = %d, c = %c\n", (int)n, c);
            }).fail([](char n){
                output_func_name();
                printf("n = %d\n", (int)n);
            }).fail([](short n) {
                output_func_name();
                printf("n = %d\n", (int)n);
            }).fail([](int &n) {
                output_func_name();
                printf("n = %d\n", (int)n);
            }).fail([](const std::string &str) {
                output_func_name();
                printf("str = %s\n", str.c_str());
            }).fail([](uint64_t n) {
                output_func_name();
                printf("n = %d\n", (int)n);
            }).then(test1)
            .then(test2)
            .then(test3)
            .always([]() {
                output_func_name();
            });
}


/***
 *
 *
 *
 * @return
 */

int main ()
{
    Defer next;

    run(next);
    printf("======  after call run ======\n");

    next.resolve(123, 'a');
    //next.reject('c');


    auto el = new EventLoop( 5);
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