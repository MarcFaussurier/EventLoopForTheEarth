//
// Created by marcfsr on 2019-02-07.
//

#include "./../../src/tests_header.h"

using namespace std;
using namespace ipolitic;
using namespace bpromise;

const int CORE_THREADS_CNT = 8;
const int SUB_THREAD_CNT = 2 * CORE_THREADS_CNT;
const int SEND_TO_SUB_THREAD_AFTER_X_MS = 10;

const auto maxActiveReactorTime = std::chrono::milliseconds(SEND_TO_SUB_THREAD_AFTER_X_MS);    // after 10 ms, action will be sent to a secondary reactor thread


// will test for both idkparser and prohiler updates
TEST_CASE("PROPHILER_TEST", "[report]")
{
        IDKParser idk;
        AssociativeArray<IDKParser::actions_data> DATA = idk.loadFromFile();
        const int taskCount = 10;
        int oldCount = 0, newCount = 0;
        cout << "[[BEFORE RUN " << DATA.operator[]("someTest").A.avg << "]]";
        oldCount = DATA.operator[]("someTest__").A.count;
        run();
        auto int_ms = std::chrono::duration_cast<std::chrono::milliseconds>(maxActiveReactorTime);
        auto el = new EventLoop(CORE_THREADS_CNT);

        el->run();

        for (int i = 0; i < taskCount; i += 1) {
            Defer nextTest = bNewPromise(el,"someTest"+to_string(i), [el](Defer d) -> void {
                d.resolve();
            }).then([el]() -> Defer {
                return bNewPromise(el,"someTest__", [](Defer d) -> void {
                    d.resolve();
                });
            }).then([]() -> void {
            });
        }
        usleep(500000);

        el->stop();

    usleep(500000);

    AssociativeArray<IDKParser::actions_data> datat = idk.loadFromFile();

    cout << "[[AFTER RUN " << datat.operator[]("someTest__").A.avg << "]]";
    cout << "[[AFTER RUN " << datat.operator[]("someTest__").A.count << "]]";
    newCount = datat.operator[]("someTest__").A.count;
    REQUIRE((newCount - oldCount) == taskCount);
}
