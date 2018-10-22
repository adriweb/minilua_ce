# minilua_ce

A (PoC of a) port of LuaJIT's MiniLua for the CE TI calculators.

MINILUA CE port by Adrien "Adriweb" Bertrand  
Original code from LuaJIT: https://github.com/LuaJIT/LuaJIT/blob/master/src/host/minilua.c  
Available on https://tiplanet.org/pb/?id=1381_1509053799_09996e41e3 and on GitHub.

Many thanks to Matt "MateoConLechuga" Waltz and Jacob "Jacobly" Young.

This is an extremely alpha version, just to demonstrate that *some* things are working as expected.  
Be advised that some features are broken, partly because of ZDS being, well... crappy.  
But things are looking nicer and nicer everyday with jacobly's eZ80-LLVM: https://github.com/jacobly0/llvm-z80/

The "console" IO (nio_ce files) is for convenience. See https://github.com/adriweb/nspire-io  

Build note: considering this gets built as more than 65k (the theoretical max for a program), you have to use the latest version of ConvHex so that it gets split into a launcher+appvars automatically.  
Then, transfer everything to Archive, launching from there (and as such, OS 5.3 is recommended, if not required).  
Free up as much RAM as you can :)

Usage example:  
![](https://i.imgur.com/7XG22s9.png)


TI-Planet topic: https://tiplanet.org/forum/viewtopic.php?f=10&t=20642

