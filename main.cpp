#include "pge_ui_elements.h"
#include <cstdio>

// Compile code with mingw in console:
// cd your_file_path
// g++ -o main.exe main.cpp -luser32 -lgdi32 -lopengl32 -lgdiplus -lShlwapi 
// -lstdc++fs -lDwmapi -static -std=c++17
// main.exe

class NewPlatformer : public olc::PixelGameEngine
{
// Parameters of class NewPlatformer
private:
	pge_ui::Menu menuMain;

	enum NewPlatformerStatus {NP_MAIN_MENU, NP_GAME, NP_LEVEL_EDITOR};
	NewPlatformerStatus nps = NP_MAIN_MENU;

	std::string sMainFolder = "./assets/";

	// Window cell (in abbreviated form - wc) size 
	olc::vi2d viWindowCellSize = {16, 16};

	// Block
	struct Block
	{
		// Block status
			// 0 - decorative block
			// 1 - physical block
			// 2 - "exit" block
			// 3 - "spike" block
			// 4 - player sprite
		int iStatus = 0;
		olc::vi2d viExternalSize = {0, 0};
		olc::vi2d viInternalSize = {0, 0};
		olc::vi2d viInternalSizePosition = {0, 0};
	};

	// Block data
	Block blkData[400];

// Methods of class NewPlatformer
private:

	void update_block_data()
	{
		char cBlockDataFileName[40] = "./assets/parameters.txt";
		FILE *fpBlockDataFile = fopen(cBlockDataFileName, "r");
		for (int i = 0; i < 400; i++)
			fscanf(fpBlockDataFile,
						"%d%d%d%d%d%d%d",
						&blkData[i].iStatus,
						&blkData[i].viExternalSize.x,
						&blkData[i].viExternalSize.y,
						&blkData[i].viInternalSize.x,
						&blkData[i].viInternalSize.y,
						&blkData[i].viInternalSizePosition.x,
						&blkData[i].viInternalSizePosition.y);
	}

	void show_main_menu()
	{
		std::string sOption = menuMain.check_buttons(this);

		if(sOption == "Game")
			nps = NP_GAME;
		else if(sOption == "Level editor")
			nps = NP_LEVEL_EDITOR;
		else if(sOption == "Exit")
			exit(0);
		else
		{
			Clear(olc::BLACK);
			menuMain.show(this);
		}
	}

// Description of subclass Game:
// Parameters of subclass Game
private:

	enum GameStatus {G_MENU, G_GAME};
	GameStatus gs = G_MENU;

	// OPENING
	pge_ui::Label labelGameOpening1;
	pge_ui::Label labelGameOpening2;
	pge_ui::Label labelGameOpening3;

	pge_ui::Form formGameOpening;

	// Game level opening menu:
		// Game level file name
		std::string sGameLevelFileName;

	// Game window:
		// Player:
			// Game start map player position (in wc)
			olc::vi2d viGameStartMapPlayerPosition = {0, 0};
			// Game map player position (in px)
			olc::vf2d vfGameMapPlayerPosition = {0.0f, 0.0f}; 
			// Game map player last position (in px)
			olc::vf2d vfGameMapPlayerLastPosition = {0.0f, 0.0f};
			// Game player speed (in px)
			olc::vf2d vfGamePlayerSpeed = {64.0f, 0.0f};
			// Game player gravity acceleration (in px)
			float fGamePlayerGravityAcceleration = 96.0f; 
			// Player up side move
			bool bPlayerUpSideMove = false;
			// Player down side move
			bool bPlayerDownSideMove = false;
			// Player right side move
			bool bPlayerRightSideMove = false;
			// Player left side move
			bool bPlayerLeftSideMove = false;
			// Player space key status
			bool bIsSpaceKeyPressed = false; 
			// Player upper left corner collision
			int iPlayerULCornerCollision = 0;
			// Player upper right corner collision
			int iPlayerURCornerCollision = 0;
			// Player lower left corner collision
			int iPlayerLLCornerCollision = 0;
			// Player lower right corner collision 
			int iPlayerLRCornerCollision = 0;
			// Player sprites
			int iPlayerSprites[7] = {360, 361, 362, 363, 364, 365, 366};
			// Player status
				// 0 - check status
				// 1 - first move status
				// 2 - second move status
				// 3 - in jump status
				// 4 - before jump status
				// 5 - after jump status
				// 6 - game over status
			int iPlayerStatus = 0;
		// Game map:
			// Game map size (in wc)
			olc::vi2d viGameMapSize = {34, 21};
			// Game map 
			int iGameMap[714] = {};
			// Game map window position (in wc)
			olc::vi2d viGameMapWindowPosition = {6, 2};	


// Methods of subclass Game
private:

