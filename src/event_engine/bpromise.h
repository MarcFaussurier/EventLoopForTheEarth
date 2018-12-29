//
// Created by marc on 29/12/18.
//

#ifndef NALP_BPROMISE_H
#define NALP_BPROMISE_H

#include <promise.hpp>

#include "EventLoop.h"

using namespace std;
namespace ipolitic {
    using namespace bpromise;
/* Create new promise object */
    template <typename FUNC>
    inline Defer bNewPromise(EventLoop * el, string UID, FUNC func) {
        Defer promise = newHeadPromise();
        el->insertAction(UID, [promise, func]() -> void {
            promise->run(func, promise);
        });
        return promise;
    }

}
#endif //NALP_BPROMISE_H
