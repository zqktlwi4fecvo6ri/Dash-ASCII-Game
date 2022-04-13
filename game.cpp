
#include "stdafx.h"
#include "game.h"
//#include "detonate.h"
#include <iostream>

Game::Game(void)
{
	//MAX_STONE = 0;
	color = 0;
	curr_diamond = 0;
}

void Game::debug(void)
{
	level.load();
	level.drawMap();
/*	//std::cout << level.map[0][y][1];
	for (int y=0; y<MAX_Y; y++)
		for (int x=0; x<MAX_X; x++)
		{
			if (level.map[x][y][1] >= 0)
			std::cout << level.map[x][y][1];
		}*/
	//explosion(76,21);

/*	for (int i=0; i<stoneVec.size(); i++)
	{
		
		stoneVec[i]->draw();
		stoneVec[i]->debug++;
	}*/
	level.debug++;
}

void Game::explosion(int _x, int _y)
{
	expl_ptr = new Detonate;
	expl_ptr->init(_x, _y);
	explVec.push_back(expl_ptr);
}

void Game::init(Player* _p_ptr)
{
	player_ptr = _p_ptr;

	color = 0;
	curr_diamond = 0;

	level.load();
	level.drawMap();

	//int i_stone=0;
	for (int y=0; y<MAX_Y; y++)
		for (int x=0; x<MAX_X; x++)
		{
			if (level.map[x][y][0] == STONE)
			{
				stone_ptr = new Stone;
				stone_ptr->setX(x);
				stone_ptr->setY(y);
				stoneVec.push_back(stone_ptr);
				level.map[x][y][1] = stoneVec.size()-1;
			}
			else if (level.map[x][y][0] == ENEMY)
			{
				enemy_ptr = new Enemy;
				enemy_ptr->setX(x);
				enemy_ptr->setY(y);
				enemyVec.push_back(enemy_ptr);
				level.map[x][y][1] = enemyVec.size()-1;
			}
			else if (level.map[x][y][0] == DIAMOND)
			{
				dia_ptr = new Diamond;
				dia_ptr->setX(x);
				dia_ptr->setY(y);
				diaVec.push_back(dia_ptr);
				level.map[x][y][1] = diaVec.size()-1;
			}
		}

	//MAX_STONE = i;
	//std::cout << i;
	_p_ptr->init(&level);

	setColor(black, 7);
	centerText(1," Lives", diaVec.size()-curr_diamond, " Diamonds", 49, " Time", 109, " Points", 1);
}

