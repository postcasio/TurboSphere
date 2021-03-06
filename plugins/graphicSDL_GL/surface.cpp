#include "main.h"
#include "surface.h"
#include "color.h"

#include <assert.h>

DECLARE_OBJECT_TEMPLATES(Surface);

void SurfaceInit(void){
    INIT_OBJECT_TEMPLATES(Surface);
    SET_CLASS_NAME(Surface, "Surface");

    ADD_TO_PROTO(Surface, "blit",           SurfaceBlit);
    ADD_TO_PROTO(Surface, "createImage",    SurfaceToImage);
    ADD_TO_PROTO(Surface, "cloneSection",   SurfaceCloneSection);

    ADD_TO_PROTO(Surface, "save",           SurfaceSave);

    ADD_TO_PROTO(Surface, "setAlpha",       SurfaceSetAlpha);
    ADD_TO_PROTO(Surface, "getClippingRectangle", SurfaceGetClippingRectangle);
    ADD_TO_PROTO(Surface, "setClippingRectangle", SurfaceSetClippingRectangle);

    ADD_TO_PROTO(Surface, "rectangle",      SurfaceRectangle);

    SET_CLASS_ACCESSOR(Surface, "width",    SurfaceGetWidth,    SurfaceSetWidth);
    SET_CLASS_ACCESSOR(Surface, "height",   SurfaceGetHeight,   SurfaceSetHeight);

}

void SurfaceClose(void){

}

v8Function SurfaceGetWidth(V8GETTERARGS) {
    int value = GET_ACCESSOR_SELF(SDL_Surface*)->w;
    return v8::Integer::New(value);
}

void SurfaceSetWidth(V8SETTERARGS) {
//    GET_SELF(SDL_Surface*)->w = value->Int32Value();
}

v8Function SurfaceGetHeight(V8GETTERARGS) {
    int value = GET_ACCESSOR_SELF(SDL_Surface*)->h;
    return v8::Integer::New(value);
}

void SurfaceSetHeight(V8SETTERARGS) {
//    GET_SELF(SDL_Surface*)->h = value->Int32Value();
}

void TS_BlitTexture(TS_Texture texture, int x[4], int y[4]){

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);

    glBegin(GL_QUADS);
        GLColor(fullmask);
        glTexCoord2i(0, 0);
        glVertex2i(x[0], y[0]);
        glTexCoord2i(1, 0);
        glVertex2i(x[1], y[1]);
        glTexCoord2i(1, 1);
        glVertex2i(x[2], y[2]);
        glTexCoord2i(0, 1);
        glVertex2i(x[3], y[3]);
    glEnd();
    glDisable(GL_TEXTURE_2D);

    glDeleteTextures(1, &texture);

}

TS_Texture TS_SurfaceToTexture(SDL_Surface *surface){

    assert(surface!=NULL);

    TS_Texture texture;

    glGenTextures(1, &texture);

    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);

    return texture;
}

void TS_SurfaceFinalizer(V8FINALIZERARGS) {

    SDL_Surface* surf = (SDL_Surface*)parameter;
    SDL_FreeSurface(surf);
    object.Dispose();
}

void TS_SurfaceReplaceColor(SDL_Surface *surface, TS_Color *oldColor, TS_Color *newColor){
    SDL_LockSurface(surface);
    intptr_t start = (intptr_t)surface->pixels;
    intptr_t last  = start+((surface->w*surface->h)*sizeof(int));

    const int      oldred      = oldColor->red;
    const uint32_t oldcolorint = oldColor->toInt();
    const uint32_t newcolorint = newColor->toInt();

    while(start<last){
        start = (intptr_t)memchr((void *)start, oldred, last-start);
        if(start&0xFF){
            continue;
        }
        else{
            if(memcmp(&oldcolorint, (void *)start, sizeof(int))!=0){
                continue;
            }
            memcpy((void *)start, &newcolorint, sizeof(int));
        }
    }

    SDL_UnlockSurface(surface);
}

