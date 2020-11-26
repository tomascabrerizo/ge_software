#include "game_engine.h"

internal bmp_image
LoadBMP(const char* Filepath)
{
    bmp_image Image = {};
    file_result File = ReadFile(Filepath);
    bmp_header* ImageFile = (bmp_header*)File.Content;
    if(ImageFile)
    {
        Image.Pixels = (uint32*)((uint8*)File.Content + ImageFile->Offset);
        Image.Width = ImageFile->Width;
        Image.Height = ImageFile->Height;
    }

    return Image;
}

internal void
DrawRect(real32 x, real32 y, int width, int height, vec4 Color, game_backbuffer* Buffer)
{
    int32 MinX = (int32)x;
    int32 MinY = (int32)y;
    int32 MaxX = (int32)x + width;
    int32 MaxY = (int32)y + height;

    if(MinX < 0)
    {
        MinX = 0;
    }
    if(MinY < 0)
    {
        MinY = 0;
    }
    if(MaxX > Buffer->Width)
    {
        MaxX = Buffer->Width;
    }
    if(MaxY > Buffer->Height)
    {
        MaxY = Buffer->Height;
    }

    Color *= 255;
    uint8* row = (uint8*)Buffer->Memory + (MinY*Buffer->Pitch+MinX*Buffer->BytesPerPixel);
    for(int y = MinY; y < MaxY; y++)
    {
        uint32* pixel = (uint32*)row;
        for(int x = MinX; x < MaxX; x++)
        {
            *pixel = ((uint8)Color.a << 24) | ((uint8)Color. r << 16) | ((uint8)Color.g << 8) | ((uint8)Color.b << 0);
            pixel++;
        }
        row += Buffer->Pitch;
    }
}

internal void
DrawRectMaxMin(int MinX, int MinY, int MaxX, int MaxY, color Color, game_backbuffer* Buffer)
{
    if(MinX < 0)
    {
        MinX = 0;
    }
    if(MinY < 0)
    {
        MinY = 0;
    }
    if(MaxX > Buffer->Width)
    {
        MaxX = Buffer->Width;
    }
    if(MaxY > Buffer->Height)
    {
        MaxY = Buffer->Height;
    }

    uint8* row = (uint8*)Buffer->Memory + (MinY*Buffer->Pitch+MinX*Buffer->BytesPerPixel);
    for(int y = MinY; y < MaxY; y++)
    {
        uint32* pixel = (uint32*)row;
        for(int x = MinX; x < MaxX; x++)
        {
            *pixel = (Color.a << 24) | (Color. r << 16) | (Color.g << 8) | (Color.b << 0);
            pixel++;
        }
        row += Buffer->Pitch;
    }
}

internal void
RenderClear(game_backbuffer* Buffer, color Color)
{
    uint8* row = (uint8*)Buffer->Memory;
    for(int y = 0; y < Buffer->Height; y++)
    {
        uint32* pixel = (uint32*)row;
        for(int x = 0; x < Buffer->Width; x++)
        {
            *pixel = Color.a << 24 | Color.r << 16 | Color.g << 8 | Color.b;
            pixel++;
        }
        row += Buffer->Pitch;
    }
}