	void check_game_buttons(float fElapsedTime = 0.0f)
	{
		// Input with keys:
		if(GetKey(olc::Key::LEFT).bHeld)
    {
      vfGameMapPlayerPosition.x -= vfGamePlayerSpeed.x * fElapsedTime;
      bPlayerLeftSideMove = true;
      bPlayerRightSideMove = false;
    }
    if(GetKey(olc::Key::RIGHT).bHeld)
    {
      vfGameMapPlayerPosition.x += vfGamePlayerSpeed.x * fElapsedTime;
      bPlayerLeftSideMove = false;
      bPlayerRightSideMove = true;
    }
    if(GetKey(olc::Key::SPACE).bPressed && !bIsSpaceKeyPressed)
    {
      vfGamePlayerSpeed.y = 96.0f;
      bIsSpaceKeyPressed = true;
    }
    if(GetKey(olc::Key::ESCAPE).bPressed)
    {
      reset_game();
      gs = G_MENU;
      nps = NP_MAIN_MENU;
    }
	}

	void open_game_level_file(const std::string& sGameLevelFileName)
	{
		char cGameLevelFileName[sGameLevelFileName.size() + 1];
		strcpy(cGameLevelFileName, sGameLevelFileName.c_str());
		FILE *fpGameLevelFile = fopen(cGameLevelFileName, "rb");
		fread(&viGameMapSize.x, sizeof(int), 1, fpGameLevelFile);
		fread(&viGameMapSize.y, sizeof(int), 1, fpGameLevelFile);
		fread(iGameMap,
					sizeof(int), 
					viGameMapSize.x * viGameMapSize.y,
					fpGameLevelFile);
		fread(&viGameStartMapPlayerPosition.x, sizeof(int), 1, fpGameLevelFile);
		fread(&viGameStartMapPlayerPosition.y, sizeof(int), 1, fpGameLevelFile);
		fclose(fpGameLevelFile);
	}

	void show_game_level_opening_menu()
	{
    if(GetKey(olc::Key::ESCAPE).bPressed)
		{
			nps = NP_MAIN_MENU;
			formGameOpening.clear_field();
		}

		sGameLevelFileName = formGameOpening.check_buttons(this);

		if(sGameLevelFileName == "")
		{
			Clear(olc::BLACK);
			labelGameOpening1.show(this);
			labelGameOpening2.show(this);
			formGameOpening.show(this);
			sGameLevelFileName = formGameOpening.get_field();
			if(is_exist_level_file_name(sMainFolder + sGameLevelFileName + ".lvl"))
			{
				labelGameOpening3.set_name("Level file found");
				labelGameOpening3.set_font_color(olc::GREEN);
			}
			else
			{
				labelGameOpening3.set_name("Level file not found");
				labelGameOpening3.set_font_color(olc::RED);
			}
			labelGameOpening3.show(this);
		}
		else if(is_exist_level_file_name(sMainFolder + sGameLevelFileName + ".lvl"))
		{
			open_game_level_file(sMainFolder + sGameLevelFileName + ".lvl");
			formGameOpening.clear_field();
			update_block_data();
			vfGameMapPlayerPosition = olc::vf2d(viGameStartMapPlayerPosition)
																* olc::vf2d(viWindowCellSize)
																+	olc::vf2d(blkData[360].viInternalSizePosition);
			gs = G_GAME;
		}
	}

	void check_game_parameters(float fElapsedTime)
	{
		// Player up/down side move check
		if(vfGamePlayerSpeed.y >= 0) 
		{ 
			bPlayerDownSideMove = false;
			bPlayerUpSideMove = true;
		}
		if(vfGamePlayerSpeed.y < 0) 
		{
			bPlayerDownSideMove = true;
			bPlayerUpSideMove = false;
		}

		// Gravity emulation
		if(bPlayerDownSideMove || bPlayerUpSideMove)
		{
			vfGameMapPlayerPosition.y -= vfGamePlayerSpeed.y * fElapsedTime;
			vfGameMapPlayerPosition.y += fGamePlayerGravityAcceleration
																		* fElapsedTime*fElapsedTime/2.0f;
			vfGamePlayerSpeed.y -= fGamePlayerGravityAcceleration * fElapsedTime;
		}

		check_collisions(fElapsedTime);

		// Reset move parameters
		bPlayerRightSideMove = false;
		bPlayerLeftSideMove = false;
		bPlayerUpSideMove = false;
		bPlayerDownSideMove = false;
	}

	void check_collisions(float fElapsedTime)
	{
		// The player is outside the map area 
		if(vfGameMapPlayerPosition.x < 0
		|| vfGameMapPlayerPosition.y < 0
		|| (vfGameMapPlayerPosition.x + float(blkData[360].viInternalSize.x))
				> viGameMapSize.x*16.0f
		|| (vfGameMapPlayerPosition.y + float(blkData[360].viInternalSize.y))
				> viGameMapSize.y*16.0f)
		{
			reset_game();
		}

		detect_collisions();

		process_collisions(fElapsedTime);
	}

