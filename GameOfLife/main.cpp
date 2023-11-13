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

const unsigned int FRAME_DELAY = 50;
const unsigned int FINAL_DELAY = 5000;

const wchar_t DEAD_CELL  = L' ';
const wchar_t ALIVE_CELL = L'*';

const int SCREEN_WIDTH  = 120;   // Console Screen Size X (columns)
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

    glider(01, 1);
    glider(31, 1);
    glider(61, 1);

    // Main Loop
    bool bGameOver = false;
    while (!bGameOver)
    {
        // Display Current Screen
        WriteConsoleOutputCharacter(hConsole, screenCurrent, SCREEN_WIDTH * SCREEN_HEIGHT, { 0,0 }, &dwBytesWritten);
        
        // Small Step = 1 Game Tick
        std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(FRAME_DELAY));

        isScreenOneCurrent = !isScreenOneCurrent;
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
    }

    // Exiting
    std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(FINAL_DELAY));
    CloseHandle(hConsole);
    return 0;
}

size_t position(int x, int y)
{
    return x + y * SCREEN_WIDTH;
    //return (x % (SCREEN_WIDTH-2)) + (y % (SCREEN_HEIGHT-2)) * SCREEN_WIDTH;
}

void drawBlankField(wchar_t* screen)
{
    for (size_t index = 0; index < SCREEN_HEIGHT * SCREEN_WIDTH; index++)
    {
        screen[index] = DEAD_CELL;
    }
}

void cell(wchar_t* screen, int x, int y, wchar_t character)
{
    screen[position(x, y)] = character;
}

void glider(int x, int y)
{
    //screen1
    cell(screen1, x + 1, y - 1, ALIVE_CELL);
    cell(screen1, x - 1, y    , ALIVE_CELL);
    cell(screen1, x + 1, y    , ALIVE_CELL);
    cell(screen1, x    , y + 1, ALIVE_CELL);
    cell(screen1, x + 1, y + 1, ALIVE_CELL);

    // screen2
    cell(screen2, x + 1, y - 1, ALIVE_CELL);
    cell(screen2, x - 1, y    , ALIVE_CELL);
    cell(screen2, x + 1, y    , ALIVE_CELL);
    cell(screen2, x    , y + 1, ALIVE_CELL);
    cell(screen2, x + 1, y + 1, ALIVE_CELL);
}

void calculateNextGeneration()
{
    for (size_t y = 0; y < SCREEN_HEIGHT; ++y)
    {
        for (size_t x = 0; x < SCREEN_WIDTH; ++x)
        {
            calculateCell(x, y);
        }
    }
}

void calculateCell(int x, int y)
{
    int neighborCellsCounter = 0;
    if (screenCurrent[position(x - 1, y - 1)] == ALIVE_CELL)
        neighborCellsCounter++;
    if (screenCurrent[position(x    , y - 1)] == ALIVE_CELL)
        neighborCellsCounter++;
    if (screenCurrent[position(x + 1, y - 1)] == ALIVE_CELL)
        neighborCellsCounter++;
    if (screenCurrent[position(x - 1, y    )] == ALIVE_CELL)
        neighborCellsCounter++;
    if (screenCurrent[position(x + 1, y    )] == ALIVE_CELL)
        neighborCellsCounter++;
    if (screenCurrent[position(x - 1, y + 1)] == ALIVE_CELL)
        neighborCellsCounter++;
    if (screenCurrent[position(x    , y + 1)] == ALIVE_CELL)
        neighborCellsCounter++;
    if (screenCurrent[position(x + 1, y + 1)] == ALIVE_CELL)
        neighborCellsCounter++;

    if (screenCurrent[position(x, y)] == DEAD_CELL)
    {//The cell in (x,y) is currently dead
        if (neighborCellsCounter == 3)
            screenNext[position(x, y)] = ALIVE_CELL;
    }
    else
    {//The cell in (x,y) is currently alive
        if (neighborCellsCounter < 2 || neighborCellsCounter > 3)
            screenNext[position(x, y)] = DEAD_CELL;
        else
            screenNext[position(x, y)] = ALIVE_CELL;
    }
}
