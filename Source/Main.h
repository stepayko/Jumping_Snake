#pragma once

#include <allegro5\allegro.h>	
#include <allegro5\allegro_image.h>
#include <allegro5\allegro_primitives.h>		
#include <allegro5\allegro_ttf.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_audio.h>
#include <allegro5\allegro_acodec.h>
#include <cstdlib> //srand
#include <ctime> // time for srand init
#include <cmath> // cos, sin
#include <vector> 
#include <string>
#include "Csnake.h"
#include "Menu.h"

using namespace std;

const int FPS = 60;
const int maxSnakeLong = 20;
const int bonuses = 4;

enum gameStates {
	HELP, 
	PAUSE, 
	GAME, 
	MAIN_MENU, 
	GAME_END, 
	LOSE, 
	WIN,
	CREDITS
};
enum screenModes {
	FULLSCREEN=1, 
	WINDOW
};
enum forms {
	CIRCLE_EMPTY=0, 
	CIRCLE_FILLED, 
	TRIANGLE_EMPTY, 
	TRIANGLE_FILLED, 
	SQUARE_EMPTY, 
	SQUARE_FILLED
};
enum mapObjectsTypes {
	EMPTY_MAP=0, 
	SNAKE, 
	WALL, 
	FOOD, 
	BONUS
};
enum bonusTypes {
	SPEED_UP, 
	CUT
};
enum drawEffectParameters {
	DRAW=0, 
	SET, 
	BLAST_IN, 
	BLAST_OUT
};
enum mainMenu {
	CONTINUE_GAME,
	NEW_GAME,
	SETTINGS,
	HELP_SCREEN,
	CREDITS_SCREEN,
	EXIT,
	ONE_PLAYER,
	TWO_PLAYERS,
	COOPERATIVE,
	VERSUS,
	SCREEN_SETTINGS,
	GAME_SETTINGS,
	FULL_SCREEN,
	WINDOW_MODE,
	CHANGE_WINDOW_WIDTH,
	CHANGE_WINDOW_HEIGHT,
	UNIT_SIZE,
	GAME_SPEED,
	BACKGROUND_COLOR,
	LOW,
	NORMAL,
	FAST,
	RED,
	GREEN,
	BLUE,
	YELLOW,
	GREY,
	BLACK,
	MENU_END
};


int gameState = MAIN_MENU;
int screenMode = WINDOW;
int screenWidth = 800;
int screenHeight = 600;

int newWidth = 800;
int newHeight = 600;

int unitSize = 20;
int bonusesLeft = bonuses;
int foodLeft = maxSnakeLong;

char playersCount = 1;

bool done = false;
bool repeat = false;
bool godMode = false;

float defaultSpeed = 2;
float speedCoef = 1;
 
vector<string> menuItems;

struct mapObjects 
{
	char type;
	char bonusType;
	char form;
	ALLEGRO_COLOR color;
	float thickness;
};

vector < vector<mapObjects> > map;

ALLEGRO_DISPLAY *display;
ALLEGRO_BITMAP *displayBitmap;
ALLEGRO_MONITOR_INFO aminfo;
ALLEGRO_EVENT_QUEUE *event_queue;
ALLEGRO_EVENT events;
ALLEGRO_TIMER *timer;
ALLEGRO_KEYBOARD_STATE keyState;

ALLEGRO_FONT *font;
ALLEGRO_FONT *fontBig;
ALLEGRO_FONT *fontMed;

ALLEGRO_SAMPLE *soundAm;
ALLEGRO_SAMPLE *soundEnd; 
ALLEGRO_SAMPLE *soundPowerUp;
ALLEGRO_SAMPLE *soundWin;
ALLEGRO_SAMPLE *musikGame;
ALLEGRO_SAMPLE *musikMenu;

ALLEGRO_SAMPLE_INSTANCE *musicGameInstance;
ALLEGRO_SAMPLE_INSTANCE *musicMenuInstance;

ALLEGRO_COLOR backgroundColor;

void initialization();
void mainMenu();
void helpScreen(bool restart);
void checkUserInput();
void menuProcessing();
void game();
void gameEnd();
void fillMap(int food);
void checkMap(Csnake *snake);
void drawMap();
void fillMap_drawWallCross(int x, int y, int lenght);
void putObject2map(struct mapObjects obj2put);
void drawEffects(int mode, int xPos, int yPos, int effectType);
void loseScreen();
void winScreen();
bool wait(double seconds);
void initGame();
void cleanMap();
void creditsScreen(bool restart);