v8Function SurfaceToImage(V8ARGS){
    BEGIN_OBJECT_WRAP_CODE;

	SDL_Surface *surface = GET_SELF(SDL_Surface*);

    TS_Texture texture = TS_SurfaceToTexture(surface);

    TS_Image *image = new TS_Image(texture, surface->w, surface->h);

    END_OBJECT_WRAP_CODE(Image, image);
}

v8Function SurfaceClone(V8ARGS){
    BEGIN_OBJECT_WRAP_CODE;

	SDL_Surface *srcsurface = GET_SELF(SDL_Surface*);

    SDL_Surface *surface = SDL_CreateRGBSurface(SDL_SWSURFACE|SDL_SRCALPHA, srcsurface->w, srcsurface->h, DEPTH, CHANNEL_MASKS);

    SDL_BlitSurface(srcsurface, NULL, surface, NULL);

    END_OBJECT_WRAP_CODE(Surface, surface)

}

v8Function SurfaceCloneSection(V8ARGS){

    if(args.Length()<4){
        THROWERROR("[" PLUGINNAME "] SurfaceCloneSection Error: Called with fewer than 4 arguments.");
    }

    CHECK_ARG_INT(0);
    CHECK_ARG_INT(1);
    CHECK_ARG_INT(2);
    CHECK_ARG_INT(3);

    BEGIN_OBJECT_WRAP_CODE;

    int x = args[0]->Int32Value();
    int y = args[1]->Int32Value();
    int w = args[2]->Int32Value();
    int h = args[3]->Int32Value();

    if(w<0){
        x+=w;
        w=-w;
    }
    if(h<0){
        y+=h;
        h=-h;
    }

	SDL_Surface *srcsurface = GET_SELF(SDL_Surface*);

    MINMEMALIGN SDL_Surface *surface = SDL_CreateRGBSurface(SDL_SWSURFACE|SDL_SRCALPHA, w, h, DEPTH, CHANNEL_MASKS);

    SDL_Rect rect = {(short int)x, (short int)y, (unsigned short int)w, (unsigned short int)h};

    SDL_BlitSurface(srcsurface, &rect, surface, NULL);

    END_OBJECT_WRAP_CODE(Surface, surface)

}

v8Function SurfaceGrab(V8ARGS){
    if(args.Length()<4){
        THROWERROR("[" PLUGINNAME "] SurfaceGrab Error: Called with fewer than 4 arguments.");
    }
    CHECK_ARG_INT(0);
    CHECK_ARG_INT(1);
    CHECK_ARG_INT(2);
    CHECK_ARG_INT(3);

    BEGIN_OBJECT_WRAP_CODE;
    int x = args[0]->Int32Value();
    int y = args[1]->Int32Value();
    int w = args[2]->Int32Value();
    int h = args[3]->Int32Value();

    if(w<0){
        x+=w;
        w=-w;
    }
    if(h<0){
        y+=h;
        h=-h;
    }

    TS_Image *image = TS_ImageGrab(x, y, w, h);

    SDL_Surface *surface = image->CreateSurface();

    delete image;

    END_OBJECT_WRAP_CODE(Surface, surface);
}

v8Function SurfaceSave(V8ARGS){

    if(args.Length()<1){
        THROWERROR("[" PLUGINNAME "] SurfaceSave Error: Called with no arguments.");
    }
    CHECK_ARG_STR(0);

    TS_Directories *TS_dirs = GetDirs();

    SDL_Surface *surface = GET_SELF(SDL_Surface*);


    v8::String::Utf8Value str(args[0]);
    const char *filename = string(TS_dirs->image).append(*str).c_str();
    if(SDL_SaveBMP(surface, filename)!=0){
        printf("[" PLUGINNAME "] SurfaceSave Error: Could not save image %s: %s\n", *str, SDL_GetError());
        SDL_ClearError();
        THROWERROR("[" PLUGINNAME "] SurfaceSave Error: Could not save image.");
    }
    return v8::Undefined();
}

