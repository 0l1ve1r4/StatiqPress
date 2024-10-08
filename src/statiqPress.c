/*******************************************************************************************
*
*   rGuiIcons v3.1 - A simple and easy-to-use raygui icons editor
*
*   FEATURES:
*       - Icon editing and preview at multiple sizes
*       - Cut, copy, paste icons for easy editing
*       - Undo/Redo system for icon changes
*       - Save and load as binary iconset file .md
*       - Export iconset as an embeddable code file (.h)
*       - Export iconset as a .png black&white image
*       - Icon name ids exported as standard PNG chunk (zTXt)
*       - Multiple UI styles for tools reference
*       - +200 custom icons for reference and basic edition
*
*   LIMITATIONS:
*       - Limitation 01
*       - Limitation 02
*
*   POSSIBLE IMPROVEMENTS:
*       - Improvement 01
*       - Improvement 02
*
*   CONFIGURATION:
*       #define CUSTOM_MODAL_DIALOGS
*           Use custom raygui generated modal dialogs instead of native OS ones
*           NOTE: Avoids including tinyfiledialogs depencency library
*
*   VERSIONS HISTORY:
*       3.1  (06-Apr-2024)  ADDED: Report Issue/Features window (Open GitHub)
*                           ADDED: New icons: WARNING, HELP_BOX, INFO_BOX
*                           REMOVED: Sponsors window
*                           REVIEWED: Main toolbar and help window
*                           UPDATED: Using raylib 5.1-dev and raygui 4.1-dev
*
*       3.0  (20-Sep-2023)  ADDED: Support macOS builds (x86_64 + arm64)
*                           ADDED: New icons for code/text editor tools
*                           REVIEWED: CUBE_* icons aspect for 3d
*                           REVIEWED: UI lock when some window shown
*                           REVIEWED: Disabled sponsors window at launch
*                           REVIEWED: Regenerated tool imagery
*                           UPDATED: Using raylib 4.6-dev and raygui 4.0
*
*       2.2  (13-Dec-2022)  ADDED: Welcome window with sponsors info
*                           REDESIGNED: Main toolbar to add tooltips
*                           REVIEWED: Help window implementation
*
*       2.1  (06-Oct-2022)  ADDED: Multiple new icons
*                           ADDED: Sponsor window for tools support
*                           REDESIGN: Iconset for editing is independant of raygui iconset
*                           Updated to raylib 4.5-dev and raygui 3.5-dev
*
*       2.0  (02-Oct-2022)  REDESIGNED: Main toolbar, for consistency with other tools
*                           ADDED: New UI visual styles
*                           Updated to raylib 4.2 and raygui 3.2
*                           Source code re-licensed to open-source
*
*       1.5  (30-Dec-2021)  ADDED: Icon descriptions as PNG extra chunks
*                           ADDED: Support multiple visual styles
*                           Updated to raylib 4.0 and raygui 3.1
*
*       1.0  (30-Sep-2019)  First release
*
*   DEPENDENCIES:
*       raylib 5.5-dev          - Windowing/input management and drawing
*       raygui 4.5-dev          - Immediate-mode GUI controls with custom styling and icons
*       rpng 1.1                - PNG chunks management
*       tinyfiledialogs 3.18    - Open/save file dialogs, it requires linkage with comdlg32 and ole32 libs
*
*   BUILDING:
*     - Windows (MinGW-w64):
*       gcc -o rguiicons.exe rguiicons.c external/tinyfiledialogs.c -s rguiicons.rc.data -Iexternal /
*           -lraylib -lopengl32 -lgdi32 -lcomdlg32 -lole32 -std=c99 -Wall
*
*     - Linux (GCC):
*       gcc -o rguiicons rguiicons.c external/tinyfiledialogs.c -s -Iexternal -no-pie -D_DEFAULT_SOURCE /
*           -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
*
*   ADDITIONAL NOTES:
*       On PLATFORM_ANDROID and PLATFORM_WEB file dialogs are not available
*
*   DEVELOPERS:
*       Ramon Santamaria (@raysan5):   Developer, supervisor, updater and maintainer.
*
*
*   LICENSE: zlib/libpng
*
*   Copyright (c) 2019-2024 raylib technologies (@raylibtech) / Ramon Santamaria (@raysan5)
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

#define TOOL_NAME               "StatiqPress"
#define TOOL_SHORT_NAME         "SP"
#define TOOL_VERSION            "1.0"
#define TOOL_DESCRIPTION        "A simple and easy-to upload Blogs"
#define TOOL_DESCRIPTION_BREAK  "A simple and easy-to upload\n Blogs to the Github-based Static Sites"
#define TOOL_RELEASE_DATE       "Oct.2024"
#define TOOL_LOGO_COLOR         0x48c9c5ff

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wempty-body"

#include "raylib.h"

// NOTE: Some raygui elements need to be defined before including raygui
#define RAYGUI_TEXTSPLIT_MAX_ITEMS        256
#define RAYGUI_TOGGLEGROUP_MAX_ITEMS      256
#define RAYGUI_GRID_ALPHA                 0.2f
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"                         // Required for: IMGUI controls

#undef RAYGUI_IMPLEMENTATION                // Avoid including raygui implementation again

#define GUI_MAIN_TOOLBAR_IMPLEMENTATION
#include "gui_main_toolbar.h"               // GUI: Main toolbar

#define GUI_WINDOW_HELP_IMPLEMENTATION
#include "gui_window_help.h"                // GUI: Help Window

#define GUI_WINDOW_ABOUT_IMPLEMENTATION
#include "gui_window_about.h"               // GUI: About Window

#define GUI_FILE_DIALOGS_IMPLEMENTATION
#include "gui_file_dialogs.h"               // GUI: File Dialogs

#define RPNG_IMPLEMENTATION
#include "external/rpng.h"                  // PNG chunks management

#pragma GCC diagnostic pop

#define GUI_NEW_POST_IMPLEMENTATION
#include "sqp_new_upload.h"                     // GUI: Add Member Window

// raygui embedded styles
// NOTE: Included in the same order as selector
#define MAX_GUI_STYLES_AVAILABLE   13       // NOTE: Included light style
#include "styles/style_jungle.h"            // raygui style: jungle
#include "styles/style_candy.h"             // raygui style: candy
#include "styles/style_lavanda.h"           // raygui style: lavanda
#include "styles/style_cyber.h"             // raygui style: cyber
#include "styles/style_terminal.h"          // raygui style: terminal
#include "styles/style_ashes.h"             // raygui style: ashes
#include "styles/style_bluish.h"            // raygui style: bluish
#include "styles/style_dark.h"              // raygui style: dark
#include "styles/style_cherry.h"            // raygui style: cherry
#include "styles/style_sunny.h"             // raygui style: sunny
#include "styles/style_enefete.h"           // raygui style: enefete
#include "styles/style_modern.h"             // raygui style: amber

#include <stdio.h>                          // Required for: fopen(), fclose(), fread()...
#include <stdlib.h>                         // Required for: malloc(), free()
#include <string.h>                         // Required for: strcmp(), strlen()
#include <stdio.h>                          // Required for: FILE, fopen(), fread(), fwrite(), ftell(), fseek() fclose()

//----------------------------------------------------------------------------------
// Defines and Macros
//----------------------------------------------------------------------------------
#if (!defined(_DEBUG) && (defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)))
bool __stdcall FreeConsole(void);       // Close console from code (kernel32.lib)
#endif

// Simple log system to avoid printf() calls if required
// NOTE: Avoiding those calls, also avoids const strings memory usage
#define SUPPORT_LOG_INFO
#if defined(SUPPORT_LOG_INFO) && defined(_DEBUG)
    #define LOG(...) printf(__VA_ARGS__)
#else
    #define LOG(...)
#endif

#define RGI_BIT_CHECK(a,b)  ((a) &  (1u<<(b)))
#define RGI_BIT_SET(a,b)    ((a) |= (1u<<(b)))
#define RGI_BIT_CLEAR(a,b)  ((a) &= ~((1u)<<(b)))

#define MAX_UNDO_LEVELS         10      // Undo levels supported for the ring buffer

//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------

// Gui icon type (one icon info)
// NOTE: This structure is not used at the moment,
// All icons are joined in a single guiIcons[] array
// and icons name id is actually defined as enum values
/*
typedef struct GuiIcon {
    unsigned int data[RAYGUI_ICON_DATA_ELEMENTS];
    unsigned char nameId[RAYGUI_ICON_MAX_NAME_LENGTH];
} GuiIcon;
*/