// Steuerung Enemy
void Game::ai(void)
{
	for (int i=0; i<enemyVec.size(); i++)
	{
		// ist Stein noch nicht kaputt?
		if (!enemyVec[i]->getState())
		{   // falls links, �ber, rechts oder unter spieler ist, enemy explodiert
			if (level.map[enemyVec[i]->getX()-1][enemyVec[i]->getY()][0] == PLAYER ||
				level.map[enemyVec[i]->getX()][enemyVec[i]->getY()-1][0] == PLAYER ||
				level.map[enemyVec[i]->getX()+1][enemyVec[i]->getY()][0] == PLAYER ||
				level.map[enemyVec[i]->getX()][enemyVec[i]->getY()+1][0] == PLAYER)
			{
				PlaySound( TEXT("explosion-02.wav"), NULL, SND_ASYNC);
				enemyVec[i]->setState(true);
				explosion(enemyVec[i]->getX(),enemyVec[i]->getY());
			}
			// falls an Enemy stelle brennt, enemy explodiert u. stirbt
			else if (level.map[enemyVec[i]->getX()][enemyVec[i]->getY()][0] == FIRE)
			{
				PlaySound( TEXT("explosion-02.wav"), NULL, SND_ASYNC);
				enemyVec[i]->setState(true);
				explosion(enemyVec[i]->getX(),enemyVec[i]->getY());
			}
			// wenn �ber Enemy sich ein stein befindet, Enemy explodiert u. stirbt
			else if (level.map[enemyVec[i]->getX()][enemyVec[i]->getY()-1][0] == STONE)
			{
				PlaySound( TEXT("explosion-02.wav"), NULL, SND_ASYNC);
				enemyVec[i]->setState(true);
				explosion(enemyVec[i]->getX(),enemyVec[i]->getY());
			}
			else
			{	
				if (enemyVec[i]->getDelay() == 0)
				{
					if (enemyVec[i]->getWay() == RECHTS)
					{					
						if (level.map[enemyVec[i]->getX()][enemyVec[i]->getY()-1][0] != EMPTY && level.map[enemyVec[i]->getX()+1][enemyVec[i]->getY()][0] == EMPTY)
						{
							enemyVec[i]->setWay(RECHTS);
							enemyVec[i]->move(&level, RECHTS);
						}
						else if (level.map[enemyVec[i]->getX()-1][enemyVec[i]->getY()-1][0] != EMPTY && level.map[enemyVec[i]->getX()][enemyVec[i]->getY()-1][0] == EMPTY)
						{
							enemyVec[i]->setWay(RAUF);
							enemyVec[i]->move(&level, RAUF);
						}
						else if (level.map[enemyVec[i]->getX()][enemyVec[i]->getY()-1][0] != EMPTY && level.map[enemyVec[i]->getX()+1][enemyVec[i]->getY()][0] != EMPTY
							&& level.map[enemyVec[i]->getX()][enemyVec[i]->getY()+1][0] == EMPTY)
						{
							enemyVec[i]->setWay(RUNTER);
							enemyVec[i]->move(&level, RUNTER);
						}
						else if (level.map[enemyVec[i]->getX()][enemyVec[i]->getY()-1][0] != EMPTY && level.map[enemyVec[i]->getX()+1][enemyVec[i]->getY()][0] != EMPTY
							&& level.map[enemyVec[i]->getX()][enemyVec[i]->getY()+1][0] != EMPTY && level.map[enemyVec[i]->getX()-1][enemyVec[i]->getY()][0] == EMPTY)
						{
							enemyVec[i]->setWay(LINKS);
							enemyVec[i]->move(&level, LINKS);
						}
						else enemyVec[i]->setWay(RUNTER);
					}
					else if (enemyVec[i]->getWay() == RUNTER)
					{					
						if (level.map[enemyVec[i]->getX()+1][enemyVec[i]->getY()][0] != EMPTY && level.map[enemyVec[i]->getX()][enemyVec[i]->getY()+1][0] == EMPTY)
						{
							enemyVec[i]->setWay(RUNTER);
							enemyVec[i]->move(&level, RUNTER);
						}
						else if (level.map[enemyVec[i]->getX()+1][enemyVec[i]->getY()-1][0] != EMPTY && level.map[enemyVec[i]->getX()+1][enemyVec[i]->getY()][0] == EMPTY)
						{
							enemyVec[i]->setWay(RECHTS);
							enemyVec[i]->move(&level, RECHTS);
						}
						else if (level.map[enemyVec[i]->getX()+1][enemyVec[i]->getY()][0] != EMPTY && level.map[enemyVec[i]->getX()][enemyVec[i]->getY()+1][0] != EMPTY
							&& level.map[enemyVec[i]->getX()-1][enemyVec[i]->getY()][0] == EMPTY)
						{
							enemyVec[i]->setWay(LINKS);
							enemyVec[i]->move(&level, LINKS);
						}
						else if (level.map[enemyVec[i]->getX()+1][enemyVec[i]->getY()][0] != EMPTY && level.map[enemyVec[i]->getX()][enemyVec[i]->getY()+1][0] != EMPTY
							&& level.map[enemyVec[i]->getX()-1][enemyVec[i]->getY()][0] != EMPTY && level.map[enemyVec[i]->getX()][enemyVec[i]->getY()-1][0] == EMPTY)
						{
							enemyVec[i]->setWay(RAUF);
							enemyVec[i]->move(&level, RAUF);
						}
						else enemyVec[i]->setWay(LINKS);
					}
					else if (enemyVec[i]->getWay() == LINKS)
					{					
						if (level.map[enemyVec[i]->getX()][enemyVec[i]->getY()+1][0] != EMPTY && level.map[enemyVec[i]->getX()-1][enemyVec[i]->getY()][0] == EMPTY)
						{
							enemyVec[i]->setWay(LINKS);
							enemyVec[i]->move(&level, LINKS);
						}
						else if (level.map[enemyVec[i]->getX()+1][enemyVec[i]->getY()+1][0] != EMPTY && level.map[enemyVec[i]->getX()][enemyVec[i]->getY()+1][0] == EMPTY)
						{
							enemyVec[i]->setWay(RUNTER);
							enemyVec[i]->move(&level, RUNTER);
						}
						else if (level.map[enemyVec[i]->getX()][enemyVec[i]->getY()+1][0] != EMPTY && level.map[enemyVec[i]->getX()-1][enemyVec[i]->getY()][0] != EMPTY
							&& level.map[enemyVec[i]->getX()][enemyVec[i]->getY()-1][0] == EMPTY)
						{
							enemyVec[i]->setWay(RAUF);
							enemyVec[i]->move(&level, RAUF);
						}
						else if (level.map[enemyVec[i]->getX()][enemyVec[i]->getY()+1][0] != EMPTY && level.map[enemyVec[i]->getX()-1][enemyVec[i]->getY()][0] != EMPTY
							&& level.map[enemyVec[i]->getX()][enemyVec[i]->getY()-1][0] != EMPTY && level.map[enemyVec[i]->getX()+1][enemyVec[i]->getY()][0] == EMPTY)
						{
							enemyVec[i]->setWay(RECHTS);
							enemyVec[i]->move(&level, RECHTS);
						}
						else enemyVec[i]->setWay(RAUF);
					}
					else if (enemyVec[i]->getWay() == RAUF)
					{					
						if (level.map[enemyVec[i]->getX()-1][enemyVec[i]->getY()][0] != EMPTY && level.map[enemyVec[i]->getX()][enemyVec[i]->getY()-1][0] == EMPTY)
						{
							enemyVec[i]->setWay(RAUF);
							enemyVec[i]->move(&level, RAUF);
						}
						else if (level.map[enemyVec[i]->getX()-1][enemyVec[i]->getY()+1][0] != EMPTY && level.map[enemyVec[i]->getX()-1][enemyVec[i]->getY()][0] == EMPTY)
						{
							enemyVec[i]->setWay(LINKS);
							enemyVec[i]->move(&level, LINKS);
						}
						else if (level.map[enemyVec[i]->getX()-1][enemyVec[i]->getY()][0] != EMPTY && level.map[enemyVec[i]->getX()][enemyVec[i]->getY()-1][0] != EMPTY
							&& level.map[enemyVec[i]->getX()+1][enemyVec[i]->getY()][0] == EMPTY)
						{
							enemyVec[i]->setWay(RECHTS);
							enemyVec[i]->move(&level, RECHTS);
						}
						else if (level.map[enemyVec[i]->getX()-1][enemyVec[i]->getY()][0] != EMPTY && level.map[enemyVec[i]->getX()][enemyVec[i]->getY()-1][0] != EMPTY
							&& level.map[enemyVec[i]->getX()+1][enemyVec[i]->getY()][0] != EMPTY && level.map[enemyVec[i]->getX()][enemyVec[i]->getY()+1][0] == EMPTY)
						{
							enemyVec[i]->setWay(RUNTER);
							enemyVec[i]->move(&level, RUNTER);
						}
						else enemyVec[i]->setWay(RECHTS);
					}
				}
				enemyVec[i]->slowdown();
				enemyVec[i]->draw();
			}
		}
	}
}

