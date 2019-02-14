/*
 * test.c
 * Example of a C program that interfaces with Lua.
 * Based on Lua 5.0 code by Pedro Martelletto in November, 2003.
 * Updated to Lua 5.1. David Manura, January 2007.
 * @see http://lua-users.org/wiki/SimpleLuaApiExample
 */

#ifndef NALP_LUAL
#define NALP_LUAL

#include <lua.hpp>
#include <lualib.h>
#include <lauxlib.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <thread>
using namespace std;

class LuaManager {
public:
    lua_State * L;

    /* call a function `f' defined in Lua */
    double f (double x, double y) {
        double z;

        /* push functions and arguments */
        lua_getglobal(L, "f");  /* function to be called */
        lua_pushnumber(L, x);   /* push 1st argument */
        lua_pushnumber(L, y);   /* push 2nd argument */

        /* do the call (2 arguments, 1 result) */
        if (lua_pcall(L, 2, 1, 0) != 0)
            printf("error running function `f': %s", lua_tostring(L, -1));

        /* retrieve result */
        if (!lua_isnumber(L, -1))
            printf("function `f' must return a number");
        z = lua_tonumber(L, -1);
        lua_pop(L, 1);  /* pop returned value */
        return z;
    }

    LuaManager() {
        int status, result, i;
        double sum;

        /*
         * All Lua contexts are held in this structure. We work with it almost
         * all the time.
         */
        L = luaL_newstate();

        luaL_openlibs(L); /* Load Lua libraries */

        /* Load the file containing the script we are going to run */
        status = luaL_loadfile(L, "./../script.lua");
        if (status) {
            /* If something went wrong, error message is at the top of */
            /* the stack */
            fprintf(stderr, "Couldn't load file: %s\n", lua_tostring(L, -1));
            exit(1);
        }

        cout << "Waiting for lua response .... " << endl;

        /* Ask Lua to run our little script */
        result = lua_pcall(L, 0, LUA_MULTRET, 0);
        if (result) {
            fprintf(stderr, "Failed to run script: %s\n", lua_tostring(L, -1));
            exit(1);
        }

        /* Get the returned value at the top of the stack (index -1) */
        //sum = lua_tonumber(L, -1);

        //printf("Script returned: %.0f\n", sum);

        lua_pop(L, 1);  /* Take the returned value out of the stack */
    }

    void close() {
        lua_close(L);   /* Cya, Lua */
    }
};

#endif