v8Function NewSurface(V8ARGS){
    if(args.Length()<1){
        THROWERROR("[" PLUGINNAME "] NewSurface Error: Called with no arguments.");
    }

    BEGIN_OBJECT_WRAP_CODE;
    SDL_Surface *surface = NULL;

    if(args.Length()==1){
        CHECK_ARG_STR(0);

        TS_Directories *TS_dirs = GetDirs();

        v8::String::Utf8Value str(args[0]);
        const char *filename = string(TS_dirs->image).append(*str).c_str();
        if(!(surface = IMG_Load(filename))){
            THROWERROR((std::string("[" PLUGINNAME "] NewSurface Error: Could not load surface from file ")+std::string(filename)).c_str());
        }
        else{
            END_OBJECT_WRAP_CODE(Surface, surface);
        }
    }
    else if(args.Length()==3){
        CHECK_ARG_INT(0);
        CHECK_ARG_INT(1);
        CHECK_ARG_OBJ(2);

        uint32_t width  = args[0]->Uint32Value();
        uint32_t height = args[1]->Uint32Value();

        surface = SDL_CreateRGBSurface(SDL_SWSURFACE|SDL_SRCALPHA, width, height, DEPTH, CHANNEL_MASKS);

        v8::Local<v8::Object> colorobj = v8::Local<v8::Object>::Cast(args[2]);
        TS_Color *color = (TS_Color*)colorobj->GetAlignedPointerFromInternalField(0);

        SDL_FillRect(surface, NULL, color->toInt());

        END_OBJECT_WRAP_CODE(Surface, surface);

    }
    else{
        THROWERROR("[" PLUGINNAME "] NewSurface Error: Requires either 1 or 3 arguments.")
    }
}

v8Function SurfaceSetAlpha(V8ARGS){
    if(args.Length()<1){
        THROWERROR("[" PLUGINNAME "] SurfaceSetAlpha Error: Called with fewer than 2 arguments.");
    }
    CHECK_ARG_INT(0);

    int rawalpha = args[0]->Int32Value();
    if(rawalpha<0)      rawalpha = 0;
    if(rawalpha>255)    rawalpha = 255;
    uint8_t alpha = rawalpha;

    SDL_Surface *surface = GET_SELF(SDL_Surface*);

    if(SDL_SetAlpha(surface, SDL_SRCALPHA, alpha)!=0){
        printf("[" PLUGINNAME "] SurfaceSetAlpha Error: Could not set alpha on surface: %s", SDL_GetError());
        SDL_ClearError();
        THROWERROR("[" PLUGINNAME "] SurfaceSetAlpha Error: Could not set alpha.");
    }

    return v8::Undefined();
}

v8Function SurfaceSetClippingRectangle(V8ARGS){
    if(args.Length()<4){
        THROWERROR("[" PLUGINNAME "] SurfaceSetClippingRectangle Error: Called with fewer than 5 arguments.");
    }
    CHECK_ARG_INT(0);
    CHECK_ARG_INT(1);
    CHECK_ARG_INT(2);
    CHECK_ARG_INT(3);

	int x = args[0]->Int32Value();
	int y = args[1]->Int32Value();
	int w = args[2]->Int32Value();
	int h = args[3]->Int32Value();

    if(w<0){
        x+=w;
        w=-w;
    }
    if(h<0){
        y+=h;
        h=-h;
    }

    SDL_Surface *surface = GET_SELF(SDL_Surface*);

    SDL_Rect rect = {(short int)x, (short int)y, (unsigned short int)w, (unsigned short int)h};

    SDL_SetClipRect(surface, &rect);

	return v8::Undefined();
}

v8Function SurfaceGetClippingRectangle(V8ARGS){

    v8::HandleScope GetClipRectscope;
    v8::Local<v8::Object> cliprect = v8::Object::New();

    SDL_Surface *surface = GET_SELF(SDL_Surface*);

    SDL_Rect rect = {0, 0, 0, 0};
    SDL_GetClipRect(surface, &rect);

    cliprect->Set(v8::String::New("x"), v8::Integer::New(rect.x));
    cliprect->Set(v8::String::New("y"), v8::Integer::New(rect.y));
    cliprect->Set(v8::String::New("w"), v8::Integer::New(rect.w));
    cliprect->Set(v8::String::New("h"), v8::Integer::New(rect.h));

	return GetClipRectscope.Close(cliprect);

}

