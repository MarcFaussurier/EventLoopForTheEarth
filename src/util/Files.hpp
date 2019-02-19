
//
// Created by marcfsr on 17/02/2019.
//

#ifndef NALP_GLOB_H
#define NALP_GLOB_H


#include <errno.h>
#include <vector>
#include <string>
#include <iostream>
#include <regex>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "String.hpp"

using namespace std;

namespace ipolitic {

    /**
     * Fetch all entities in a directory, or all entities that match filePattern if provided
     * @param dir       dir to scan
     * @param files
     * @param filePattern
     * @return
     */
    static inline vector<string> getdir (string dir, string filePattern = "")
    {
        vector<string> files;
        DIR *dp;
        struct dirent *dirp;
        if((dp  = opendir(dir.c_str())) == NULL) {
            cout << "Error(" << errno << ") opening " << dir << endl;
            exit(0);
        }
        while ((dirp = readdir(dp)) != NULL) {
            // we avoid . and .. folders
            if (((string) (dirp->d_name) != ".") && ((string) dirp->d_name != "..")) {
                string dirName = dir + string(dirp->d_name);
                // add trailing slash if directry
                if(dirp->d_type == DT_DIR) {
                    dirName += "/";
                    files.push_back(dirName);
                } else {
                    if (filePattern != "") {
                        if (std::regex_match (dirName, std::regex(dir + filePattern) ))
                            files.push_back(dirName);
                    } else {
                        files.push_back(dirName);
                    }
                }
            }
        }
        closedir(dp);
        return files;
    }

    /**
     * Fetch all files in a directory that match the given filePattern (returns vector of string)
     * @param directory
     * @param filePattern
     * @param recursively
     * @return
     */
    static inline  std::vector<std::string> glob(std::string directory, std::string filePattern, bool recursively = false) {
        vector<string> files = getdir(directory, filePattern);
        vector<string> output;
        for (unsigned int i = 0; i < files.size(); i++) {
          if (recursively) {
              // if got files is a directory, we proceed using recursivity
              if (stringEndsWith(files[i], '/')) {
                  cout << "Found ddirectory: " << files[i].c_str() << endl;
                  vector<string> dataToMerge = glob(files[i], filePattern, recursively);
                  for (unsigned long y = 0; y < dataToMerge.size(); y += 1) {
                      // avoid merging dirs
                      if (!stringEndsWith(dataToMerge.at(y), '/'))
                         output.push_back(dataToMerge.at(y));
                  }
              }
              // else we add the file to the output stack
              else {
                  output.push_back(files.at(i));
              }
          }
        }
        return output;
    }

}

#endif //NALP_GLOB_H