	void reset_game()
	{
		vfGamePlayerSpeed = olc::vf2d(64.0f, 0);
		vfGameMapPlayerPosition = olc::vf2d(viGameStartMapPlayerPosition)
															* olc::vf2d(viWindowCellSize)
															+ olc::vf2d(blkData[360].viInternalSizePosition);
		bPlayerRightSideMove = false;
		bPlayerLeftSideMove = false;
		bPlayerUpSideMove = false;
		bPlayerDownSideMove = false;
		bIsSpaceKeyPressed = false;
	}

	bool detect_collisions()
	{
		// // Var 1
		// int iULCornerBlockNumber =  int(vfGameMapPlayerPosition.y)/16 *
		// 													viGameMapSize.x +
		// 													int(vfGameMapPlayerPosition.x)/16;
		// int iURCornerBlockNumber = iULCornerBlockNumber+1;
		// int iLLCornerBlockNumber =  iULCornerBlockNumber+viGameMapSize.x;
		// int iLRCornerBlockNumber = iLLCornerBlockNumber+1;

		// Var 2
		int iULCornerBlockNumber = int(vfGameMapPlayerPosition.y) / 16
																* viGameMapSize.x
																+ int(vfGameMapPlayerPosition.x) / 16;
		int iURCornerBlockNumber = int(vfGameMapPlayerPosition.y) / 16
																* viGameMapSize.x
																+ (int(vfGameMapPlayerPosition.x)
																+ blkData[360].viInternalSize.x - 1) / 16;
		int iLLCornerBlockNumber = (int(vfGameMapPlayerPosition.y)
																+ blkData[360].viInternalSize.y - 1) / 16
																* viGameMapSize.x
																+ int(vfGameMapPlayerPosition.x) / 16;
		int iLRCornerBlockNumber = (int(vfGameMapPlayerPosition.y)
																+ blkData[360].viInternalSize.y - 1) / 16
																* viGameMapSize.x
																+ (int(vfGameMapPlayerPosition.x)
																+ blkData[360].viInternalSize.x - 1) / 16;

		iPlayerULCornerCollision = detect_collision(iULCornerBlockNumber);
		iPlayerURCornerCollision = detect_collision(iURCornerBlockNumber);
		iPlayerLLCornerCollision = detect_collision(iLLCornerBlockNumber);
		iPlayerLRCornerCollision = detect_collision(iLRCornerBlockNumber);

		if (!iPlayerLLCornerCollision && !iPlayerLRCornerCollision 
		&& !iPlayerULCornerCollision && !iPlayerURCornerCollision)
			return false;

		// std::cout << "{" << std::endl;
		// std::cout << iPlayerULCornerCollision 
		// << iPlayerURCornerCollision << std::endl;
		// std::cout << iPlayerLLCornerCollision 
		// << iPlayerLRCornerCollision << std::endl;
		// std::cout << "}" << std::endl;

		return true;
	}

	int detect_collision(int iMapBlockNumber = 0)
	{
		if (iMapBlockNumber > 713)
			return 0;

		// Block object
		Block blkObject = blkData[iGameMap[iMapBlockNumber]];
		
		if (blkObject.iStatus == 0)
			return 0;

		olc::vf2d vfObjectMinimumPosition = 
													olc::vf2d(olc::vi2d(iMapBlockNumber%viGameMapSize.x,
																							iMapBlockNumber/viGameMapSize.x)
													* viWindowCellSize)
													+ olc::vf2d(blkObject.viInternalSizePosition);
		olc::vf2d vfObjectMaximumPosition = vfObjectMinimumPosition
																				+ olc::vf2d(blkObject.viInternalSize);

		// Block player
		Block blkPlayer = blkData[iPlayerSprites[iPlayerStatus]];
		olc::vf2d vfPlayerMinimumPosition = vfGameMapPlayerPosition;
		olc::vf2d vfPlayerMaximumPosition = vfPlayerMinimumPosition
																				+ olc::vf2d(blkPlayer.viInternalSize)
																				- olc::vf2d(1.0f, 1.0f);

		if(vfPlayerMaximumPosition.x < vfObjectMinimumPosition.x 
		|| vfPlayerMinimumPosition.x > vfObjectMaximumPosition.x)
			return 0;
		if(vfPlayerMaximumPosition.y < vfObjectMinimumPosition.y 
		|| vfPlayerMinimumPosition.y > vfObjectMaximumPosition.y)
			return 0;
		
		return blkObject.iStatus;
	}

