#ifndef WIN32_GAME_ENGINE
#define WIN32_GAME_ENGINE

struct win32_backbuffer
{
    BITMAPINFO BitMapInfo;
    HBITMAP BitMap;
    void* Memory;
    int Width;
    int Height;
    int Pitch;
    int BytesPerPixel;
};

struct win32_dimension
{
    int Width;
    int Height;
};

struct win32_keyboard_input
{
    bool32 Keys[256];
};

#endif
