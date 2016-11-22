#include "Csnake.h"

using namespace std;

Csnake::Csnake()
	:mJumpMaxZoom (0.9), mJumpPower (3)
{
}
 
Csnake::~Csnake()
{
	if (SnakeBody)
	{
		delete SnakeBody;
		delete moveHistory;
	}
}

void Csnake::initSnake(int aScrWidth, int aScrHeight, int aUSize, int aMaxSnakeSize, Snake aHead )
{
	mScreenWidth = aScrWidth;
	mScreenHeight = aScrHeight;
	mUnitSize = aUSize;
	mHalfUnitSize = mUnitSize /2;
	mMaxLong = aMaxSnakeSize;
	mSnakeBodySize = -1;
	mJumpLong = 0;
	mJumpTop = 0;
	if (!SnakeBody)
		SnakeBody = new Snake[mMaxLong];
	
	//setting snake position and parameters
	mSnakeHead = aHead;

	//allocating memory for snake moving history 
	if (moveHistory == 0)
		moveHistory = new sHistory [54000]; //array history is enought for 15 minutes of record:  60(FPS)*60(sec)*15(min) = 54 000
	else
		moveSnake(true, false); //resets snake history

	if (!bonus_event_queue)
	{
		bonus_event_queue = al_create_event_queue();
		bonusSpeedUpTimer = al_create_timer( 10 ); //10 seconds
		al_register_event_source(bonus_event_queue, al_get_timer_event_source(bonusSpeedUpTimer) );
	}

	if (mSpeedUpTimersStarted)
	{
		mSpeedUpTimersStarted = 0;
		al_stop_timer (bonusSpeedUpTimer);
		al_flush_event_queue (bonus_event_queue);
	}

	mSpeedUpTimersStarted = 0;

	if (!soundJump)
	{
		soundJump = al_load_sample ("GameData\\jump.ogg");
	}
}

