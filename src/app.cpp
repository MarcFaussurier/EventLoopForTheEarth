// promise example
#include <iostream>       // cout
#include <functional>     // ref
#include <thread>         // thread
#include <any>
#include <string>
#include <chrono>
#include <unistd.h>
#include <mutex>
#include <bits/stdc++.h>
#include <promise.hpp>

#include "event_engine/action.h"
#include "util/AssociativeArray.hpp"
#include "event_engine/Reactor.h"
#include "event_engine/EventLoop.h"
#include "event_engine/bpromise.h"

using namespace std;
using namespace ipolitic;
using namespace bpromise;



const int CORE_THREADS_CNT = 4;
const int SUB_THREAD_CNT = 2 * CORE_THREADS_CNT;
const int SEND_TO_SUB_THREAD_AFTER_X_MS = 10;

const auto maxActiveReactorTime = std::chrono::milliseconds(SEND_TO_SUB_THREAD_AFTER_X_MS);    // after 10 ms, action will be sent to a secondary reactor thread


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
    auto el = new EventLoop(CORE_THREADS_CNT);
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

    AssociativeArray<vec_action_stats> cpy = el->getAssocArrCpy();
    long amountOfActionName = cpy.Size();
    for (long i = 0 ; i < amountOfActionName; i += 1) {
        cout << "i="<<i<<" name="<<cpy.GetItemName(i) << " times="<<cpy.operator[](i).executionTimes.size() << endl;
    }

    cout << "Stopping ... " << endl;
    el->stop();
    cout << "Shutdown" << endl;
    return 0;
}