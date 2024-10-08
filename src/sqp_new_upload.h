#ifndef ADD_POST_H
#define ADD_POST_H

#if defined(ADD_POST_H)

#include "raylib.h"
#include "macros.h"
#include <string.h>     
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "raygui.h"

//----------------------------------------------------------------------------------
// Defines and Macros
//----------------------------------------------------------------------------------
//...

//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------

typedef struct {
    char variable_type[__UINT8_MAX__];
    char label_name[__UINT8_MAX__];
    char default_value[__UINT8_MAX__];
    bool isRequired;    
    bool isList;    
    bool boxActive;
    bool editMode;


} MarkdownVariable_t;

//----------------------------------------------------------------------------------
// Internal Module Variables Definition
//----------------------------------------------------------------------------------

static const uint16_t maxContentSize = __UINT16_MAX__;
static const uint8_t maxInputChars = __UINT8_MAX__;
static bool isMissingRequiredVariable = false;

static MarkdownVariable_t markdownVariables[] = {
    {"title", "Title", "",                                          true, false, false, false},
    {"date", "Date", "2024-10-03T10:00:00-03:00",                   true, false, false, false},
    {"tags", "Tags", "LowLevelProgrammimg,Neural Networks",         true, true, false, false},
    {"categories", "Categories", "Artificial Inteligence,Python",   true, true, false, false},
    {"description", "Description", "",                              true, false, false, false},
    {"banner", "Banner", "",                                        true, false, false, false},
    {"authors", "Authors", "",                                      true, true, false, false},

};

#define NUM_MARKDOWN_VARIABLES (int) (sizeof(markdownVariables)/sizeof(MarkdownVariable_t))

//----------------------------------------------------------------------------------
// Global Variables Definition
//----------------------------------------------------------------------------------

char markdownContentIn[__UINT16_MAX__] = { 0 };

//----------------------------------------------------------------------------------
// Internal Module Functions Definition
//----------------------------------------------------------------------------------


static char* getMarkdownMainContent(void);

#define UPLOADS_SAVE_FOLDER "uploads/"
#define UPLOADS_SAVE_FILE "uploads/post.md" 

int getFilePath(char *inFilePath, bool isCustomModelDialog, char fileExtension[], 
                char fileDescription[]);

void saveMarkdownPost(void);

int8_t newUpload(void){
    int labelWidth = 200;
    int labelHeight = 30;
    int inputWidth = 500;
    int inputHeight = 30;

    int padding = 20;  // Padding from the edges of the screen
    int xLabel = padding;  // Align labels to the left
    int xInput = labelWidth + padding;  // Align inputs to the right
    int initialY = 150;  // Space from the top
    int yStep = 60;      // Vertical space between elements
    int contentBoxWidth = GetScreenWidth()/2;
    int contentBoxHeight = (NUM_MARKDOWN_VARIABLES + 3) * yStep;
    int xContentBox = labelWidth + padding + inputWidth + padding; // Align content box to the right
    int yContentBox = initialY; // Keep it aligned with other elements

    int result = WINDOW_BAR("New Upload", "", "Deploy to your Gitub Repository");
    for (int i = 0; i < NUM_MARKDOWN_VARIABLES; i++) {
        // Display the label on the left
        if (markdownVariables[i].isRequired) {
            GuiLabel((Rectangle){ xLabel, initialY + i * yStep, labelWidth, labelHeight }, 
                TextFormat("%s*", markdownVariables[i].label_name));
        } else {
            GuiLabel((Rectangle){ xLabel, initialY + i * yStep, labelWidth, labelHeight }, 
            markdownVariables[i].label_name);
        }
        if (markdownVariables[i].editMode) {
            // Allow editing when editMode is true
            if (GuiTextBox((Rectangle){ xInput, initialY + i * yStep, inputWidth, inputHeight }, markdownVariables[i].default_value, maxInputChars, true)) {
                markdownVariables[i].editMode = true;
            }
        } else {
            // Regular display mode
            GuiTextBox((Rectangle){ xInput, initialY + i * yStep, inputWidth, inputHeight }, markdownVariables[i].default_value, maxInputChars, false);
        }

        // Check for mouse click to toggle edit mode
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            Rectangle textBoxRect = (Rectangle){ xInput, initialY + i * yStep, inputWidth, inputHeight };
            if (CheckCollisionPointRec(GetMousePosition(), textBoxRect)) {
                markdownVariables[i].editMode = true; 
            } else {
                markdownVariables[i].editMode = false; 
            }
        }
    }

    if (GuiButton((Rectangle){ xInput, initialY + ((NUM_MARKDOWN_VARIABLES) * yStep), 
        inputWidth, labelHeight }, "Save Draft")) {
            saveMarkdownPost();
        }

    // Button to get image path using file dialog, aligned to the right
    if (GuiButton((Rectangle){ xInput, initialY + ((NUM_MARKDOWN_VARIABLES+1) * yStep), 
        inputWidth, labelHeight }, "Get Banner")) {
            getFilePath(markdownVariables[5].default_value, false, "*.png", "PNG Files (*.png)");
        }

    // Button to get post content, aligned to the right
    if (GuiButton((Rectangle){ xInput, initialY + ((NUM_MARKDOWN_VARIABLES+2) * yStep), 
        inputWidth, labelHeight }, "Get Post Content")) {
        strcpy(markdownContentIn, getMarkdownMainContent()); // Update the content variable
    }

    

    // Display the content box
    GuiLabel((Rectangle){ xContentBox, yContentBox - labelHeight, contentBoxWidth, labelHeight }, "Content* (.md style, max letters: 4096) ");
    GuiTextBoxMulti((Rectangle){ xContentBox, yContentBox, contentBoxWidth, contentBoxHeight }, markdownContentIn, maxInputChars, true);

    if (isMissingRequiredVariable) {
        missingVariableMessage();
    }

    if (result == 0) return -1;
    if (result == 1) {
        return -1;
    }

    return 0;
}
/* Returns 1 if file was selected, 0 if dialog was canceled, -1 if an error occurred */
int getFilePath(char *inFilePath, bool isCustomModelDialog, char fileExtension[], 
                char fileDescription[]) {
    
    GuiLock();
    
    int result;
    if (isCustomModelDialog) {
        result = GuiFileDialog(DIALOG_MESSAGE, "Load file ...",  inFilePath, "Ok", 
                                fileDescription);
    } else {
        result = GuiFileDialog(DIALOG_OPEN_FILE,  "Load file", inFilePath, fileExtension, 
                                fileDescription);
    }
    GuiUnlock();
    return result;
}

