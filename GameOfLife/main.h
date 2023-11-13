#pragma once

size_t position(int x, int y);

void drawBlankField(wchar_t* screen);

void cell(wchar_t* screen, int x, int y);

void glider(int x, int y);

void calculateNextGeneration();

void calculateCell(int x, int y);
