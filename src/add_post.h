#ifndef ADD_POST_H
#define ADD_POST_H

#if defined(ADD_POST_H)

#include "raylib.h"
#include "macros.h"
#include <string.h>     
#include <stdlib.h>
#include <stdio.h>

#if defined(PLATFORM_DESKTOP) && !defined(CUSTOM_MODAL_DIALOGS)
    #include "external/tinyfiledialogs.h"  
#else
    #include "raygui.h"
#endif

#define MAX_TEXT_SIZE 2048

void UpdateTextbox(int screenWidth, int screenHeight, char text[], bool *editingText) {
    // Draw the text box and manage editing mode
    
    WINDOW_BAR("Add new Post", "", "Send");
    
    bool boxActive = GuiTextBox((Rectangle){50, 70, screenWidth - 90, screenHeight - 120}, text, MAX_TEXT_SIZE, *editingText);

    // Check if the text box is active and editing
    if (boxActive) {
        *editingText = true; // Ensure we are in editing mode
    } else if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        // Toggle editing mode when clicking outside the textbox
        *editingText = false;
    }
}

#endif
#endif