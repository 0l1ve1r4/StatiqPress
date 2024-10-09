/*******************************************************************************************
*
*   raylib projects creator
*
*   FEATURES:
*       - Generate complete build systems: Makefile, VSCode, VS2022
*       - Generate complete GitHub project, ready to upload
-       - Generate preconfigured GitHub Actions, ready to run
*       - WEB: Download generated template as a .zip file
*
*   LIMITATIONS:
*       - Script: build.bat requires Makefile, it could be a cmd/shell script instead
*       - VSCode: Requires compiler tools (make.exe) in the system path
*
*   CONFIGURATION:
*       #define CUSTOM_MODAL_DIALOGS
*           Use custom raygui generated modal dialogs instead of native OS ones
*           NOTE: Avoids including tinyfiledialogs depencency library
*
*   VERSIONS HISTORY:
*       1.0  (26-Sep-2024)  First release
*
*   DEPENDENCIES:
*       raylib 5.5-dev          - Windowing/input management and drawing
*       raygui 4.5-dev          - Immediate-mode GUI controls with custom styling and icons
*       tinyfiledialogs 3.18    - Open/save file dialogs, it requires linkage with comdlg32 and ole32 libs
*       miniz 2.2.0             - Save .zip package file (required for multiple images export)
*
*   COMPILATION (Windows - MinGW):
*       gcc -o $(NAME_PART).exe $(FILE_NAME) -I../../src -lraylib -lopengl32 -lgdi32 -std=c99
*
*
*   LICENSE: zlib/libpng
*
*   Copyright (c) 2024 Ramon Santamaria (@raysan5)
*
*   This software is provided "as-is", without any express or implied warranty. In no event
*   will the authors be held liable for any damages arising from the use of this software.
*
*   Permission is granted to anyone to use this software for any purpose, including commercial
*   applications, and to alter it and redistribute it freely, subject to the following restrictions:
*
*     1. The origin of this software must not be misrepresented; you must not claim that you
*     wrote the original software. If you use this software in a product, an acknowledgment
*     in the product documentation would be appreciated but is not required.
*
*     2. Altered source versions must be plainly marked as such, and must not be misrepresented
*     as being the original software.
*
*     3. This notice may not be removed or altered from any source distribution.
*
**********************************************************************************************/

#include "raylib.h"
#include <stdio.h>

#if defined(PLATFORM_WEB)
    #define CUSTOM_MODAL_DIALOGS            // Force custom modal dialogs usage
    #include <emscripten/emscripten.h>      // Emscripten library - LLVM to JavaScript compiler
#endif

#define RAYGUI_IMPLEMENTATION
#include "external/raygui.h"

#undef RAYGUI_IMPLEMENTATION        // Avoid including raygui implementation again

#define GUI_FILE_DIALOGS_IMPLEMENTATION
#include "gui_file_dialogs.h"       // GUI: File Dialogs

#define GUI_MAIN_TOOLBAR_IMPLEMENTATION
#include "gui_main_toolbar.h"       // GUI: Main Toolbar

#define GUI_WINDOW_ABOUT_IMPLEMENTATION
#include "gui_window_about.h"

#define GUI_WINDOW_HELP_IMPLEMENTATION
#include "gui_window_help.h"

#include "style_modern.h"            // raygui style: modern

// miniz: Single C source file zlib-replacement library
// https://github.com/richgel999/miniz
#include "external/miniz.h"         // ZIP packaging functions definition
#include "external/miniz.c"         // ZIP packaging implementation

// C standard library
#include <stdlib.h>                 // Required for: NULL, calloc(), free()
#include <string.h>                 // Required for: memcpy()
#include <stdint.h>                 // Recquired for: MAX_INT4

//----------------------------------------------------------------------------------
// Defines and Macros
//----------------------------------------------------------------------------------
#if (!defined(_DEBUG) && (defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)))
bool __stdcall FreeConsole(void);       // Close console from code (kernel32.lib)
#endif