internal void
RenderBMP(vec2 Position, bmp_image* Image, game_backbuffer* Buffer)
{
    int32 MinY = (int32)Position.y;
    int32 MinX = (int32)Position.x;
    int32 MaxY = (int32)Position.y + Image->Height;
    int32 MaxX = (int32)Position.x + Image->Width;

    int32 SOffsetY = 0;
    int32 SOffsetX = 0;
    if(MinY < 0)
    {
        SOffsetY = -MinY;
        MinY = 0;
    }
    if(MaxY > Buffer->Height)
    {
        MaxY = Buffer->Height;
    }
    if(MinX < 0)
    {
        SOffsetX = -MinX;
        MinX = 0;
    }
    if(MaxX > Buffer->Width)
    {
        MaxX = Buffer->Width;
    }

    uint32* imageRow = (uint32*)Image->Pixels + Image->Width*(Image->Height-1);
    imageRow += -SOffsetY*Image->Width + SOffsetX;
    uint8* row = (uint8*)Buffer->Memory +  (MinY*Buffer->Pitch+MinX*Buffer->BytesPerPixel);
    for(int y = MinY; y < MaxY; y++)
    {
        uint32* Pixel = (uint32*)row;
        uint32* ImagePixel = imageRow;
        for(int x = MinX; x < MaxX; x++)
        {
            real32 SAlpha   = (real32)((*ImagePixel >> 24) & 0x0000FF) / 255.0f;
            real32 SRed   = (real32)((*ImagePixel >> 16) & 0x0000FF);
            real32 SGreen = (real32)((*ImagePixel >> 8) & 0x0000FF);
            real32 SBlue  = (real32)((*ImagePixel >> 0) & 0x0000FF);

            uint32 Color =  0 << 24 | (uint8)SRed << 16| (uint8)SGreen << 8| (uint8)SBlue << 0;
            *Pixel = Color;
            Pixel++;
            ImagePixel++;
        }
        row += Buffer->Pitch;
        imageRow -= (Image->Width);
    }
}

internal void
RenderBMPA(vec2 Position, bmp_image* Image, game_backbuffer* Buffer)
{
    int32 MinY = (int32)Position.y;
    int32 MinX = (int32)Position.x;
    int32 MaxY = (int32)Position.y + Image->Height;
    int32 MaxX = (int32)Position.x + Image->Width;

    int32 SOffsetY = 0;
    int32 SOffsetX = 0;
    if(MinY < 0)
    {
        SOffsetY = -MinY;
        MinY = 0;
    }
    if(MaxY > Buffer->Height)
    {
        MaxY = Buffer->Height;
    }
    if(MinX < 0)
    {
        SOffsetX = -MinX;
        MinX = 0;
    }
    if(MaxX > Buffer->Width)
    {
        MaxX = Buffer->Width;
    }

    uint32* imageRow = (uint32*)Image->Pixels + Image->Width*(Image->Height-1);
    imageRow += -SOffsetY*Image->Width + SOffsetX;
    uint8* row = (uint8*)Buffer->Memory +  (MinY*Buffer->Pitch+MinX*Buffer->BytesPerPixel);
    for(int y = MinY; y < MaxY; y++)
    {
        uint32* Pixel = (uint32*)row;
        uint32* ImagePixel = imageRow;
        for(int x = MinX; x < MaxX; x++)
        {
            real32 SAlpha   = (real32)((*ImagePixel >> 24) & 0x0000FF) / 255.0f;
            real32 SRed   = (real32)((*ImagePixel >> 16) & 0x0000FF);
            real32 SGreen = (real32)((*ImagePixel >> 8) & 0x0000FF);
            real32 SBlue  = (real32)((*ImagePixel >> 0) & 0x0000FF);

            real32 DRed   = (real32)((*Pixel >> 16) & 0x0000FF);
            real32 DGreen = (real32)((*Pixel >> 8) & 0x0000FF);
            real32 DBlue  = (real32)((*Pixel >> 0) & 0x0000FF);

            uint32 Color =  0 << 24 |
                            (uint8)((1 - SAlpha)*DRed + SAlpha*SRed) << 16|
                            (uint8)((1 - SAlpha)*DGreen + SAlpha*SGreen) << 8|
                            (uint8)((1 - SAlpha)*DBlue + SAlpha*SBlue) << 0;
            *Pixel = Color;
            Pixel++;
            ImagePixel++;
        }
        row += Buffer->Pitch;
        imageRow -= (Image->Width);
    }
}

internal texture
GetTextureFromSpriteSheet(bmp_image* SpriteSheet, int SpriteWidth, int SpriteHeight,
    int32 x, int32 y)
{
    uint32 NewY = ((SpriteSheet->Height-SpriteHeight)/SpriteWidth -y);
    uint32 TileX = x * SpriteWidth;
    uint32 TileY = NewY * SpriteWidth;
    texture NewTexture = {};
    NewTexture.Width = SpriteWidth;
    NewTexture.Height = SpriteHeight;
    uint32* TexturePointer = (uint32*)SpriteSheet->Pixels + (TileX + (TileY*SpriteSheet->Width));
    NewTexture.Pixels = TexturePointer;

    return NewTexture;
}