// Full icons set
// NOTE: Used for undo/redo system
typedef struct GuiIconSet {
    unsigned int count;
    unsigned int iconSize;
    unsigned int *values;
} GuiIconSet;

//----------------------------------------------------------------------------------
// Global Variables Definition
//----------------------------------------------------------------------------------
static const char *toolName = TOOL_NAME;
static const char *toolVersion = TOOL_VERSION;
static const char *toolDescription = TOOL_DESCRIPTION;

// NOTE: Max length depends on OS, in Windows MAX_PATH = 256
static char inFileName[512] = { 0 };        // Input file name (required in case of drag & drop over executable)

static bool saveChangesRequired = false;    // Flag to notice save changes are required

// Icons name id text
static char guiIconsName[RAYGUI_ICON_MAX_ICONS][32] = {
    "NONE",
    "FOLDER_FILE_OPEN",
    "FILE_SAVE_CLASSIC",
    "FOLDER_OPEN",
    "FOLDER_SAVE",
    "FILE_OPEN",
    "FILE_SAVE",
    "FILE_EXPORT",
    "FILE_ADD",
    "FILE_DELETE",
    "FILETYPE_TEXT",
    "FILETYPE_AUDIO",
    "FILETYPE_IMAGE",
    "FILETYPE_PLAY",
    "FILETYPE_VIDEO",
    "FILETYPE_INFO",
    "FILE_COPY",
    "FILE_CUT",
    "FILE_PASTE",
    "CURSOR_HAND",
    "CURSOR_POINTER",
    "CURSOR_CLASSIC",
    "PENCIL",
    "PENCIL_BIG",
    "BRUSH_CLASSIC",
    "BRUSH_PAINTER",
    "WATER_DROP",
    "COLOR_PICKER",
    "RUBBER",
    "COLOR_BUCKET",
    "TEXT_T",
    "TEXT_A",
    "SCALE",
    "RESIZE",
    "FILTER_POINT",
    "FILTER_BILINEAR",
    "CROP",
    "CROP_ALPHA",
    "SQUARE_TOGGLE",
    "SYMMETRY",
    "SYMMETRY_HORIZONTAL",
    "SYMMETRY_VERTICAL",
    "LENS",
    "LENS_BIG",
    "EYE_ON",
    "EYE_OFF",
    "FILTER_TOP",
    "FILTER",
    "TARGET_POINT",
    "TARGET_SMALL",
    "TARGET_BIG",
    "TARGET_MOVE",
    "CURSOR_MOVE",
    "CURSOR_SCALE",
    "CURSOR_SCALE_RIGHT",
    "CURSOR_SCALE_LEFT",
    "UNDO",
    "REDO",
    "REREDO",
    "MUTATE",
    "ROTATE",
    "REPEAT",
    "SHUFFLE",
    "EMPTYBOX",
    "TARGET",
    "TARGET_SMALL_FILL",
    "TARGET_BIG_FILL",
    "TARGET_MOVE_FILL",
    "CURSOR_MOVE_FILL",
    "CURSOR_SCALE_FILL",
    "CURSOR_SCALE_RIGHT_FILL",
    "CURSOR_SCALE_LEFT_FILL",
    "UNDO_FILL",
    "REDO_FILL",
    "REREDO_FILL",
    "MUTATE_FILL",
    "ROTATE_FILL",
    "REPEAT_FILL",
    "SHUFFLE_FILL",
    "EMPTYBOX_SMALL",
    "BOX",
    "BOX_TOP",
    "BOX_TOP_RIGHT",
    "BOX_RIGHT",
    "BOX_BOTTOM_RIGHT",
    "BOX_BOTTOM",
    "BOX_BOTTOM_LEFT",
    "BOX_LEFT",
    "BOX_TOP_LEFT",
    "BOX_CENTER",
    "BOX_CIRCLE_MASK",
    "POT",
    "ALPHA_MULTIPLY",
    "ALPHA_CLEAR",
    "DITHERING",
    "MIPMAPS",
    "BOX_GRID",
    "GRID",
    "BOX_CORNERS_SMALL",
    "BOX_CORNERS_BIG",
    "FOUR_BOXES",
    "GRID_FILL",
    "BOX_MULTISIZE",
    "ZOOM_SMALL",
    "ZOOM_MEDIUM",
    "ZOOM_BIG",
    "ZOOM_ALL",
    "ZOOM_CENTER",
    "BOX_DOTS_SMALL",
    "BOX_DOTS_BIG",
    "BOX_CONCENTRIC",
    "BOX_GRID_BIG",
    "OK_TICK",
    "CROSS",
    "ARROW_LEFT",
    "ARROW_RIGHT",
    "ARROW_DOWN",
    "ARROW_UP",
    "ARROW_LEFT_FILL",
    "ARROW_RIGHT_FILL",
    "ARROW_DOWN_FILL",
    "ARROW_UP_FILL",
    "AUDIO",
    "FX",
    "WAVE",
    "WAVE_SINUS",
    "WAVE_SQUARE",
    "WAVE_TRIANGULAR",
    "CROSS_SMALL",
    "PLAYER_PREVIOUS",
    "PLAYER_PLAY_BACK",
    "PLAYER_PLAY",
    "PLAYER_PAUSE",
    "PLAYER_STOP",
    "PLAYER_NEXT",
    "PLAYER_RECORD",
    "MAGNET",
    "LOCK_CLOSE",
    "LOCK_OPEN",
    "CLOCK",
    "TOOLS",
    "GEAR",
    "GEAR_BIG",
    "BIN",
    "HAND_POINTER",
    "LASER",
    "COIN",
    "EXPLOSION",
    "1UP",
    "PLAYER",
    "PLAYER_JUMP",
    "KEY",
    "DEMON",
    "TEXT_POPUP",
    "GEAR_EX",
    "CRACK",
    "CRACK_POINTS",
    "STAR",
    "DOOR",
    "EXIT",
    "MODE_2D",
    "MODE_3D",
    "CUBE",
    "CUBE_FACE_TOP",
    "CUBE_FACE_LEFT",
    "CUBE_FACE_FRONT",
    "CUBE_FACE_BOTTOM",
    "CUBE_FACE_RIGHT",
    "CUBE_FACE_BACK",
    "CAMERA",
    "SPECIAL",
    "LINK_NET",
    "LINK_BOXES",
    "LINK_MULTI",
    "LINK",
    "LINK_BROKE",
    "TEXT_NOTES",
    "NOTEBOOK",
    "SUITCASE",
    "SUITCASE_ZIP",
    "MAILBOX",
    "MONITOR",
    "PRINTER",
    "PHOTO_CAMERA",
    "PHOTO_CAMERA_FLASH",
    "HOUSE",
    "HEART",
    "CORNER",
    "VERTICAL_BARS",
    "VERTICAL_BARS_FILL",
    "LIFE_BARS",
    "INFO",
    "CROSSLINE",
    "HELP",
    "FILETYPE_ALPHA",
    "FILETYPE_HOME",
    "LAYERS_VISIBLE",
    "LAYERS",
    "WINDOW",
    "HIDPI",
    "FILETYPE_BINARY",
    "HEX",
    "SHIELD",
    "FILE_NEW",
    "FOLDER_ADD",
    "ALARM",
    "CPU",
    "ROM",
    "STEP_OVER",
    "STEP_INTO",
    "STEP_OUT",
    "RESTART",
    "BREAKPOINT_ON",
    "BREAKPOINT_OFF",
    "BURGER_MENU",
    "CASE_SENSITIVE",
    "REG_EXP",
    "FOLDER",
    "FILE",
    "TEMPO",
    "WARNING",
    "HELP_BOX",
    "INFO_BOX"
};

