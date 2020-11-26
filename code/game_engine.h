#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H

#define BACKBUFFER_WIDTH 960
#define BACKBUFFER_HIEGHT 540

#define internal static
#define global_variable static
#define local_persist static

typedef char int8;
typedef unsigned char uint8;
typedef short int16;
typedef unsigned short uint16;
typedef int int32;
typedef unsigned int uint32;
typedef long long int64;
typedef unsigned long long uint64;
typedef float real32;
typedef double real64;
typedef unsigned int bool32;

#define Round(X) (int)((X)+0.5)

#define KiloBytes(Value) ((Value)*1024)
#define MegaBytes(Value) (KiloBytes(Value)*1024)
#define GigaBytes(Value) (MegaBytes(Value)*1024)
#define TeraBytes(Value) (GigaBytes(Value)*1024)

//TODO(tomi): Try to not use c runtime library
#include <math.h>
#include <thread>
//------------------------
/*Include GameFiles HERE*/
#include "game_engine_math.cpp"

#pragma pack(push, 1)
struct bmp_header
{
    uint16 FileType;
    uint32 FileSize;
    uint32 Reserved;
    uint32 Offset;
    uint32 HeaderSize;
    uint32 Width;
    uint32 Height;
    uint16 Planes;
    uint16 BitsPerPixel;
    uint32 Compression;
    uint32 SizeOfBitmap;
    int32 HorzResolution;
    int32 VertResolution;
    uint32 ColorsUsed;
    uint32 ColorsImportant;

    uint32 RedMask;
    uint32 GreenMask;
    uint32 BlueMask;
};
#pragma pack(pop)

struct texture
{
    int32 Width;
    int32 Height;
    uint32* Pixels;
};

enum tile_type
{
    FLOOR_TILE,
    WALL_TILE,
    GRASS_TILE,
};

struct tile
{
    tile_type Type;
    vec2 Position;
    texture* Texture;
};

struct tile_map
{
    tile* Tiles;
    uint32 Width;
    uint32 Height;
};


struct bmp_image
{
    int32 Width;
    int32 Height;
    uint32* Pixels;
};

struct game_memory
{
    bool32 Initiallize;
    uint64 PermanentStorageSize;
    void* PermanentStorage;
};

struct game_input
{
    bool32 Keys[256];
};

struct game_backbuffer
{
    void* Memory;
    int Width;
    int Height;
    int Pitch;
    int BytesPerPixel;
};

struct file_result
{
    uint32 FileSize;
    void* Content;
};

struct game_state
{
    vec2 Position;
    bmp_image SpriteSheet;
    bmp_image Background;
    bmp_image PlayerSprite;
    bmp_image HeadSprite;
    bmp_image BodySprite;
    bmp_image LegsSprite;

    texture PlayerRight;
    texture PlayerLeft;
    texture PlayerTop;
    texture PlayerBottom;
    texture* CurrentPlayer;

    texture PlayerHead[4];
    texture* CurrentHead;
    texture PlayerBody[4];
    texture* CurrentBody;
    texture PlayerLegs[4];
    texture* CurrentLegs;

    texture FloorTile;
    texture RockTile;
    texture GrassTile;

    vec2 vel;

    file_result TileMapFile;
    tile_map TileMap;

    std::thread* BufferThread;
    uint32 BufferThreadSize;
};

//Functions provided by the platform to the platform
internal file_result
ReadFile(const char* Filepath);

internal void*
Alloc(int32 sizeInBytes);

internal void
Free(void* Memory, uint32 size);

//Functions provided by the game to the platform
internal void
InitiallizeGame(game_memory* Memory);
internal void
UpdateGame(game_input* Input, real32 DeltaTime, game_memory* Memory);
internal void
RenderGame(game_backbuffer* Buffer, game_memory* Memory);

#endif
