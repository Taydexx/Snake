
#include "pch.h"
#include <iostream>
#include <conio.h>
#include <Windows.h>
#include <string>
#include <ctime>
#include <fstream>


const char MOVE_MENU_CURSOR_UP = 'w';
const char MOVE_MENU_CURSOR_DOWN = 's';
const char MOVE_MENU_CURSOR_ACCEPT = 'f';
const int QUIT = -1;
const int MAIN_MENU_OPTIONS_AMOUNT = 4;
const int OPTIONS_AMOUNT = 5;

using namespace std;

struct SNAKE
{
	int *x;
	int *y;
};

void fill(char **array, int height, int width);
void show(char **array, int height, int width, int score);
void clear();
void clear(int i);
bool moveSnake(char **map, int *x, int *y, int x_dest, int y_dest, int snakePosition, int snakeSize);
bool makeMove(int ch, char **map, int *x, int *y, int height, int width, int snakeSize, bool gamemode);
void deletePointers(char **map, int width, int *x, int *y);
bool checkCollision(int colX, int colY, int *x, int *y, int snakeSize);
SNAKE createSnake(int snakeSize);
SNAKE extendSnake(SNAKE snake, int *snakeSize);
char **createMap(int height, int width);
bool checkForPoint(int destX, int destY, int x, int y, int *score);
void createApple(int *x, int *y, char **map, int height, int width);
void setTerminalWindow(int width, int height);
int playGame(int width, int height, int delay, bool gamemode);
void showOptions(int menuCursor);
void runOptions();
void createGame();
void mainMenu();
void saveScore(string nick, int score);
void exit();
int useMenuOption(int i);
void showMenu(int menuCursor);
int manageMenu(int menuCursor);
void runMenu();
void showHighscores();
void mainMenu();
int moveCursorDown(int menuCursor, int length);
int moveCursorUp(int menuCursor, int length);

int width = 60;
int height = 40;
int delay = 20;
bool gamemode = 0;

int main()
{
	mainMenu();
	return 0;
}

void fill(char **array, int height, int width)
{
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			array[j][i] = ' ';
		}
	}
}

void show(char **array, int height, int width, int score)
{
	string toPrint = "Score: ";
	toPrint += to_string(score);
	toPrint += "\n";
	for (int i = -3; i < width + 3; i++)
	{
		toPrint += "!";
	}
	toPrint += "\n";
	for (int i = 0; i < height; i++)
	{
		toPrint += "!!!";
		for (int j = 0; j < width; j++)
		{
			toPrint += array[j][i];
		}
		toPrint += "!!!\n";
	}
	for (int i = -3; i < width + 3; i++)
	{
		toPrint += "!";
	}
	toPrint += "\n";
	cout << toPrint;
}

void clear()
{
	COORD cur = { 0, 0 };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cur);
}

void clear(int i)
{
	COORD cur = { 0, i };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cur);
}

bool moveSnake(char **map, int *x, int *y, int x_dest, int y_dest, int snakePosition, int snakeSize)
{
	int x_old = x[snakePosition];
	int y_old = y[snakePosition];
	map[x_old][y_old] = ' ';
	x[snakePosition] = x_dest;
	y[snakePosition] = y_dest;
	map[x[snakePosition]][y[snakePosition]] = '#';

	if (snakePosition != snakeSize)
	{
		moveSnake(map, x, y, x_old, y_old, snakePosition + 1, snakeSize);
	}

	return 1;
}

bool makeMove(int ch, char **map, int *x, int *y, int height, int width, int snakeSize, bool gamemode)
{
	switch (ch)
	{
	case 'a':
		if (x[0] == 0)
		{
			return gamemode ? moveSnake(map, x, y, width - 1, y[0], 0, snakeSize) : 0;
		}
		else moveSnake(map, x, y, x[0] - 1, y[0], 0, snakeSize);
		break;
	case 'd':
		if (x[0] == width - 1)
		{
			return gamemode ? moveSnake(map, x, y, 0, y[0], 0, snakeSize) : 0;
		}
		else moveSnake(map, x, y, x[0] + 1, y[0], 0, snakeSize);
		break;
	case 'w':
		if (y[0] == 0)
		{
			return gamemode ? moveSnake(map, x, y, x[0], height - 1, 0, snakeSize) : 0;
		}
		else moveSnake(map, x, y, x[0], y[0] - 1, 0, snakeSize);
		break;
	case 's':
		if (y[0] == height - 1)
		{
			return gamemode ? moveSnake(map, x, y, x[0], 0, 0, snakeSize) : 0;
		}
		else moveSnake(map, x, y, x[0], y[0] + 1, 0, snakeSize);
		break;
	case 27: return 0;
	}
	clear();
	return 1;
}

