#include "game_engine.h"
#include "game_engine.cpp"

#include <Windows.h>
#include <stdio.h>
#include "win32_game_engine.h"

global_variable bool GlobalRunning;
global_variable win32_backbuffer GlobalBackBuffer;

internal void*
Alloc(int32 sizeInBytes)
{
    void* Memory;
    Memory = VirtualAlloc(0, sizeInBytes, MEM_RESERVE|MEM_COMMIT, PAGE_EXECUTE_READWRITE);
    return Memory;
}

internal void
Free(void* Memory, uint32 size)
{
    VirtualFree(Memory, size, MEM_RELEASE);
}

internal file_result
ReadFile(const char* Filepath)
{
    file_result Result = {};
    HANDLE FileHande = CreateFileA(Filepath, GENERIC_READ, FILE_SHARE_READ, 0,
                                           OPEN_EXISTING, 0, 0);
    LARGE_INTEGER FileSize;
    if(GetFileSizeEx(FileHande, &FileSize))
    {
        Result.FileSize = FileSize.QuadPart;
        Result.Content = VirtualAlloc(0, Result.FileSize, MEM_RESERVE|MEM_COMMIT, PAGE_EXECUTE_READWRITE);
        if(Result.Content)
        {
            DWORD BytesRead;
            if(ReadFile(FileHande, Result.Content, Result.FileSize, &BytesRead, 0) &&
                        Result.FileSize == BytesRead)
            {
                //NOTE(tomi): Read Succesfulee!
            }
            else
            {
                VirtualFree(Result.Content, 0, MEM_RELEASE);
            }
        }
    }
    return Result;
}

internal win32_dimension
Win32GetWindowDimension(HWND Window)
{
    RECT ClientRect;
    GetClientRect(Window, &ClientRect);
    win32_dimension Result;
    Result.Width = ClientRect.right - ClientRect.left;
    Result.Height = ClientRect.bottom - ClientRect.top;

    return Result;
}

internal void
Win32ResizeDibSection(HDC DeviceContext, win32_backbuffer* Buffer, int Width, int Height)
{
    if(Buffer->Memory)
    {
        VirtualFree(Buffer->Memory, 0, MEM_RELEASE);
    }
    Buffer->Width = Width;
    Buffer->Height = Height;
    Buffer->BytesPerPixel = 4;
    Buffer->Pitch = Width * Buffer->BytesPerPixel;

    Buffer->BitMapInfo.bmiHeader.biSize = sizeof(Buffer->BitMapInfo);
    Buffer->BitMapInfo.bmiHeader.biWidth = Buffer->Width;
    Buffer->BitMapInfo.bmiHeader.biHeight = -Buffer->Height;
    Buffer->BitMapInfo.bmiHeader.biPlanes = 1;
    Buffer->BitMapInfo.bmiHeader.biBitCount = 32;
    Buffer->BitMapInfo.bmiHeader.biCompression = BI_RGB;

    Buffer->Memory = (void*)VirtualAlloc(0, (Buffer->Width*Buffer->Height*Buffer->BytesPerPixel),
                MEM_COMMIT, PAGE_EXECUTE_READWRITE);

    Buffer->BitMap = CreateDIBSection(DeviceContext, &Buffer->BitMapInfo,
        DIB_RGB_COLORS, &Buffer->Memory, 0, 0);
};

internal void
Win32UpdateWindow(HDC DeviceContext, win32_backbuffer* Buffer, int Width, int Height)
{
    int offsetX = (Width/2)-(Buffer->Width/2);
    int offsety = (Height/2)-(Buffer->Height/2);

    HDC hMemDC;
    hMemDC = CreateCompatibleDC (DeviceContext) ;
    SelectObject (hMemDC, Buffer->BitMap) ;
    BitBlt (DeviceContext, offsetX, offsety, Buffer->Width, Buffer->Height, hMemDC, 0, 0, SRCCOPY) ;
    DeleteDC (hMemDC) ;
};

internal void
Win32InitiallizeGameMemory(game_memory* Memory)
{
    Memory->PermanentStorage = (void*)VirtualAlloc(0, Memory->PermanentStorageSize,
                MEM_COMMIT, PAGE_EXECUTE_READWRITE);
}

internal void
Win32ProcessInput(HWND Window, game_input* Input)
{
    MSG Message;
    while(PeekMessageA(&Message, Window, 0, 0, PM_REMOVE))
    {
        switch(Message.message)
        {
            case WM_SYSKEYDOWN:
            case WM_KEYDOWN:
            {
                Input->Keys[Message.wParam] = true;
            }break;
            case WM_SYSKEYUP:
            case WM_KEYUP:
            {
                Input->Keys[Message.wParam] = false;
            }break;
            default:
            {
                TranslateMessage(&Message);
                DispatchMessage(&Message);
            }break;
        }
    }
}

