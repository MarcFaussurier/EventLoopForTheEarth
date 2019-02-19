//
// Created by marcfsr on 18/02/2019.
//

#ifndef NALP_KEYWORDS_H
#define NALP_KEYWORDS_H

#include <vector>
#include <string>
#include <functional>
#include <regex>

#include "../AssociativeArray.hpp"

using namespace std;
using namespace ipolitic;

int MAX_KEYWORDS = 100;
int MAX_KEYWORDS_LENGTH = 255;

struct keyword {
    string name;
    string innerRegex;

    function<void( AssociativeArray<string> *, string*, string*)> innerAction;

    keyword(string name, string innerRegex, function<void( AssociativeArray<string> *, string*, string*)> innerAction) {
        this->name = name;
        this->innerRegex = innerRegex;
        this->innerAction = innerAction;
    }
};

static vector<keyword> keywords;

void load() {
    keywords.empty();

    // assignation keyword
    keywords.emplace_back(keyword("ASSIGNATION", "(\\s)|(:=)|(≔)", [](
            AssociativeArray<string> * innerMemPtr,
            string * leftSide,
            string * rightSide
            ) -> void {

    }));

    // group keyword
    /*keywords.emplace_back(keyword("ASSIGNATION", "(\\s)|(:=)|(≔)"));

    // string keyword
    keywords.emplace_back(keyword("ASSIGNATION", "(\\s)|(:=)|(≔)"));

    // action separator keyword
    keywords.emplace_back(keyword("ASSIGNATION", "(\\s)|(:=)|(≔)"));

    // division keyword
    keywords.emplace_back(keyword("ASSIGNATION", "(\\s)|(:=)|(≔)"));

    // multiplication keywword
    keywords.emplace_back(keyword("ASSIGNATION", "(\\s)|(:=)|(≔)"));

    // addition keyword

    // include keyword*/


}

void proceed(string  * buffer, string * leftSide, string * rightSide) {

    for (int i = 0; i < keywords.size(); i += 1) {
        cout << keywords.at(i).name << endl;
        cout << keywords.at(i).innerRegex << endl;

        if (std::regex_match (*buffer, std::regex(keywords.at(i).innerRegex) ))
            std::cout << "string literal matched : " << keywords.at(i).name << "\n";

    }
}

#endif //NALP_KEYWORDS_H