// Simple log system to avoid printf() calls if required
// NOTE: Avoiding those calls, also avoids const strings memory usage
#define SUPPORT_LOG_INFO
#if defined(SUPPORT_LOG_INFO)// && defined(_DEBUG)
    #define LOG(...) printf(__VA_ARGS__)
#else
    #define LOG(...)
#endif

//#define BUILD_TEMPLATE_INTO_EXE

//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------

// Project configuration data
// NOTE: Some fields could be not used at the moment
typedef struct ProjectConfig {
    struct {
        int type;                   // Project type to generate: Basic, Advanced, Custom
        char title[64];             // Post Title
        char author[64];            // Post Author
        char description[256];      // Post Descripton
        char tags[64];              // Post Tags
        char category[64];          // Post Category
        char srcBannerPath[256];    // Post banner image path
        char srcContentPath[256];   // Post content path
    } project;
    struct {
        int flags;                      // Hugo, Zola, Jekyll, Eleventy
        char gitRepositoryUrl[256];     // git remote repository (to be cloned)
        char contentFolderPath[256];    // content folder to create the new post
        char imageFolderPath[256];      // image folder
    } building;
} ProjectConfig;

// Packed file entry
// NOTE: Used for template packing and attach to executable
typedef struct PackFileEntry {
    int fileSize;
    int compFileSize;
    char filePath[256];
} PackFileEntry;

//----------------------------------------------------------------------------------
// Module Functions Declaration
//----------------------------------------------------------------------------------

// GUI: Load Source Files Dialog
static void getFilePath(ProjectConfig *config);
static void uploadProject(ProjectConfig *config);

//----------------------------------------------------------------------------------
// Global Variables Definition
//----------------------------------------------------------------------------------

static bool showLoadBannerFileDialog = false;
static bool showLoadMarkdownFileDialog = false;

static bool showLoadResourcePathDialog = false;
static bool showLoadRaylibSourcePathDialog = false;
static bool showLoadCompilerPathDialog = false;
static bool showLoadOutputPathDialog = false;
static bool showExportProjectProgress = false;
static bool showInfoMessagePanel = false;
static const char *infoTitle = NULL;
static const char *infoMessage = NULL;
static const char *infoButton = NULL;

static bool lockBackground = false;

static char **srcFileNameList = NULL;

static bool screenSizeDouble = false; // Scale screen x2 (useful for HighDPI/4K screens)

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
#if defined(BUILD_TEMPLATE_INTO_EXE)
    // Attach template data into generated executable on first run
    int exeFileDataSize = 0;
    unsigned char *exeFileData = LoadFileData(argv[0], &exeFileDataSize);

    // Check if template already added to not add it again
    char fourcc[5] = { 0 };
    memcpy(fourcc, exeFileData + exeFileDataSize - 4, 4);

    if ((fourcc[0] != 'r') || (fourcc[1] != 'p') || (fourcc[2] != 'c') || (fourcc[3] != 'h'))
    {
        // No template data attached to exe, so we attach it
        int packDataSize = 0;
        char *packData = PackDirectoryData(TextFormat("%s/template", GetApplicationDirectory()), &packDataSize);

        int outExeFileDataSize = exeFileDataSize + packDataSize;
        char *outExeFileData = (char *)RL_CALLOC(outExeFileDataSize, 1);
        memcpy(outExeFileData, exeFileData, exeFileDataSize);
        memcpy(outExeFileData + exeFileDataSize, packData, packDataSize);

        SaveFileData(TextFormat("%s.template.exe", GetFileNameWithoutExt(argv[0])), outExeFileData, outExeFileDataSize);

        RL_FREE(outExeFileData);
        RL_FREE(packData);
    }
#endif

#if !defined(_DEBUG)
    SetTraceLogLevel(LOG_NONE);         // Disable raylib trace log messsages
#endif

#if (!defined(_DEBUG) && (defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)))
    // WARNING (Windows): If program is compiled as Window application (instead of console),
    // no console is available to show output info... solution is compiling a console application
    // and closing console (FreeConsole()) when changing to GUI interface
    //FreeConsole();