LRESULT CALLBACK
WindowProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
    switch(Message)
    {
        case WM_CREATE:
        {
            return 0;
        }break;
        case WM_COMMAND:
        {
                return 0;
        }break;
        case WM_SIZE:
        {
            return 0;
        }break;
        case WM_PAINT:
        {
            PAINTSTRUCT Paint;
            HDC DeviceContext = BeginPaint(hwnd, &Paint);
            win32_dimension Dim = Win32GetWindowDimension(hwnd);
            Win32UpdateWindow(DeviceContext, &GlobalBackBuffer, Dim.Width, Dim.Height);
            EndPaint(hwnd, &Paint);
            return 0;

        }break;
        case WM_CLOSE:
        {
            GlobalRunning = false;
            return 0;
        }break;
        case WM_DESTROY:
        {
            GlobalRunning = false;
            return 0;
        }break;
        default:
        {
            return DefWindowProcA(hwnd, Message, wParam, lParam);
        }break;
    }
}

int WINAPI
wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    WNDCLASS WindowClass = {};
    WindowClass.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
    WindowClass.lpfnWndProc = WindowProc;
    WindowClass.cbClsExtra = 0;
    WindowClass.cbWndExtra = 0;
    WindowClass.hInstance = hInstance;
    WindowClass.hIcon = 0;
    WindowClass.hCursor = LoadCursorA(0, IDC_CROSS);
    WindowClass.hbrBackground = CreateSolidBrush(RGB(0, 0, 0));
    WindowClass.lpszMenuName = 0;
    WindowClass.lpszClassName = "GameEngineWindowClass";

    if(RegisterClass(&WindowClass))
    {
        HWND EngineWindow = CreateWindowExA(0, WindowClass.lpszClassName, "GameEngine",
                                            WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                                            CW_USEDEFAULT, CW_USEDEFAULT,
                                            CW_USEDEFAULT, CW_USEDEFAULT,
                                            0, 0, hInstance, 0);
        if(EngineWindow)
        {
            game_input GameInput = {};
            game_memory GameMemory = {};
            GameMemory.PermanentStorageSize = MegaBytes(64);
            Win32InitiallizeGameMemory(&GameMemory);
            HDC DeviceContext = GetDC(EngineWindow);
            Win32ResizeDibSection(DeviceContext, &GlobalBackBuffer, BACKBUFFER_WIDTH, BACKBUFFER_HIEGHT);

            game_backbuffer GameBackBuffer = {};
            GameBackBuffer.Memory = GlobalBackBuffer.Memory;
            GameBackBuffer.Width = GlobalBackBuffer.Width;
            GameBackBuffer.Height = GlobalBackBuffer.Height;
            GameBackBuffer.Pitch = GlobalBackBuffer.Pitch;
            GameBackBuffer.BytesPerPixel = GlobalBackBuffer.BytesPerPixel;

            uint32 UpdateHZ = 30;
            real32 TargetSecondsPerFrame = (1.0f / (real32)UpdateHZ);
            bool32 SleepIsGranular = (timeBeginPeriod(1) == TIMERR_NOERROR);
            LARGE_INTEGER Frequency;
            QueryPerformanceFrequency(&Frequency);
            LARGE_INTEGER LastTime;
            QueryPerformanceCounter(&LastTime);
            real32 DeltaTime  = 0.0f;

            GlobalRunning = true;
            InitiallizeGame(&GameMemory, &GameBackBuffer);

            while(GlobalRunning)
            {
                Win32ProcessInput(EngineWindow, &GameInput);
                win32_dimension Dim = Win32GetWindowDimension(EngineWindow);
                Win32UpdateWindow(DeviceContext, &GlobalBackBuffer, Dim.Width, Dim.Height);

                UpdateGame(&GameInput, DeltaTime, &GameMemory);
                RenderGame(&GameBackBuffer, &GameMemory);

                LARGE_INTEGER CurrentTime;
                QueryPerformanceCounter(&CurrentTime);
                real32 SecondsPassed = (real32)(CurrentTime.QuadPart - LastTime.QuadPart) / (real32)Frequency.QuadPart;

                if(SecondsPassed < TargetSecondsPerFrame)
                {
                    if(SleepIsGranular)
                    {
                        DWORD MsSleep = (DWORD)(1000.0f*(TargetSecondsPerFrame - SecondsPassed));
                        if(MsSleep > 0)
                        {
                            Sleep(MsSleep);
                        }
                    }
                }

                QueryPerformanceCounter(&CurrentTime);
                DeltaTime = (real32)(CurrentTime.QuadPart - LastTime.QuadPart) / Frequency.QuadPart;
                int64 DeltaInterger = (CurrentTime.QuadPart - LastTime.QuadPart);
                LastTime = CurrentTime;

                int32 FPSCounter = Frequency.QuadPart / DeltaInterger;
                char buffer[256];
                sprintf(buffer, "GameEngine - FPS: %d - MS: %f\n", FPSCounter, DeltaTime*1000);
                SetWindowTextA(EngineWindow, buffer);
            }

            ReleaseDC(EngineWindow, DeviceContext);
        }
    }
    return 0;
}