internal void
RenderTextureFromTileSheep(vec2 Position, texture* Texture, game_backbuffer* Buffer, uint32 TileSheetWidth)
{
    int32 MinY = (int32)Position.y;
    int32 MinX = (int32)Position.x;
    int32 MaxY = (int32)Position.y + Texture->Height;
    int32 MaxX = (int32)Position.x + Texture->Width;

    int32 SOffsetY = 0;
    int32 SOffsetX = 0;
    if(MinY < 0)
    {
        SOffsetY = -MinY;
        MinY = 0;
    }
    if(MaxY > Buffer->Height)
    {
        MaxY = Buffer->Height;
    }
    if(MinX < 0)
    {
        SOffsetX = -MinX;
        MinX = 0;
    }
    if(MaxX > Buffer->Width)
    {
        MaxX = Buffer->Width;
    }

    int32 TextureOffSet = (-SOffsetY*TileSheetWidth) + SOffsetX;
    uint32* TextureRow = (uint32*)Texture->Pixels + TileSheetWidth*(Texture->Height-1);
    TextureRow += TextureOffSet;
    uint8* row = (uint8*)Buffer->Memory +  (MinY*Buffer->Pitch+MinX*Buffer->BytesPerPixel);
    for(int y = MinY; y < MaxY; y++)
    {
        uint32* Pixel = (uint32*)row;
        uint32* TexturePixel = TextureRow;
        for(int x = MinX; x < MaxX; x++)
        {
            real32 SAlpha   = (real32)((*TexturePixel >> 24) & 0x0000FF) / 255.0f;
            real32 SRed   = (real32)((*TexturePixel >> 16) & 0x0000FF);
            real32 SGreen = (real32)((*TexturePixel >> 8) & 0x0000FF);
            real32 SBlue  = (real32)((*TexturePixel >> 0) & 0x0000FF);

            real32 DRed   = (real32)((*Pixel >> 16) & 0x0000FF);
            real32 DGreen = (real32)((*Pixel >> 8) & 0x0000FF);
            real32 DBlue  = (real32)((*Pixel >> 0) & 0x0000FF);

            uint32 Color =  0 << 24 |
                            (uint8)((1 - SAlpha)*DRed + SAlpha*SRed) << 16|
                            (uint8)((1 - SAlpha)*DGreen + SAlpha*SGreen) << 8|
                            (uint8)((1 - SAlpha)*DBlue + SAlpha*SBlue) << 0;
            *Pixel = Color;
            Pixel++;
            TexturePixel++;
        }
        row += Buffer->Pitch;
        TextureRow -= TileSheetWidth;
    }
}

