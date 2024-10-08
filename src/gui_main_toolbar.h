/*******************************************************************************************
*
*   Main Toolbar
*
*   MODULE USAGE:
*       #define GUI_MAIN_TOOLBAR_IMPLEMENTATION
*       #include "gui_main_toolbar.h"
*
*       INIT: GuiMainToolbarState state = InitGuiMainToolbar();
*       DRAW: GuiMainToolbar(&state);
*
*   LICENSE: zlib/libpng
*
*   Copyright (c) 2019-2022 raylib technologies (@raylibtech) / Ramon Santamaria (@raysan5)
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

// WARNING: raygui implementation is expected to be defined before including this header

#ifndef GUI_MAIN_TOOLBAR_H
#define GUI_MAIN_TOOLBAR_H

typedef struct {

    // Anchors for panels
    Vector2 anchorFile;
    Vector2 anchorEdit;
    Vector2 anchorTools;
    Vector2 anchorVisuals;
    Vector2 anchorRight;
    
    // Visual options
    int visualStyleActive;
    int prevVisualStyleActive;
    int btnReloadStylePressed;
    int languageActive;

    // Info options
    bool btnHelpPressed;
    bool btnAboutPressed;
    bool btnIssuePressed;
    bool btnUserPressed;

    // Custom variables
    // TODO.

} GuiMainToolbarState;

#ifdef __cplusplus
extern "C" {            // Prevents name mangling of functions
#endif

//----------------------------------------------------------------------------------
// Defines and Macros
//----------------------------------------------------------------------------------
//...

//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------
// ...

//----------------------------------------------------------------------------------
// Global Variables Definition
//----------------------------------------------------------------------------------
//...

//----------------------------------------------------------------------------------
// Internal Module Functions Definition
//----------------------------------------------------------------------------------
//...

//----------------------------------------------------------------------------------
// Module Functions Declaration
//----------------------------------------------------------------------------------
GuiMainToolbarState InitGuiMainToolbar(void);
void GuiMainToolbar(GuiMainToolbarState *state, int screenWidth);

#ifdef __cplusplus
}
#endif

#endif // GUI_MAIN_TOOLBAR_H

/***********************************************************************************
*
*   GUI_MAIN_TOOLBAR IMPLEMENTATION
*
************************************************************************************/
#if defined(GUI_MAIN_TOOLBAR_IMPLEMENTATION)

#include "raygui.h"

GuiMainToolbarState InitGuiMainToolbar(void)
{
    GuiMainToolbarState state = { 0 };

    // Anchors for panels
    state.anchorFile = (Vector2){ 0, 0 };
    state.anchorEdit = (Vector2){ state.anchorFile.x + 132 - 1, 0 };
    state.anchorTools = (Vector2){ state.anchorEdit.x + 168 - 1, 0 };
    state.anchorVisuals = (Vector2){ 0, 0 };    // Anchor right, depends on screen width
    state.anchorRight = (Vector2){ 0, 0 };      // Anchor right, depends on screen width

    // Visuals options
    state.visualStyleActive = 0;
    state.prevVisualStyleActive = 0;
    state.languageActive = 0;

    // Info options
    state.btnHelpPressed = false;
    state.btnAboutPressed = false;
    state.btnIssuePressed = false;
    state.btnUserPressed = false;
    
    // Custom variables
    // TODO.

    // Enable tooltips by default
    GuiEnableTooltip();

    return state;
}

void GuiMainToolbar(GuiMainToolbarState *state, int screenWidth) {
    // Toolbar panels
    state->anchorRight.x = screenWidth - 104;       // Update right-anchor panel
    state->anchorVisuals.x = state->anchorRight.x - 190 + 1;    // Update right-anchor panel

    GuiPanel((Rectangle){ state->anchorFile.x, state->anchorFile.y, 132, 40 }, NULL);
    GuiPanel((Rectangle){ state->anchorEdit.x, state->anchorEdit.y, 168, 40 }, NULL);
    GuiPanel((Rectangle){ state->anchorTools.x, state->anchorTools.y, state->anchorVisuals.x - state->anchorTools.x + 1, 40 }, NULL);
    GuiPanel((Rectangle){ state->anchorVisuals.x, state->anchorVisuals.y, 220, 40 }, NULL);
    GuiPanel((Rectangle){ state->anchorRight.x, state->anchorRight.y, 104, 40 }, NULL);

    // Info options
    GuiSetTooltip("Show help window (F1)");
    state->btnHelpPressed = GuiButton((Rectangle){ state->anchorRight.x + (screenWidth - state->anchorRight.x) - 12 - 72 - 8, state->anchorRight.y + 8, 24, 24 }, "#221#"); 
    GuiSetTooltip("Show info window (F2)");
    state->btnAboutPressed = GuiButton((Rectangle){ state->anchorRight.x + (screenWidth - state->anchorRight.x) - 12 - 48 - 4, state->anchorRight.y + 8, 24, 24 }, "#222#");
    GuiSetTooltip("Report an issue (F3)");
    state->btnIssuePressed = GuiButton((Rectangle){ state->anchorRight.x + (screenWidth - state->anchorRight.x) - 12 - 24, state->anchorRight.y + 8, 24, 24 }, "#220#");

    GuiSetTooltip(NULL);
}

const char guiIconsName[RAYGUI_ICON_MAX_ICONS][32] = {
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


#endif // GUI_MAIN_TOOLBAR_IMPLEMENTATION
