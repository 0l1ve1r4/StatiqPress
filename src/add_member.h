#ifndef ADD_MEMBER_H
#define ADD_MEMBER_H

#if defined(ADD_MEMBER_H)

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

#define MAX_INPUT_CHARS 64

typedef struct {
    char yaml_syntax[MAX_INPUT_CHARS];
    char label[MAX_INPUT_CHARS];
    char input[MAX_INPUT_CHARS];
    bool boxActive;
    bool editMode;
} Label_t;

static Label_t labels[] = {
    {"name", "Name*", "", false, false},
    {"email", "Email *", "", false, false},
    {"position", "Position", "Student / Teacher / Researcher", false, false},
    {"avatar", "Avatar Path", "/home/yks/Desktop/avatar.png", false, false},
    {"linkedin_link", "Linkedin Link", "https://www.linkedin.com/in/YOU/", false, false},
    {"github_link", "Github Link", "https://github.com/YOU", false, false}, 
    {"personalSite_link", "Personal Site", "", false, false}, 
    {"text", "Your Description", "I am... I do... I like..", false, false}
}; 

#define NUM_LABELS 8
#define SAVE_FOLDER "member/"

void saveYamlFile(Label_t members[]) {
    char filename[MAX_INPUT_CHARS * 2];
    snprintf(filename, sizeof(filename), "%s%s.yaml", SAVE_FOLDER,members[0].input);

    printf("Saving to %s\n", filename);

    FILE* fptr = fopen(filename, "w");
    if (fptr == NULL) {
        fprintf(stderr, "Failed to open file.\n");
        return;
    }

    for (int i = 0; i < NUM_LABELS; i++) {
        char yaml_variable[MAX_INPUT_CHARS + 4];

        // Populate yaml_variable
        snprintf(yaml_variable, sizeof(yaml_variable), "%s: %s\n", members[i].yaml_syntax, members[i].input);
        
        // Write to file
        fputs(yaml_variable, fptr);
    }

    fclose(fptr); // Close the file when done
}

void send_to_git(void) {
    // Send the file to git
    
}

void deploy(Label_t members[]) {
    saveYamlFile(members);
    send_to_git();
}

void erase(void) {
    // Clear the labels by resetting each element
    const Label_t newLabels[NUM_LABELS] = {
    {"name", "Name*", "", false, false},
    {"email", "Email *", "", false, false},
    {"position", "Position", "Student / Teacher / Researcher", false, false},
    {"avatar", "Avatar Path", "/home/yks/Desktop/avatar.png", false, false},
    {"linkedin_link", "Linkedin Link", "https://www.linkedin.com/in/YOU/", false, false},
    {"github_link", "Github Link", "https://github.com/YOU", false, false}, 
    {"personalSite_link", "Personal Site", "", false, false}, 
    {"text", "Your Description", "I am... I do... I like..", false, false}
}; 

    // Copy new labels into the existing labels array
    for (int i = 0; i < NUM_LABELS; i++) {
        labels[i] = newLabels[i];
    }
}

int addNewMember(void){
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
    
    int result = WINDOW_BAR("Add New Member", "", "Add");

    for (int i = 0; i < NUM_LABELS; i++) {
        // Display the label
        GuiLabel((Rectangle){ xLabel - xTextAdjust, initialY + i * yStep, labelWidth, labelHeight }, labels[i].label);

        // Handle the textbox for editing
        if (labels[i].editMode) {
            // Allow editing when editMode is true
            if (GuiTextBox((Rectangle){ xInput, initialY + i * yStep, inputWidth, inputHeight }, labels[i].input, MAX_INPUT_CHARS, true)) {
                // Check if the text box is active and allow editing
                labels[i].editMode = true;
            }
        } else {
            // Regular display mode
            GuiTextBox((Rectangle){ xInput, initialY + i * yStep, inputWidth, inputHeight }, labels[i].input, MAX_INPUT_CHARS, false);
        }

        // Check for mouse click to toggle edit mode
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            // Check if the mouse is within the textbox area
            Rectangle textBoxRect = (Rectangle){ xInput, initialY + i * yStep, inputWidth, inputHeight };
            if (CheckCollisionPointRec(GetMousePosition(), textBoxRect)) {
                labels[i].editMode = true; 
            } else {
                labels[i].editMode = false; 
            }
        }
    }

    int erasePressed = GuiButton((Rectangle){ xLabel - xTextAdjust, 
        initialY + ((NUM_LABELS+1) * yStep), labelWidth, labelHeight }, "Erase");

    if (result == 0) return -1;
    if (result == 1) {
        deploy(labels);
        return -1;
    }

    if (erasePressed) erase(); 

    return 0;

}


#endif
#endif