#include "main.h"		//TODO LIST:	
							
Csnake ObjSnake[2];
Menu ObjMenu;

Snake firstSnake;
Snake secondSnake;
 
int main()
{
	initialization();
	al_start_timer(timer);
	
	while(!done)	//main game loop
	{
		al_wait_for_event(event_queue, &events);	//waits for next frame or button
		
		if(events.type != ALLEGRO_EVENT_TIMER)
		{
			checkUserInput();
		}
		else
		{
			al_flip_display();	
			switch(gameState)
			{
				case GAME:
					game();
					break;

				case GAME_END:
					gameEnd();
					break;

				case HELP:
					helpScreen(false);
					break;
				
				case MAIN_MENU:
					mainMenu();
					break;
				
				case LOSE:
					loseScreen();
					break;

				case WIN:
					winScreen();
					break;

				case CREDITS:
					creditsScreen(false);
					break;
			}
		}
	}

	return (0);
}

void initialization()
{
	al_init();
	al_get_monitor_info(0 , &aminfo);
	
	if (screenMode == WINDOW)
	{
		al_set_new_display_flags (ALLEGRO_RESIZABLE );
		display = al_create_display (screenWidth, screenHeight);
	}
	if (screenMode == FULLSCREEN)
	{
		screenWidth = aminfo.x2 - aminfo.x1;
		screenHeight = aminfo.y2 - aminfo.y1;
		al_set_new_display_flags (ALLEGRO_FULLSCREEN | ALLEGRO_DIRECT3D_INTERNAL );
		display = al_create_display (screenWidth, screenHeight);
		if (!display)	//if FULLSCREEN + DIRECT3D - doesn't work - start FULLSCREEN_WINDOW mode
		{		
			al_set_new_display_flags (ALLEGRO_FULLSCREEN_WINDOW);
			display = al_create_display (screenWidth, screenHeight);
		}
	}
	
	al_set_window_title (display, "Jumping Snake ver_0.9 using Allegro5 by Stepayko SJ");
	
	//addons initialisation
	al_init_image_addon();
	displayBitmap = al_load_bitmap("GameData\\icon.png");
	al_set_display_icon (display, displayBitmap);
	al_init_primitives_addon ();
	al_install_keyboard ();

	//event_queues & timers
	event_queue = al_create_event_queue();
	al_register_event_source(event_queue, al_get_keyboard_event_source() );
	timer = al_create_timer(1.0/FPS);
	al_register_event_source(event_queue, al_get_timer_event_source(timer) );

	//мышь
	al_install_mouse();
	al_register_event_source(event_queue, al_get_mouse_event_source());	//регистрируем в ней события мыши

	//fonts
	al_init_font_addon ();
	al_init_ttf_addon ();

	//colors
	backgroundColor = al_map_rgb(50,50,50);
	//audio
	al_install_audio ();
	al_init_acodec_addon ();
	al_reserve_samples (10);
	soundAm = al_load_sample ("GameData\\am.ogg");
	soundEnd = al_load_sample ("GameData\\end.ogg");
	soundPowerUp = al_load_sample ("GameData\\PowerUp.ogg");
	soundWin = al_load_sample ("GameData\\win.ogg");
	//music
	musikGame = al_load_sample ("GameData\\PPKresurrection.ogg");
	musicGameInstance = al_create_sample_instance (musikGame);
	al_set_sample_instance_playmode (musicGameInstance, ALLEGRO_PLAYMODE_LOOP);
	al_attach_sample_instance_to_mixer (musicGameInstance, al_get_default_mixer() );
	al_set_sample_instance_gain(musicGameInstance, 0.5); // music volume 50%

	musikMenu = al_load_sample ("GameData\\WaterloopWait.ogg");
	musicMenuInstance = al_create_sample_instance (musikMenu);
	al_set_sample_instance_playmode (musicMenuInstance, ALLEGRO_PLAYMODE_LOOP);
	al_attach_sample_instance_to_mixer (musicMenuInstance, al_get_default_mixer() );
	al_set_sample_instance_gain(musicMenuInstance, 0.8); // music volume 80%

	al_play_sample_instance (musicMenuInstance);
	//random
	srand (time(0) );
	//main menu items
	menuItems.push_back ("CONTINUE GAME");	//shoul be equal to mainMenu enum 
	menuItems.push_back ("NEW GAME");
	menuItems.push_back ("SETTINGS");
	menuItems.push_back ("HELP");
	menuItems.push_back ("CREDITS");
	menuItems.push_back ("EXIT");
	menuItems.push_back ("ONE PLAYER");
	menuItems.push_back ("TWO PLAYERS");
	menuItems.push_back ("COOPERATIVE");
	menuItems.push_back ("VERSUS");
	menuItems.push_back ("SCREEN SETTINGS");
	menuItems.push_back ("GAME SETTINGS");
	menuItems.push_back ("SET FULLSCREEN MODE");
	menuItems.push_back ("SET WINDOW MODE");
	menuItems.push_back ("change window width");
	menuItems.push_back ("change window height");
	menuItems.push_back ("UNIT SIZE (10-50)");
	menuItems.push_back ("GAME SPEED");
	menuItems.push_back ("BACKGROUND COLOR");
	menuItems.push_back ("LOW");
	menuItems.push_back ("NORMAL");
	menuItems.push_back ("FAST");
	menuItems.push_back ("RED");
	menuItems.push_back ("GREEN");
	menuItems.push_back ("BLUE");
	menuItems.push_back ("YELLOW");
	menuItems.push_back ("GREY");
	menuItems.push_back ("BLACK");

	initGame();
}