#endif

    // GUI usage mode - Initialization
    //--------------------------------------------------------------------------------------
    // Initialization
    //---------------------------------------------------------------------------------------
    int screenWidth = 800;
    int screenHeight = 536;

    InitWindow(screenWidth, screenHeight, "StatiqPress");
    GuiMainToolbarState toolbarState = InitGuiMainToolbar();
    SetExitKey(0);

    RenderTexture2D screenTarget = LoadRenderTexture(screenWidth, screenHeight);
    SetTextureFilter(screenTarget.texture, TEXTURE_FILTER_POINT);

#if !defined(PLATFORM_WEB)
    int monitorWidth = GetMonitorWidth(GetCurrentMonitor());
    int monitorHeight = GetMonitorHeight(GetCurrentMonitor());
    if ((GetWindowScaleDPI().x > 1.0f) || (monitorWidth > (screenWidth*2)))
    {
        // NOTE: We need to consider app window title bar and possible OS bottom bar
        if ((monitorHeight - 24 - 40)  > (screenHeight*2))
        {
            screenSizeDouble = true;
            SetWindowSize(screenWidth*2, screenHeight*2);
            SetMouseScale(0.5f, 0.5f);

            SetWindowPosition(monitorWidth/2 - screenWidth, monitorHeight/2 - screenHeight);
        }
    }
