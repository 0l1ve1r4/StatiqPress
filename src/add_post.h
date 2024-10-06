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
#define MAX_INPUT_CHARS 64

/*
+++
title = "Introdução às Redes Neurais Artificiais"
date = "2024-10-03T10:00:00+02:00"
tags = ["inteligência artificial", "aprendizado de máquina", "redes neurais"]
categories = ["inteligência artificial"]
description = "Um guia introdutório sobre redes neurais artificiais, cobrindo conceitos como neurônios, camadas, e o processo de aprendizado."
banner = "img/banners/ann-banner.png"
authors = ["Guilherme Oliveira"]
+++

*/

typedef struct {
    char variable_type[__UINT8_MAX__];
    char label_name[__UINT8_MAX__];
    char default_value[__UINT8_MAX__];
    bool boxActive;
    bool editMode;

} MarkdownVariable_t;

static MarkdownVariable_t markdownVariables[] = {
    {"title", "Title *", "", false, false},
    {"date", "Date *", "2024-10-03T10:00:00+02:00", false, false},
    {"tags", "Tags *", "aprendizado de máquina, redes neurais", false, false},
    {"categories", "Categories *", "inteligência artificial", false, false},
    {"description", "Description *", "", false, false},
    {"banner", "Banner", "/desktop/banner.png", false, false},
    {"authors", "Authors *", "", false, false}
};

#define NUM_MARKDOWN_VARIABLES 7
#define POSTS_SAVE_FOLDER "posts/"

int UpdateTextbox(int screenWidth, int screenHeight, char text[], bool *editingText) {
    int labelWidth = 200;
    int labelHeight = 30;
    int inputWidth = 500;
    int inputHeight = 30;
    int xLabel = (GetScreenWidth() - labelWidth) / 2;  // Center horizontally
    int xInput = (GetScreenWidth() - inputWidth) / 2;  // Center inputs horizontally
    int initialY = 150;  // Increase this value for more or less space from the top
    int yStep = 60;      // Increase the step for more vertical space between elements
    int xTextAdjust = 400;  // Adjust text x position to align with text boxes

    // Labels
    
    int result = WINDOW_BAR("Add New Post", "", "Add");

    for (int i = 0; i < NUM_MARKDOWN_VARIABLES; i++) {
        // Display the label
        GuiLabel((Rectangle){ xLabel - xTextAdjust, initialY + i * yStep, labelWidth, labelHeight }, 
            markdownVariables[i].label_name);

        // Handle the textbox for editing
        if (markdownVariables[i].editMode) {
            // Allow editing when editMode is true
            if (GuiTextBox((Rectangle){ xInput, initialY + i * yStep, inputWidth, inputHeight }, markdownVariables[i].default_value, MAX_INPUT_CHARS, true)) {
                // Check if the text box is active and allow editing
                markdownVariables[i].editMode = true;
            }
        } else {
            // Regular display mode
            GuiTextBox((Rectangle){ xInput, initialY + i * yStep, inputWidth, inputHeight }, markdownVariables[i].default_value, MAX_INPUT_CHARS, false);
        }

        // Check for mouse click to toggle edit mode
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            // Check if the mouse is within the textbox area
            Rectangle textBoxRect = (Rectangle){ xInput, initialY + i * yStep, inputWidth, inputHeight };
            if (CheckCollisionPointRec(GetMousePosition(), textBoxRect)) {
                markdownVariables[i].editMode = true; 
            } else {
                markdownVariables[i].editMode = false; 
            }
        }
    }

    if (GuiButton((Rectangle){ xLabel - xTextAdjust, initialY + ((NUM_LABELS+1) * yStep), 
        labelWidth, labelHeight }, "Erase")) {
            erase();
        }

    if (result == 0) return -1;
    if (result == 1) {
        return -1;
    }

    return 0;

}

#endif
#endif