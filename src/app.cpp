// promise example
#include <iostream>       // cout
#include <functional>     // ref
#include <thread>         // thread
#include <any>
#include <string>
#include <chrono>
#include <unistd.h>
#include <mutex>
// #include <bits/stdc++.h>
#include <promise.hpp>

#include "event_engine/action.h"
#include "util/AssociativeArray.hpp"
#include "event_engine/Reactor.h"
#include "event_engine/bpromise.h"
#include "event_engine/EventLoop.h"

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
    el->initHistory();
    el->run();

    for (int i = 0; i < 10; i += 1) {
        Defer nextTest = bNewPromise(el,"someTest", [el](Defer d) -> void {
            cout << "Promise before resolve" << endl;
            d.resolve();
        }).then([el]() -> Defer {
            cout << "After resolve" << endl;
            return bNewPromise(el,"someTest2", [](Defer d) -> void {
                cout << "Promise before resolve2" << endl;
                d.resolve();
            });
        }).then([]() -> void {
            cout << "resolved2" << endl;
        });
    }

    sleep(2);

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