	void process_collisions(float fElapsedTime) 
	{
		if(bPlayerDownSideMove
		&& (iPlayerLLCornerCollision == 1
				|| iPlayerLRCornerCollision == 1
				|| (iPlayerLLCornerCollision == 1
						&& iPlayerLRCornerCollision == 1)))
		{
			vfGameMapPlayerPosition.y -= 1.0f;
			if(!detect_collisions()) 
			{
				vfGamePlayerSpeed.y = 0.0f;
				bIsSpaceKeyPressed = false;
				bPlayerDownSideMove = false;
				return;
			}
			//else process_collisions(fElapsedTime);
		}

		if(bPlayerUpSideMove
		&& (iPlayerULCornerCollision == 1
				|| iPlayerURCornerCollision == 1
				|| (iPlayerULCornerCollision == 1
						&& iPlayerURCornerCollision == 1)))
		{
			vfGameMapPlayerPosition.y += 1.0f;
			if(!detect_collisions())
			{
				vfGamePlayerSpeed.y = 0.0f;
				bPlayerUpSideMove = false;
				return;
			}
			//else process_collisions(fElapsedTime);
		}

		if(bPlayerLeftSideMove
		&& (iPlayerLLCornerCollision == 1
				|| iPlayerULCornerCollision == 1
				|| (iPlayerLLCornerCollision == 1
			 			&& iPlayerULCornerCollision == 1)))
		{
			vfGameMapPlayerPosition.x += 1.0f;
			if(!detect_collisions())
			{
				bPlayerLeftSideMove = false;
				return;
			}
		}

		if(bPlayerRightSideMove
		&& (iPlayerLRCornerCollision == 1
				|| iPlayerURCornerCollision == 1
				|| (iPlayerLRCornerCollision == 1
						&& iPlayerURCornerCollision == 1)))
		{
			vfGameMapPlayerPosition.x -= 1.0f;
			if(!detect_collisions())
			{
				bPlayerRightSideMove = false;
				return;
			}
		}

		if(iPlayerULCornerCollision == 3 
		|| iPlayerURCornerCollision == 3 
		|| iPlayerLLCornerCollision == 3 
		|| iPlayerLRCornerCollision == 3)
		{
			reset_game();
			return;
		}

		if(iPlayerULCornerCollision == 2 
		|| iPlayerURCornerCollision == 2 
		|| iPlayerLLCornerCollision == 2 
		|| iPlayerLRCornerCollision == 2)
		{
			reset_game();
			gs = G_MENU;
			nps = NP_MAIN_MENU;
			return;
		}
	}

	void show_game_window(float fElapsedTime)
	{
		// // Update game map player last position
		// vfGameMapPlayerLastPosition = vfGameMapPlayerPosition;

		check_game_buttons(fElapsedTime);
		
		check_game_parameters(fElapsedTime);

		// Drawing game map
		Clear(olc::BLACK);
		DrawRect((viGameMapWindowPosition) * viWindowCellSize - olc::vi2d(2, 2),
							viGameMapSize * viWindowCellSize + olc::vi2d(3, 3));
		for (int i = 0; i < viGameMapSize.y; i++)
		{
			for (int j = 0; j < viGameMapSize.x; j++)
			{
				DrawPartialSprite((viGameMapWindowPosition + olc::vi2d(j, i))
														* viWindowCellSize,
													sprTileMap.get(),
													olc::vi2d(iGameMap[viGameMapSize.x * i + j] % 20,
													iGameMap[viGameMapSize.x * i + j] / 20)
														* viWindowCellSize,
													viWindowCellSize); 
			}
		}

		// Drawing game map player position - test version
		DrawPartialSprite(olc::vf2d(viGameMapWindowPosition * viWindowCellSize)
												+ vfGameMapPlayerPosition,
											sprTileMap.get(),
											olc::vi2d(0, 18) * viWindowCellSize
												+ blkData[360].viInternalSizePosition,
											blkData[360].viInternalSize);
	}

	void show_game(float fElapsedTime)
	{
		switch(gs)
		{
			case G_MENU:
				show_game_level_opening_menu();
				break;
			case G_GAME:
				show_game_window(fElapsedTime);
				break;
		}
	}

// Description of subclass LevelEditor:
// Parameters of subclass LevelEditor
private:

	enum LevelEditorStatus
	{
		LE_MENU,
		LE_CREATION,
		LE_CREATION_ERROR,
		LE_OPENING,
		LE_OPENING_ERROR,
		LE_EDITOR
	};
	LevelEditorStatus les = LE_MENU;

	// MENU
	pge_ui::Menu menuEditor;
	
	// CREATION
	pge_ui::Label labelCreation1;
	pge_ui::Label labelCreation2;
	pge_ui::Form formCreation;
	
	// CREATION ERROR
	pge_ui::Label labelCreationError;
	pge_ui::Menu menuCreationError;
	
	// OPENING
	pge_ui::Label labelOpening1;
	pge_ui::Label labelOpening2;
	pge_ui::Form formOpening;

	// OPENING ERROR
	pge_ui::Label labelOpeningError;
	pge_ui::Menu menuOpeningError;

	std::string sLevelFileName;

	// EDITOR
	// Level file save status 
	bool bIsLevelFileSaved = true;
	pge_ui::Label labelSaveStatus;
	// Buttons
	pge_ui::Button buttonExit;
	pge_ui::Button buttonSave;
	pge_ui::Button buttonClear;
	pge_ui::Button buttonBack;
	pge_ui::Button buttonNext;