internal tile_map
ProcessTileMapFile(file_result MapFile, game_memory* Memory, game_backbuffer* Buffer)
{
    tile* TimeMap;
    game_state* GameState = (game_state*)Memory->PermanentStorage;

    uint32 MaxArrayWidth = 0;

    int8* row = (int8*)MapFile.Content;
    while(*row != '\0')
    {
        real32 x = 0;
        int8* colum = row;
        while(*colum != '\r' && *colum != '\0')
        {
            MaxArrayWidth++;
            colum++;
        }
        row = colum+2;
        GameState->TileMap.Height++;
        GameState->TileMap.Width  = GameState->TileMap.Width < MaxArrayWidth ? MaxArrayWidth : GameState->TileMap.Width;
        MaxArrayWidth = 0;
    }

    //TODO(tomi): Change this malloc to my own memory
    GameState->TileMap.Tiles = (tile*)Alloc(GameState->TileMap.Width*GameState->TileMap.Height*sizeof(tile));
    row = (int8*)MapFile.Content;
    for(int y = 0; y < GameState->TileMap.Height; y++)
    {
        int8* colum = row;
        for(int x = 0; x < GameState->TileMap.Width; x++)
        {
            if(*colum != '\r')
            {
                char Tile = *colum;
                if(Tile == '#')
                {
                    tile NewTile = tile{WALL_TILE, vec2{(real32)x*32, (real32)y*32}, &GameState->RockTile};
                    GameState->TileMap.Tiles[x+(y*GameState->TileMap.Width)] = NewTile;
                }
                else if(Tile == '.')
                {
                    tile NewTile = tile{FLOOR_TILE, vec2{(real32)x*32, (real32)y*32}, &GameState->FloorTile};
                    GameState->TileMap.Tiles[x+(y*GameState->TileMap.Width)] = NewTile;

                }
                else if(Tile == 'm')
                {
                    tile NewTile = tile{GRASS_TILE, vec2{(real32)x*32, (real32)y*32}, &GameState->GrassTile};
                    GameState->TileMap.Tiles[x+(y*GameState->TileMap.Width)] = NewTile;
                }
                colum++;
                //TODO(tomi): give a position to NULL tiles
            }
        }
        row = colum+2;
    }

    return GameState->TileMap;
}

internal bool32
IsTileSolid(int TileX, int TileY)
{
    //TODO(tomi): Make this function
}

void RenderTilesThread(int32 Height, int32 Width, int32 index, game_state* GameState, game_backbuffer* Buffer)
{
    for(int32 y = index; y < Height; y++)
    {
        for(int32 x = 0; x < Width; x++)
        {
            texture* NewTexture = GameState->TileMap.Tiles[x+(y*GameState->TileMap.Width)].Texture;
            if(NewTexture)
            {
                real32 CenterX = GameState->Position.x;
                real32 CenterY = GameState->Position.y;

                real32 OffsetX = Buffer->Width/2 - CenterX;
                real32 OffsetY = Buffer->Height/2 - CenterY;

                vec2 NewTilePos = GameState->TileMap.Tiles[x+(y*GameState->TileMap.Width)].Position + vec2{OffsetX, OffsetY};
                RenderTextureFromTileSheep(NewTilePos, NewTexture, Buffer, GameState->SpriteSheet.Width);
            }
        }
    }
}

