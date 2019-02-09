//
// Created by marcfsr on 2019-02-07.
//Ò


#ifndef EXAMPLE_H
#define EXAMPLE_H
    #define CATCH_CONFIG_MAIN
    #include "./../libs/catch2/single_include/catch2/catch.hpp"

    // promise example
    #include "./luatest.cpp"

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
    #include "util/IDKParser.hpp"
#endif