v8Function SurfaceGetPixel(V8ARGS){
    if(args.Length()<2){
        THROWERROR("[" PLUGINNAME "] SurfaceGetPixel Error: Called with fewer than 2 arguments.");
    }
    CHECK_ARG_INT(0);
    CHECK_ARG_INT(1);

    BEGIN_OBJECT_WRAP_CODE;

    int x = args[0]->Int32Value();
    int y = args[1]->Int32Value();

    SDL_Surface *surface = GET_SELF(SDL_Surface*);
    int *pixels = static_cast<int*>(GET_SELF(SDL_Surface*)->pixels);

    SDL_LockSurface(surface);
    int color   = pixels[x+((surface->w)*y)];
    SDL_UnlockSurface(surface);

    TS_Color *colorobj = new TS_Color((color&(Frmask)), (color&(Fgmask))>>8, (color&(Fbmask))>>16, (color&(Famask))>>24);

    END_OBJECT_WRAP_CODE(Color, colorobj);
}

v8Function SurfaceSetPixel(V8ARGS){
    if(args.Length()<3){
        THROWERROR("[" PLUGINNAME "] SurfaceSetPixel Error: Called with fewer than 3 arguments.");
    }
    CHECK_ARG_INT(0);
    CHECK_ARG_INT(1);
    CHECK_ARG_OBJ(2);

    int x = args[0]->Int32Value();
    int y = args[1]->Int32Value();

    SDL_Surface *surface = GET_SELF(SDL_Surface*);
    int *pixels = static_cast<int*>(GET_SELF(SDL_Surface*)->pixels);

    v8::Local<v8::Object> colorobj = v8::Local<v8::Object>::Cast(args[2]);
    TS_Color *color = (TS_Color*)colorobj->GetAlignedPointerFromInternalField(0);

    SDL_LockSurface(surface);
    pixels[x+((surface->w)*y)] = color->toInt();
    SDL_UnlockSurface(surface);

    return v8::Undefined();

}

v8Function SurfaceRectangle(V8ARGS){
    if(args.Length()<5){
        THROWERROR("[" PLUGINNAME "] SurfaceRectangle Error: Called with fewer than 5 arguments.");
    }
    CHECK_ARG_INT(0);
    CHECK_ARG_INT(1);
    CHECK_ARG_INT(2);
    CHECK_ARG_INT(3);
    CHECK_ARG_OBJ(4);

	int x = args[0]->Int32Value();
	int y = args[1]->Int32Value();
	int w = args[2]->Int32Value();
	int h = args[3]->Int32Value();

    if(w<0){
        x+=w;
        w=-w;
    }
    if(h<0){
        y+=h;
        h=-h;
    }

    v8::Local<v8::Object> colorobj = v8::Local<v8::Object>::Cast(args[4]);
    TS_Color *color = (TS_Color*)colorobj->GetAlignedPointerFromInternalField(0);

    SDL_Surface *surface = GET_SELF(SDL_Surface*);

    SDL_Rect rect = {(short int)x, (short int)y, (unsigned short int)w, (unsigned short int)h};

    if(SDL_FillRect(surface, &rect, color->toInt())!=0){
        printf("[" PLUGINNAME "] SurfaceRectangle Error: Could not set alpha on surface: %s", SDL_GetError());
        SDL_ClearError();
        THROWERROR("[" PLUGINNAME "] SurfaceRectangle Error: Could not draw rectangle.");
    }

	return v8::Undefined();
}

v8Function SurfaceBlit(V8ARGS){
    if(args.Length()<2){
        THROWERROR("[" PLUGINNAME "] SurfaceBlit Error: Called with fewer than 2 arguments.");
    }
    CHECK_ARG_INT(0);
    CHECK_ARG_INT(1);

	int x = args[0]->Int32Value();
	int y = args[1]->Int32Value();

	SDL_Surface *surface = GET_SELF(SDL_Surface*);

    TS_Texture texture = TS_SurfaceToTexture(surface);

    int xs[4] = {x, x+surface->w, x+surface->w, x};
    int ys[4] = {y, y, y+surface->h, y+surface->h};

    TS_BlitTexture(texture, xs, ys);

    glDeleteTextures(1, &texture);

    return v8::Undefined();
}