#endif

    // Initialize project config default
    ProjectConfig *config = (ProjectConfig *)RL_CALLOC(1, sizeof(ProjectConfig));
    config->project.type = 2;  // Custom files
    strcpy(config->project.title, "");
    strcpy(config->project.author, "");
    strcpy(config->project.description, "");
    strcpy(config->project.tags, "");
    strcpy(config->project.category, "");
    strcpy(config->building.contentFolderPath, "content/blog/");
    strcpy(config->building.gitRepositoryUrl, "https://github.com/Discovery-Data-Lab/blog.git");
    strcpy(config->building.imageFolderPath, "static/img/");

    // Source file names (without path) are used for display on source textbox
    srcFileNameList = (char **)RL_CALLOC(256, sizeof(char *)); // Max number of input source files supported
    for (int i = 0; i < 256; i++) srcFileNameList[i] = (char *)RL_CALLOC(256, sizeof(char));

    // GUI: Main Layout
    //-----------------------------------------------------------------------------------
    Vector2 anchorProject = { 8, 64 };
    Vector2 anchorBuilding = { 8, 258 };

    bool projectNameEditMode = false;
    strcpy(config->project.title, "");
    bool productNameEditMode = false;
    strcpy(config->project.author, "");
    bool projectDeveloperEditMode = false;
    strcpy(config->project.tags, "");
    bool projectDeveloperWebEditMode = false;
    strcpy(config->project.category, "");
    bool projectDescriptionEditMode = false;
    strcpy(config->project.description, "");
    bool projectSourceFilePathEditMode = false;
    bool buildingRaylibPathEditMode = false;
    bool buildingCompilerPathEditMode = false;
    bool buildingOutputPathEditMode = false;

    GuiLoadStyleAmber();    // Load UI style

    GuiEnableTooltip();     // Enable tooltips by default
    //----------------------------------------------------------------------------------

    // GUI: Exit Window
    //-----------------------------------------------------------------------------------
    bool closeWindow = false;
    bool showExitWindow = false;
    //-----------------------------------------------------------------------------------

    infoTitle = "WELCOME! LET'S CREATE A PROJECT!";
    infoMessage = "Provide some source code files (.c) to generate project!";// \nOr choose a default project type!";
    infoButton = "Sure! Let's start!";
    showInfoMessagePanel = false;

    LOG("INIT: Ready to show project generation info...\n");

    SetTargetFPS(60);

    GuiWindowHelpState windowHelpState = InitGuiWindowHelp();
    GuiWindowAboutState windowAboutState = InitGuiWindowAbout();
    bool showIssueReportWindow = false;
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!closeWindow)    // Detect window close button
    {

        // Clicking Logic:
        if (toolbarState.btnHelpPressed) windowHelpState.windowActive = true;       // Help button logic
        if (toolbarState.btnAboutPressed) windowAboutState.windowActive = true;     // About window button logic
        if (toolbarState.btnIssuePressed) showIssueReportWindow = true;             // Issue report window button logic
        //if (toolbarState.btnIssuePressed) showIssueReportWindow = true;             // Issue report window button logic

        // WARNING: ASINCIFY requires this line,
        // it contains the call to emscripten_sleep() for PLATFORM_WEB
        if (WindowShouldClose()) closeWindow = true;

        //----------------------------------------------------------------------------------

        // Keyboard shortcuts
        //------------------------------------------------------------------------------------

        // Basic program flow logic
        //----------------------------------------------------------------------------------
#if !defined(PLATFORM_WEB)
        // Window scale logic to support 4K/HighDPI monitors
        if (IsKeyPressed(KEY_F10))
        {
            screenSizeDouble = !screenSizeDouble;
            if (screenSizeDouble)
            {
                // Screen size x2
                if (GetScreenWidth() < screenWidth*2)
                {
                    SetWindowSize(screenWidth*2, screenHeight*2);
                    SetMouseScale(0.5f, 0.5f);
                    SetWindowPosition(monitorWidth/2 - screenWidth, monitorHeight/2 - screenHeight);
                }
            }
            else
            {
                // Screen size x1
                if (screenWidth*2 >= GetScreenWidth())
                {
                    SetWindowSize(screenWidth, screenHeight);
                    SetMouseScale(1.0f, 1.0f);
                    SetWindowPosition(monitorWidth/2 - screenWidth/2, monitorHeight/2 - screenHeight/2);
                }
            }
        }
#endif

        if (showExitWindow ||
            showInfoMessagePanel ||
            showLoadMarkdownFileDialog ||
            showLoadBannerFileDialog ||
            showLoadResourcePathDialog ||
            showLoadRaylibSourcePathDialog ||
            showLoadCompilerPathDialog ||
            showLoadOutputPathDialog ||
            showExportProjectProgress) lockBackground = true;
        else lockBackground = false;

        if (lockBackground) GuiLock();
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        // Render all screen to texture (for scaling)
        BeginTextureMode(screenTarget);
            ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));

            // GUi: Toolbar
            GuiMainToolbar(&toolbarState, GetScreenWidth());
            GuiWindowHelp(&windowHelpState);
            GuiWindowAbout(&windowAboutState);



            GuiGroupBox((Rectangle){ anchorProject.x + 0, anchorProject.y + 0, 784, 190 }, "PROJECT SETTINGS");
            GuiLabel((Rectangle){ anchorProject.x + 8, anchorProject.y + 24, 104, 24 }, "POST TITLE:");
            GuiSetTooltip("Just the title");
            if (GuiTextBox((Rectangle){ anchorProject.x + 112, anchorProject.y + 24, 280, 24 }, config->project.title, 128, projectNameEditMode)) projectNameEditMode = !projectNameEditMode;

            GuiSetTooltip("For multiple Authors, separate them by comma ','");
            GuiLabel((Rectangle){ anchorProject.x + 408, anchorProject.y + 24, 80, 24 }, "AUTHOR(S):");
            if (GuiTextBox((Rectangle){ anchorProject.x + 496, anchorProject.y + 24, 280, 24 }, config->project.author, 128, productNameEditMode)) productNameEditMode = !productNameEditMode;

            GuiSetTooltip("A short description of the post, max 256 characters");
            GuiLabel((Rectangle){ anchorProject.x + 8, anchorProject.y + 56, 104, 24 }, "DESCRIPTION:");
            if (GuiTextBox((Rectangle){ anchorProject.x + 112, anchorProject.y + 56, 664, 24 }, config->project.description, 128, projectDescriptionEditMode)) projectDescriptionEditMode = !projectDescriptionEditMode;

            GuiSetTooltip("For multiple Tags, separate them by comma ','");
            GuiLabel((Rectangle){ anchorProject.x + 8, anchorProject.y + 88, 104, 24 }, "TAG(S):");
            if (GuiTextBox((Rectangle){ anchorProject.x + 112, anchorProject.y + 88, 280, 24 }, config->project.tags, 128, projectDeveloperEditMode)) projectDeveloperEditMode = !projectDeveloperEditMode;

            GuiSetTooltip("Use just one category");
            GuiLabel((Rectangle){ anchorProject.x + 408, anchorProject.y + 88, 80, 24 }, "CATEGORY:");
            if (GuiTextBox((Rectangle){ anchorProject.x + 496, anchorProject.y + 88, 280, 24 }, config->project.category, 128, projectDeveloperWebEditMode)) projectDeveloperWebEditMode = !projectDeveloperWebEditMode;

            if (config->project.type != 2) GuiDisable();

            GuiSetTooltip("The path to the directory containing the content of the Post");
            GuiLabel((Rectangle){ anchorProject.x + 8, anchorProject.y + 128, 104, 24 }, "SOURCE (.md):");
            GuiSetStyle(TEXTBOX, TEXT_READONLY, 1);
            GuiTextBox((Rectangle){ anchorProject.x + 112, anchorProject.y + 128, 536, 24 }, config->project.srcContentPath, 256, projectSourceFilePathEditMode);//) projectSourceFilePathEditMode = !projectSourceFilePathEditMode;
            GuiSetStyle(TEXTBOX, TEXT_READONLY, 0);
            if (GuiButton((Rectangle){ anchorProject.x + 656, anchorProject.y + 128, 120, 24 }, "#4#Browse")) showLoadMarkdownFileDialog = true;

            GuiSetTooltip("The path to the directory containing the banner for the Post");
            GuiLabel((Rectangle){ anchorProject.x + 8, anchorProject.y + 160, 104, 24 }, "BANNER (.png):");
            GuiSetStyle(TEXTBOX, TEXT_READONLY, 1);
            GuiTextBox((Rectangle){ anchorProject.x + 112, anchorProject.y + 160, 536, 24 }, config->project.srcBannerPath, 256, projectSourceFilePathEditMode);//) projectSourceFilePathEditMode = !projectSourceFilePathEditMode;
            GuiSetStyle(TEXTBOX, TEXT_READONLY, 0);
            if (GuiButton((Rectangle){ anchorProject.x + 656, anchorProject.y + 160, 120, 24 }, "#4#Browse")) showLoadBannerFileDialog = true;

            GuiSetTooltip(NULL);

            GuiEnable();

            GuiGroupBox((Rectangle){ anchorBuilding.x + 0, anchorBuilding.y + 10, 784, 136 }, "BUILD SETTINGS");
            GuiLabel((Rectangle){ anchorBuilding.x + 8, anchorBuilding.y + 16, 104, 24 }, "GITHUB REPO:");
            if (GuiTextBox((Rectangle){ anchorBuilding.x + 112, anchorBuilding.y + 16, 536, 24 }, config->building.gitRepositoryUrl, 128, buildingRaylibPathEditMode)) buildingRaylibPathEditMode = !buildingRaylibPathEditMode;

            if (GuiButton((Rectangle){ anchorBuilding.x + 656, anchorBuilding.y + 16, 120, 24 }, "#4#Browse")) showLoadRaylibSourcePathDialog = true;
            GuiEnable();

            GuiLabel((Rectangle){ anchorBuilding.x + 8, anchorBuilding.y + 48, 104, 24 }, "CONTENT PATH:");
            if (GuiTextBox((Rectangle){ anchorBuilding.x + 112, anchorBuilding.y + 48, 536, 24 }, config->building.contentFolderPath, 128, buildingCompilerPathEditMode)) buildingCompilerPathEditMode = !buildingCompilerPathEditMode;
            GuiEnable();

            GuiLabel((Rectangle){ anchorBuilding.x + 8, anchorBuilding.y + 80, 104, 24 }, "IMAGES PATH:");
            if (GuiTextBox((Rectangle){ anchorBuilding.x + 112, anchorBuilding.y + 80, 536, 24 }, config->building.imageFolderPath, 128, buildingOutputPathEditMode)) buildingOutputPathEditMode = !buildingOutputPathEditMode;
            if (GuiButton((Rectangle){ anchorBuilding.x + 656, anchorBuilding.y + 80, 120, 24 }, "#4#Browse")) showLoadOutputPathDialog = true;
            GuiEnable();

            if (GuiButton((Rectangle){ anchorBuilding.x + 656, anchorBuilding.y + 48, 120, 24 }, "#4#Browse")) showLoadCompilerPathDialog = true;
            GuiEnable();
            GuiLabel((Rectangle){ anchorBuilding.x + 8, anchorBuilding.y + 110, 104, 32 }, "BUILD SYSTEMS:");

            if (!lockBackground) GuiLock();
            bool buildSystem = true;
            GuiToggle((Rectangle){ anchorBuilding.x + 112, anchorBuilding.y + 110, 100, 32 }, "Hugo", &buildSystem);
            GuiToggle((Rectangle){ anchorBuilding.x + 112 + 166, anchorBuilding.y + 110, 100, 32 }, "Zola", &buildSystem);
            GuiToggle((Rectangle){ anchorBuilding.x + 112 + 166*2, anchorBuilding.y + 110, 100, 32 }, "Jekyll", &buildSystem);
            GuiToggle((Rectangle){ anchorBuilding.x + 112 + 166*3, anchorBuilding.y + 110, 100, 32 }, "Eleventy", &buildSystem);
            if (!lockBackground) GuiUnlock();


            //if (config->project.srcFileCount == 0) GuiDisable();
            if (GuiButton((Rectangle){ 8, 450, 784, 40 }, "#7#UPLOAD POST TO YOUR SITE"))
            {
                showExportProjectProgress = true;
            }
            //GuiEnable();

            if (!lockBackground && CheckCollisionPointRec(GetMousePosition(), (Rectangle){ 0, GetScreenHeight() - 32, screenWidth, 32 })) SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
            else SetMouseCursor(MOUSE_CURSOR_DEFAULT);


            if (GuiButton((Rectangle){ 0, screenHeight - 32, screenWidth, 32 },
                "#15#Did you find this tool useful? Check More about the author!")) {
                OpenURL("https://0l1ve1r4.netlify.app/");
            }

            if (showIssueReportWindow) {
                Rectangle messageBox = { (float)screenWidth/2 - 300/2, (float)screenHeight/2 - 190/2 - 20, 300, 190 };
                int result = GuiMessageBox(messageBox, "#220#Report Issue",
                            "Do you want to report any issue or\nfeature request for this program?\n\n"
                            "https://github.com/0l1ve1r4/StatiqPress", "#186#Report on GitHub");

                // Report issue pressed
                if (result == 1) {
                    OpenURL("https://github.com/0l1ve1r4/StatiqPress/issues");
                    showIssueReportWindow = false;
                }
                else if (result == 0) showIssueReportWindow = false;
            }

            //----------------------------------------------------------------------------------

            // NOTE: If some overlap window is open and main window is locked, we draw a background rectangle
            if (lockBackground) DrawRectangle(0, 0, screenWidth, screenHeight, Fade(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)), 0.85f));

            // WARNING: Before drawing the windows, we unlock them
            GuiUnlock();

            // GUI: Show info message panel
            //----------------------------------------------------------------------------------------
            if (showInfoMessagePanel)
            {
                Vector2 textSize = MeasureTextEx(GuiGetFont(), infoMessage, GuiGetFont().baseSize*2, 3);
                GuiPanel((Rectangle){ -10, screenHeight/2 - 180, screenWidth + 20, 290 }, NULL);

                GuiSetStyle(DEFAULT, TEXT_SIZE, GuiGetFont().baseSize*3);
                GuiSetStyle(DEFAULT, TEXT_SPACING, 3);
                GuiSetStyle(LABEL, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);
                GuiSetStyle(LABEL, TEXT_COLOR_NORMAL, GuiGetStyle(DEFAULT, TEXT_COLOR_FOCUSED));
                GuiLabel((Rectangle){ -10, screenHeight/2 - 140, screenWidth + 20, 30 }, infoTitle);
                GuiSetStyle(LABEL, TEXT_COLOR_NORMAL, GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL));
                GuiSetStyle(DEFAULT, TEXT_SIZE, GuiGetFont().baseSize*2);
                GuiLabel((Rectangle){ -10, screenHeight/2 - textSize.y - 30, screenWidth + 20, 30 }, infoMessage);

                if (GuiButton((Rectangle){ screenWidth/4, screenHeight/2 + 40, screenWidth/2, 40 }, infoButton))
                {
                    showInfoMessagePanel = false;

                    infoTitle = "WARNING! READ CAREFULLY!";
                    infoMessage = NULL;
                    infoButton = "I understand implications";
                }

                GuiSetStyle(LABEL, TEXT_ALIGNMENT, TEXT_ALIGN_LEFT);
                GuiSetStyle(DEFAULT, TEXT_SIZE, GuiGetFont().baseSize);
                GuiSetStyle(DEFAULT, TEXT_SPACING, 1);
            }
            //----------------------------------------------------------------------------------------

            // GUI: Exit Window
            //----------------------------------------------------------------------------------------
            if (showExitWindow)
            {
                int result = GuiMessageBox((Rectangle){ (float)screenWidth/2 - 125, (float)screenHeight/2 - 50, 250, 100 }, "#159#Closing raylib project creator", "Do you really want to exit?", "Yes;No");

                if ((result == 0) || (result == 2)) showExitWindow = false;
                else if (result == 1) closeWindow = true;
            }

            //----------------------------------------------------------------------------------------

            // GUI: Load Files Dialog
            getFilePath(config);

            // GUI: Upload Post Dialog
            if (showExportProjectProgress) uploadProject(config);

        EndTextureMode();

        BeginDrawing();
            ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));

            // Draw render texture to screen
            if (screenSizeDouble) DrawTexturePro(screenTarget.texture, (Rectangle){ 0, 0, (float)screenTarget.texture.width, -(float)screenTarget.texture.height }, (Rectangle){ 0, 0, (float)screenTarget.texture.width*2, (float)screenTarget.texture.height*2 }, (Vector2){ 0, 0 }, 0.0f, WHITE);
            else DrawTextureRec(screenTarget.texture, (Rectangle){ 0, 0, (float)screenTarget.texture.width, -(float)screenTarget.texture.height }, (Vector2){ 0, 0 }, WHITE);
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    RL_FREE(config);

    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}