typedef enum { 
    WRITING_MODE, 
    PREVIEW_MODE, 
    NULL_MODE,
    NEW_MEMBER_MODE,
} GuiMode;  



// Keep a pointer to original gui iconset as backup
static unsigned int currentIcons[RAYGUI_ICON_MAX_ICONS*RAYGUI_ICON_DATA_ELEMENTS] = { 0 };

// Edit the main box:
int getTextIndex(char text[]);

//----------------------------------------------------------------------------------
// Module Functions Declaration
//----------------------------------------------------------------------------------



//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
#if !defined(_DEBUG)
    SetTraceLogLevel(LOG_NONE);         // Disable raylib trace log messsages
#endif
#if (!defined(_DEBUG) && (defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)))
    // WARNING (Windows): If program is compiled as Window application (instead of console),
    // no console is available to show output info... solution is compiling a console application
    // and closing console (FreeConsole()) when changing to GUI interface
    FreeConsole();
#endif

    // GUI usage mode - Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 1600;
    const int screenHeight = 960;

    InitWindow(screenWidth, screenHeight, TextFormat("%s v%s | %s", toolName, toolVersion, toolDescription));
    SetExitKey(0);

    // Create a RenderTexture2D to be used for render to texture
    RenderTexture2D target = LoadRenderTexture(screenWidth, screenHeight);
    SetTextureFilter(target.texture, TEXTURE_FILTER_POINT);

    // GUI: Main layout
    //-----------------------------------------------------------------------------------
    // ToggleGroup() text
    // NOTE: Every icon requires 6 text characters: "#001#;"
    char toggleIconsText[RAYGUI_ICON_MAX_ICONS*6] = { 0 };

    for (int i = 0; i < RAYGUI_ICON_MAX_ICONS; i++)
    {
        if ((i + 1)%16 == 0) strncpy(toggleIconsText + 6*i, TextFormat("#%03i#\n", i), 6);
        else strncpy(toggleIconsText + 6*i, TextFormat("#%03i#;", i), 6);
    }

    toggleIconsText[RAYGUI_ICON_MAX_ICONS*6 - 1] = '\0';

    bool screenSizeActive = false;
    //-----------------------------------------------------------------------------------

    // GUI: Main toolbar panel (file and visualization)
    //-----------------------------------------------------------------------------------
    GuiMainToolbarState mainToolbarState = InitGuiMainToolbar();
    //-----------------------------------------------------------------------------------

    // GUI: Help Window
    //-----------------------------------------------------------------------------------
    GuiWindowHelpState windowHelpState = InitGuiWindowHelp();
    //-----------------------------------------------------------------------------------

    // GUI: About Window
    //-----------------------------------------------------------------------------------
    GuiWindowAboutState windowAboutState = InitGuiWindowAbout();
    //-----------------------------------------------------------------------------------

    // GUI: Issue Report Window
    //-----------------------------------------------------------------------------------
    bool showIssueReportWindow = false;
    //-----------------------------------------------------------------------------------

    // GUI: Export Window
    //-----------------------------------------------------------------------------------
    bool showExportWindow = false;


    //-----------------------------------------------------------------------------------

    // GUI: Exit Window
    //-----------------------------------------------------------------------------------
    bool closeWindow = false;
    bool showExitWindow = false;
    //-----------------------------------------------------------------------------------

    // GUI: Custom file dialogs
    //-----------------------------------------------------------------------------------
    bool showLoadFileDialog = false;
    bool showSaveFileDialog = false;
    bool showExportFileDialog = false;

    //bool showExportIconImageDialog = false;   // No need to export one single image
    //-----------------------------------------------------------------------------------

    // Check if an input file has been provided on command line
    if ((inFileName[0] != '\0') && (IsFileExtension(inFileName, ".md")))
    {
        char **tempIconsName = GuiLoadIcons(inFileName, true);
        for (int i = 0; i < RAYGUI_ICON_MAX_ICONS; i++) { strcpy(guiIconsName[i], tempIconsName[i]); free(tempIconsName[i]); }
        free(tempIconsName);

        SetWindowTitle(TextFormat("%s v%s | File: %s", toolName, toolVersion, GetFileName(inFileName)));
    }

    // Editor Variables
    GuiMode CurrentGuiMode = NULL_MODE;

    SetTargetFPS(60);       // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Starting at the eight theme/style
    #define DEFAULT_STYLE 13
    mainToolbarState.visualStyleActive = DEFAULT_STYLE - 1;
    mainToolbarState.btnAboutPressed = true; // show this when starting the program
    
    // Main game loop
    while (!closeWindow)    // Detect window close button
    {
        // Change Font

        // WARNING: ASINCIFY requires this line,
        // it contains the call to emscripten_sleep() for PLATFORM_WEB
        if (WindowShouldClose()) showExitWindow = true;

        // Undo icons change logic
        //----------------------------------------------------------------------------------
       
        // Dropped files logic
        //----------------------------------------------------------------------------------
        if (IsFileDropped())
        {
            FilePathList droppedFiles = LoadDroppedFiles();

            if (IsFileExtension(droppedFiles.paths[0], ".md"))
            {
                // Load .md data into raygui icons set (and gui icon names for the tool)
                char **tempIconsName = GuiLoadIcons(droppedFiles.paths[0], true);
                memcpy(currentIcons, guiIcons, RAYGUI_ICON_MAX_ICONS*RAYGUI_ICON_DATA_ELEMENTS*sizeof(unsigned int));
                for (int i = 0; i < RAYGUI_ICON_MAX_ICONS; i++) { strcpy(guiIconsName[i], tempIconsName[i]); free(tempIconsName[i]); }
                free(tempIconsName);

                strcpy(inFileName, droppedFiles.paths[0]);
                SetWindowTitle(TextFormat("%s v%s | File: %s", toolName, toolVersion, GetFileName(inFileName)));
            }
            else if (IsFileExtension(droppedFiles.paths[0], ".png"))
            {
                // TODO: Support icons loading from image
            }
            else if (IsFileExtension(droppedFiles.paths[0], ".rgs")) GuiLoadStyle(droppedFiles.paths[0]);

            UnloadDroppedFiles(droppedFiles);    // Unload filepaths from memory
        }
        //----------------------------------------------------------------------------------

        // Keyboard shortcuts (+main toolbar related buttons)
        //------------------------------------------------------------------------------------
        if ((IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_N)) || mainToolbarState.btnNewFilePressed) {
            // TODO
        }

        // Show dialog: load icons data (.md)
        if ((IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_O)) || mainToolbarState.btnLoadFilePressed) {
            // TODO
        }
        // Show dialog: save icons data (.md)
        if ((IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_S)) || mainToolbarState.btnSaveFilePressed) {
            // TODO
        }

        // Show dialog: export icons data (.png, .h)
        if ((IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_E)) || mainToolbarState.btnExportFilePressed) {
            // TODO

        }

        // Toggle window: help
        if (IsKeyPressed(KEY_F1)) windowHelpState.windowActive = !windowHelpState.windowActive;

        // Toggle window: about
        if (IsKeyPressed(KEY_F2)) windowAboutState.windowActive = !windowAboutState.windowActive;

        // Toggle window: sponsor
        if (IsKeyPressed(KEY_F3)) showIssueReportWindow = !showIssueReportWindow;

        // Toggle window editing/viewing mode:
        if (IsKeyPressed(KEY_F5)) mainToolbarState.btnPreviewPressed = !mainToolbarState.btnPreviewPressed;

        // Show closing window on ESC
        if (IsKeyPressed(KEY_ESCAPE)) {
            if (windowHelpState.windowActive) windowHelpState.windowActive = false;
            else if (windowAboutState.windowActive) windowAboutState.windowActive = false;
            else if (showIssueReportWindow) showIssueReportWindow = false;
            else if (showExportWindow) showExportWindow = false;
            else showExitWindow = !showExitWindow;
        }

        // Select visual style
        if (mainToolbarState.visualStyleActive < 0) mainToolbarState.visualStyleActive = MAX_GUI_STYLES_AVAILABLE - 1;
        else if (mainToolbarState.visualStyleActive > (MAX_GUI_STYLES_AVAILABLE - 1)) mainToolbarState.visualStyleActive = 0;

        // Visual options logic
        if (mainToolbarState.visualStyleActive != mainToolbarState.prevVisualStyleActive)
        {
            // When a new template style is selected, everything is reseted
            GuiLoadStyleDefault();

            switch (mainToolbarState.visualStyleActive)
            {
                case 1: GuiLoadStyleJungle(); break;
                case 2: GuiLoadStyleCandy(); break;
                case 3: GuiLoadStyleLavanda(); break;
                case 4: GuiLoadStyleCyber(); break;
                case 5: GuiLoadStyleTerminal(); break;
                case 6: GuiLoadStyleAshes(); break;
                case 7: GuiLoadStyleBluish(); break;
                case 8: GuiLoadStyleDark(); break;
                case 9: GuiLoadStyleCherry(); break;
                case 10: GuiLoadStyleSunny(); break;
                case 11: GuiLoadStyleEnefete(); break;
                case 12: GuiLoadStyleAmber(); break;
                default: break;
            }

            saveChangesRequired = false;

            mainToolbarState.prevVisualStyleActive = mainToolbarState.visualStyleActive;
        }

        // Help options logic
        if (mainToolbarState.btnNewFilePressed) CurrentGuiMode = WRITING_MODE;           // New post logic
        if (mainToolbarState.btnHelpPressed) windowHelpState.windowActive = true;       // Help button logic
        if (mainToolbarState.btnAboutPressed) windowAboutState.windowActive = true;     // About window button logic
        if (mainToolbarState.btnIssuePressed) showIssueReportWindow = true;             // Issue report window button logic
        if (mainToolbarState.btnPreviewPressed) CurrentGuiMode = PREVIEW_MODE;           // Preview mode button logic
        //----------------------------------------------------------------------------------

        // WARNING: Some windows should lock the main screen controls when shown
        if (windowHelpState.windowActive ||
            windowAboutState.windowActive ||
            showIssueReportWindow ||
            showExitWindow ||
            showExportWindow ||
            showLoadFileDialog ||
            showSaveFileDialog ||
            showExportFileDialog) GuiLock();
        //----------------------------------------------------------------------------------

        // Basic program flow logic
        //----------------------------------------------------------------------------------

        // Screen scale logic (x2)
        //----------------------------------------------------------------------------------
        if (screenSizeActive)
        {
            // Screen size x2
            if (GetScreenWidth() < screenWidth*2)
            {
                SetWindowSize(screenWidth*2, screenHeight*2);
                SetMouseScale(0.5f, 0.5f);
            }
        }
        else
        {
            // Screen size x1
            if (screenWidth*2 >= GetScreenWidth())
            {
                SetWindowSize(screenWidth, screenHeight);
                SetMouseScale(1.0f, 1.0f);
            }
        }
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        // Render all screen to texture (for scaling)
        BeginTextureMode(target);
            ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));

            // GUI: Main toolbar panel
            //----------------------------------------------------------------------------------
            GuiMainToolbar(&mainToolbarState);

            //if (iconDataToCopy) DrawIconData(iconData, mainToolbarState.anchorEdit.x + 12 + 72 + 16 + 4, mainToolbarState.anchorEdit.y + 8 + 4, 1, GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL)));
            //----------------------------------------------------------------------------------

            // GUI: Main screen controls
            

            // GUI: Status bar
            //--------------------------------------------------------------------------------
            int textPadding = GuiGetStyle(STATUSBAR, TEXT_PADDING);
            GuiSetStyle(STATUSBAR, TEXT_PADDING, 15);
            //GuiStatusBar((Rectangle){ 0, screenHeight - 24, 351, 24 }, TextFormat("LETTERS WROTE: %i/%i", getTextIndex(content), MAX_TEXT_SIZE));
            //GuiStatusBar((Rectangle){ 350, screenHeight - 24, screenWidth - 350, 24 }, TextFormat("SELECTED: %i - %s", selectedIcon, guiIconsName[selectedIcon]));
            GuiSetStyle(STATUSBAR, TEXT_PADDING, textPadding);
            //--------------------------------------------------------------------------------

            // NOTE: If some overlap window is open and main window is locked, we draw a background rectangle
            if (GuiIsLocked()) DrawRectangle(0, 0, screenWidth, screenHeight, Fade(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)), 0.85f));

            // WARNING: Before drawing the windows, we unlock them
            GuiUnlock();

            // GUI: Help Window
            //----------------------------------------------------------------------------------------
            windowHelpState.windowBounds.x = (float)screenWidth/2 - windowHelpState.windowBounds.width/2;
            windowHelpState.windowBounds.y = (float)screenHeight/2 - windowHelpState.windowBounds.height/2;
            GuiWindowHelp(&windowHelpState);
            //----------------------------------------------------------------------------------------

            // GUI: About Window
            //----------------------------------------------------------------------------------------
            windowAboutState.windowBounds.x = (float)screenWidth/2 - windowAboutState.windowBounds.width/2;
            windowAboutState.windowBounds.y = (float)screenHeight/2 - windowAboutState.windowBounds.height/2;
            GuiWindowAbout(&windowAboutState);
            //----------------------------------------------------------------------------------------

            // GUI: Issue Report Window
            //----------------------------------------------------------------------------------------
            if (showIssueReportWindow)
            {
                Rectangle messageBox = { (float)screenWidth/2 - 300/2, (float)screenHeight/2 - 190/2 - 20, 300, 190 };
                int result = GuiMessageBox(messageBox, "#220#Report Issue",
                    "Do you want to report any issue or\nfeature request for this program?\n\nhttps://github.com/0l1ve1r4/StatiqPress", "#186#Report on GitHub");

                // Report issue pressed
                if (result == 1) {
                    OpenURL("https://github.com/0l1ve1r4/StatiqPress/issues");
                    showIssueReportWindow = false;
                }
                else if (result == 0) showIssueReportWindow = false;
            }
            //----------------------------------------------------------------------------------------

            // GUI: Export Window
            //----------------------------------------------------------------------------------------
            if (showExportWindow){}
            //----------------------------------------------------------------------------------

            // GUI: Exit Window
            //----------------------------------------------------------------------------------------
            if (showExitWindow)
            {
                int result = GuiMessageBox((Rectangle){ screenWidth/2 - ((screenWidth/4)/2), screenHeight/2 - 50, screenWidth/4, 150 }, TextFormat("#159#Closing %s", toolName), "Do you really want to exit?", "Yes;No");

                if ((result == 0) || (result == 2)) showExitWindow = false;
                else if (result == 1) closeWindow = true;
            }
            //----------------------------------------------------------------------------------------

            // GUI: Load File Dialog (and loading logic)
            //----------------------------------------------------------------------------------------
            if (showLoadFileDialog)
            {

                int result = GuiFileDialog(DIALOG_OPEN_FILE, "Load Markdown file", inFileName, "*.md", "Markdown Files (*.md)");
                if (result == 1) {
                    // Load gui icons data (and gui icon names for the tool)
                    char **tempIconsName = GuiLoadIcons(inFileName, true);
                    memcpy(currentIcons, guiIcons, RAYGUI_ICON_MAX_ICONS*RAYGUI_ICON_DATA_ELEMENTS*sizeof(unsigned int));
                    for (int i = 0; i < RAYGUI_ICON_MAX_ICONS; i++) { strcpy(guiIconsName[i], tempIconsName[i]); free(tempIconsName[i]); }
                    free(tempIconsName);

                    SetWindowTitle(TextFormat("%s v%s | File: %s", toolName, toolVersion, GetFileName(inFileName)));
                    saveChangesRequired = false;
                }

                if (result >= 0) showLoadFileDialog = false;
            }
            //----------------------------------------------------------------------------------------

        EndTextureMode();

        BeginDrawing();
            ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));

            // Draw render texture to screen
            if (screenSizeActive) DrawTexturePro(target.texture, (Rectangle){ 0, 0, (float)target.texture.width, -(float)target.texture.height }, (Rectangle){ 0, 0, (float)target.texture.width*2, (float)target.texture.height*2 }, (Vector2){ 0, 0 }, 0.0f, WHITE);
            else DrawTextureRec(target.texture, (Rectangle){ 0, 0, (float)target.texture.width, -(float)target.texture.height }, (Vector2){ 0, 0 }, WHITE);

            if (CurrentGuiMode == WRITING_MODE) {
                if(newUpload() == -1) {
                    CurrentGuiMode = NULL_MODE;
                }
            } 

            else if (CurrentGuiMode == NULL_MODE) {
                // TODO: some icon or wharever
            }

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();              // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

int getTextIndex(char text[]) {
    int i = 0;
    while (text[i] != '\0') {
        i++;
    }

    return i;
}