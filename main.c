/************************************************/
//
// MINILUA CE port by Adrien "Adriweb" Bertrand
// Original code from LuaJIT: https://github.com/LuaJIT/LuaJIT/blob/master/src/host/minilua.c
// Available on https://tiplanet.org/pb/?id=1381_1509053799_09996e41e3 and on GitHub
//
// Many thanks to Matt "MateoConLechuga" Waltz and Jacob "Jacobly" Young
//
// This is an extremely alpha version, just to demonstrate that *some* things are working as expected
// Be advised that some features are broken, partly because of ZDS being, well... crappy.
// But things are looking nicer and nicer everyday with jacobly's eZ80-LLVM: https://github.com/jacobly0/llvm-z80/
//
// The "console" IO (nio_ce files) is for convenience. See https://github.com/adriweb/nspire-io
//
// Build note: considering this gets built as more than 65k (the theoretical max for a program), you have to use
// the latest version of ConvHex so that it gets split into a launcher+appvars automatically.
// Then, transfer everything to Archive, launching from there (and as such, OS 5.3 is recommended, if not required).
// Free up as much RAM as you can :)
//
// Usage example: https://i.imgur.com/7XG22s9.png
//
/************************************************/


/******************************************************************************
/* This is a heavily customized and minimized copy of Lua 5.1.5. */
/* It's only used to build LuaJIT. It does NOT have all standard functions! */
/******************************************************************************
* Copyright (C) 1994-2012 Lua.org, PUC-Rio.  All rights reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining
* a copy of this software and associated documentation files (the
* "Software"), to deal in the Software without restriction, including
* without limitation the rights to use, copy, modify, merge, publish,
* distribute, sublicense, and/or sell copies of the Software, and to
* permit persons to whom the Software is furnished to do so, subject to
* the following conditions:
*
* The above copyright notice and this permission notice shall be
* included in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
******************************************************************************/

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <tice.h>

#include "nio_ce.h"

#undef NDEBUG
#include <assert.h>
#include <debug.h>

#include "minilua.h"


static char text[401] = {0};


void main(void)
{
    nio_console c1;

    bool declared = false;

    lua_State* L = luaL_newstate();

    nio_init(&c1, NIO_MAX_COLS, NIO_MAX_ROWS, 0, 0, NIO_COLOR_BLACK, NIO_COLOR_WHITE, true);

    nio_set_default(&c1);
    nio_printf("MINILUA built on %s, %s\n\n", __DATE__, __TIME__);

    luaL_openlibs(L); // Load Lua libraries

    while(1)
    {
        nio_printf("> ");

        // If no text was entered, exit
        if(!nio_getsn(text, 400))
            continue;

        if (!strcmp(text, "clear")) {
            nio_clear(&c1);
        }
        else if (!strcmp(text, "exit")) {
            break;
        }
        else if (!strcmp(text, "undef")) {
            if (declared) {
                lua_pushnil(L);
                lua_setglobal(L, "func");
                declared = false;
            } else {
                nio_printf("Function 'func(n) ... end' was not declared\n\n");
            }
        }
        else if (strstr(text, "def ") == text) { // for now, let the user declare a func(n) function, returning an int
            //luaL_loadbuffer(L, c_function, strlen(c_function), c_function);
            luaL_loadbuffer(L, text+4, strlen(text+4), text+4);
            if (lua_pcall(L,0,0,0) == 0) {
                declared = true;
                nio_printf("Function 'func(n) ... end' declared OK\n\n");
            } else {
                declared = false;
                nio_printf("Function could not get declared\n\n");
            }
        }
        else if (strstr(text, "exec ") == text)
        {

            if (!declared) {
                nio_printf("Function 'func(n) ... end' is not declared\n\n");
            } else {
                int n = atoi(text+5);
                int r = -1;
                dbg_sprintf((char*)dbgout, "calling func(%d)...\n", n);
                lua_getfield(L, -10002 /* LUA_GLOBALSINDEX */, "func");
                lua_pushinteger(L, n);
                lua_call(L,1,1); // call func, with one argument, and expect one result
                dbg_sprintf((char*)dbgout, "     after lua_call\n");
                r = luaL_checkinteger(L, -1); // the top of the Lua stack should contain our result
                nio_printf("Function result: %d\n\n", r);
            }
        } else {
            nio_printf("Unknown command\n\n");
        }
    }

    lua_close(L);

    nio_free(&c1);

    return;
}