	// Mouse window position (in wc)
	olc::vi2d viMouseWindowPosition = {0, 0};

	// Editor map:
		// Editor map size (in wc)
		olc::vi2d viEditorMapSize = {34, 21};
		// Editor map 
		int iEditorMap[714] = {};
		// Editor map window position (in wc)
		olc::vi2d viEditorMapWindowPosition = {10, 4};
		// Sprite tile map
		std::unique_ptr<olc::Sprite> sprTileMap;
		// Sprite tile transparent map
		std::unique_ptr<olc::Sprite> sprTileTransparentMap;
		// Editor start map player position
		olc::vi2d viEditorStartMapPlayerPosition = {0, 0};

	// Selected block:
		// Selected block code
		int iSelectedBlockCode = 0;
		// Selected block window position (in wc)
		olc::vi2d viSelectedBlockWindowPosition = {3, 19};

	// Editor block map:
		// Editor block map page status
		int iEditorBlockMapPageStatus = 0;
		// Editor block map size (in wc)
		olc::vi2d viEditorBlockMapSize = {8, 10};
		// Editor block map window position (in wc)
		olc::vi2d viEditorBlockMapWindowPosition = {1, 6};

// Methods of subclass LevelEditor
private:

	void check_level_editor_buttons()
	{
		// Getting mouse position:
		viMouseWindowPosition = olc::vi2d(GetMouseX(), GetMouseY())
														/ viWindowCellSize;

		// Input with mouse keys:
		// Editor map:
		if(viMouseWindowPosition.x >= viEditorMapWindowPosition.x 
		&& viMouseWindowPosition.x < viEditorMapWindowPosition.x + viEditorMapSize.x
		&& viMouseWindowPosition.y >= viEditorMapWindowPosition.y
		&& viMouseWindowPosition.y < viEditorMapWindowPosition.y
																	+ viEditorMapSize.y)
		{
			if(GetMouse(0).bHeld) 
			{
				iEditorMap[viEditorMapSize.x
									 * (viMouseWindowPosition.y - viEditorMapWindowPosition.y)
									 + (viMouseWindowPosition.x - viEditorMapWindowPosition.x)]
									 = iSelectedBlockCode;
				bIsLevelFileSaved = false;
			}
			if(GetMouse(1).bHeld) 
			{
				iEditorMap[viEditorMapSize.x
									 * (viMouseWindowPosition.y - viEditorMapWindowPosition.y)
									 + (viMouseWindowPosition.x - viEditorMapWindowPosition.x)]
									 = 0;
				bIsLevelFileSaved = false;
			}
			if(GetMouse(2).bPressed) 
			{
				viEditorStartMapPlayerPosition = viMouseWindowPosition
																				 - viEditorMapWindowPosition;
				bIsLevelFileSaved = false;
			}
		}

		// Editor block map:
		if(viMouseWindowPosition.x >= viEditorBlockMapWindowPosition.x
		&& viMouseWindowPosition.x < viEditorBlockMapWindowPosition.x
																	+ viEditorBlockMapSize.x
		&& viMouseWindowPosition.y >= viEditorBlockMapWindowPosition.y
		&& viMouseWindowPosition.y < viEditorBlockMapWindowPosition.y
																	+ viEditorBlockMapSize.y)
		{
			if(GetMouse(0).bPressed) 
				iSelectedBlockCode = iEditorBlockMapPageStatus * 80
														 + viEditorBlockMapSize.x
														 * (viMouseWindowPosition.y
														 - viEditorBlockMapWindowPosition.y)
														 + (viMouseWindowPosition.x
														 - viEditorBlockMapWindowPosition.x);
		}

		if(GetKey(olc::Key::ESCAPE).bPressed || buttonExit.is_pressed(this))
		{
			for (int i = 0; i < 714; i++)
				iEditorMap[i] = 0;
			viEditorStartMapPlayerPosition = olc::vi2d(0, 0);
			iEditorBlockMapPageStatus = 0;
			iSelectedBlockCode = 0;
			les = LE_MENU;
			bIsLevelFileSaved = true;
		}

		if(GetKey(olc::Key::S).bPressed || buttonSave.is_pressed(this))
		{
			rewrite_level_file(sMainFolder + sLevelFileName + ".lvl");
			bIsLevelFileSaved = true;
		}

		if(GetKey(olc::Key::C).bPressed || buttonClear.is_pressed(this))
		{
			for (int i = 0; i < 714; i++)
				iEditorMap[i] = 0;
			bIsLevelFileSaved = false;
		}

		if(GetKey(olc::Key::LEFT).bPressed || buttonBack.is_pressed(this))
		{
			if(iEditorBlockMapPageStatus == 0)
				iEditorBlockMapPageStatus = 4;
			else
				iEditorBlockMapPageStatus--;
		}

		if(GetKey(olc::Key::RIGHT).bPressed || buttonNext.is_pressed(this))
		{
			if(iEditorBlockMapPageStatus == 4)
				iEditorBlockMapPageStatus = 0;
			else
				iEditorBlockMapPageStatus++;
		}

		// Input with arrow keys:
		if(GetKey(olc::Key::DOWN).bPressed)
		{
			if(iSelectedBlockCode == 399)
				iSelectedBlockCode = 0;
			else
				iSelectedBlockCode++;
		}

		if(GetKey(olc::Key::UP).bPressed)
		{
			if(iSelectedBlockCode == 0)
				iSelectedBlockCode = 399;
			else
				iSelectedBlockCode--;
		}
	}