void deletePointers(char **map, int width, int *x, int *y)
{
	for (int i = 0; i < width; i++)
	{
		delete[] map[i];
	}
	delete[] map;
	delete[] x;
	delete[] y;
}

bool checkCollision(int colX, int colY, int *x, int *y, int snakeSize)
{
	for (int i = 1; i < snakeSize; i++)
	{
		if (colX == x[i] && colY == y[i])
		{
			return 0;
		}
	}
	return 1;
}

SNAKE createSnake(int snakeSize)
{
	int *x = new int[snakeSize];
	int *y = new int[snakeSize];
	srand(time(NULL));
	SNAKE snake;
	for (int i = 0; i < snakeSize; i++)
	{
		x[i] = 3;
		y[i] = 5;
	}
	snake.x = x;
	snake.y = y;
	return snake;
}

SNAKE extendSnake(SNAKE snake, int *snakeSize)
{
	SNAKE newSnake;
	(*snakeSize)++;
	newSnake.x = new int[*snakeSize];
	newSnake.y = new int[*snakeSize];
	for (int i = 0; i < (*snakeSize) - 1; i++)
	{
		newSnake.x[i] = snake.x[i];
		newSnake.y[i] = snake.y[i];
	}
	newSnake.x[*snakeSize - 1] = snake.x[*snakeSize - 2];
	newSnake.y[*snakeSize - 1] = snake.y[*snakeSize - 2];
	return newSnake;
}

char **createMap(int height, int width)
{
	char **map = new char*[width];
	for (int i = 0; i < width; i++)
	{
		map[i] = new char[height];
	}
	return map;
}

bool checkForPoint(int destX, int destY, int x, int y, int *score)
{
	if (destX == x && destY == y)
	{
		*score += 1000;
		return 1;
	}
	return 0;
}

void createApple(int *x, int *y, char **map, int height, int width)
{
	int celX = rand() % height;
	int celY = rand() % width;
	while (map[celX][celY] != ' ')
	{
		celX = rand() % height;
		celY = rand() % width;
	}
	map[celX][celY] = '0';
	*x = celX;
	*y = celY;
}

void setTerminalWindow(int width, int height)
{
	HWND console = GetConsoleWindow();
	RECT r;
	GetWindowRect(console, &r);
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	MoveWindow(console, r.left, r.top, width * 10 + 4, (height + 4) * 18, TRUE);
}
int playGame(int width, int height, int delay, bool gamemode)
{
	setTerminalWindow(width, height);
	int score = 0;
	//COORD coord;
	//coord.X = width*2;
	//coord.Y = height*2;
	//SetConsoleScreenBufferSize(handle, coord);
	char **map = createMap(height, width);
	fill(map, height, width);
	int snakeSize = 3;
	SNAKE snake = createSnake(snakeSize);
	snake = extendSnake(snake, &snakeSize);
	int *x = snake.x;
	int *y = snake.y;
	bool stan = 1;
	char direction = 'd';
	int appleX;
	int appleY;
	createApple(&appleX, &appleY, map, height, width);
	while (stan)
	{
		clear();
		show(map, height, width, score);
		if (_kbhit())
		{
			char key = _getch();
			switch (direction)
			{
			case 'd': if (key == 'a') key = 'd'; break;
			case 'a': if (key == 'd') key = 'a'; break;
			case 'w': if (key == 's') key = 'w'; break;
			case 's': if (key == 'w') key = 's'; break;
			}
			if (key == 'a' || key == 's' || key == 'd' || key == 'w' || key == 27)
			{
				direction = key;
			}
		}
		stan = makeMove(direction, map, snake.x, snake.y, height, width, snakeSize - 1, gamemode) && checkCollision(snake.x[0], snake.y[0], snake.x, snake.y, snakeSize);
		if (checkForPoint(snake.x[0], snake.y[0], appleX, appleY, &score))
		{
			createApple(&appleX, &appleY, map, height, width); snake = extendSnake(snake, &snakeSize);
		}
		Sleep(delay);
		score++;
	}
	deletePointers(map, width, snake.x, snake.y);
	system("cls");
	return score;
}

void createGame()
{
	clear();
	saveScore("test", playGame(width, height, delay, gamemode));
}

void exit()
{
	system("cls");
}