void Csnake::drawSnake()
{	
	float jm;	//jm is the jump multiplier
	for (int i = mSnakeBodySize; i >= 0; i--)	//snake body drawing
	{	
		jm = SnakeBody[i].jump + 1;	//jm is the jump multiplier

		rotate_On (SnakeBody[i].xPos, SnakeBody[i].yPos, SnakeBody[i].dir); // Setup the rotation transformation 

		//snake body segments drawing
		switch (SnakeBody[i].form)
		{
			case CIRCLE_FILLED:
				al_draw_filled_circle (	SnakeBody[i].xPos, SnakeBody[i].yPos, 
										mHalfUnitSize * jm, SnakeBody[i].color); 
				break;
			case (CIRCLE_EMPTY):
				al_draw_circle (SnakeBody[i].xPos, SnakeBody[i].yPos, 
								mHalfUnitSize * jm, SnakeBody[i].color, SnakeBody[i].thickness * jm); 
				break;
			case (TRIANGLE_FILLED):
				al_draw_filled_triangle (	SnakeBody[i].xPos + mHalfUnitSize * jm, SnakeBody[i].yPos,    
											SnakeBody[i].xPos - mHalfUnitSize * jm, SnakeBody[i].yPos + mHalfUnitSize * jm, 
											SnakeBody[i].xPos - mHalfUnitSize * jm, SnakeBody[i].yPos - mHalfUnitSize * jm, 
											SnakeBody[i].color);
				break;
			case (TRIANGLE_EMPTY):
					al_draw_triangle (	SnakeBody[i].xPos + mHalfUnitSize * jm, SnakeBody[i].yPos,    
										SnakeBody[i].xPos - mHalfUnitSize * jm, SnakeBody[i].yPos + mHalfUnitSize * jm, 
										SnakeBody[i].xPos - mHalfUnitSize * jm, SnakeBody[i].yPos - mHalfUnitSize * jm, 
										SnakeBody[i].color, SnakeBody[i].thickness * jm);
				break;
		}
		
		// ribs drawing
		al_draw_arc(	SnakeBody[i].xPos - mHalfUnitSize* jm, SnakeBody[i].yPos, mHalfUnitSize * jm, 5.38, 1.91,
						mSnakeHead.color, ((float) mUnitSize / 10) * jm);	
		
		rotate_Off();// Turn off the rotation 

		//snake spine drawing
		if (i < mSnakeBodySize)
			if (abs(SnakeBody[i].xPos - SnakeBody[i+1].xPos) <= mUnitSize + 5 
				&& abs(SnakeBody[i].yPos - SnakeBody[i+1].yPos) <= mUnitSize + 5 )	//don`t draw spine if snake divided by screen
					al_draw_line (	SnakeBody[i].xPos, SnakeBody[i].yPos, 
									SnakeBody[i+1].xPos, SnakeBody[i+1].yPos, 
									mSnakeHead.color, ((float) mUnitSize / 6) * jm);
	}
	
	//snake head drawing	
	rotate_On (mSnakeHead.xPos, mSnakeHead.yPos, mSnakeHead.dir); // Setup the rotation transformation 
	jm = mSnakeHead.jump + 1;
	al_draw_triangle (	mSnakeHead.xPos + mHalfUnitSize * jm, mSnakeHead.yPos,    
						mSnakeHead.xPos - mHalfUnitSize * jm, mSnakeHead.yPos + mHalfUnitSize * jm, 
						mSnakeHead.xPos - mHalfUnitSize * jm, mSnakeHead.yPos - mHalfUnitSize * jm, 
						mSnakeHead.color, mSnakeHead.thickness * jm);
	al_draw_filled_circle (	mSnakeHead.xPos, mSnakeHead.yPos + (mUnitSize / 4) * jm,	
							(mUnitSize / 5) * jm, al_map_rgb (0, 220, 0) );		//eye 1
	al_draw_filled_circle (	mSnakeHead.xPos, mSnakeHead.yPos + (mUnitSize / 4) * jm, 
							(mUnitSize / 15) * jm, al_map_rgb (220, 0, 0) );	//eye pupil 1
	al_draw_filled_circle (	mSnakeHead.xPos, mSnakeHead.yPos - (mUnitSize / 4) * jm, 
							(mUnitSize / 5) * jm, al_map_rgb (0, 220, 0) );		//eye 2
	al_draw_filled_circle (	mSnakeHead.xPos, mSnakeHead.yPos - (mUnitSize / 4) * jm, 
							(mUnitSize / 15) * jm, al_map_rgb (220, 0, 0) );	//eye pupil 2
	rotate_Off();// Turn off the rotation
	
	if (mSnakeBodySize >= 0)	//snake neck spine drawing
		if (abs(mSnakeHead.xPos - SnakeBody[0].xPos) <= mUnitSize + 5 
				&& abs(mSnakeHead.yPos - SnakeBody[0].yPos) <= mUnitSize + 5 )
		al_draw_line (	mSnakeHead.xPos - (mSnakeHead.xPos - SnakeBody[0].xPos) / 2, 
						mSnakeHead.yPos - (mSnakeHead.yPos - SnakeBody[0].yPos) / 2, 
						SnakeBody[0].xPos, SnakeBody[0].yPos, 
						mSnakeHead.color, ((float) mUnitSize / 4) * jm);
}

void Csnake::directionChange (float aDC)
{
	mSnakeHead.dir += aDC;
}