	bool is_exist_level_file_name(std::string sLevelFileName)
	{
		char cLevelFileName[sLevelFileName.size() + 1];
		strcpy(cLevelFileName, sLevelFileName.c_str());
		FILE *fpLevelFile = fopen(cLevelFileName, "rb");
		if(!fpLevelFile)
			return false;
		else
			fclose(fpLevelFile);
		return true;
	}

	void show_level_creation_window()
	{
		if(GetKey(olc::Key::ESCAPE).bPressed) 
		{
			les = LE_MENU;
			formCreation.clear_field();
		}

		sLevelFileName = formCreation.check_buttons(this);

		if(sLevelFileName == "")
		{
			Clear(olc::BLACK);
			labelCreation1.show(this);
			labelCreation2.show(this);
			formCreation.show(this);
		}
		else if(is_exist_level_file_name(sMainFolder+sLevelFileName+".lvl"))
			les = LE_CREATION_ERROR;
		else
		{
			rewrite_level_file(sMainFolder + sLevelFileName + ".lvl");
			open_level_file(sMainFolder + sLevelFileName + ".lvl");
			formCreation.clear_field();
			les = LE_EDITOR;
		}
	}

	void show_level_creation_error_window()
	{
		std::string sOption = menuCreationError.check_buttons(this);

		if(sOption == "Rewrite and open level")
		{
			rewrite_level_file(sMainFolder + sLevelFileName + ".lvl");
			open_level_file(sMainFolder + sLevelFileName + ".lvl");
			formCreation.clear_field();
			les = LE_EDITOR;
		}
		else if(sOption == "Open level")
		{
			open_level_file(sMainFolder + sLevelFileName + ".lvl");
			formCreation.clear_field();
			les = LE_EDITOR;
		}
		else if(sOption == "Back")
			les = LE_CREATION;
		else
		{
			Clear(olc::BLACK);
			labelCreationError.show(this);
			menuCreationError.show(this);
		}
	}

	void show_level_opening_window()
	{
		if(GetKey(olc::Key::ESCAPE).bPressed) 
		{
			les = LE_MENU;
			formOpening.clear_field();
		}

		sLevelFileName = formOpening.check_buttons(this);

		if(sLevelFileName == "")
		{
			Clear(olc::BLACK);
			labelOpening1.show(this);
			labelOpening2.show(this);
			formOpening.show(this);
		}
		else if(!is_exist_level_file_name(sMainFolder + sLevelFileName + ".lvl"))
			les = LE_OPENING_ERROR;
		else
		{
			open_level_file(sMainFolder + sLevelFileName + ".lvl");
			formOpening.clear_field();
			les = LE_EDITOR;
		}
	}

	void show_level_opening_error_window()
	{
		std::string sOption = menuOpeningError.check_buttons(this);

		if(sOption == "Create and open level")
		{
			rewrite_level_file(sMainFolder + sLevelFileName + ".lvl");
			open_level_file(sMainFolder + sLevelFileName + ".lvl");
			formOpening.clear_field();
			les = LE_EDITOR;
		}
		else if(sOption == "Back")
			les = LE_OPENING;
		else
		{
			Clear(olc::BLACK);
			labelOpeningError.show(this);
			menuOpeningError.show(this);
		}
	}

	void rewrite_level_file(std::string sLevelFileName)
	{
		char cLevelFileName[sLevelFileName.size() + 1];
		strcpy(cLevelFileName, sLevelFileName.c_str());
		FILE *fpLevelFile = fopen(cLevelFileName, "wb");
		fwrite(&viEditorMapSize.x, sizeof(int), 1, fpLevelFile);
		fwrite(&viEditorMapSize.y, sizeof(int), 1, fpLevelFile);
		for (int i = 0; i < viEditorMapSize.y; i++) 
		{ 
			for (int j = 0; j < viEditorMapSize.x; j++)
				fwrite(&iEditorMap[viEditorMapSize.x * i + j],
							 sizeof(int), 1, fpLevelFile);
		}
		fwrite(&viEditorStartMapPlayerPosition.x, sizeof(int), 1, fpLevelFile);
		fwrite(&viEditorStartMapPlayerPosition.y, sizeof(int), 1, fpLevelFile);
		fclose(fpLevelFile);
	}