void mainMenu()
{
	al_clear_to_color (backgroundColor);
	ObjMenu.draw ();
}

void helpScreen(bool restart)
{	
	static int helpStep = 0;

	if (restart)
		helpStep = 0;

	switch (helpStep)
	{
		case 0:
			al_clear_to_color (backgroundColor); 
			ObjSnake[0].drawSnake();
			ObjSnake[1].drawSnake();
			if (wait(2) ) return;	//if two seconds passed
			helpStep++;
			return;

		case 1:
			al_clear_to_color (backgroundColor ); 
			ObjSnake[0].drawSnake();
			ObjSnake[1].drawSnake();
			al_draw_text (fontMed, al_map_rgb(250,20,20), screenWidth / 2, screenHeight /2 - unitSize*3, ALLEGRO_ALIGN_CENTER, 
							"THIS IS YOUR SNAKES");
			if (wait(2) ) return;	//if two seconds passed
			helpStep++;
			return;

		case 2:
			al_clear_to_color (backgroundColor ); 
			ObjSnake[0].drawSnake();
			ObjSnake[1].drawSnake();
			al_draw_text (fontMed, al_map_rgb(250,20,20), screenWidth / 2, screenHeight /2 - unitSize*3, ALLEGRO_ALIGN_CENTER, 
							"THIS IS YOUR SNAKES");
			al_draw_text (font, al_map_rgb(80,80,250), screenWidth / 8 * 7, screenHeight /2 + unitSize, ALLEGRO_ALIGN_RIGHT, 
						"USE LEFT, RIGHT & UP ARROWS TO CONTROL IT"); 
			al_draw_text (font, al_map_rgb(220,0,0), screenWidth / 8, screenHeight /2 + unitSize * 3, ALLEGRO_ALIGN_LEFT, 
						"PLAYER 2 CONTROLS IS A, D & W BUTTONS"); 
			if (wait(5) ) return;	//if three seconds passed
			helpStep++;
			return;

		case 3:
			al_clear_to_color (backgroundColor ); 
			drawMap();
			ObjSnake[0].drawSnake();
			ObjSnake[1].drawSnake();
			if (wait(3) ) return;	//if three seconds passed
			helpStep++;
			return;

		case 4:
			al_clear_to_color (backgroundColor ); 
			drawMap();
			ObjSnake[0].drawSnake();
			ObjSnake[1].drawSnake();
			{	//this block created to skip error C2360
				char str[50] = "Collect all food and bonuses to win";
				int strLng = al_get_text_width(fontMed, str);
				al_draw_filled_rectangle (screenWidth / 2 - strLng / 2, screenHeight /3 , screenWidth/2 + strLng / 2, screenHeight /3 + unitSize*2, 
											backgroundColor );
				al_draw_text (fontMed, al_map_rgb(20,250,20), screenWidth / 2, screenHeight /3 , ALLEGRO_ALIGN_CENTER, str);
	
				strcpy_s (str, "Avoid walls and self-eating");
				strLng = al_get_text_width(fontMed, str);
				al_draw_filled_rectangle (screenWidth / 2 - strLng / 2, screenHeight /2 , screenWidth / 2 + strLng / 2, screenHeight /2 + unitSize*2, backgroundColor );	
				al_draw_text (fontMed, al_map_rgb(20,250,20), screenWidth / 2, screenHeight /2 , ALLEGRO_ALIGN_CENTER, str);		
				if (wait(6) ) return;	//if two seconds passed
				helpStep++;
			}
			return;
	
		case 5:
			gameState = MAIN_MENU;
			al_flush_event_queue (event_queue);
			helpStep = 0;
			return;
	}
}