// Steuerung Steine
void Game::gravity(void)
{
	// Steine
	for (int i=0; i<stoneVec.size(); i++)
	{
		// ist Stein noch nicht kaputt?
		if (!stoneVec[i]->getState())
		{
			if (level.map[stoneVec[i]->getX()][stoneVec[i]->getY()][0] == FIRE)
			{
				stoneVec[i]->setState(true);
			}
			else
			{	// stein f�llt
				if (stoneVec[i]->getDelay() == 0)
				{
					if (level.map[stoneVec[i]->getX()][stoneVec[i]->getY()+1][0] == EMPTY)
					{
						stoneVec[i]->move(&level, RUNTER);
						if (level.map[stoneVec[i]->getX()][stoneVec[i]->getY()+1][0] == PLAYER)
						{
							// Stein kolliediert mit Spieler
							PlaySound( TEXT("explosion-02.wav"), NULL, SND_ASYNC);
							explosion(player_ptr->getX(), player_ptr->getY());
						}
						else if (level.map[stoneVec[i]->getX()][stoneVec[i]->getY()+1][0] == ENEMY)
						{
							//std::cout << "Stein kollidiert mit Enemy!";
							PlaySound( TEXT("explosion-02.wav"), NULL, SND_ASYNC);
							explosion(enemyVec[level.map[stoneVec[i]->getX()][stoneVec[i]->getY()+1][1]]->getX(), enemyVec[level.map[stoneVec[i]->getX()][stoneVec[i]->getY()+1][1]]->getY());
						}
					}
					else if (level.map[stoneVec[i]->getX()-1][stoneVec[i]->getY()][0] == EMPTY && level.map[stoneVec[i]->getX()-1][stoneVec[i]->getY()+1][0] == EMPTY
						&& (level.map[stoneVec[i]->getX()][stoneVec[i]->getY()+1][0] == STONE || level.map[stoneVec[i]->getX()][stoneVec[i]->getY()+1][0] == WALL || level.map[stoneVec[i]->getX()][stoneVec[i]->getY()+1][0] == DIAMOND))
					{
						stoneVec[i]->move(&level, LINKS);
					}
					else if (level.map[stoneVec[i]->getX()+1][stoneVec[i]->getY()][0] == EMPTY && level.map[stoneVec[i]->getX()+1][stoneVec[i]->getY()+1][0] == EMPTY
						&& (level.map[stoneVec[i]->getX()][stoneVec[i]->getY()+1][0] == STONE || level.map[stoneVec[i]->getX()][stoneVec[i]->getY()+1][0] == WALL || level.map[stoneVec[i]->getX()][stoneVec[i]->getY()+1][0] == DIAMOND))
					{
						stoneVec[i]->move(&level, RECHTS);
					}
				}
				stoneVec[i]->slowdown();
			}
		}
	    ///////////////////////////////////////////////
		// Diamond
		// ist Diamond noch nicht kaputt?
		if (diaVec.size()>i)
		{
			if (!diaVec[i]->getState())
			{
				if (level.map[diaVec[i]->getX()][diaVec[i]->getY()][0] == FIRE)
				{
					diaVec[i]->setState(true);
				}
				else if (level.map[diaVec[i]->getX()][diaVec[i]->getY()][0] == PLAYER)
				{
					diaVec[i]->setState(true);
					curr_diamond++;
					setColor(black, 7);
					centerText(1," Lives", diaVec.size()-curr_diamond, " Diamonds", 49, " Time", 109, " Points", 1);
				}
				else
				{	// Diamond f�llt
					if (diaVec[i]->getDelay() == 0)
					{
						if (level.map[diaVec[i]->getX()][diaVec[i]->getY()+1][0] == EMPTY)
						{
							diaVec[i]->move(&level, RUNTER);
							if (level.map[diaVec[i]->getX()][diaVec[i]->getY()+1][0] == PLAYER)
							{
								PlaySound( TEXT("explosion-02.wav"), NULL, SND_ASYNC);
								explosion(player_ptr->getX(), player_ptr->getY());
							}
							else if (level.map[diaVec[i]->getX()][diaVec[i]->getY()+1][0] == ENEMY)
							{
								//std::cout << "Stein kollidiert mit Enemy!";
								PlaySound( TEXT("explosion-02.wav"), NULL, SND_ASYNC);
								explosion(enemyVec[level.map[diaVec[i]->getX()][diaVec[i]->getY()+1][1]]->getX(), enemyVec[level.map[diaVec[i]->getX()][diaVec[i]->getY()+1][1]]->getY());
							}
						}
						else if (level.map[diaVec[i]->getX()-1][diaVec[i]->getY()][0] == EMPTY && level.map[diaVec[i]->getX()-1][diaVec[i]->getY()+1][0] == EMPTY
							&& (level.map[diaVec[i]->getX()][diaVec[i]->getY()+1][0] == STONE || level.map[diaVec[i]->getX()][diaVec[i]->getY()+1][0] == WALL || level.map[diaVec[i]->getX()][diaVec[i]->getY()+1][0] == DIAMOND))
						{
							diaVec[i]->move(&level, LINKS);
						}
						else if (level.map[diaVec[i]->getX()+1][diaVec[i]->getY()][0] == EMPTY && level.map[diaVec[i]->getX()+1][diaVec[i]->getY()+1][0] == EMPTY
							&& (level.map[diaVec[i]->getX()][diaVec[i]->getY()+1][0] == STONE || level.map[diaVec[i]->getX()][diaVec[i]->getY()+1][0] == WALL || level.map[diaVec[i]->getX()][diaVec[i]->getY()+1][0] == DIAMOND))
						{
							diaVec[i]->move(&level, RECHTS);
						}
					}
					diaVec[i]->slowdown();
				}
			}
		}
	}
}