static void getFilePath(ProjectConfig *config){
    char fileName[UINT8_MAX] = { 0 };

    if (showLoadMarkdownFileDialog){
        int result = GuiFileDialog(DIALOG_OPEN_FILE, "Load source file(s)...",
                                    fileName, "*.md", "Markdown File (*.md)");
        if (result == 1) {
            showLoadMarkdownFileDialog = false;
            strcpy(config->project.srcContentPath, fileName);
        }

        else if (result >= 0) {
            showLoadMarkdownFileDialog = false;
        }

    }

    else if (showLoadBannerFileDialog) {
        int result = GuiFileDialog(DIALOG_OPEN_FILE, "Load the banner image...",
            fileName, "*.png;*.jpg;*.jpeg", "Image File (*.png;*.jpg;*.jpeg)");

        if (result == 1) {
            showLoadBannerFileDialog = false;
            printf("%s", config->project.srcBannerPath);
            strcpy(config->project.srcBannerPath, fileName);
        }

        else if (result >= 0) {
            showLoadBannerFileDialog = false;
        }
    }

}

static void uploadProject(ProjectConfig *config){
    printf("%s\n", config->project.title);
    printf("%s\n", config->project.author);
    printf("%s\n", config->project.description);
    printf("%s\n", config->project.tags);
    printf("%s\n", config->project.category);
    printf("%s\n", config->project.srcContentPath);
    printf("%s\n", config->project.srcBannerPath);

    printf("%s\n", config->building.gitRepositoryUrl); // github
    printf("%s\n", config->building.contentFolderPath); // content/blog
    printf("%s\n", config->building.imageFolderPath); // static/img

    exit(0);

}
