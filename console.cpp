﻿// Game.cpp : Definiert den Einstiegspunkt für die Konsolenanwendung.
//

#include "stdafx.h"
#include "player.h"
//#include "enemy.h"
#include "level.h"
#include "stone.h"
#include "game.h"





DWORD LoopStartTime;



// Cursor Position setzen
void gotoxy(int x, int y)
{
    COORD point;
    point.X = x;
    point.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), point);
}

// Bild löschen, nicht genutzt
void clrscr()
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD bufferSize, charsWritten;

    COORD topleft = {0, 0};
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    GetConsoleScreenBufferInfo(hConsole, &csbi);
    bufferSize = csbi.dwSize.X * csbi.dwSize.Y;
    FillConsoleOutputCharacter(hConsole, TEXT(' '), bufferSize, topleft, &charsWritten);
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    FillConsoleOutputAttribute(hConsole, csbi.wAttributes, bufferSize, topleft, &charsWritten);
    SetConsoleCursorPosition(hConsole, topleft);
}



// Farbe für Schrift
void setColor(int colorBack, int colorFore)
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


// Text ausdrucken
void centerText(const char *message, int ypos)
{
    int len = strlen(message);
    if (len > 0)
    {
        int xpos = (CONSOLE_WIDTH - len) / 2;
        gotoxy(xpos, ypos);
        printf(message);
    }
}

void cursorHide(HANDLE *h)
{
	CONSOLE_CURSOR_INFO info;

	info.bVisible = 0;
	info.dwSize = 1;
	
	SetConsoleCursorInfo(h, &info);
}

int main()
{
	HANDLE hwnd;
	hwnd = GetStdHandle(STD_OUTPUT_HANDLE);
	cursorHide(&hwnd);

    bool exit = false;

	Player player(SPEED_PLAYER);


	//level.drawMap();

/*	Enemy enemy[2];
	enemy[0].init(&level, 16, 9, SPEED_ENEMY);
	enemy[1].init(&level, 63, 16, SPEED_ENEMY);*/
    setColor(black, 7);
    centerText(" 1 Lives |  0 Diamonds | 109 Time | 00032 Points ", 1);

	Game game;
	game.init(&player);

	player.draw();

    do
    {


		if (_kbhit())
		{
			int key = _getch();
			if (key == KEY_ESCAPE) exit = true;

			if (player.getDelay() == 0)
			{
				
				if (!key || key == 0xe0)
					key = _getch();

					switch (key)
					{
						case KEY_LEFT: game.move(LINKS); break;
						case KEY_RIGHT: game.move(RECHTS); break;
						case KEY_UP: game.move(RAUF); break;
						case KEY_DOWN: game.move(RUNTER); break;
						case KEY_RETURN: game.restart(); break;
					}
			}
			player.slowdown();
		}
		else player.setDelay(0) ;  // waiting wird auf 0 gesetzt, wenn spieler richtung ändert (direkte steuerung)

		// ToDo: Hier Aufruf von Enemies
//		enemy[0].KI();
//		enemy[1].KI();

		// ToDo: Spieler Anzeigen
//		enemy[0].draw();
//		enemy[1].draw();

		//player.draw();

		game.update();

        //clrscr();*/

		// ====================================================== 
		LoopStartTime = GetTickCount();
		while ((GetTickCount() - LoopStartTime) < 40);

    } while (!exit);

}