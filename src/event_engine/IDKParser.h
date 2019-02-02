//
// Created by marcfsr on 02/02/2019.
//

#ifndef NALP_IDKPARSER_H
#define NALP_IDKPARSER_H

#include <string>
#include <iostream>

#include "EventLoop.h"


using namespace std;

namespace ipolitic {
    class IDKParser {
        const string cacheFileName = "actions-data.idk";
    public:
        fstream IDKfstream;

        void initHistory() {
            ofstream outputFile;
            outputFile.open(IDKParser::cacheFileName, ios::app);
            this->loadFromFile();
        }

        void insert(string key, int left, int right) {

        }

        void loadFromFile() {
            string line;
            this->IDKfstream = fstream(IDKParser::cacheFileName);
            AssociativeArray<vec_action_stats> gotHistory;
            vec_action_stats currentItem;
            if (this->IDKfstream.is_open())
            {
                while ( getline (this->IDKfstream,line) )
                {
                    string bline(line);

                    std::size_t foundOne = bline.find(':');
                    std::size_t foundTwo = bline.find("=>");

                    if (foundOne != string::npos && foundTwo == string::npos) {
                        char * linePtr = (char*) line.c_str();
                        int gotKeySize = strlen(linePtr);
                        char actionName[gotKeySize];
                        int i = 0;
                        for(; i < (gotKeySize - 1);i++) {
                            actionName[i] = *(linePtr + i);
                        }
                        actionName[gotKeySize - 1] = '\0';
                        cout << "ONE ITEM : "  << actionName << endl ;
                        currentItem = vec_action_stats();
                        continue;
                    }
                    string leftSide = "";
                    string rightSide = "";

                    bool currentIsLeft = true;
                    int bSize = bline.length();

                    char lastTwoChars[2] = {'0','0'};

                    for (int i = 0; i < bSize; i += 1) {
                        lastTwoChars[0] =  lastTwoChars[1];
                        lastTwoChars[1] = line[i];

                        if (lastTwoChars[0] == '=' && lastTwoChars[1] == '>') {
                            leftSide = leftSide.substr(0, leftSide.length() - 2);
                            cout << "CHANGED§!!!!!" << endl;
                            currentIsLeft = false;
                            continue;
                        }

                        if (currentIsLeft) {
                            leftSide += line[i];
                        } else {
                            rightSide += line[i];
                        }
                    }
                    cout << "left : " << leftSide << " right : " << rightSide << endl;
                    //cout << line << '\n';
                }
                this->IDKfstream.close();
            }

            else cout << "Unable to open file";

        }
    };
}


#endif //NALP_IDKPARSER_H
