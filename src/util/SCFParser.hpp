//
// Created by marcfsr on 17/02/2019.
//

#ifndef NALP_SCFPARSER_H
#define NALP_SCFPARSER_H

#include <string>
#include <iostream>
#include <string>

#include <fstream>
#include <regex>
#include "Files.hpp"
#include "String.hpp"
#include "AssociativeArray.hpp"
#include "scf_engine/keywords.h"
using namespace std;

namespace ipolitic {
    class SCFParser {
    public:
        // Inner memory is simply a vector of string array
        AssociativeArray<string> InnerMemory;

        void loadFrom(string path) {
            cout << "Loading scf file using path : " << path << endl;

            string buffer;
            ifstream f(path);
            f.seekg(0, ios::end);
            buffer.resize(f.tellg());
            f.seekg(0);
            f.read(buffer.data(), buffer.size());

            cout << "buffer : " << buffer << endl;

            vector<string> exploded = explode(buffer, "\n");

            bool displaySRC = true;
            for (int i = 0; i < exploded.size(); i += 1) {

                proceed(exploded.at(i),"","");


                if (displaySRC) {
                    cout << i << ": " << exploded.at(i) << endl;
                }
                string currentSourceCodeLine                = exploded.at(i);
                const char * currentSourceCodeLineCSTR      = currentSourceCodeLine.c_str();
                unsigned long currentSourceCodeLineLength   = currentSourceCodeLine.size();

                unsigned long currentCharPos = 0;

                string currentBuffer = "";

                for(;currentCharPos < currentSourceCodeLineLength; currentCharPos += 1) {
                    currentBuffer += currentSourceCodeLine.at(currentCharPos);


                }




            }
            cout << "Number of lines : " << exploded.size() << endl;

        }

        void saveAs(string path) {

        }

        SCFParser() {
            cout << "Inside SCFParser " << endl;
            InnerMemory.AddItem("test", to_string(0.25));
            cout << "Inserted data : " << endl;
            cout << InnerMemory.operator[]("test") << endl;
        }

    };

}
#endif //NALP_SCFPARSER_H
