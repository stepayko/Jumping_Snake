#pragma once

#include <allegro5\allegro.h>
#include <allegro5\allegro_primitives.h>	
#include <cmath> // cos, sin
#include <allegro5\allegro_audio.h>
#include <allegro5\allegro_acodec.h>
 
struct Snake 
{
	float xPos; 
	float yPos; 
	float speed;
	int form;
	float dir;
	float jump;
	float thickness;
	ALLEGRO_COLOR color;
};

class Csnake
{
public:
	Csnake();
	~Csnake();
	void initSnake(int aScrWidth, int aScrHeight, int aUSize, int aMaxSnakeSize, Snake aHead );
	void moveSnake(bool aRestart, bool aRepeat);
	void directionChange (float aDC);
	void drawSnake();
	void increase(int aForm, ALLEGRO_COLOR aColor, float aThickness);
	void setSnakeSpeed(int aSpd);
	char getSnakeBodySize();
	void reduse();
	Snake getSnakeHeadStruct();
	Snake getSecondSegmentStruct();
	Snake getLastSegmentStruct();
	void SpeedUpBonusPicked ();
	void setJump();
	void rotate_On (float aXpos, float aYpos, float aDir);
	void rotate_Off();

private:
	enum forms {
		CIRCLE_EMPTY=0, 
		CIRCLE_FILLED, 
		TRIANGLE_EMPTY, 
		TRIANGLE_FILLED, 
		SQUARE_EMPTY, 
		SQUARE_FILLED
	};

	int mScreenWidth;
	int mScreenHeight; 
	int mUnitSize;
	int mHalfUnitSize;
	int mHistoryCursor;
	int mMaxLong;
	const int mJumpPower; // how many units snake can jump over

	char mSnakeBodySize;
	char mSpeedUpTimersStarted;
	
	float mJumpLong;
	float mJumpTop;
	const float mJumpMaxZoom;

	struct sHistory {
		float xPos; 
		float yPos; 
		float dir;
		float speed; 
		float jump;
	};

	sHistory *moveHistory;
	Snake *SnakeBody;
	Snake mSnakeHead;

	ALLEGRO_EVENT_QUEUE *bonus_event_queue;
	ALLEGRO_TIMER *bonusSpeedUpTimer;
	ALLEGRO_EVENT bonusevents;
	ALLEGRO_TRANSFORM trans;
	ALLEGRO_SAMPLE *soundJump;
};