void checkUserInput()
{
	if (events.type == ALLEGRO_EVENT_DISPLAY_CLOSE)	//exit by window close button
		exit (0);

	if (events.type == ALLEGRO_EVENT_KEY_UP && events.keyboard.keycode == ALLEGRO_KEY_PAUSE		//game pause
		|| events.type == ALLEGRO_EVENT_DISPLAY_SWITCH_OUT ) 
	{
		al_draw_text (fontBig, al_map_rgb(250,250,250), screenWidth / 2, screenHeight /2 - unitSize*3, ALLEGRO_ALIGN_CENTER, "PAUSE");
		al_flip_display();
		al_stop_timer(timer);
		al_flush_event_queue (event_queue);

		al_wait_for_event(event_queue, &events);
		al_start_timer(timer);
	}
	
	switch (gameState)
	{
		case MAIN_MENU:	
			if (	(events.type == ALLEGRO_EVENT_KEY_UP && events.keyboard.keycode == ALLEGRO_KEY_ESCAPE) ||  //menu exit
					(events.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN && (events.mouse.button & 2) )	)
				ObjMenu.initMenuBlock(NEW_GAME, EXIT);
			menuProcessing();
			break;
		
		case GAME:
			if (events.type == ALLEGRO_EVENT_KEY_UP && events.keyboard.keycode == ALLEGRO_KEY_ESCAPE)  //game mode exit
			{
				if (repeat == true)	//sets win conditions for finishing repeat
					foodLeft = bonusesLeft = 0;
				else
				{
					gameState = MAIN_MENU;
					al_stop_sample_instance (musicGameInstance);
					al_play_sample_instance (musicMenuInstance);
					ObjMenu.initMenuBlock(CONTINUE_GAME, EXIT);
				}
			}
			break;

		case HELP:
			if (events.type == ALLEGRO_EVENT_KEY_UP && events.keyboard.keycode == ALLEGRO_KEY_ESCAPE)  //helpScreen exit
			{
				gameState = MAIN_MENU;
				ObjMenu.initMenuBlock(NEW_GAME, EXIT);
			}
			break;

		case GAME_END:
			repeat = false;
			ObjSnake[0].initSnake (screenWidth, screenHeight, unitSize, maxSnakeLong, firstSnake);
			ObjSnake[1].initSnake (screenWidth, screenHeight, unitSize, maxSnakeLong, secondSnake);

			if (events.type == ALLEGRO_EVENT_KEY_UP && events.keyboard.keycode == ALLEGRO_KEY_LEFT )
			{
				repeat = true;
				fillMap(0);
				repeat = false;
				al_play_sample_instance (musicGameInstance);
				gameState = GAME;
				al_flush_event_queue (event_queue);
			}
			else if (events.type == ALLEGRO_EVENT_KEY_UP && events.keyboard.keycode == ALLEGRO_KEY_RIGHT )
			{
				al_play_sample_instance (musicGameInstance);
				cleanMap ();
				fillMap(maxSnakeLong);
				gameState = GAME;
				al_flush_event_queue (event_queue);
			}
			else if (events.type == ALLEGRO_EVENT_KEY_UP && events.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
			{
				al_play_sample_instance (musicMenuInstance);
				cleanMap();
				fillMap(maxSnakeLong);
				ObjMenu.initMenuBlock(NEW_GAME, EXIT);
				gameState = MAIN_MENU;
			}
			break;

		case CREDITS:
			if (events.type == ALLEGRO_EVENT_KEY_UP && events.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
				gameState = MAIN_MENU;
	}
}

void menuProcessing()
{
	if (!ObjMenu.checkInput(events) )	//check is some menu item selected
		return;
	
	int tmp;
	switch (ObjMenu.getSelectedItem() )
	{
		case EXIT:
			exit (0);
				
		case HELP_SCREEN:
			helpScreen(true);
			gameState = HELP;
			break;
				
		case CONTINUE_GAME:
			al_stop_sample_instance (musicMenuInstance);
			al_play_sample_instance (musicGameInstance);
			gameState = GAME;
			break;

		case NEW_GAME:
			ObjMenu.initMenuBlock(ONE_PLAYER, TWO_PLAYERS);
			break;
				
		case ONE_PLAYER:
			playersCount = 1;
			cleanMap();
			fillMap(maxSnakeLong);
			bonusesLeft = bonuses;
			foodLeft = maxSnakeLong;
			ObjSnake[0].initSnake (screenWidth, screenHeight, unitSize, maxSnakeLong, firstSnake);
			bonusesLeft = bonuses;
			foodLeft = maxSnakeLong;
			gameState = GAME;
			al_stop_sample_instance (musicMenuInstance);
			al_play_sample_instance (musicGameInstance);
			al_flush_event_queue (event_queue);
			break;
				
		case TWO_PLAYERS:
			playersCount = 2;
			cleanMap();
			fillMap(maxSnakeLong);
			bonusesLeft = bonuses;
			foodLeft = maxSnakeLong;
			ObjSnake[0].initSnake (screenWidth, screenHeight, unitSize, maxSnakeLong, firstSnake);
			ObjSnake[1].initSnake (screenWidth, screenHeight, unitSize, maxSnakeLong, secondSnake);
			gameState = GAME;
			al_stop_sample_instance (musicMenuInstance);
			al_play_sample_instance (musicGameInstance);
			al_flush_event_queue (event_queue);
			break;
				
		case SETTINGS:
			ObjMenu.initMenuBlock(SCREEN_SETTINGS, GAME_SETTINGS);
			break;
				
		case SCREEN_SETTINGS:
			ObjMenu.initMenuBlock(FULL_SCREEN, CHANGE_WINDOW_HEIGHT);
			break;

		case GAME_SETTINGS:
			ObjMenu.initMenuBlock(UNIT_SIZE, BACKGROUND_COLOR);
			break;
			
		case FULL_SCREEN:
			al_destroy_display (display);
			screenWidth = aminfo.x2 - aminfo.x1;
			screenHeight = aminfo.y2 - aminfo.y1;
			al_set_new_display_flags (ALLEGRO_FULLSCREEN | ALLEGRO_DIRECT3D_INTERNAL );
			display = al_create_display (screenWidth, screenHeight);
			if (!display)	//if FULLSCREEN + DIRECT3D - doesn't work - start FULLSCREEN_WINDOW mode
			{		
				al_set_new_display_flags (ALLEGRO_FULLSCREEN_WINDOW);
				display = al_create_display (screenWidth, screenHeight);
			}
			initGame();
			break;
				
		case WINDOW_MODE:
			al_destroy_display (display);
			screenWidth = newWidth;
			screenHeight = newHeight;
			al_set_new_display_flags (ALLEGRO_RESIZABLE );
			display = al_create_display (screenWidth, screenHeight);
			initGame();
			break;
				
		case CHANGE_WINDOW_WIDTH:
			tmp = ObjMenu.getInputedData();
			if (tmp >= 100 && tmp <= (aminfo.x2 - aminfo.x1) ) 
			{
				newWidth = tmp;
				string str;
				str.assign("width ");
				str += to_string((long long)newWidth);
				ObjMenu.setMenuItemName (CHANGE_WINDOW_WIDTH, str);
			}
			else
				ObjMenu.setDataInputMode(4);
			break;

		case CHANGE_WINDOW_HEIGHT:
			tmp = ObjMenu.getInputedData();
			if (tmp >= 100 && tmp <= (aminfo.y2 - aminfo.y1) ) 
			{
				newHeight = tmp;
				string str;
				str.assign("height ");
				str += to_string((long long)newHeight);
				ObjMenu.setMenuItemName (CHANGE_WINDOW_HEIGHT, str);
			}
			else
				ObjMenu.setDataInputMode(4);
			break;
	
		case UNIT_SIZE:
			tmp = ObjMenu.getInputedData();
			if (tmp >= 10 && tmp <= 50 ) 
			{
				unitSize = tmp;
				initGame();
			}
			else
				ObjMenu.setDataInputMode(4);
			break;

		case GAME_SPEED:
			ObjMenu.initMenuBlock(LOW, FAST);
			break;

		case LOW:
			ObjMenu.setMenuItemName (GAME_SPEED, "GAME SPEED is LOW");
			speedCoef = 0.6;
			firstSnake.speed = defaultSpeed * speedCoef;
			secondSnake.speed = defaultSpeed * speedCoef;
			ObjMenu.initMenuBlock(UNIT_SIZE, BACKGROUND_COLOR);
			break;

		case NORMAL:
			ObjMenu.setMenuItemName (GAME_SPEED, "GAME SPEED is NORM");
			speedCoef = 1;
			firstSnake.speed = defaultSpeed * speedCoef;
			secondSnake.speed = defaultSpeed * speedCoef;
			ObjMenu.initMenuBlock(UNIT_SIZE, BACKGROUND_COLOR);
			break;

		case FAST:
			ObjMenu.setMenuItemName (GAME_SPEED, "GAME SPEED is FAST");
			speedCoef = 1.4;
			firstSnake.speed = defaultSpeed * speedCoef;
			secondSnake.speed = defaultSpeed * speedCoef;
			ObjMenu.initMenuBlock(UNIT_SIZE, BACKGROUND_COLOR);
			break;

		case BACKGROUND_COLOR:
			ObjMenu.initMenuBlock(RED, BLACK);
			break;
		
		case RED:
			backgroundColor = al_map_rgb (50, 0, 0);
			break;

		case GREEN:
			backgroundColor = al_map_rgb (0, 50, 0);
			break;

		case BLUE:
			backgroundColor = al_map_rgb (0, 0, 50);
			break;

		case YELLOW:
			backgroundColor = al_map_rgb (50, 50, 0);
			break;

		case GREY:
			backgroundColor = al_map_rgb (50, 50, 50);
			break;

		case BLACK:
			backgroundColor = al_map_rgb (0, 0, 0);
			break;

		case CREDITS_SCREEN:
			gameState = CREDITS;
			creditsScreen(true);
			break;
	}

}

void game()
{
	al_get_keyboard_state(&keyState);	
			
	if (al_key_down(&keyState, ALLEGRO_KEY_RIGHT) )
		ObjSnake[0].directionChange(+0.08);
				
	if (al_key_down(&keyState, ALLEGRO_KEY_LEFT) )
		ObjSnake[0].directionChange(-0.08);
	
	if (al_key_down(&keyState, ALLEGRO_KEY_UP) )
		ObjSnake[0].setJump();

	if (playersCount > 1 && al_key_down(&keyState, ALLEGRO_KEY_D) )
		ObjSnake[1].directionChange(+0.08);
				
	if (playersCount > 1 && al_key_down(&keyState, ALLEGRO_KEY_A) )
		ObjSnake[1].directionChange(-0.08);

	if (playersCount > 1 && al_key_down(&keyState, ALLEGRO_KEY_W) )
		ObjSnake[1].setJump();
				
						
	al_clear_to_color (backgroundColor ); 
			
	drawMap();
			
	for (int i = 0; i < playersCount; i++)
	{
	ObjSnake[i].moveSnake(false, repeat);
	ObjSnake[i].drawSnake();
	checkMap(&ObjSnake[i]);
	}

	drawEffects(DRAW, 0, 0, 0);
}

void gameEnd()
{
	al_clear_to_color (backgroundColor );
	al_draw_text (fontMed, al_map_rgb(250,250,20), screenWidth / 2, screenHeight /2 - unitSize*4, 
		ALLEGRO_ALIGN_CENTER, "Press LEFT to retry,");
	al_draw_text (fontMed, al_map_rgb(250,250,20), screenWidth / 2, screenHeight /2 - unitSize*2 , 
		ALLEGRO_ALIGN_CENTER, "RIGHT to start new game,");
	al_draw_text (fontMed, al_map_rgb(250,250,20), screenWidth / 2, screenHeight /2 , 
		ALLEGRO_ALIGN_CENTER, "ESC - exit to Main Menu");
}

void checkMap(Csnake *snake)
{
	//marking and cleaning snake on map
	if (snake->getSnakeBodySize() > 1)
	{	//marking
		int secondSegmXpos = snake->getSecondSegmentStruct().xPos / unitSize;
		int secondSegmYpos = snake->getSecondSegmentStruct().yPos / unitSize;
		if (map[secondSegmXpos][secondSegmYpos].type == EMPTY_MAP && !snake->getSnakeHeadStruct().jump)	
			map[secondSegmXpos][secondSegmYpos].type = SNAKE; 
		//cleaning
		int lastSegmXpos = snake->getLastSegmentStruct().xPos / unitSize;
		int lastSegmYpos = snake->getLastSegmentStruct().yPos / unitSize;
		if (map[lastSegmXpos][lastSegmYpos].type == SNAKE)
			map[lastSegmXpos][lastSegmYpos].type = EMPTY_MAP; 
	}

	//map check in snake head position
	if (snake->getSnakeHeadStruct().jump)	//is snake in jump - checkMap not neccessary
		return;
	int x = snake->getSnakeHeadStruct().xPos / unitSize;
	int y = snake->getSnakeHeadStruct().yPos / unitSize;
	mapObjects object = map[x][y];

	if (foodLeft == 0 && bonusesLeft == 0) //if all food and bonuses collected - WIN
	{
		al_stop_sample_instance (musicGameInstance);
		al_play_sample (soundWin, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
		ObjSnake[0].initSnake (screenWidth, screenHeight, unitSize, maxSnakeLong, firstSnake);
		ObjSnake[1].initSnake (screenWidth, screenHeight, unitSize, maxSnakeLong, secondSnake);
		gameState = WIN;
		return;
	}

	if (object.type == EMPTY_MAP)
		return;

	if (!godMode && (object.type == SNAKE || object.type == WALL))	//collision with own body or wall - GAME OVER
	{
		gameState = LOSE;
		return;
	}

	if (object.type == FOOD && snake->getSnakeBodySize() < maxSnakeLong - 1) 
	{
		al_play_sample (soundAm, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
		map[x][y].type = EMPTY_MAP;
		foodLeft--;
		snake->increase (object.form, object.color, object.thickness);
		drawEffects(SET, x, y, BLAST_OUT);
	}

	if (object.type == BONUS)
	{
		al_play_sample (soundPowerUp, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);

		if (object.bonusType == SPEED_UP)
		{
			map[x][y].type = EMPTY_MAP;

			//cleaning map from snake mark
			int i, j;
			for (i=0; i < screenWidth / unitSize; i++)
			{
				for (j=0; j < screenHeight / unitSize; j++)
					if (map[i][j].type == SNAKE)
						map[i][j].type = EMPTY_MAP;
			}

			snake->SpeedUpBonusPicked(); //ускорение змейки
			bonusesLeft--;	
		}
		
		if (object.bonusType == CUT && snake->getSnakeBodySize() >=0)
		{
			map[x][y].type = EMPTY_MAP; //bonus picked up from map
			
			//cleaning map from snake mark
			int i, j;
			for (i=0; i < screenWidth / unitSize; i++)
			{
				for (j=0; j < screenHeight / unitSize; j++)
					if (map[i][j].type == SNAKE)
						map[i][j].type = EMPTY_MAP;
			}

			//deleted segment properties we store in tmpObj
			mapObjects tmpObj;
			
			tmpObj.type = FOOD;
			tmpObj.form = snake->getLastSegmentStruct().form;
			tmpObj.thickness = snake->getLastSegmentStruct().thickness;	
			tmpObj.color = snake->getLastSegmentStruct().color;

			//map position of deleted segment
			int lastSegmXpos = snake->getLastSegmentStruct().xPos / unitSize;
			int lastSegmYpos = snake->getLastSegmentStruct().yPos / unitSize;
			map[lastSegmXpos][lastSegmYpos] = tmpObj; //put deleted segment to map
			snake->reduse();	//cutting snake body
			bonusesLeft--;	
			foodLeft++;
			drawEffects(SET, lastSegmXpos, lastSegmYpos, BLAST_OUT); //drawing visual effect
		}
	}
}

void fillMap(int food)	
{	
	static vector < vector<mapObjects> > historyMap;
	if (repeat)
	{
		map = historyMap;
		return;
	}

	//putting wals to map
	int xSize = screenWidth/unitSize;
	int ySize = screenHeight/unitSize;
	int lenght = min(xSize, ySize) / 3;

	fillMap_drawWallCross (xSize / 4,				ySize / 4,			lenght);
	fillMap_drawWallCross (xSize / 2 + xSize / 4,   ySize/2 + ySize/4,	lenght);
	fillMap_drawWallCross (xSize/4,					ySize/2 + ySize/4,	lenght);
	fillMap_drawWallCross (xSize / 2 + xSize / 4,   ySize / 4,			lenght);
	
	for (int i=0; i<bonuses / 2; i++) //putting bonuses to map (2 in 1 cycle)
	{
		mapObjects tmpObj;
		tmpObj.type = BONUS;
		tmpObj.bonusType = SPEED_UP;
		tmpObj.color = al_map_rgb (250, 0, 0);
		putObject2map (tmpObj); 

		tmpObj.bonusType = CUT;
		tmpObj.color = al_map_rgb (0, 0, 250);
		putObject2map (tmpObj); 	
	}
	
	for (int i=0; i < food; i++)	//putting food to map
	{
		mapObjects tmpObj;
		tmpObj.type = FOOD;
		tmpObj.form = rand()%4; // CIRCLE_EMPTY=0, CIRCLE_FILLED=1, TRIANGLE_EMPTY=2, TRIANGLE_FILLED=3
		tmpObj.thickness = rand()%3 + 2;	
		tmpObj.color = al_map_rgb (rand()%150+100, rand()%150+100, rand()%150+100);
		putObject2map (tmpObj); 
	}

	if (!repeat) //copying map to history array (for repeat or replay)
		historyMap = map;				
	
}

void fillMap_drawWallCross(int x, int y, int lenght)
{
	int form = rand()%2 + 4; // SQUARE_EMPTY=4 or SQUARE_FILLED=5
	ALLEGRO_COLOR color = al_map_rgb (rand()%150+100, rand()%150+100, rand()%150+100);
	int thickness = rand()%3 + 2;	

	for (int i = lenght*-0.5; i <= lenght/2; i++)	//horizontal walls
	{
		map[x+i][y].type = WALL;
		map[x+i][y].form = form; // SQUARE_EMPTY=4 or SQUARE_FILLED=5
		map[x+i][y].color = color;
		map[x+i][y].thickness = thickness;	
	}
	for (int i = lenght*-0.5; i <= lenght/2; i++)	//vertical walls
	{
		map[x][y+i].type = WALL;
		map[x][y+i].form = form; // SQUARE_EMPTY=4 or SQUARE_FILLED=5
		map[x][y+i].color = color;
		map[x][y+i].thickness = thickness;	
	}
}

void putObject2map(mapObjects obj2put)
{
	int x = rand()%(screenWidth/unitSize);
	int y = rand()%(screenHeight/unitSize);
	if (map[x][y].type == EMPTY_MAP)
			map[x][y] = obj2put;
	else
		putObject2map (obj2put);
}

void drawMap()
{	
	static short shine = 80, change = 1;
	shine += FPS / 6 * change;
	if (shine < 80 || shine > 240) 
		change *= -1;
		
	int sizeX = screenWidth/unitSize, sizeY = screenHeight/unitSize;
	al_draw_rectangle (0, 0, sizeX*unitSize, sizeY*unitSize, al_map_rgb(180,180,180), 4);	//game field border
	
	for (int i = 0; i < sizeX; i++)
	{
		for (int j = 0; j < sizeY; j++)
		{
			if (map[i][j].type == FOOD)
			{	
				if (map[i][j].form == CIRCLE_FILLED)
					al_draw_filled_circle ( i * unitSize + unitSize/2, j * unitSize + unitSize/2, unitSize / 2, map[i][j].color); 
				if (map[i][j].form == CIRCLE_EMPTY)
					al_draw_circle ( i * unitSize + unitSize/2, j * unitSize + unitSize/2, unitSize / 2, map[i][j].color, map[i][j].thickness); 
				if (map[i][j].form == TRIANGLE_FILLED)
					al_draw_filled_triangle		(i * unitSize + unitSize/2,	 j * unitSize,		
												 i * unitSize,				 j * unitSize + unitSize, 
												 i * unitSize + unitSize,	 j * unitSize + unitSize,	map[i][j].color);
				if (map[i][j].form == TRIANGLE_EMPTY)
					al_draw_triangle			(i * unitSize + unitSize/2,	 j * unitSize,		
												 i * unitSize,				 j * unitSize + unitSize, 
												 i * unitSize + unitSize,	 j * unitSize + unitSize,	map[i][j].color, map[i][j].thickness);
			}
			
			if (map[i][j].type == BONUS)
			{
				if (map[i][j].bonusType == SPEED_UP)
				{
					al_draw_filled_circle ( i * unitSize + unitSize/2, j * unitSize + unitSize/2, unitSize / 2, al_map_rgb (0, shine, 0) ); 
					al_draw_text (font, map[i][j].color, i * unitSize + unitSize / 5, j * unitSize - unitSize / 7, ALLEGRO_ALIGN_LEFT, "S");
				}
				if (map[i][j].bonusType == CUT)
				{
					al_draw_filled_circle ( i * unitSize + unitSize/2, j * unitSize + unitSize/2, unitSize / 2, al_map_rgb (shine, shine, 0) ); 
					al_draw_text (font, map[i][j].color, i * unitSize + unitSize / 5, j * unitSize - unitSize / 7, ALLEGRO_ALIGN_LEFT, "B");
				}
			}
			
			if (map[i][j].type == WALL)
			{
				if (map[i][j].form == SQUARE_FILLED)
					al_draw_filled_rectangle	( i * unitSize, j * unitSize, i * unitSize + unitSize, j * unitSize + unitSize, map[i][j].color); 
				if (map[i][j].form == SQUARE_EMPTY)
					al_draw_rectangle	( i * unitSize, j * unitSize, i * unitSize + unitSize, j * unitSize + unitSize,	map[i][j].color, map[i][j].thickness); 	
			}
	
//				if (map[i][j].type == SNAKE) //show snake map marks (for debug)
//				al_draw_filled_circle ( i * unitSize + unitSize/2, j * unitSize + unitSize/2, unitSize / 4, al_map_rgb (0, shine, 0) ); 
			
		}
	}

	if (repeat)
	{
		al_draw_text (fontBig, al_map_rgb(shine,shine,20), screenWidth / 2, screenHeight /2 - unitSize*3, 
			ALLEGRO_ALIGN_CENTER, "REPLAY");
			al_draw_text (font, al_map_rgb(shine,shine,20), screenWidth / 2, screenHeight /2 + unitSize, 
			ALLEGRO_ALIGN_CENTER, "hit esc to skip");
	}
}

void drawEffects(int mode, int mapXPos, int mapYPos, int effectType)
{
	const char maxEffectsNumber = 10;
	static int effects[maxEffectsNumber][4];
	static char effectsCount = 0;
	
	if (mode == SET)
	{
		effects[effectsCount][0] = effectType;
		effects[effectsCount][1] = mapXPos;
		effects[effectsCount][2] = mapYPos;
		if (effectType == BLAST_OUT)
			effects[effectsCount][3] = 0;
		if (effectType == BLAST_IN)
			effects[effectsCount][3] = 50;
		effectsCount++;
		if (effectsCount == maxEffectsNumber-1)
			effectsCount = 0;
	}
	
	if (mode == DRAW)
		for (int i = 0; i < maxEffectsNumber; i++)
		{
			if (effects[i][0] == 0)
				continue;
			if (effects[i][0] == BLAST_OUT || effects[i][0] == BLAST_IN)
			{
				char color = 250 - (effects[i][3] * 3);
				ALLEGRO_COLOR pixelColor = al_map_rgb (color, color, color);
				al_draw_pixel ( effects[i][1] * unitSize + effects[i][3]/2, effects[i][2] * unitSize + effects[i][3]/2, pixelColor );
				al_draw_pixel ( effects[i][1] * unitSize - effects[i][3]/2, effects[i][2] * unitSize - effects[i][3]/2, pixelColor );
				al_draw_pixel ( effects[i][1] * unitSize + effects[i][3]/2, effects[i][2] * unitSize - effects[i][3]/2, pixelColor );
				al_draw_pixel ( effects[i][1] * unitSize - effects[i][3]/2, effects[i][2] * unitSize + effects[i][3]/2, pixelColor );
				al_draw_pixel ( effects[i][1] * unitSize + effects[i][3], effects[i][2] * unitSize + effects[i][3], pixelColor );
				al_draw_pixel ( effects[i][1] * unitSize - effects[i][3], effects[i][2] * unitSize - effects[i][3], pixelColor );
				al_draw_pixel ( effects[i][1] * unitSize + effects[i][3], effects[i][2] * unitSize - effects[i][3], pixelColor );
				al_draw_pixel ( effects[i][1] * unitSize - effects[i][3], effects[i][2] * unitSize + effects[i][3], pixelColor );
				al_draw_pixel ( effects[i][1] * unitSize + effects[i][3], effects[i][2] * unitSize, pixelColor );
				al_draw_pixel ( effects[i][1] * unitSize, effects[i][2] * unitSize + effects[i][3], pixelColor );
				al_draw_pixel ( effects[i][1] * unitSize - effects[i][3], effects[i][2] * unitSize, pixelColor );
				al_draw_pixel ( effects[i][1] * unitSize, effects[i][2] * unitSize - effects[i][3], pixelColor );
				if (effects[i][0] == BLAST_OUT)
					effects[i][3] ++;
				else
					effects[i][3] --;

				if (effects[i][3] > 50 || effects[i][3] < 0)
					effects[i][0] = 0;
			}	
		}
}

void loseScreen()
{
	static bool state = 0;
	if (!state)
	{
		al_stop_sample_instance (musicGameInstance);
		al_play_sample (soundEnd, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
		bonusesLeft = bonuses;
		foodLeft = maxSnakeLong;
		state = 1;
	}
	al_clear_to_color (backgroundColor);
	al_draw_text (fontBig, al_map_rgb(250,20,20), screenWidth / 2, screenHeight /2 - unitSize*3, ALLEGRO_ALIGN_CENTER, "GAME OVER");

	if (wait(4) ) return; //wait 4 seconds

	al_flush_event_queue (event_queue);
	state = 0;
	gameState = GAME_END;
}

void winScreen()
{
	al_clear_to_color (backgroundColor ); 
	al_draw_text (fontBig, al_map_rgb(250,250,20), screenWidth / 2, screenHeight /2 - unitSize*3, 
		ALLEGRO_ALIGN_CENTER, "YOU WIN");
	if (wait (4)) //wait 4 sec to show "you win" message
		return; 
	al_flush_event_queue (event_queue);
	if (repeat == true)	//game loop exit in the end or repeat
	{	
		gameState = GAME_END;
	}	
	else	 //game repeat start (REPLAY)
	{
		repeat = true;
		gameState = GAME; //because repeat plays in GAME mode
	}
	ObjSnake[0].moveSnake(true, true);
	ObjSnake[1].moveSnake(true, true);
	fillMap(0);
	bonusesLeft = bonuses;
	foodLeft = maxSnakeLong;
	
}

bool wait (double seconds)
{
	static double waitTime = al_get_time();
	if (al_get_time() - waitTime > seconds+1) //refresh wait time
		waitTime = al_get_time();
	if (al_get_time() - waitTime < seconds) //wait 4 seconds
		return (true);
	waitTime-=100;
	return (false);
}

void initGame()
{
	//normalizing game parameter - unitSize
	if ( ((screenWidth * screenHeight) / (unitSize * unitSize) ) < 200)	//if screen is very small
		unitSize = (int) sqrt ( (float) (screenWidth * screenHeight) / 200);
	if ( (screenWidth + screenHeight) / 2 / unitSize > 50 )	// if screen is very big
		unitSize = (int) (screenWidth + screenHeight) / 2 / 50;

	//normalizing game parameter - defaultSpeed
	defaultSpeed = (float) ( (screenWidth + screenHeight) / 2) / FPS / 6;	//to cross the screen in 6 seconds
	defaultSpeed *= speedCoef;	//apply user speed choise

	//fonts reload
	font = al_load_font ("GameData\\Caviar_Dreams_Bold.ttf", unitSize, 0);
	fontBig = al_load_font ("GameData\\Caviar_Dreams_Bold.ttf", unitSize * 3, 0);
	fontMed = al_load_font ("GameData\\Caviar_Dreams_Bold.ttf", unitSize * 2, 0);

	//display events register
	al_register_event_source(event_queue, al_get_display_event_source(display) );

	//display_icon set
	al_set_display_icon (display, displayBitmap);

	//menu reinitialisation
	ObjMenu.initMenu(menuItems, screenWidth, screenHeight, unitSize);
	ObjMenu.initMenuBlock (NEW_GAME, EXIT);

	//snakes reinitialisation
	firstSnake.xPos = screenWidth / 8 * 7;
	firstSnake.yPos = screenHeight / 2;
	firstSnake.color = al_map_rgb (220, 0, 0);
	firstSnake.thickness = 5.0;
	firstSnake.dir = 3.1415;
	firstSnake.speed = defaultSpeed;

	secondSnake.xPos = screenWidth / 8;
	secondSnake.yPos = screenHeight / 2;
	secondSnake.color = al_map_rgb (80, 80, 250);
	secondSnake.thickness = 5.0;
	secondSnake.dir = 0;
	secondSnake.speed = defaultSpeed;

	ObjSnake[0].initSnake (screenWidth, screenHeight, unitSize, maxSnakeLong, firstSnake);
	ObjSnake[1].initSnake (screenWidth, screenHeight, unitSize, maxSnakeLong, secondSnake);

	//map reinitialisation
	map.resize(screenWidth/unitSize); // allocating memory for strings
	for(int i = 0; i < (screenWidth/unitSize); i++)
		map[i].resize(screenHeight/unitSize);// allocating memory for columns

	cleanMap ();
	fillMap(maxSnakeLong);	
}

void cleanMap()
{
	for (int i=0; i < screenWidth / unitSize; i++) //map cleaning
	{
		for (int j=0; j < screenHeight / unitSize; j++)
			map[i][j].type = EMPTY_MAP;
	}
}

void creditsScreen(bool restart)
{
	static float height = screenHeight;
	static float shine = 1;
	static float shineDir = 1;
	static float move = 1;
	
	if (restart)
		height = screenHeight;

	al_clear_to_color (backgroundColor);
	
	if (shine < 0.7 || shine >= 1.0 )
		shineDir *= -1;
	
	shine += 0.005 * shineDir;

	al_draw_text (fontBig, al_map_rgb(250*shine, 50*shine, 50*shine), screenWidth / 2, height, ALLEGRO_ALIGN_CENTER, 
		"JUMPING SNAKE GAME");
	al_draw_text (fontMed, al_map_rgb(100,100,250), screenWidth / 2 * move, height + unitSize*4, ALLEGRO_ALIGN_CENTER, 
		"created by Stepayko Sergey");
	al_draw_text (fontMed, al_map_rgb(100,100,250), screenWidth / 2 * 1/move, height + unitSize*7, ALLEGRO_ALIGN_CENTER, 
		"in octorer 2016");
	al_draw_text (fontMed, al_map_rgb(100,100,250), screenWidth / 2 * move, height + unitSize*10, ALLEGRO_ALIGN_CENTER, 
		"used program language: C++");
	al_draw_text (fontMed, al_map_rgb(100,100,250), screenWidth / 2 * 1/move, height + unitSize*13, ALLEGRO_ALIGN_CENTER, 
		"used IDE: Microsoft Visual Studio 2010");
	al_draw_text (fontMed, al_map_rgb(100,100,250), screenWidth / 2 * move, height + unitSize*16, ALLEGRO_ALIGN_CENTER, 
		"used game engine: Allegro 5.0.10");
	al_draw_text (fontMed, al_map_rgb(100,100,250), screenWidth / 2 * 1/move, height + unitSize*19, ALLEGRO_ALIGN_CENTER, 
		"game music: PPK - resurrection");
	al_draw_text (fontMed, al_map_rgb(100,100,250), screenWidth / 2 * move, height + unitSize*22, ALLEGRO_ALIGN_CENTER, 
		"menu music: Waterloop - Wait");

	if (height <= unitSize*3)
	{
		move = shine+0.15;
		return;
	}
	height -= (float)FPS/80; 
}