void changeOption(int menuCursor)
{
	switch (menuCursor)
	{
	case 0:
	case 1:
	case 2:
	case 3:
	case 4: exit();  break;
	}
}

int manageOptions(int menuCursor)
{
	char key = _getch();
	switch (key)
	{
	case MOVE_MENU_CURSOR_DOWN: return moveCursorDown(menuCursor, OPTIONS_AMOUNT);
	case MOVE_MENU_CURSOR_UP: return moveCursorUp(menuCursor, OPTIONS_AMOUNT);
	case MOVE_MENU_CURSOR_ACCEPT: changeOption(menuCursor); return QUIT;
	}
	return menuCursor;
}

void showOptions(int menuCursor)
{
	string choices[OPTIONS_AMOUNT] = { "Width:   " + to_string(width) + "    ", "Height:   " + to_string(height) + "    ", "Speed:   " + to_string(delay) + "    ", "Walls: " + to_string(gamemode) + "    ", "Exit   " };
	string toPrint = "";
	toPrint = "\n";
	for (int i = 0; i < OPTIONS_AMOUNT; i++)
	{
		toPrint += "  ";
		if (i == menuCursor)
		{
			toPrint += " >";
		}
		toPrint += choices[i];
		toPrint += "\n";
	}
	cout << toPrint;
}

void runOptions()
{
	system("cls");
	int menuCursor = 0;
	string toPrint = "";
	do
	{
		showOptions(menuCursor);
		menuCursor = manageOptions(menuCursor);
		clear();
	} while (menuCursor >= 0);
}

int useMenuOption(int i)
{
	switch (i)
	{
	case 0: createGame(); break;
	case 1: showHighscores(); break;
	case 2: runOptions(); break;
	case 3: exit(); return QUIT; break;
	}
	return 0;
}

void showMenu(int menuCursor)
{
	string choices[MAIN_MENU_OPTIONS_AMOUNT] = { "Start Game   ", "Highscores   ", "Options   ", "Exit   " };
	string toPrint = "";
	toPrint = "\n";
	for (int i = 0; i < MAIN_MENU_OPTIONS_AMOUNT; i++)
	{
		toPrint += "  ";
		if (i == menuCursor)
		{
			toPrint += " >";
		}
		toPrint += choices[i];
		toPrint += "\n";
	}
	cout << toPrint;
}

int moveCursorDown(int menuCursor, int length)
{
	menuCursor == (length - 1) ? menuCursor = 0 : menuCursor++;
	return menuCursor;
}

int moveCursorUp(int menuCursor, int length)
{
	menuCursor == 0 ? menuCursor = (length - 1) : menuCursor--;
	return menuCursor;
}

int manageMenu(int menuCursor)
{

	char key = _getch();
	switch (key)
	{
	case MOVE_MENU_CURSOR_DOWN: return moveCursorDown(menuCursor, MAIN_MENU_OPTIONS_AMOUNT);
	case MOVE_MENU_CURSOR_UP: return moveCursorUp(menuCursor, MAIN_MENU_OPTIONS_AMOUNT);
	case MOVE_MENU_CURSOR_ACCEPT: return useMenuOption(menuCursor);;
	}
	return menuCursor;
}

void runMenu()
{
	system("cls");
	int menuCursor = 0;
	string toPrint = "";
	do
	{
		showMenu(menuCursor);
		menuCursor = manageMenu(menuCursor);
		clear();
	} while (menuCursor >= 0);
}

void saveScore(string nick, int score)
{
	fstream file;
	file.open("highscores.txt");
	int i = 0;
	string sNick;
	int sScore;
	string toSave = "";
	while (file >> sNick && file >> sScore)
	{
		if (score >= sScore)
		{
			toSave += nick + "\n" + to_string(score) + "\n";
			score = sScore;
			nick = sNick;
		}
		else
		{
			toSave += sNick + "\n" + to_string(sScore) + "\n";
		}
		i++;
	}
	if (i < 10)
	{
		toSave += nick + "\n" + to_string(score);
	}
	file.close();
	file.open("highscores.txt", fstream::out);
	file << toSave;
}

void showHighscores()
{
	system("cls");
	ifstream file;
	file.open("highscores.txt");
	int i = 1;
	string nick;
	int score;
	while (file >> nick && file >> score && i <= 10)
	{
		cout << i << " " << nick << "   " << score << endl;
		i++;
	}
	file.close();
	cout << endl << "   >Press any button";
	_getch();
	system("cls");
	//runMenu();
}

void mainMenu()
{
	runMenu();
}