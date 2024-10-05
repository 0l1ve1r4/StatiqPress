#include "raylib.h"
#include "../src/external/raygui.h"
#include <string.h>

#define MAX_TEXT_SIZE 1024

void UpdateTextbox(int screenWidth, int screenHeight, char text[], bool *editingText) {

    // Draw the text box and manage editing mode
    bool boxActive = GuiTextBox((Rectangle){50, 70, screenWidth - 90, screenHeight - 120}, text, MAX_TEXT_SIZE, *editingText);

    // Check if the text box is active and editing
    if (boxActive) {
        *editingText = true; // Ensure we are in editing mode
    } else if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        // Toggle editing mode when clicking outside the textbox
        *editingText = false;
    }

    // Handle input for multi-line text only if editing
    if (*editingText) {
        // Check for Enter key for newline
        if (IsKeyPressed(KEY_ENTER)) {
            // Append a newline character if there's space
            if (strlen(text) < MAX_TEXT_SIZE - 1) {
                strcat(text, "\n");  // Append a newline character
            }
        }

        // Handle Backspace key
        if (IsKeyPressed(KEY_BACKSPACE) && strlen(text) > 0) {
            text[strlen(text) - 1] = '\0'; // Remove the last character
        }
    }
}

void DrawTextBox(char text[]) {
    // Draw each line of text
    char *line = strtok(text, "\n"); // Tokenize text by newlines
    int lineHeight = 24; // Example line height (adjust as necessary)

    int yPosition = 70; // Starting Y position
    while (line != NULL) {
        DrawText(line, 50, yPosition, 20, DARKGRAY); // Draw the line of text
        yPosition += lineHeight; // Move down for the next line
        line = strtok(NULL, "\n"); // Get the next line
    }
}

int main(void) {
    // Initialization
    const int screenWidth = 800;
    const int screenHeight = 600;
    InitWindow(screenWidth, screenHeight, "Multi-line Text Box Example");

    char text[MAX_TEXT_SIZE] = {0}; // Initialize text buffer
    bool editingText = false; // Editing mode initially off

    SetTargetFPS(60); // Set the game to run at 60 frames-per-second

    while (!WindowShouldClose()) {
        UpdateTextbox(screenWidth, screenHeight, text, &editingText); // Update the textbox

        // Drawing
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("Type something:", 50, 40, 20, DARKGRAY);

        // Draw the updated text box content
        DrawTextBox(text);
        
        EndDrawing();
    }

    CloseWindow(); // Close window and OpenGL context
    return 0;
}
//gcc -o markdowneditor editor.c -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
