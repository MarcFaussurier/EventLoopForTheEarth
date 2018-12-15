# C++ Linux boiler plate featuring ready-to-use useful bash scripts and libs.

This boiler plate is meant to be use with 'header-only' libs only and traditional CMAKE libs so that it is really easy to add / update all your dependencies using GIT only.

## Features : 
- GIT-based dependencies manager written in bash (See ./deps file for sample, when adding a lib you'll also have to
specify its folder in CMakeLists.txt)

-  **Async functions for the shell** (see ./async) so that you can quickly write nice bash scripts

- **Source code watcher written in bash** using inotifywait instead of a weird stuff. It will recompile your software and run it at each source code change 

- **TDD-Compliant test watcher written in bash** using still inotifywait it will recompile your tests and will display a green or red icon (using notify-send) according your tests results to your desktop.

- **Read-to-use LuaJIT** provided with a sample cpp and lua file 

- Easy to understand **CMAKE build script**

- Ready to use **header-only libraries**, rang for terminal output, a serializer, a json tool, a yaml tool, a test tool, and even a .env file. 

## List of commands :

- `./deps` Install or update your project dependencies 
- `./make` Compile your software
- `./run` Run your software
- `./run-tests` Run all your tests
- `./clean` Clean your repository (tests, CMake files and all dependencies) so that you can quickly release / version it
- `./watch` Will wait for source code change for compiling and running your application.
- `./watch-test` Will wait for source code change for compiling and running your tests. It will then send you a desktop notification with a green or red icon according your tests results at each time.




 
  