void Game::update(void)
{
	// falls eine explosion gez�ndet ist
	if (explVec.size() > 0)
	{
		for (int i=0; i<explVec.size(); i++)
		{
			explVec[i]->firing(&level);
			// explosion aus Vector l�schen nach ende
			if (explVec[i]->getIndex()>4)
				explVec.erase(explVec.begin() + i);
		}
	}
	// wenn spieler brennt/explodiert, spieler stirbt
	if (level.map[player_ptr->getX()][player_ptr->getY()][0] == FIRE)
	{
		player_ptr->setState(true);
        setColor(black, 7);
        centerText("                   GAME OVER                    ", 1);
	}
	// spieler gewinnt
	else if (diaVec.size() - curr_diamond == 0)
	{
		player_ptr->setState(true);
        setColor(black, 7);
        centerText("                    YOU WIN!                    ", 1);
	}

	gravity();
	ai();

}

// Steuerung Spieler
void Game::move(int _v)
{
	// ist spieler tot?
	if (!player_ptr->getState())
	{
		if (_v == RECHTS)
		{
			// Nur wenn rechts Sand, Diamond oder Leer ist
			if (level.map[player_ptr->getX()+1][player_ptr->getY()][0] == SAND || level.map[player_ptr->getX()+1][player_ptr->getY()][0] == EMPTY || level.map[player_ptr->getX()+1][player_ptr->getY()][0] == DIAMOND)
			{
				level.map[player_ptr->getX()][player_ptr->getY()][0] = EMPTY;
				level.draw(player_ptr->getX(), player_ptr->getY(), EMPTY);
				player_ptr->move(RECHTS);
				level.map[player_ptr->getX()][player_ptr->getY()][0] = PLAYER; 
			}
			// Spieler schiebt stein
			else if (level.map[player_ptr->getX()+1][player_ptr->getY()][0] == STONE && level.map[player_ptr->getX()+2][player_ptr->getY()][0] == EMPTY)
			{
				// Wenn schieben geklappt hat, soll neue position auch im map-index eingef�gt werden
				stoneVec[level.map[player_ptr->getX()+1][player_ptr->getY()][1]]->move(&level, RECHTS);
			}
		
		
		}
		else if (_v == LINKS)
		{
			// Nur wenn links Sand oder Leer ist
			if (level.map[player_ptr->getX()-1][player_ptr->getY()][0] == SAND || level.map[player_ptr->getX()-1][player_ptr->getY()][0] == EMPTY || level.map[player_ptr->getX()-1][player_ptr->getY()][0] == DIAMOND)
			{
				level.map[player_ptr->getX()][player_ptr->getY()][0] = EMPTY;
				level.draw(player_ptr->getX(), player_ptr->getY(), EMPTY);
				// Wenn rechts ein stein liegt, stein muss neu gezeichnet werden
				if (level.map[player_ptr->getX()+1][player_ptr->getY()][0] == STONE)
					level.draw(player_ptr->getX()+1, player_ptr->getY(), STONE);
				else if (level.map[player_ptr->getX()+1][player_ptr->getY()][0] == DIAMOND)
					level.draw(player_ptr->getX()+1, player_ptr->getY(), DIAMOND);

				player_ptr->move(LINKS);
				level.map[player_ptr->getX()][player_ptr->getY()][0] = PLAYER;
				//draw();
			}
			// Spieler schiebt stein links
			else if (level.map[player_ptr->getX()-1][player_ptr->getY()][0] == STONE && level.map[player_ptr->getX()-2][player_ptr->getY()][0] == EMPTY)
			{
				// Wenn schieben geklappt hat, soll neue position auch im map-index eingef�gt werden
				stoneVec[level.map[player_ptr->getX()-1][player_ptr->getY()][1]]->move(&level, LINKS);
			}
		}
		else if (_v == RUNTER)
		{
			if (level.map[player_ptr->getX()][player_ptr->getY()+1][0] == SAND || level.map[player_ptr->getX()][player_ptr->getY()+1][0] == EMPTY || level.map[player_ptr->getX()][player_ptr->getY()+1][0] == DIAMOND)
			{
				level.map[player_ptr->getX()][player_ptr->getY()][0] = EMPTY;
				level.draw(player_ptr->getX(), player_ptr->getY(), EMPTY);
				// Wenn rechts ein stein liegt, stein muss neu gezeichnet werden
				if (level.map[player_ptr->getX()+1][player_ptr->getY()][0] == STONE)
					level.draw(player_ptr->getX()+1, player_ptr->getY(), STONE);
				else if (level.map[player_ptr->getX()+1][player_ptr->getY()][0] == DIAMOND)
					level.draw(player_ptr->getX()+1, player_ptr->getY(), DIAMOND);

				player_ptr->move(RUNTER);
				level.map[player_ptr->getX()][player_ptr->getY()][0] = PLAYER;
				//draw();
			}
		}
		else if (_v == RAUF) 
		{
			if (level.map[player_ptr->getX()][player_ptr->getY()-1][0] == SAND || level.map[player_ptr->getX()][player_ptr->getY()-1][0] == EMPTY || level.map[player_ptr->getX()][player_ptr->getY()-1][0] == DIAMOND)
			{
				level.map[player_ptr->getX()][player_ptr->getY()][0] = EMPTY;
				level.draw(player_ptr->getX(), player_ptr->getY(), EMPTY);
				// Wenn rechts ein stein liegt, stein muss neu gezeichnet werden
				if (level.map[player_ptr->getX()+1][player_ptr->getY()][0] == STONE)
					level.draw(player_ptr->getX()+1, player_ptr->getY(), STONE);
				else if (level.map[player_ptr->getX()+1][player_ptr->getY()][0] == DIAMOND)
					level.draw(player_ptr->getX()+1, player_ptr->getY(), DIAMOND);
				player_ptr->move(RAUF);
				level.map[player_ptr->getX()][player_ptr->getY()][0] = PLAYER;
				//draw();
			}
		}
	}
}

