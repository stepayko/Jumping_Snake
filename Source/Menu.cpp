#include "Menu.h"
 

Menu::Menu(void)
	:mSelectedItem (-1), mDataInputMode(false), mInputedData(0)
{
}

Menu::~Menu(void)
{
}

void Menu::initMenu(vector<string> mItems, int aScrWidth, int aScrHeight, int aUnSize)
{
	mMenuItems = mItems;
	mDataInputMode = false;
	mScreenWidth = aScrWidth;
	mScreenWidthCenter = mScreenWidth / 2;
	mScreenHeight = aScrHeight;
	mUnitSize = aUnSize;
	mTextSize = mUnitSize * 2;
	mLineSpacing = mUnitSize;
	usualFont = al_load_font ("GameData\\CaviarDreams.ttf", mTextSize, 0);
	selectedFont = al_load_font ("GameData\\Caviar_Dreams_Bold.ttf", mTextSize, 0);
	itemBackgroundColor = al_map_rgb(100,100,250);
	itemTextColor = al_map_rgb(200,200,200);
	selectedItemBackgroundColor = al_map_rgb(50,50,250);
	selectedItemTextColor = al_map_rgb(250,250,250);
	if (!soundCick)
	{
		soundCick = al_load_sample ("GameData\\click.ogg");
		soundSelect = al_load_sample ("GameData\\select.ogg");
	}
}

void Menu::initMenuBlock (int aBegin, int aEnd)
{
	mDataInputMode = false;
	mMenuBlockFirstItem = aBegin;
	mMenuBlockLastItem = aEnd;
	mMenuBlockItemsCount = aEnd - aBegin + 1;
	mMenuItemsCoord.resize(mMenuBlockItemsCount);
	mSelectedItem = aBegin;
	mMenuBlockWidth = 0;
	mMenuBlockHeight = (aEnd - aBegin) * (mTextSize + mLineSpacing); 
	mStartHeight = (mScreenHeight - mMenuBlockHeight) / 2 - mLineSpacing;	
	
	//finds max menu item string lenght
	for (int i = aBegin; i <= aEnd; i++)
	{
		if (mMenuBlockWidth < al_get_text_width(selectedFont, &mMenuItems[i][0]) )
			mMenuBlockWidth = al_get_text_width(selectedFont, &mMenuItems[i][0]);
	}
	mMenuBlockWidth *= 1.1;	// menuBlockWidth 10% wider then max string lenght
	mMenuBlockWidthCenter = mMenuBlockWidth / 2;

	//fills items coordinates for mouse
	int i, j;
	for (i = mMenuBlockFirstItem, j = 0; i <= mMenuBlockLastItem; i++, j++)
	{
		mMenuItemsCoord[j].menuItemNumber = i;
		mMenuItemsCoord[j].startX	= mScreenWidthCenter - mMenuBlockWidthCenter;
		mMenuItemsCoord[j].startY	= mStartHeight + j * (mTextSize + mLineSpacing);
		mMenuItemsCoord[j].endX		= mScreenWidthCenter + mMenuBlockWidthCenter; 
		mMenuItemsCoord[j].endY		= mStartHeight + j * (mTextSize + mLineSpacing) + mTextSize + mLineSpacing / 4 ;
	}
}

void Menu::draw()
{
	int i, j;
	for ( i = mMenuBlockFirstItem, j = 0; i <= mMenuBlockLastItem; i++, j++)
	{
		if (i == mSelectedItem)	//drawing selected item
		{
			al_draw_filled_rectangle (
				mScreenWidthCenter - mMenuBlockWidthCenter, 
				mStartHeight + j * (mTextSize + mLineSpacing) , 
				mScreenWidthCenter + mMenuBlockWidthCenter, 
				mStartHeight + j * (mTextSize + mLineSpacing) + mTextSize + mLineSpacing / 4 , 
				selectedItemBackgroundColor );
		
			al_draw_rectangle (
				mScreenWidthCenter - mMenuBlockWidthCenter, 
				mStartHeight + j * (mTextSize + mLineSpacing), 
				mScreenWidthCenter + mMenuBlockWidthCenter, 
				mStartHeight + j * (mTextSize + mLineSpacing) + mTextSize + mLineSpacing / 4, 
				selectedItemTextColor,
				2);
			al_draw_text (selectedFont, selectedItemTextColor, mScreenWidthCenter, mStartHeight + j*(mTextSize + mLineSpacing), 
				ALLEGRO_ALIGN_CENTER, &mMenuItems[i][0]);
		}
		else	//drawing unselected item
		{
			al_draw_filled_rectangle (
				mScreenWidthCenter - mMenuBlockWidthCenter, 
				mStartHeight + j * (mTextSize + mLineSpacing), 
				mScreenWidthCenter + mMenuBlockWidthCenter, 
				mStartHeight + j * (mTextSize + mLineSpacing) + mTextSize + mLineSpacing / 4, 
				itemBackgroundColor );
		
			al_draw_rectangle (
				mScreenWidthCenter - mMenuBlockWidthCenter, 
				mStartHeight + j * (mTextSize + mLineSpacing), 
				mScreenWidthCenter + mMenuBlockWidthCenter, 
				mStartHeight + j * (mTextSize + mLineSpacing) + mTextSize + mLineSpacing / 4, 
				itemTextColor,
				2);

			al_draw_text (usualFont, itemTextColor, mScreenWidthCenter, mStartHeight + j*(mTextSize + mLineSpacing), 
				ALLEGRO_ALIGN_CENTER, &mMenuItems[i][0]);
		}
	}

	if (mDataInputMode)
	{
		int pos = mSelectedItem - mMenuBlockFirstItem;
		al_draw_filled_rectangle (
			mScreenWidthCenter - mMenuBlockWidthCenter, 
			mStartHeight + pos * (mTextSize + mLineSpacing) , 
			mScreenWidthCenter + mMenuBlockWidthCenter, 
			mStartHeight + pos * (mTextSize + mLineSpacing) + mTextSize + mLineSpacing / 4 , 
			selectedItemBackgroundColor );
		
		al_draw_rectangle (
			mScreenWidthCenter - mMenuBlockWidthCenter, 
			mStartHeight + pos * (mTextSize + mLineSpacing), 
			mScreenWidthCenter + mMenuBlockWidthCenter, 
			mStartHeight + pos * (mTextSize + mLineSpacing) + mTextSize + mLineSpacing / 4, 
			selectedItemTextColor,
			2);
		al_draw_text (selectedFont, selectedItemTextColor, mScreenWidthCenter, mStartHeight + pos * (mTextSize + mLineSpacing), 
			ALLEGRO_ALIGN_CENTER, mInputString.c_str() );
	
	}
}

