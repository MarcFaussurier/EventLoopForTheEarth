//
// Created by marcfsr on 03/02/2019.
//

#ifndef NALP_TRIM_H
#define NALP_TRIM_H

#include <algorithm>
#include <cctype>
#include <locale>
#include <utility>
#include <sstream>
#include <vector>
#include <string>

#include <iostream>

using namespace std;

namespace ipolitic {
    /**
     * Will explode the given inputString using the given delimiter and return a vector a string
     * Act similar as PHP's explode()
     * @see http://php.net/manual/fr/function.explode.php
     * @param inputString
     * @param delimiter
     * @return
     */
    static inline vector<string> explode(string const & inputString, string const & delimiter)
    {
        unsigned long inputLen          = inputString.size();
        unsigned long delimiterSize     = delimiter.size();
        const char * delimiterCSTR      = delimiter.c_str();
        const char * inputCSTR          = inputString.c_str();
        string previousInputSinceLastClean   = "";

        char delimiterBuffer[delimiterSize];
        vector<string> output;

        // for each input characters
        for (int i = 0; i < inputLen; i += 1) {
            // move buffer for making one character free to the end
            for (int y = 1; y < (delimiterSize - 1) ; y += 1) {
                delimiterBuffer[y-1] = delimiterBuffer[y];
            }
            // add current input character to the buffer, and then trailing null
            delimiterBuffer[delimiterSize - 1] = inputCSTR[i];
            delimiterBuffer[delimiterSize]     = '\0';

            // add current character to the element buffer
            previousInputSinceLastClean += inputString.at(i);
            if (((int) *delimiterBuffer) == ((int) *delimiterCSTR)) {
                cout << "LINE BREAK" << endl;
                output.push_back(previousInputSinceLastClean.substr(0, previousInputSinceLastClean.size() - delimiter.size()));
                previousInputSinceLastClean = "";
            }
        }
        return output;
    }
    
    /**
    * Check for last string character
    * @param str    Input string
    * @param chr    Character to match
    * @return
    */
    static inline bool stringEndsWith(string str, char chr) {
        return str.c_str()[str.size() - 1] == chr;
    }

    /**
     *  Will trim left side of the given string
     * @param s     Input string
     */
    static inline void ltrim(string &s) {
        s.erase(s.begin(),find_if(s.begin(), s.end(), [](int ch) {
            return !isspace(ch);
        }));
    }

    /**
     * Will trim right side of the given string
     * @param s     Input string
     */
    static inline void rtrim(string &s) {
        s.erase(find_if(s.rbegin(), s.rend(), [](int ch) {
            return !isspace(ch);
        }).base(), s.end());
    }

    /**
     * Will trim both sides of the given string
     * @param s     Input string
     */
    static inline void trim(string &s) {
        ltrim(s);
        rtrim(s);
    }

    /**
     *  Will trim left side of the given string (will copy input)
     * @param s     Input string
     */
    static inline string ltrim_copy(string s) {
        ltrim(s);
        return s;
    }

    /**
     *  Will trim right side of the given string (will copy input)
     * @param s     Input string
     */
    static inline string rtrim_copy(string s) {
        rtrim(s);
        return s;
    }

    /**
     *  Will trim both sides of the given string (will copy input)
     * @param s     Input string
     */
    static inline string trim_copy(string s) {
        trim(s);
        return s;
    }

}

#endif //NALP_TRIM_H
