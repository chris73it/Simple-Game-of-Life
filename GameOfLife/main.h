#pragma once

void drawBlankField(wchar_t* screen);
void glider(wchar_t* screen, size_t x, size_t y);
void calculateNextGeneration();
void calculateCell(size_t x, size_t y);
