//
// Created by marcfsr on 02/02/2019.
//

#ifndef NALP_IDKPARSER_H
#define NALP_IDKPARSER_H

#include <string>
#include <iostream>
#include <fstream>

#include "AssociativeArray.hpp"
#include "../event_engine/action.h"

#include "trim.h"

using namespace std;

namespace ipolitic {
    class IDKParser {
        const string cacheFileName = "actions-data.idk";
    public:
        fstream IDKfstream;

        struct actions_data_item {
            float avg = 0;
            int count = 0;
        };

        struct actions_data {
            actions_data_item A;
            actions_data_item B;
            actions_data_item C;
            actions_data_item D;
        };

        /**
        * Save the associative array data to idk file
        * @param data
        */
        void save(AssociativeArray<actions_data> data) {
            cout << "IN SAVE __________" << endl;
            string output = "";
            long data_length = data.Size();
            int nb_space_separator = 15;
            for (long i = 0; i < data_length; i += 1) {
                string current_key = data.GetItemName(i);
                output += current_key + ":\n";
                cout << current_key << endl;
                actions_data mem = data.operator[](i);
                for (long y = 0; y <= 3; y += 1) {
                    char currentChar = (char) ('A' + y);
                    cout << "char : " << currentChar << endl;
                    actions_data_item gs;
                    switch (currentChar) {
                        case 'A':
                            gs = mem.A;
                            break;
                        case 'B':
                            gs = mem.B;
                            break;
                        case 'C':
                            gs = mem.C;
                            break;
                        case 'D':
                            gs = mem.D;
                            break;
                        default:
                            break;
                    }
                    string avg  = to_string(gs.avg);
                    string count   = to_string(gs.count);
                    unsigned long x = 0;
                    output +=
                            "\t" + avg + std::string( (x = nb_space_separator - avg.length()) > 0 ? x : 1, ' ')
                            + "=> " + count + "\n";

                    cout << "\t" + avg + std::string( (x = nb_space_separator - avg.length()) > 0 ? x : 1, ' ')
                            + "=> " + count + "\n";
                }
            }
            std::ofstream out(IDKParser::cacheFileName);
            out << output;
            out.close();

            cout << "ENDOF SAVE __________" << endl;
        }


        /**
         * Will create the idk file if it not already exists
         */
        void initHistory() {
            ofstream outputFile;
            outputFile.open(IDKParser::cacheFileName, ios::app);
        }

        /**
         * fetch the idk file as associative array
         * @return
         */
        AssociativeArray<actions_data> loadFromFile() {
            this->initHistory();
            AssociativeArray<actions_data> output;
            string line;
            this->IDKfstream = fstream(IDKParser::cacheFileName);
            AssociativeArray<vec_action_stats> gotHistory;
            vec_action_stats currentItem;
            bool isFirstAItem = true;
            if (this->IDKfstream.is_open()) {
                actions_data currentInnerMemory = actions_data{};
                string currentKey;
                int y = 0;
                char currentChar = (char) ('A');
                for (; getline(this->IDKfstream, line); y=y+1) {
                    string bline(line);
                    std::size_t foundOne = bline.find(':');
                    std::size_t foundTwo = bline.find("=>");
                    if (foundOne != string::npos && foundTwo == string::npos) {
                        char *linePtr = (char *) line.c_str();
                        int gotKeySize = strlen(linePtr);
                        char actionName[gotKeySize];
                        int i = 0;
                        for (; i < (gotKeySize - 1); i++) {
                            actionName[i] = *(linePtr + i);
                        }
                        actionName[gotKeySize - 1] = '\0';
                        cout << "ONE ITEM : " << actionName << endl;
                        currentItem = vec_action_stats();
                        if (!currentKey.empty()) {
                            output.AddItem(currentKey, currentInnerMemory);
                            currentInnerMemory = actions_data{};
                        }
                        currentKey = actionName;
                        continue;
                    } else if (foundOne == string::npos && foundTwo != string::npos) {
                        if ((currentChar >= 'D')|| isFirstAItem ) {
                            currentChar = 'A';
                            isFirstAItem = false;
                        } else {
                            currentChar = (char) (currentChar + 1);
                        }
                    }
                    string leftSide = "", rightSide = "";
                    bool currentIsLeft = true;
                    int bSize = bline.length();
                    char lastTwoChars[2] = {'0', '0'};
                    for (int i = 0; i < bSize; i += 1) {
                        lastTwoChars[0] = lastTwoChars[1];
                        lastTwoChars[1] = line[i];
                        if (lastTwoChars[0] == '=' && lastTwoChars[1] == '>') {
                            leftSide = leftSide.substr(0, leftSide.length() - 2);
                            currentIsLeft = false;
                            continue;
                        }
                        if (currentIsLeft) {
                            leftSide += line[i];
                        } else {
                            rightSide += line[i];
                        }
                    }
                    trim(leftSide);
                    actions_data_item currentGrpStats = actions_data_item{};
                    currentGrpStats.avg = atof(leftSide.c_str());
                    currentGrpStats.count = atoi(rightSide.c_str());;
                    switch(currentChar) {
                        case 'A':
                            currentInnerMemory.A = currentGrpStats;
                            break;
                        case 'B':
                            currentInnerMemory.B = currentGrpStats;
                            break;
                        case 'C':
                            currentInnerMemory.C = currentGrpStats;
                            break;
                        default:
                            currentInnerMemory.D = currentGrpStats;
                            break;
                    }
                  //  cout << "left : " << leftSide << " right : " << rightSide << "current char : " << currentChar<< endl;
                }
                if (!currentKey.empty()) {
                    output.AddItem(currentKey, currentInnerMemory);
                }
                this->IDKfstream.close();
                cout << "ENDOFFROMFILE" << endl;
                return output;
            }
            cout << "Unable to open file" << endl;
            return output;


        }
    };
}


#endif //NALP_IDKPARSER_H
