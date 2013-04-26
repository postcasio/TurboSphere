
#include <gl/gl.h>
#include "graphic.h"
#include "../../configmanager/opengame.h"
#define PLUGINNAME "SDL_GL"
#include "../common/plugin.h"
#include "../common/graphic_algorithm.h"
#include <cmath>
//POS standard libraries!
#ifndef UINT16_MAX
    #define UINT16_MAX 0xffff
#endif

#ifndef INT16_MAX
    #define INT16_MAX 0x7fff
#endif

#ifndef INT16_MIN
    #define INT16_MIN -0x7ffe
#endif

#define NUMFUNCS 21
#define NUMVARS  0

#ifndef _WIN32
    //this is a stop-gap until V8 has the full API for aligned pointers to internal fields. Probably will not be necessary in V8 v3.21
    //#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif

extern SDL_Rect cmpltscreen;

extern SDL_Event event;
extern SDL_Surface *screen;