void Menu::changeSelectedItem (int aChange)
{
	int result = mSelectedItem + aChange;
	if (result <= mMenuBlockLastItem && result >= mMenuBlockFirstItem)
		mSelectedItem += aChange;
	else if (result > mMenuBlockLastItem)
		mSelectedItem = mMenuBlockFirstItem;
	else 
		mSelectedItem = mMenuBlockLastItem;
	al_play_sample (soundCick, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
}

bool Menu::checkInput(ALLEGRO_EVENT events)
{
	//keyboard input check
	if (events.type == ALLEGRO_EVENT_KEY_UP)
	{
		if (events.keyboard.keycode == ALLEGRO_KEY_DOWN)
		{
			changeSelectedItem(+1);
			mDataInputMode = false;
		}
		else if (events.keyboard.keycode == ALLEGRO_KEY_UP)
		{
			changeSelectedItem(-1);
			mDataInputMode = false;
		}
		else if (mDataInputMode && events.keyboard.keycode > 26 && events.keyboard.keycode < 47 && mSimbolsToInput > 0)
		{
			string tmp = al_keycode_to_name(events.keyboard.keycode);
			mInputString += tmp.back();
			mInputedData = atoi (mInputString.c_str() );
			mSimbolsToInput--;
		}
		else if (events.keyboard.keycode == ALLEGRO_KEY_ENTER || events.keyboard.keycode == ALLEGRO_KEY_PAD_ENTER) 
		{
			mDataInputMode = false;
			al_play_sample (soundSelect, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
			return (true);	
		}
	}
	//mouse input check
	if (!mDataInputMode && events.type == ALLEGRO_EVENT_MOUSE_AXES)
	{
		for (int i = 0; i < mMenuBlockItemsCount; i++)
		{
			if (events.mouse.x > mMenuItemsCoord[i].startX &&
				events.mouse.x < mMenuItemsCoord[i].endX &&
				events.mouse.y > mMenuItemsCoord[i].startY &&
				events.mouse.y < mMenuItemsCoord[i].endY)
			{
				if (mSelectedItem != mMenuItemsCoord[i].menuItemNumber)
				{
					al_play_sample (soundCick, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
					mSelectedItem = mMenuItemsCoord[i].menuItemNumber;
				}
				return (false);
			}
		}
	}
	
	if (!mDataInputMode && events.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN && (events.mouse.button & 1) ) // mouse button pressed
	{
		for (int i = 0; i < mMenuBlockItemsCount; i++)
		{
			if (events.mouse.x > mMenuItemsCoord[i].startX &&
				events.mouse.x < mMenuItemsCoord[i].endX &&
				events.mouse.y > mMenuItemsCoord[i].startY &&
				events.mouse.y < mMenuItemsCoord[i].endY)
			{
				mSelectedItem = mMenuItemsCoord[i].menuItemNumber;
				al_play_sample (soundSelect, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
				return (true);
			}
		}

	}

	return (false);
}

int Menu::getSelectedItem ()
{
	return (mSelectedItem);
}

int Menu::getInputedData ()
{
	int tmp = mInputedData;
	mInputedData = 0;
	return (tmp);
}

void Menu::setDataInputMode(int aSimbols)
{
	mDataInputMode = true;
	mInputString.clear();
	mSimbolsToInput = aSimbols;
}

void Menu::setMenuItemName(int aItem, string aStr )
{
	mMenuItems[aItem].assign(aStr);
}
