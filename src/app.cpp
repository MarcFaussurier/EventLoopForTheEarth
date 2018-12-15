
#include <stdio.h>
#include <lua.hpp>
#include <lualib.h>
#include <lauxlib.h>

static void close_state(lua_State **L) { lua_close(*L); }
#define cleanup(x) __attribute__((cleanup(x)))
#define auto_lclose cleanup(close_state)

int main(int argc, char *argv[])
{
    auto_lclose lua_State *L = luaL_newstate();
    if (!L)
        return 1;
    luaL_openlibs(L);

    if (argc > 1) {
        luaL_loadfile(L, argv[1]);
        int ret = lua_pcall(L, 0, 0, 0);
        if (ret != 0) {
            fprintf(stderr, "%s\n", lua_tostring(L, -1));
            return 1;
        }
    }

    lua_getglobal(L, "address");
    lua_getglobal(L, "port");
    printf("address: %s, port: %ld\n",
           lua_tostring(L, -2), lua_tointeger(L, -1));
    lua_settop(L, 0);
    return 0;
}



/*
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
    cout << "Hello world" << endl;
}*/