#pragma once


//debugging levels: 0 is verbose, 1 is info, 2 is warning, 3 is error
#define DEBUG_LEVEL 1
#define DEBUG_LEVEL_VERBOSE 0
#define DEBUG_LEVEL_INFO 1
#define DEBUG_LEVEL_WARNING 2
#define DEBUG_LEVEL_ERROR 3

//general settings
#define MOVEMENT_SPEED 10.0f
#define SAVE_FILE "world.txt"

#define WINDOW_HEIGHT 1080
#define WINDOW_WIDTH 1920
#define MAPFILE "models/landscape.obj"
#define MAPSIZE 20

//imgui settings
#define ACTIVE_BUTTON_COLOR { 0, 0, 0, 0.5f }
#define INACTIVE_BUTTON_COLOR  { 0, 0, 0, 0.3f }
#define BUTTON_SIZE {230, 40}
#define SLIDER_SENSITIVITY 1.0f