internal void
InitiallizeGame(game_memory* Memory, game_backbuffer* Buffer)
{
    game_state* GameState = (game_state*)Memory->PermanentStorage;
    if(!Memory->Initiallize)
    {
        //NOTE(tomi):Player Position relative the tilemap//TODO(tomi): make a real player
        GameState->Position = vec2{(real32)33*2, (real32)33*4};

        GameState->SpriteSheet = LoadBMP("data/spritesheet/DungeonCrawl.bmp");
        GameState->Background = LoadBMP("data/spritesheet/SpaceBack.bmp");
        GameState->PlayerSprite = LoadBMP("data/spritesheet/Player64x.bmp");

        GameState->HeadSprite = LoadBMP("data/spritesheet/PlayerHead64x.bmp");
        GameState->BodySprite = LoadBMP("data/spritesheet/PlayerTorsoCuero64x.bmp");
        GameState->LegsSprite = LoadBMP("data/spritesheet/PlayerLegsGreen64x.bmp");

        GameState->FloorTile = GetTextureFromSpriteSheet(&GameState->SpriteSheet, 32, 32, 29, 13);
        GameState->RockTile = GetTextureFromSpriteSheet(&GameState->SpriteSheet, 32, 32, 22, 13);
        GameState->GrassTile = GetTextureFromSpriteSheet(&GameState->SpriteSheet, 32, 32, 58, 14);

        GameState->PlayerTop = GetTextureFromSpriteSheet(&GameState->PlayerSprite, 64, 64, 0, 0);
        GameState->PlayerLeft = GetTextureFromSpriteSheet(&GameState->PlayerSprite, 64, 64, 0, 1);
        GameState->PlayerBottom = GetTextureFromSpriteSheet(&GameState->PlayerSprite, 64, 64, 0, 2);
        GameState->PlayerRight = GetTextureFromSpriteSheet(&GameState->PlayerSprite, 64, 64, 0, 3);

        GameState->PlayerHead[0] = GetTextureFromSpriteSheet(&GameState->HeadSprite, 64, 64, 0, 0);
        GameState->PlayerHead[1] = GetTextureFromSpriteSheet(&GameState->HeadSprite, 64, 64, 0, 1);
        GameState->PlayerHead[2] = GetTextureFromSpriteSheet(&GameState->HeadSprite, 64, 64, 0, 2);
        GameState->PlayerHead[3] = GetTextureFromSpriteSheet(&GameState->HeadSprite, 64, 64, 0, 3);

        GameState->PlayerBody[0] = GetTextureFromSpriteSheet(&GameState->BodySprite, 64, 64, 0, 0);
        GameState->PlayerBody[1] = GetTextureFromSpriteSheet(&GameState->BodySprite, 64, 64, 0, 1);
        GameState->PlayerBody[2] = GetTextureFromSpriteSheet(&GameState->BodySprite, 64, 64, 0, 2);
        GameState->PlayerBody[3] = GetTextureFromSpriteSheet(&GameState->BodySprite, 64, 64, 0, 3);

        GameState->PlayerLegs[0] = GetTextureFromSpriteSheet(&GameState->LegsSprite, 64, 64, 0, 0);
        GameState->PlayerLegs[1] = GetTextureFromSpriteSheet(&GameState->LegsSprite, 64, 64, 0, 1);
        GameState->PlayerLegs[2] = GetTextureFromSpriteSheet(&GameState->LegsSprite, 64, 64, 0, 2);
        GameState->PlayerLegs[3] = GetTextureFromSpriteSheet(&GameState->LegsSprite, 64, 64, 0, 3);

        Memory->Initiallize = true;

        GameState->TileMapFile = ReadFile("data/maps/lvl1.map");
        GameState->TileMap = ProcessTileMapFile(GameState->TileMapFile, Memory, Buffer);

        GameState->BufferThreadSize = 10;
        GameState->BufferThread = (std::thread*)Alloc(sizeof(std::thread)*GameState->BufferThreadSize);

        GameState->CurrentPlayer = &GameState->PlayerBottom;
        GameState->CurrentHead = &GameState->PlayerHead[2];
        GameState->CurrentBody = &GameState->PlayerBody[2];
        GameState->CurrentLegs = &GameState->PlayerLegs[2];
    }
}

internal void
UpdateGame(game_input* Input, real32 DeltaTime, game_memory* Memory)
{
    game_state* GameState = (game_state*)Memory->PermanentStorage;
    vec2 NewPosition = GameState->Position;
    vec2 a = {};
    if(Input->Keys['W'])
    {
        a.y = -1;
        a.x = 0;
        GameState->CurrentPlayer = &GameState->PlayerTop;
        GameState->CurrentHead = &GameState->PlayerHead[0];
        GameState->CurrentBody = &GameState->PlayerBody[0];
        GameState->CurrentLegs = &GameState->PlayerLegs[0];
    }
    if(Input->Keys['S'])
    {
        a.y = 1;
        a.x = 0;
        GameState->CurrentPlayer = &GameState->PlayerBottom;
        GameState->CurrentHead = &GameState->PlayerHead[2];
        GameState->CurrentBody = &GameState->PlayerBody[2];
        GameState->CurrentLegs = &GameState->PlayerLegs[2];
    }
    if(Input->Keys['A'])
    {
        a.x = -1;
        a.y = 0;
        GameState->CurrentPlayer = &GameState->PlayerLeft;
        GameState->CurrentHead = &GameState->PlayerHead[1];
        GameState->CurrentBody = &GameState->PlayerBody[1];
        GameState->CurrentLegs = &GameState->PlayerLegs[1];
    }
    if(Input->Keys['D'])
    {
        a.x = 1;
        a.y = 0;
        GameState->CurrentPlayer = &GameState->PlayerRight;
        GameState->CurrentHead = &GameState->PlayerHead[3];
        GameState->CurrentBody = &GameState->PlayerBody[3];
        GameState->CurrentLegs = &GameState->PlayerLegs[3];
    }

    real32 ALengthSqr = LengthSqr(a);
    if(ALengthSqr > 1)
    {
        a *= 1/sqrtf(ALengthSqr);
    }

    real32 Speed = 2000.0f;
    a*=Speed;
    //NOTE(tomi): Add some force
    a += 10*-GameState->vel;

    NewPosition = (0.5*a*(DeltaTime*DeltaTime))+(GameState->vel*DeltaTime)+GameState->Position;
    GameState->vel = (a*DeltaTime)+GameState->vel;

    int32 TileX = (NewPosition.x+GameState->PlayerLeft.Width/2)/(real32)GameState->FloorTile.Width;
    int32 TileY = (NewPosition.y+GameState->PlayerLeft.Height*0.8)/(real32)GameState->FloorTile.Height;
    tile Tile = GameState->TileMap.Tiles[TileX+(TileY*GameState->TileMap.Width)];

    if(Tile.Type != WALL_TILE)
    {
        GameState->Position = NewPosition;
    }
}

