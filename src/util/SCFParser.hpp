//
// Created by marcfsr on 17/02/2019.
//

#ifndef NALP_SCFPARSER_H
#define NALP_SCFPARSER_H

#include <string>
#include <iostream>
#include <string>

#include "Files.hpp"
#include "AssociativeArray.hpp"

using namespace std;

namespace ipolitic {
    class SCFParser {
    public:
        // Inner memory is simply a vector of string array
        AssociativeArray<string> InnerMemory;

        void loadFrom(string path) {
        }

        void saveAs(string path) {

        }

        SCFParser() {
            cout << "Inside SCFParser " << endl;
            InnerMemory.AddItem("test", to_string(0.25));
            cout << "Inserted data : " << endl;
            cout << InnerMemory.operator[]("test") << endl;
            exit(0);
        }

    };

}
#endif //NALP_SCFPARSER_H