	void open_level_file(std::string sLevelFileName)
	{
		char cLevelFileName[sLevelFileName.size() + 1];
		strcpy(cLevelFileName, sLevelFileName.c_str());
		FILE *fpLevelFile = fopen(cLevelFileName, "rb");
		fread(&viEditorMapSize.x, sizeof(int), 1, fpLevelFile);
		fread(&viEditorMapSize.y, sizeof(int), 1, fpLevelFile);
		fread(iEditorMap,
					sizeof(int),
					viEditorMapSize.x * viEditorMapSize.y,
					fpLevelFile);
		fread(&viEditorStartMapPlayerPosition.x, sizeof(int), 1, fpLevelFile);
		fread(&viEditorStartMapPlayerPosition.y, sizeof(int), 1, fpLevelFile);
		fclose(fpLevelFile);
	}

	void show_level_editor_window()
	{
		check_level_editor_buttons();
		Clear(olc::BLACK);
		
		// Drawing editor map
		DrawRect((viEditorMapWindowPosition) * viWindowCellSize - olc::vi2d(2, 2),
							viEditorMapSize * viWindowCellSize + olc::vi2d(3, 3));
		for (int i = 0; i < viEditorMapSize.y; i++)
		{
			for (int j = 0; j < viEditorMapSize.x; j++)
			{
				DrawPartialSprite((viEditorMapWindowPosition + olc::vi2d(j, i))
														* viWindowCellSize,
													sprTileMap.get(),
													olc::vi2d(iEditorMap[viEditorMapSize.x * i + j] % 20,
																		iEditorMap[viEditorMapSize.x * i + j] / 20)
														* viWindowCellSize,
													viWindowCellSize);
			}
		}

		// Drawing editor start map player position
		DrawRect((viEditorMapWindowPosition + viEditorStartMapPlayerPosition)
								* viWindowCellSize - olc::vi2d(1, 1),
							viWindowCellSize + olc::vi2d(1, 1), olc::BLUE);

		// Drawing editor map mouse cursor
		if(viMouseWindowPosition.x >= viEditorMapWindowPosition.x
		&& viMouseWindowPosition.x < viEditorMapWindowPosition.x
																	+ viEditorMapSize.x
		&& viMouseWindowPosition.y >= viEditorMapWindowPosition.y
		&& viMouseWindowPosition.y < viEditorMapWindowPosition.y
																	+ viEditorMapSize.y)
		{
			DrawRect(viMouseWindowPosition * viWindowCellSize - olc::vi2d(1, 1),
							 viWindowCellSize + olc::vi2d(1, 1), olc::RED);
		}

		// Drawing selected block
		DrawPartialSprite(viSelectedBlockWindowPosition * viWindowCellSize,
											sprTileMap.get(),
											olc::vi2d(iSelectedBlockCode % 20,
																iSelectedBlockCode / 20)
												* viWindowCellSize,
											viWindowCellSize, 4);
		DrawRect(viSelectedBlockWindowPosition * viWindowCellSize - olc::vi2d(2, 2),
						 olc::vi2d(4, 4) * viWindowCellSize + olc::vi2d(3, 3));
		
		// Drawing editor block map
		for (int i = 0; i < 10; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				DrawPartialSprite((viEditorBlockMapWindowPosition + olc::vi2d(j, i))
														* viWindowCellSize,
													sprTileMap.get(),
													olc::vi2d(
															(i * 8 + j + iEditorBlockMapPageStatus * 80) % 20,
															(i * 8 + j + iEditorBlockMapPageStatus * 80) / 20)
														* viWindowCellSize,
													viWindowCellSize);
			}
		}
		DrawRect(
						viEditorBlockMapWindowPosition * viWindowCellSize - olc::vi2d(2, 2),
						viEditorBlockMapSize * viWindowCellSize + olc::vi2d(3, 3));
		
		// Drawing editor block map selected block cursor
		if(iEditorBlockMapPageStatus * 80 <= iSelectedBlockCode
		&& (iEditorBlockMapPageStatus + 1) * 80 > iSelectedBlockCode)
		{
			DrawRect((viEditorBlockMapWindowPosition
									+ olc::vi2d(iSelectedBlockCode % 80 % 8,
															iSelectedBlockCode % 80 / 8))
									* viWindowCellSize-olc::vi2d(1, 1),
								viWindowCellSize+olc::vi2d(1, 1), olc::GREEN);
		}

		buttonBack.show(this);
		buttonNext.show(this);
		buttonExit.show(this);
		buttonSave.show(this);
		buttonClear.show(this);