internal void
RenderGame(game_backbuffer* Buffer, game_memory* Memory)
{
    game_state* GameState = (game_state*)Memory->PermanentStorage;

    //RenderClear(Buffer, color{255, 0, 255, 255});
    RenderBMP(vec2{0, 0}, &GameState->Background, Buffer);

//NOTE(tomi): threaded Tile map renderer goes Wrong!
#if 0
    uint32 NumRoutines = GameState->BufferThreadSize;
    void (*RenderTilePtr)(int32 y, int32 Width, int index, game_state* GameState, game_backbuffer* Buffer);
    RenderTilePtr = RenderTilesThread;
    int index = 0;
    for(int i = 0; i < NumRoutines; i++)
    {
        int height = GameState->TileMap.Height/NumRoutines * (i + 1);
        GameState->BufferThread[i] = std::thread(RenderTilePtr, height, GameState->TileMap.Width, index, GameState, Buffer);
        index += GameState->TileMap.Height/NumRoutines;
    }
    for(int i = 0; i < NumRoutines; i++)
    {
        GameState->BufferThread[i].join();
    }

#else
    for(int y = 0; y < GameState->TileMap.Height; y++)
    {
        for(int x = 0; x < GameState->TileMap.Width; x++)
        {
            texture* NewTexture = GameState->TileMap.Tiles[x+(y*GameState->TileMap.Width)].Texture;
            if(NewTexture)
            {
                real32 CenterX = GameState->Position.x;
                real32 CenterY = GameState->Position.y;

                real32 OffsetX = Buffer->Width/2 - CenterX;
                real32 OffsetY = Buffer->Height/2 - CenterY;

                vec2 NewTilePos = GameState->TileMap.Tiles[x+(y*GameState->TileMap.Width)].Position + vec2{OffsetX, OffsetY};
                RenderTextureFromTileSheep(NewTilePos, NewTexture, Buffer, GameState->SpriteSheet.Width);
            }
        }
    }

#endif

    RenderTextureFromTileSheep(vec2{(real32)Buffer->Width/2, (real32)Buffer->Height/2}, GameState->CurrentPlayer, Buffer, GameState->PlayerSprite.Width);

    RenderTextureFromTileSheep(vec2{(real32)Buffer->Width/2, (real32)Buffer->Height/2}, GameState->CurrentHead, Buffer, GameState->HeadSprite.Width);
    RenderTextureFromTileSheep(vec2{(real32)Buffer->Width/2, (real32)Buffer->Height/2}, GameState->CurrentBody, Buffer, GameState->BodySprite.Width);
    RenderTextureFromTileSheep(vec2{(real32)Buffer->Width/2, (real32)Buffer->Height/2}, GameState->CurrentLegs, Buffer, GameState->LegsSprite.Width);
}