void erase() {
    //TODO
}

// Returns 1 if there is a missing required variable, 0 otherwise
static void checkMissingVariable(void) {
    for (int i = 0; i < NUM_MARKDOWN_VARIABLES; i++) {
        if (markdownVariables[i].isRequired && strlen(markdownVariables[i].default_value) == 0) {
            markdownVariables[i].boxActive = true;
            isMissingRequiredVariable = true;
            return;
        }
    }
    isMissingRequiredVariable = false;
}

void missingVariableMessage(void) {
    char** missingVariables = (char **)malloc(NUM_MARKDOWN_VARIABLES * sizeof(char *));
    for (int i = 0; i < NUM_MARKDOWN_VARIABLES; i++) {
        if (markdownVariables[i].boxActive) {
            missingVariables[i] = markdownVariables[i].label_name;
        } else {
            missingVariables[i] = "";
        }
    }


    int isPressedBox = GuiMessageBox((Rectangle){ 0, 0, GetScreenWidth(), GetScreenHeight() }, 
                    "Missing Required Variable", "Please fill in all required variables before saving.", 
                    "Ok");
    if (isPressedBox == 0 || isPressedBox == 1) {
        isMissingRequiredVariable = false;
    }
}

void saveMarkdownPost(void){
    checkMissingVariable();
        
    FILE *file = fopen(UPLOADS_SAVE_FILE, "w+");
    if (file == NULL) {
        printf("Error opening file!\n");
        exit(1);
    }
    
    
    fprintf(file, "+++\n");
    for (int i = 0; i < NUM_MARKDOWN_VARIABLES; i++) {
        if (markdownVariables[i].isList) {
            char *token = strtok(markdownVariables[i].default_value, ",");
            fprintf(file, "%s: [", markdownVariables[i].variable_type);
            while (token != NULL) {
                fprintf(file, "\"%s\"", token);
                token = strtok(NULL, ",");
                if (token != NULL) {
                    fprintf(file, ", ");
                }
            }
            fprintf(file, "]\n");
        } else {

        fprintf(file, "%s: \"%s\"\n", markdownVariables[i].variable_type, 
                markdownVariables[i].default_value);
        }
    }
    fprintf(file, "+++\n");

    fprintf(file, "%s", markdownContentIn);
    fclose(file);

}


char* getMarkdownMainContent(void) {
    static char inFilePath[512] = { 0 };

    if (getFilePath(inFilePath, false, "*.md", "Markdown Files (*.md)") == 1) {
        FILE *file = fopen(inFilePath, "r");
        if (file == NULL) {
            printf("Error opening file!\n");
            exit(1);
        }

        char *content = (char *)malloc(maxContentSize);
        if (content == NULL) {
            printf("Error allocating memory!\n");
            exit(1);  
        }

        // Initialize content to avoid undefined behavior with strcat
        content[0] = '\0'; 

        char line[maxContentSize];
        while (fgets(line, sizeof(line), file)) {
            if (strlen(content) + strlen(line) >= maxContentSize) {
                printf("File content too large!\n");
                free(content);
                fclose(file);

                // Returns only the content read so far
                return content;
            }
            strcat(content, line);
                printf("%s", line);
            #if defined(_DEBUG)
                printf("%s", line);
            #endif  
        }

        fclose(file);

        return content;
    }
    return NULL; // If file was not selected or dialog canceled
}


#endif
#endif