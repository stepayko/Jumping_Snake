#pragma once
#include <allegro5\allegro.h>				 
#include <allegro5\allegro_primitives.h>		
#include <allegro5\allegro_ttf.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_audio.h>
#include <allegro5\allegro_acodec.h>
#include <vector> 
#include <string> 
#include <cstdlib>
 
using namespace std;

class Menu
{
public:
	Menu(void);
	~Menu(void);

	void initMenu (vector<string> mItems, int aScrWidth, int aScrHeight, int aUnSize);
	void initMenuBlock (int aBegin, int aEnd);
	void changeSelectedItem (int aChange);
	void draw();
	bool checkInput(ALLEGRO_EVENT events);
	int getSelectedItem ();
	void setDataInputMode(int aSimbols);
	int getInputedData ();
	void setMenuItemName(int aItem, string aStr );


private:
	int mScreenWidth;
	int mScreenWidthCenter;
	int mScreenHeight;
	int mUnitSize;
	int mTextSize;
	int mLineSpacing;
	int mSelectedItem;
	
	vector<string> mMenuItems;
	
	//menu block parameters
	int mMenuBlockFirstItem;
	int mMenuBlockLastItem;
	int mMenuBlockItemsCount;
	int mMenuBlockWidth;
	int mMenuBlockWidthCenter;
	int mMenuBlockHeight;
	int mStartHeight;
	int mSimbolsToInput;

	struct coord
	{
		int menuItemNumber;
		float startX;
		float startY;
		float endX;
		float endY;
	};

	vector <coord> mMenuItemsCoord;
	
	//data input parameters	
	bool mDataInputMode;
	string mInputString;
	int mInputedData;

	ALLEGRO_SAMPLE *soundCick;
	ALLEGRO_SAMPLE *soundSelect; 
	ALLEGRO_FONT *usualFont;
	ALLEGRO_FONT *selectedFont;
	ALLEGRO_COLOR itemBackgroundColor;
	ALLEGRO_COLOR itemTextColor;
	ALLEGRO_COLOR selectedItemBackgroundColor;
	ALLEGRO_COLOR selectedItemTextColor;

	void data_Input();
};