void Csnake::moveSnake (bool aRestart, bool aRepeat)
{
	if (aRestart)
		{
			mSnakeBodySize = -1;
			mHistoryCursor = 0;	//return cursor to start position
			return;
		}
	if (!aRepeat)	//move snake head with history record 
	{	//move 
		mSnakeHead.xPos += cos (mSnakeHead.dir) * mSnakeHead.speed;	//move 
		mSnakeHead.yPos += sin (mSnakeHead.dir) * mSnakeHead.speed;

		//jump
		if (mJumpLong)
		{
			if (mJumpLong > mJumpTop)
				mSnakeHead.jump = mJumpMaxZoom * (mJumpTop / mJumpLong);
			else
				mSnakeHead.jump = mJumpMaxZoom * (mJumpLong / mJumpTop); 
			mJumpLong--;
			if (mJumpLong < 1)
				mJumpLong = mSnakeHead.jump = 0;
		}

		//head transfer to the other side of the screen
		if (mSnakeHead.xPos >= mScreenWidth - mScreenWidth % mUnitSize)	
			mSnakeHead.xPos = 0;
		if (mSnakeHead.xPos < 0)
			mSnakeHead.xPos = mScreenWidth-1 - mScreenWidth % mUnitSize;
		if (mSnakeHead.yPos < 0)
			mSnakeHead.yPos = mScreenHeight-1 - mScreenHeight % mUnitSize;
		if (mSnakeHead.yPos >= mScreenHeight - mScreenHeight % mUnitSize)
			mSnakeHead.yPos = 0;
	
		//snake head history record 
		moveHistory[mHistoryCursor].xPos = mSnakeHead.xPos;
		moveHistory[mHistoryCursor].yPos = mSnakeHead.yPos;
		moveHistory[mHistoryCursor].dir = mSnakeHead.dir;
		moveHistory[mHistoryCursor].speed = mSnakeHead.speed;
		moveHistory[mHistoryCursor].jump = mSnakeHead.jump;
		mHistoryCursor++;		
	}
	else	//playing repeat
	{	mSnakeHead.xPos = moveHistory[mHistoryCursor].xPos;
		mSnakeHead.yPos = moveHistory[mHistoryCursor].yPos;
		mSnakeHead.dir  = moveHistory[mHistoryCursor].dir;
		mSnakeHead.jump = moveHistory[mHistoryCursor].jump;
		mHistoryCursor ++;
		if (moveHistory[mHistoryCursor - 1].jump == 0 && moveHistory[mHistoryCursor].jump > 0 )	//jump sound
			al_play_sample (soundJump, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
	}
		
	//moving snake body using snake head history
	int cursorBias = 0;
	float j = 0;
	
	for (int i = 0; i <= mSnakeBodySize; i++)
	{
		while (j < mUnitSize * (i+1) && mHistoryCursor-cursorBias > 0)	//search of history point in one unit behind head or previous segment
		{
			cursorBias++;
			j += moveHistory [mHistoryCursor - cursorBias].speed;
		}
		//moving body segment
		SnakeBody[i].xPos	= moveHistory [mHistoryCursor - cursorBias ].xPos; 
		SnakeBody[i].yPos	= moveHistory [mHistoryCursor - cursorBias ].yPos;
		SnakeBody[i].dir	= moveHistory [mHistoryCursor - cursorBias ].dir;
		SnakeBody[i].jump	= moveHistory [mHistoryCursor - cursorBias ].jump;
	}


	if (mHistoryCursor >= 54000) //if history array finished
		mHistoryCursor = 0;	//start from begin

	//disabling speedup bonus
	if (mSpeedUpTimersStarted)
	{
		if (al_get_next_event (bonus_event_queue, &bonusevents)) 
		{
			mSnakeHead.speed /= 2;
			mSpeedUpTimersStarted--;
			if (mSpeedUpTimersStarted == 0)
				al_stop_timer (bonusSpeedUpTimer);
		}
	}

}

void Csnake::increase(int aForm, ALLEGRO_COLOR aColor, float aThickness)
{
	mSnakeBodySize++;
	SnakeBody[mSnakeBodySize].form = aForm;
	SnakeBody[mSnakeBodySize].color = aColor;
	SnakeBody[mSnakeBodySize].thickness = aThickness;
	SnakeBody[mSnakeBodySize].speed = mSnakeHead.speed;
}

void Csnake::setSnakeSpeed(int aSpd)
{
	mSnakeHead.speed = aSpd;
}

char Csnake::getSnakeBodySize()
{
	return (mSnakeBodySize);
}

Snake Csnake::getSnakeHeadStruct()
{
	return (mSnakeHead);
}

Snake Csnake::getSecondSegmentStruct()
{
		return (SnakeBody[1]);
}

void Csnake::reduse()
{
	mSnakeBodySize--;
}

Snake Csnake::getLastSegmentStruct()
{
	return (SnakeBody[mSnakeBodySize]);
}

void Csnake::SpeedUpBonusPicked ()
{
	if (mSpeedUpTimersStarted == 0)
		al_start_timer(bonusSpeedUpTimer);
	mSpeedUpTimersStarted++;
	mSnakeHead.speed *= 2;
}

void Csnake::setJump()
{
	if (mJumpLong)	
		return;
	
	al_play_sample (soundJump, 1.5, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
	mJumpLong = mUnitSize * mJumpPower / mSnakeHead.speed;
	mJumpTop = mJumpLong / 2;

}

void Csnake::rotate_On (float aXpos, float aYpos, float aDir)
{	// Setup the rotation transformation 
	al_identity_transform(&trans); //var init
	al_translate_transform(&trans, -aXpos, -aYpos);	//setting "rotation center" point
	al_rotate_transform(&trans, aDir);		// rotation angle
	al_translate_transform(&trans, aXpos, aYpos);	//restoring normal view center
	al_use_transform(&trans);	// transformation activation
}

void Csnake::rotate_Off ()
{	// Turn off the rotation 
		al_identity_transform(&trans);	// var deinitialisation
		al_use_transform(&trans);	//transformation deactivation
}