#ifndef MACROS_H
#define MACROS_H

#include "raygui.h"

#define WINDOW_BAR_X 0  // Padding of the window bar
#define WINDOW_BAR_Y 60 // Padding of the window bar
#define WINDOW_BAR_WIDTH GetScreenWidth()
#define WINDOW_BAR_HEIGHT GetScreenHeight() - WINDOW_BAR_Y

#define WINDOW_BAR_BOX (Rectangle) { WINDOW_BAR_X , WINDOW_BAR_Y, WINDOW_BAR_WIDTH, WINDOW_BAR_HEIGHT }

// Top Text, middle text, end text.
#define WINDOW_BAR(top, middle, end) GuiMessageBox(WINDOW_BAR_BOX, top, middle, end)



#endif