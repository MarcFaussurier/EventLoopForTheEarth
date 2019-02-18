//
// Created by marcfsr on 03/02/2019.
//

#ifndef NALP_TRIM_H
#define NALP_TRIM_H

#include <algorithm>
#include <cctype>
#include <locale>

using namespace std;

namespace ipolitic {
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
    static inline void ltrim(std::string &s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
            return !std::isspace(ch);
        }));
    }

    /**
     * Will trim right side of the given string
     * @param s     Input string
     */
    static inline void rtrim(std::string &s) {
        s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
            return !std::isspace(ch);
        }).base(), s.end());
    }

    /**
     * Will trim both sides of the given string
     * @param s     Input string
     */
    static inline void trim(std::string &s) {
        ltrim(s);
        rtrim(s);
    }

    /**
     *  Will trim left side of the given string (will copy input)
     * @param s     Input string
     */
    static inline std::string ltrim_copy(std::string s) {
        ltrim(s);
        return s;
    }

    /**
     *  Will trim right side of the given string (will copy input)
     * @param s     Input string
     */
    static inline std::string rtrim_copy(std::string s) {
        rtrim(s);
        return s;
    }

    /**
     *  Will trim both sides of the given string (will copy input)
     * @param s     Input string
     */
    static inline std::string trim_copy(std::string s) {
        trim(s);
        return s;
    }

}

#endif //NALP_TRIM_H
