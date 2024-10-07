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

#define MAX_TEXT_SIZE 4096
#define MAX_INPUT_CHARS 64

char* getMarkdownMainContent(void);

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
    {"banner", "Banner", "", false, false},
    {"authors", "Authors *", "", false, false},

};

#define BANNER_IINDEX 5
#define NUM_MARKDOWN_VARIABLES 7
#define UPLOADS_SAVE_FOLDER "uploads/"
#define UPLOADS_SAVE_FILE "uploads/post.md" 

char content[MAX_INPUT_CHARS] = ""; // Variable to store the content.


int getFilePath(char *inFilePath, bool isCustomModelDialog, char fileExtension[], 
                char fileDescription[]);

void saveMarkdownPost(void);

int UpdateTextbox(int screenWidth, int screenHeight, char text[], bool *editingText) {
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

    int result = WINDOW_BAR("Add New Post", "", "Deploy to your Gitub Repository");
    for (int i = 0; i < NUM_MARKDOWN_VARIABLES; i++) {
        // Display the label on the left
        GuiLabel((Rectangle){ xLabel, initialY + i * yStep, labelWidth, labelHeight }, 
            markdownVariables[i].label_name);
        
        if (markdownVariables[i].editMode) {
            // Allow editing when editMode is true
            if (GuiTextBox((Rectangle){ xInput, initialY + i * yStep, inputWidth, inputHeight }, markdownVariables[i].default_value, MAX_INPUT_CHARS, true)) {
                markdownVariables[i].editMode = true;
            }
        } else {
            // Regular display mode
            GuiTextBox((Rectangle){ xInput, initialY + i * yStep, inputWidth, inputHeight }, markdownVariables[i].default_value, MAX_INPUT_CHARS, false);
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
        strcpy(content, getMarkdownMainContent()); // Update the content variable
    }

    

    // Display the content box
    GuiLabel((Rectangle){ xContentBox, yContentBox - labelHeight, contentBoxWidth, labelHeight }, "Content* (.md style, max letters: 4096) ");
    GuiTextBoxMulti((Rectangle){ xContentBox, yContentBox, contentBoxWidth, contentBoxHeight }, content, MAX_INPUT_CHARS, true);

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

void saveMarkdownPost(void){
    FILE *file = fopen(UPLOADS_SAVE_FILE, "w+");
    if (file == NULL) {
        printf("Error opening file!\n");
        exit(1);
    }
    
    fprintf(file, "+++\n");
    for (int i = 0; i < NUM_MARKDOWN_VARIABLES; i++) {
        fprintf(file, "%s: %s\n", markdownVariables[i].variable_type, markdownVariables[i].default_value);
    }
    fprintf(file, "+++\n");

    fprintf(file, "%s", content);

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

        char *content = (char *)malloc(MAX_TEXT_SIZE);
        if (content == NULL) {
            printf("Error allocating memory!\n");
            exit(1);  
        }

        // Initialize content to avoid undefined behavior with strcat
        content[0] = '\0'; 

        char line[MAX_TEXT_SIZE];
        while (fgets(line, sizeof(line), file)) {
            if (strlen(content) + strlen(line) >= MAX_TEXT_SIZE) {
                printf("File content too large!\n");
                free(content);
                fclose(file);

                // Returns only the content read so far
                return content;
            }
            strcat(content, line);
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