		if(bIsLevelFileSaved)
		{
			labelSaveStatus.set_name("Level saved");
			labelSaveStatus.set_font_color(olc::GREEN);
		}
		else
		{
			labelSaveStatus.set_name("Level not saved yet");
			labelSaveStatus.set_font_color(olc::RED);
		}
		labelSaveStatus.show(this);
	}

	void show_level_editor_menu()
	{
		std::string sOption = menuEditor.check_buttons(this);

		if(sOption == "Create a new level")
			les = LE_CREATION;
		else if(sOption == "Open an existing level")
			les = LE_OPENING;
		else if(sOption == "Back")
			nps = NP_MAIN_MENU;
		else
		{
			Clear(olc::BLACK);
			menuEditor.show(this);
		}
	}

	void show_level_editor()
	{
		switch(les)
		{
			case LE_MENU:
				show_level_editor_menu();
				break;
			case LE_CREATION:
				show_level_creation_window();
				break;
			case LE_CREATION_ERROR:
				show_level_creation_error_window();
				break;
			case LE_OPENING:
				show_level_opening_window();
				break;
			case LE_OPENING_ERROR:
				show_level_opening_error_window();
				break;
			case LE_EDITOR:
				show_level_editor_window();
				break;
		}
	}

// Constructor of class NewPlatformer
public:
	NewPlatformer()
	{
		sAppName = "Test";
	}

// Methods of class NewPlatformer
public:

	bool OnUserCreate() override
	{
		// GAME
		sprTileMap = std::make_unique<olc::Sprite>(sMainFolder
																		+ "textures/monochrome_tilemap_packed.png");
		sprTileTransparentMap = std::make_unique<olc::Sprite>(sMainFolder
												+ "textures/monochrome_tilemap_transparent_packed.png");

		// OPENING
		labelGameOpening1.set_name("Game level opening");
		labelGameOpening1.set_window_position(olc::vi2d(1, 1));

		labelGameOpening2.set_name("Enter existing game level name: ");
		labelGameOpening2.set_window_position(olc::vi2d(1, 9));
		
		labelGameOpening3.set_window_position(olc::vi2d(1, 17));
		
		formGameOpening.set_window_position(olc::vi2d(257, 9));

		// MAIN MENU
		menuMain.add_option("Game");
		menuMain.add_option("Level editor");
		menuMain.add_option("Exit");

		// LEVEL EDITOR
		menuEditor.add_option("Create a new level");
		menuEditor.add_option("Open an existing level");
		menuEditor.add_option("Back");

		// LEVEL CREATION
		labelCreation1.set_name("Level creation");
		labelCreation1.set_window_position(olc::vi2d(1, 1));

		labelCreation2.set_name("Enter new level name: ");
		labelCreation2.set_window_position(olc::vi2d(1, 9));

		formCreation.set_window_position(olc::vi2d(177, 9));

		// LEVEL CREATION ERROR
		labelCreationError.set_name("Level already exists. What do you want to do?");
		labelCreationError.set_window_position(olc::vi2d(1, 1));

		menuCreationError.set_window_position(olc::vi2d(1, 9));
		menuCreationError.add_option("Rewrite and open level");
		menuCreationError.add_option("Open level");
		menuCreationError.add_option("Back");

		// LEVEL OPENING
		labelOpening1.set_name("Level opening");
		labelOpening1.set_window_position(olc::vi2d(1, 1));

		labelOpening2.set_name("Enter existing level name: ");
		labelOpening2.set_window_position(olc::vi2d(1, 9));

		formOpening.set_window_position(olc::vi2d(217, 9));

		// LEVEL OPENING ERROR
		labelOpeningError.set_name("Level is not found. What do you want to do?");
		labelOpeningError.set_window_position(olc::vi2d(1, 1));

		menuOpeningError.set_window_position(olc::vi2d(1, 9));
		menuOpeningError.add_option("Create and open level");
		menuOpeningError.add_option("Back");

		// LEVEL EDITOR
		labelSaveStatus.set_window_position(olc::vi2d(336, 32));

		buttonExit.set_name("EXIT");
		buttonExit.set_window_position(olc::vi2d(16, 16));
		buttonExit.set_font_size(4);

		buttonSave.set_name("SAVE");
		buttonSave.set_window_position(olc::vi2d(176, 16));
		buttonSave.set_font_size(4);
		
		buttonClear.set_name("CLEAR");
		buttonClear.set_window_position(olc::vi2d(528, 16));
		buttonClear.set_font_size(4);
		
		buttonBack.set_name("<<");
		buttonBack.set_window_position(olc::vi2d(32, 272));
		buttonBack.set_font_size(2);

		buttonNext.set_name(">>");
		buttonNext.set_window_position(olc::vi2d(96, 272));
		buttonNext.set_font_size(2);

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		switch(nps)
		{
			case NP_MAIN_MENU:
				show_main_menu();
				break;
			case NP_GAME:
				show_game(fElapsedTime);
				break;
			case NP_LEVEL_EDITOR:
				show_level_editor();
				break;
		}
		return true;
	}
};

int main()
{
	NewPlatformer demo;
	if (demo.Construct(736, 416, 1, 2, false, false, false))
		demo.Start();

	return 0;
}