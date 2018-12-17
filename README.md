![C with minimalism logo](https://github.com/MarcFaussurier/CWithMinimalism/raw/master/images/CWM.png)

Minimalism is art. So, coding with minimalism in mind makes you an artist.
______________________________

The goal of this repository is to provide the easiest sample about how to use C(++) ecosystems in a portable environement with  still productivity in mind. The setup is about 3 shells commands only (see Get started bellow). Happy coding !

## Get started
`git clone https://github.com/MarcFaussurier/cmake-lua-shell-boilerplate.git yourproject`

`cd yourproject`

`./deps && ./make && ./run` (next time only use make and/or run)

The base application (sources files are [/script.lua](https://github.com/MarcFaussurier/cmake-lua-shell-boilerplate/blob/master/script.lua)
 and [/src/app.cpp](https://github.com/MarcFaussurier/cmake-lua-shell-boilerplate/blob/master/src/app.cpp)) should output :

`The table the script received has:
1	2
2	4
3	6
4	8
5	10
Returning data back to C
Script returned: 30`

Congratulation you successfully ran your first lua script from C++ using luajit  ! 

## Features 

- **Tiny GIT-based dependencies manager written in bash** (See ./deps file for sample, when adding a lib you'll also have to
specify its folder in CMakeLists.txt)

-  **Async functions for the shell** (see ./async) so that you can quickly write nice bash scripts

- **Source code watcher written in bash** using inotifywait instead of a weird stuff. It will recompile your software and run it at each source code change 

- **TDD-Compliant test watcher written in bash** using still inotifywait it will recompile your tests and will display a green or red icon (using notify-send) according your tests results to your desktop.

- **Read-to-use LuaJIT** provided with a sample cpp and lua file 

- Easy to understand & CLion compatible, **CMAKE build script** 

- Bundled with ready to use minimalists **libraries**, such as [rang](https://github.com/agauniyal/rang) for beautiful terminal output, [catch2](https://github.com/catchorg/Catch2.git) for intuitive application testing,  [cpp-dotenv](https://github.com/adeharo9/cpp-dotenv) because the .env file extension follow our vision of programming, [picojson](https://github.com/kazuho/picojson.git) as in a web-2.0 world, jsons are everywhere and [eyalz800/serializer](https://github.com/eyalz800/serializer) as everyone may need serialisation.

## Project commands (pure bash)

- `./deps` Install or update your project dependencies 
- `./make` Compile your software
- `./run` Run your software
- `./run-tests` Run all your tests
- `./clean` Clean your repository (tests, CMake files and all dependencies) so that you can quickly release / version it
- `./watch` Will wait for source code change for compiling and running your application.
- `./watch-test` Will wait for source code change for compiling and running your tests. It will then send you a desktop notification with a green or red icon according your tests results at each time.




 
  