void Game::setColor(int colorBack, int colorFore)
{
    int back = 0;
    if (colorBack & 1) back |= BACKGROUND_BLUE;
    if (colorBack & 2) back |= BACKGROUND_GREEN;
    if (colorBack & 4) back |= BACKGROUND_RED;
    if (colorBack & 8) back |= BACKGROUND_INTENSITY;

    int fore = 0;
    if (colorFore & 1) fore |= FOREGROUND_BLUE;
    if (colorFore & 2) fore |= FOREGROUND_GREEN;
    if (colorFore & 4) fore |= FOREGROUND_RED;
    if (colorFore & 8) fore |= FOREGROUND_INTENSITY;

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), back | fore);
}

void Game::cursorHide()
{
	HANDLE hwnd;
	hwnd = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO info;

	info.bVisible = 0;
	info.dwSize = 1;
	
	SetConsoleCursorInfo(hwnd, &info);
}

void Game::centerText(const char *message, int ypos)
{
    int len = strlen(message);
    if (len > 0)
    {
        int xpos = (CONSOLE_WIDTH - len) / 2;
        gotoxy(xpos, ypos);
        printf(message);
    }
}

void Game::centerText(int _l, const char *text1, int _d, const char *text2, int _t, const char *text3, int _p, const char *text4, int ypos)
{
	gotoxy(15, ypos);
	std::cout << " " << _l << text1 << " |  ";

	if (_d < 10)
		gotoxy(27, ypos);
	else gotoxy(26, ypos);
    //printf("%i,%c",_l, text1);
	std::cout << _d << text2 << " |  ";

	gotoxy(39, ypos);
	std::cout << " " << _t << text3 << " |  ";

	gotoxy(49, ypos);
	std::cout << " " << _p << text4 << " ";

	cursorHide(); 
}

void Game::gotoxy(int _x, int _y)
{
    COORD point;
    point.X = _x;
    point.Y = _y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), point);
}

Game::~Game()
{
	delete [] expl_ptr;
	delete [] stone_ptr;
	delete [] enemy_ptr;
	delete [] expl_ptr;
}


void Game::restart(void)
{
	//PlaySound(TEXT("test.wav"), NULL, SND_FILENAME);
	PlaySound( TEXT("walk.wav"), NULL, SND_ASYNC);
}