#include "raylib.h"
#include "../external/raygui.h"
#include <stdio.h>
#include <stdlib.h>

// Function to read a file's content and return it as a string
char* read_markdown_file(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file %s\n", filename);
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long filesize = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* content = (char*)malloc(filesize + 1);
    fread(content, 1, filesize, file);
    content[filesize] = '\0';  // Null-terminate the string

    fclose(file);
    return content;
}

// Main function to display markdown content using raygui
void display_markdown(const char* filepath) {
    // Initialize the raylib window
    InitWindow(800, 600, "Markdown Viewer");

    // Read the markdown file
    char* markdown_content = read_markdown_file(filepath);
    if (markdown_content == NULL) {
        CloseWindow();
        return;
    }

    bool exitWindow = false;
    
    // Main application loop
    while (!WindowShouldClose()) {
        // Start drawing
        BeginDrawing();
        ClearBackground(RAYWHITE);
        
        // Display the markdown content using raygui's text box (non-editable)
        GuiTextBox((Rectangle){ 50, 50, 700, 500 }, markdown_content, 512, false);

        EndDrawing();
    }

    // Clean up
    free(markdown_content);
    CloseWindow();
}

int main() {
    // Call the display_markdown function with the path to your markdown file
    display_markdown("path/to/your/markdown/file.md");
    
    return 0;
}

// gcc -o markdown_viewer markdown_viewer.c -lraylib -lraygui -lm -lpthread -ldl -lrt -lX11
