/*
* ASCII Game of Life
*    Inspired by "Code-It-Yourself! Tetris - Programming from Scratch (Quick and Simple C++)": https://youtu.be/8OK8_tHeCIA?si=kOIxABm-g3ZwT_nd
*
* Legend:
*   a . (dot) means the cell is dead.
*   an * (asterisk) means the cell is alive.
* 
* Birth Rule: a dead cell with exactly 3 neighboring cells, will become alive within the next generation, ex.:
*   ..*        ..*
*   ..*   ->   .**
*   *..        *..
*
* Death Rule #1 (isolation): a live cell with only 0 or 1 neighbor cells, will die within the next generation, ex.:
*   ...        ...        ...        ...
*   .*.   ->   ...   or   .*.   ->   ...
*   .*.        .*.        ...        ...
* 
* Death Rule #2 (overcrowding): a live cell with 4 or more neighbor cells, will die within the next generation, ex.:
*   **.        **.        ***        ***
*   .**   ->   ..*   or   .**   ->   ..*   Etc.
*   .*.        .*.        .*.        .*.
*
* Survival Rule: a live cell with 2 or 3 neighboring cells, will survice to the next generation, ex.:
*   *..        *..        .*.        .*.
*   **.   ->   **.   or   .*.   ->   .*.
*   ...        ...        *.*        *.*
*/

#include <Windows.h>
#include <chrono>
#include <thread>
#include "main.h"

const unsigned int FRAME_DELAY = 1000;
const unsigned int FINAL_DELAY = 5000;

const int SCREEN_WIDTH = 120;    // Console Screen Size X (columns)
const int SCREEN_HEIGHT = 30;    // Console Screen Size Y (rows)

bool isScreenOneCurrent = true;
wchar_t* screen1 = new wchar_t[SCREEN_WIDTH * SCREEN_HEIGHT];
wchar_t* screen2 = new wchar_t[SCREEN_WIDTH * SCREEN_HEIGHT];
wchar_t* screenCurrent = screen1;
wchar_t* screenNext = screen2;

int main()
{
    /*
    * Initialize data structures.
    */

    HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    SetConsoleActiveScreenBuffer(hConsole);
    DWORD dwBytesWritten = 0;

    drawBlankField(screen1);
    drawBlankField(screen2);

    glider(screen1, 1, 1);
    glider(screen2, 1, 1);

    // Main Loop
    bool bGameOver = false;
    while (!bGameOver)
    {        
        // Display Frame
        if (isScreenOneCurrent)
        {
            screenCurrent = screen1;
            screenNext = screen2;
            drawBlankField(screen2);
        }
        else
        {
            screenCurrent = screen2;
            screenNext = screen1;
            drawBlankField(screen1);
        }

        calculateNextGeneration();

        WriteConsoleOutputCharacter(hConsole, screenNext, SCREEN_WIDTH * SCREEN_HEIGHT, { 0,0 }, &dwBytesWritten);

        isScreenOneCurrent = !isScreenOneCurrent;

        std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(FRAME_DELAY)); // Small Step = 1 Game Tick
    }

    // Exiting
    std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(FINAL_DELAY));
    CloseHandle(hConsole);
    return 0;
}

size_t position(size_t x, size_t y)
{
    return x + y * SCREEN_WIDTH;
}

void drawBlankField(wchar_t* screen)
{
    for (size_t index = 0; index < SCREEN_HEIGHT * SCREEN_WIDTH; index++)
    {
        screen[index] = L' ';
    }
}

void glider(wchar_t* screen, size_t x, size_t y)
{
    screen[position(x + 1, y - 1)] = L'*';
    screen[position(x - 1, y    )] = L'*';
    screen[position(x + 1, y    )] = L'*';
    screen[position(x    , y + 1)] = L'*';
    screen[position(x + 1, y + 1)] = L'*';
}

void calculateNextGeneration()
{
    for (size_t y = 1; y < SCREEN_HEIGHT - 1; ++y)
    {
        for (size_t x = 1; x < SCREEN_WIDTH - 1; ++x)
        {
            calculateCell(x, y);
        }
    }
}

void calculateCell(size_t x, size_t y)
{
    int neighborCellsCounter = 0;
    if (screenCurrent[position(x - 1, y - 1)] == L'*')
        neighborCellsCounter++;
    if (screenCurrent[position(x    , y - 1)] == L'*')
        neighborCellsCounter++;
    if (screenCurrent[position(x + 1, y - 1)] == L'*')
        neighborCellsCounter++;
    if (screenCurrent[position(x - 1, y    )] == L'*')
        neighborCellsCounter++;
    if (screenCurrent[position(x + 1, y    )] == L'*')
        neighborCellsCounter++;
    if (screenCurrent[position(x - 1, y + 1)] == L'*')
        neighborCellsCounter++;
    if (screenCurrent[position(x    , y + 1)] == L'*')
        neighborCellsCounter++;
    if (screenCurrent[position(x + 1, y + 1)] == L'*')
        neighborCellsCounter++;

    if (screenCurrent[position(x, y)] == L' ')
    {//The cell in (x,y) is currently dead
        if (neighborCellsCounter == 3)
            screenNext[position(x, y)] = L'*';
    }
    else
    {//The cell in (x,y) is currently alive
        if (neighborCellsCounter < 2 || neighborCellsCounter > 3)
            screenNext[position(x, y)] = L' ';
        else
            screenNext[position(x, y)] = L'*';
    }
}