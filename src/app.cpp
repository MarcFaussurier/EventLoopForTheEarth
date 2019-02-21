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

#include <SipYAML.hpp>

#include "event_engine/action.h"
#include "util/AssociativeArray.hpp"
#include "event_engine/Reactor.h"
#include "event_engine/bpromise.h"
#include "event_engine/EventLoop.h"
#include "http_server/web.h"
#include "util/Files.hpp"

using namespace std;
using namespace ipolitic;
using namespace bpromise;



const int CORE_THREADS_CNT = 8;
const int SUB_THREAD_CNT = 2 * CORE_THREADS_CNT;
const int SEND_TO_SUB_THREAD_AFTER_X_MS = 10;

const auto maxActiveReactorTime = std::chrono::milliseconds(SEND_TO_SUB_THREAD_AFTER_X_MS);    // after 10 ms, action will be sent to a secondary reactor thread

const int WAIT_TIME_BEFORE_EXIT = 5;

EventLoop * EVL;


void eventLoopHandler(int s){
    printf("Stopping server with code: %d\n",s);
    EVL->stop();
    cout << "Sleeping for " << WAIT_TIME_BEFORE_EXIT << " seconds before exit ... " << endl;
    sleep(WAIT_TIME_BEFORE_EXIT);
    exit(1);
}

void indent(size_t amount)
{
    for (size_t i = 0; i != amount; ++i)
    {
        cout << '\t';
    }
}

void showYAMLValue(Sip::YAMLDocumentUTF8::Node *node, size_t amount = 0)
{
    while (node)
    {
        indent(amount);
        cout << "Node: ";
        switch (node->type() & 0xF)
        {
            case Sip::Sequence:
                cout << "Sequence";
                break;
            case Sip::Mapping:
                cout << "Mapping";
                break;
            case Sip::Comment:
                cout << "Comment";
        }
        cout << endl;
        if (node->key())
        {
            indent(amount);
            cout << "  Key: \"";
            cout.write(node->key(), node->keySize());
            cout << "\"" << endl;
        }
        if (node->value())
        {
            indent(amount);
            cout << "  Value: \"";
            cout.write(node->value(), node->valueSize());
            cout << "\"" << endl;
        }
        if (node->firstChild())
        {
            showYAMLValue(node->firstChild(), amount + 1);
        }
        node = node->nextSibling();
    }
}

/***
 *
 *
 *
 * @return
 */

int main ()
{
    cout << "Fetching all scf files ... " << endl;
    vector<string> configFiles = glob("./../www/", ".*.yaml", true);
    cout << "OUTPUT : " << endl;
    for(unsigned long i = 0; i < configFiles.size(); i +=1) {
        cout << configFiles.at(i) << endl;


        string line;
        ifstream myfile (configFiles.at(i));
        if (myfile.is_open())
        {
            string sourceCodeContent;
            while ( getline (myfile,line) )
            {
                cout << line << '\n';
                sourceCodeContent += line;
                sourceCodeContent += '\n';
            }
            myfile.close();
            const char* source = sourceCodeContent.c_str();
            Sip::YAMLDocumentUTF8 doc1;
            doc1.parse(source);
            cout << "Iterating through keys: " << endl;
            showYAMLValue(doc1.firstChild());

        }
        else cout << "Unable to open file";
    }

    // registring shell signals
    struct sigaction sigIntHandler;
    sigIntHandler.sa_handler = eventLoopHandler;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;
    sigaction(SIGINT, &sigIntHandler, NULL);

    // run the event loop
    auto el = new EventLoop(CORE_THREADS_CNT);
    EVL = el;
    el->run();
    runServer(el);
    // exit if server stopped
    sleep(3);
    el->stop();
    return 0;
}