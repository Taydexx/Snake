
#define _CRT_SECURE_NO_DEPRECATE
#include <conio.h>
#include <Windows.h>
#include <ctime>
#include <string>

const char KEY_UP = 'w';
const char KEY_DOWN = 's';
const char KEY_LEFT = 'a';
const char KEY_RIGHT = 'd';
const char KEY_ACCEPT = 'f';

const char KEY_UP2 = 72;
const char KEY_DOWN2 = 80;
const char KEY_LEFT2 = 75;
const char KEY_RIGHT2 = 77;
const char KEY_ACCEPT2 = 13;

const int QUIT = -1;
const int MAIN_MENU_OPTIONS_AMOUNT = 3;
const int OPTIONS_AMOUNT = 5;

using namespace std;

struct SNAKE
{
	int *x;
	int *y;
};


int manageMenu(int optionsAmount, int menuCursor, int(*functionHandler)(int));
void showMenu(string choices[], int choicesAmount, string toPrint, int menuCursor);
void setTerminalWindow(int width, int height);
char **createMap();
void fill(char **array);
void show(char **array, int score);
void clear();
bool moveSnake(char **map, int *x, int *y, int x_dest, int y_dest, int snakePosition, int snakeSize);
bool makeMove(int ch, char **map, int *x, int *y, int snakeSize);
void deletePointers(char **map, int *x, int *y);
SNAKE createSnake(int snakeSize);
SNAKE extendSnake(SNAKE snake, int *snakeSize);
bool checkCollision(int colX, int colY, int *x, int *y, int snakeSize);
bool checkForPoint(int destX, int destY, int x, int y, int *score);
void createApple(int *x, int *y, char **map);
int playGame();
void createGame();
char * getNick();
void exit();
int setIntValue();
int setGameSpeed();
bool changeGamemode();
void showHighscores();
int moveCursorDown(int menuCursor, int length);
int moveCursorUp(int menuCursor, int length);
int useMenuOption(int i);
void runMenu();
int selectOption(int menuCursor);
void runOptions();
string getGameSpeed();
int selectSpeed(int menuCursor);
void mainMenu();
void saveScore(char * nick, int score);
void loadOptions();
void saveOptions();
void mainMenu();

int width = 30;
int height = 20;
int delay = 40;
bool gamemode = 1;
time_t startTime;
int playTime;

int main()
{
	mainMenu();
	return 0;
}

void setTerminalWindow(int width, int height)
{
	HWND console = GetConsoleWindow();
	RECT r;
	GetWindowRect(console, &r);
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	MoveWindow(console, r.left, r.top, (width + 6) * 12, (height + 5) * 20, TRUE);
}

char **createMap()
{
	char **map = new char*[width];
	for (int i = 0; i < width; i++)
	{
		map[i] = new char[height];
	}
	return map;
}

void fill(char **array)
{
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			array[j][i] = ' ';
		}
	}
}

void show(char **array, int score)
{
	string toPrint = "Punkty: ";
	toPrint += to_string(score);
	toPrint += "    Czas:  ";
	playTime = time(NULL) - startTime;
	toPrint += to_string(playTime);
	toPrint += "s\n";
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
	printf("%s", toPrint.c_str());
}

void clear()
{
	COORD cur = { 0, 0 };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cur);
}

bool moveSnake(char **map, int *x, int *y, int x_dest, int y_dest, int snakePosition, int snakeSize)
{
	int x_old = x[snakePosition];
	int y_old = y[snakePosition];
	x[snakePosition] = x_dest;
	y[snakePosition] = y_dest;
	map[x[snakePosition]][y[snakePosition]] = snakePosition? '#':'O';
	if (snakePosition != snakeSize)
	{
		moveSnake(map, x, y, x_old, y_old, snakePosition + 1, snakeSize);
	}
	else map[x_old][y_old] = map[x_old][y_old] == 'O'? 'O':' ';
	return 1;
}

bool makeMove(int ch, char **map, int *x, int *y, int snakeSize)
{
	switch (ch)
	{
	case KEY_LEFT:
		if (x[0] == 0)
		{
			return gamemode ? moveSnake(map, x, y, width - 1, y[0], 0, snakeSize) : 0;
		}
		else moveSnake(map, x, y, x[0] - 1, y[0], 0, snakeSize);
		break;
	case KEY_RIGHT:
		if (x[0] == width - 1)
		{
			return gamemode ? moveSnake(map, x, y, 0, y[0], 0, snakeSize) : 0;
		}
		else moveSnake(map, x, y, x[0] + 1, y[0], 0, snakeSize);
		break;
	case KEY_UP:
		if (y[0] == 0)
		{
			return gamemode ? moveSnake(map, x, y, x[0], height - 1, 0, snakeSize) : 0;
		}
		else moveSnake(map, x, y, x[0], y[0] - 1, 0, snakeSize);
		break;
	case KEY_DOWN:
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

void deletePointers(char **map, int *x, int *y)
{
	for (int i = 0; i < width; i++)
	{
		delete[] map[i];
	}
	delete[] map;
	delete[] x;
	delete[] y;
}

SNAKE createSnake(int snakeSize)
{
	int *x = new int[snakeSize];
	int *y = new int[snakeSize];
	srand(time(NULL));
	SNAKE snake;
	for (int i = 0; i < snakeSize; i++)
	{
		x[i] = width/2;
		y[i] = height/2;
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

bool checkForPoint(int destX, int destY, int x, int y, int *score)
{
	if (destX == x && destY == y)
	{
		*score +=1;
		return 1;
	}
	return 0;
}

void createApple(int *x, int *y, char **map)
{
	int celX, celY;
	do
	{
		celX = rand() % width;
		celY = rand() % height;
	} while (map[celX][celY] != ' ');
	map[celX][celY] = '0';
	*x = celX;
	*y = celY;
}

int playGame()
{
	setTerminalWindow(width, height);
	int score = 0;
	char **map = createMap();
	fill(map);
	int snakeSize = 10;
	system("cls");
	SNAKE snake = createSnake(snakeSize);
	snake = extendSnake(snake, &snakeSize);
	int *x = snake.x;
	int *y = snake.y;
	bool stan = 1;
	char direction = 'd';
	int appleX;
	int appleY;
	createApple(&appleX, &appleY, map);
	startTime = time(NULL);
	while (stan)
	{
		clear();
		show(map, score);
		if (_kbhit())
		{
			char key = _getch();
			switch (direction)
			{
			case KEY_RIGHT: if (key == KEY_LEFT || key == KEY_LEFT2 || tolower(key) == KEY_LEFT) key = KEY_RIGHT; break;
			case KEY_LEFT: if (key == KEY_RIGHT || key == KEY_RIGHT2 || tolower(key) == KEY_RIGHT) key = KEY_LEFT; break;
			case KEY_UP: if (key == KEY_DOWN || key == KEY_DOWN2 || tolower(key) == KEY_DOWN) key = KEY_UP; break;
			case KEY_DOWN: if (key == KEY_UP || key == KEY_UP2 || tolower(key) == KEY_UP) key = KEY_DOWN; break;
			}
			if (key == KEY_LEFT2 || key == KEY_DOWN2 || key == KEY_RIGHT2 || key == KEY_UP2 || key == 27)
			{
				direction = key;
			}
			switch (key)
			{
			case KEY_LEFT2: direction = KEY_LEFT; break;
			case KEY_RIGHT2: direction = KEY_RIGHT; break;
			case KEY_UP2: direction = KEY_UP; break;
			case KEY_DOWN2: direction = KEY_DOWN; break;
			}
			key = tolower(key);
			if (key == KEY_LEFT || key == KEY_DOWN || key == KEY_RIGHT || key == KEY_UP || key == 27)
			{
				direction = key;
			}
		}
		stan = makeMove(direction, map, snake.x, snake.y, snakeSize - 2) && checkCollision(snake.x[0], snake.y[0], snake.x, snake.y, snakeSize - 1);
		if (checkForPoint(snake.x[0], snake.y[0], appleX, appleY, &score))
		{
			createApple(&appleX, &appleY, map); snake = extendSnake(snake, &snakeSize);
		}
		Sleep(delay);
	}
	deletePointers(map, snake.x, snake.y);
	system("cls");
	setTerminalWindow(60, 20);
	return score;
}

void createGame()
{
	runOptions();
	clear();
	saveScore(getNick(), playGame());
}

char * getNick()
{
	static char charArray[16];
	printf("Podaj imie (maksymalna dlugosc 15) \n");
	scanf_s("%15s", charArray, (unsigned)_countof(charArray));
	system("cls");
	return charArray;
}

void exit()
{
	system("cls");
}

int setIntValue()
{
	int value;
	int buff;
	system("cls");
	printf("Podaj wartosc (minimalna wartosc 8) \n");
	while (!scanf_s("%d", &buff) || buff <8)
	{
		system("cls");
		printf("Prosze podac prawidlowa wartosc (minimalna wartosc 8) \n");
	} 
	system("cls");
	value = buff;
	return value;
}

int setGameSpeed()
{
	system("cls");
	int menuCursor = 0;
	string choices[3] = { "Wolno   ", "Standardowo   ", "Szybko   " };
	string toPrint = "Wybierz predkosc rozgrywki\n";
	toPrint = "\n";
	do
	{
		showMenu(choices, 3,toPrint, menuCursor);
		menuCursor = manageMenu(3, menuCursor, selectSpeed);
		clear();
	} while (menuCursor >= 0);
	return -menuCursor;
}

bool changeGamemode()
{
	return !gamemode;
}

void showHighscores()
{
	system("cls");
	FILE *file = NULL;
	file = fopen("highscores.txt", "r");
	int i = 1;
	char nick[16];
	char score[10];
	char time[10];
	printf("\n   Nick              Punkty   Czas\n");
	while (fgets(nick, 15, file) && fgets(score, 10, file) && fgets(time, 10, file) && i <= 10)
	{
		strtok(nick, "\n");
		strtok(score, "\n");
		strtok(time, "\n");
		printf("%-2d %-15s   %6s   %4s\n", i, nick, score, time);
		i++;
	}
	fclose(file);
	printf("\n   >Nacisnij przycisk");
	_getch();
	system("cls");
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

int useMenuOption(int i)
{
	switch (i)
	{
	case 0: createGame(); break;
	case 1: showHighscores(); break;
	case 2: exit(); return QUIT; break;
	}
	return 0;
}

void showMenu(string choices[], int choicesAmount, string toPrint, int menuCursor)
{
	for (int i = 0; i < choicesAmount; i++)
	{
		toPrint += "  ";
		if (i == menuCursor)
		{
			toPrint += " >";
		}
		toPrint += choices[i];
		toPrint += "\n";
	}
	printf("%s", toPrint.c_str());
}

void runMenu()
{
	system("cls");
	setTerminalWindow(60, 20);
	int menuCursor = 0;
	string choices[MAIN_MENU_OPTIONS_AMOUNT] = { "Rozpocznij Gre   ", "Najlepsze wyniki   ", "Wyjscie   " };
	string toPrint = "\n";
	do
	{
		showMenu(choices, MAIN_MENU_OPTIONS_AMOUNT, toPrint, menuCursor);
		menuCursor = manageMenu(MAIN_MENU_OPTIONS_AMOUNT, menuCursor, useMenuOption);
		clear();
	} while (menuCursor >= 0);
}

int selectOption(int menuCursor)
{
	switch (menuCursor)
	{
	case 0: width = setIntValue(); break;
	case 1: height = setIntValue(); break;
	case 2:	delay = setGameSpeed(); break;
	case 3: gamemode = changeGamemode(); break;
	case 4: exit(); return QUIT;
	}
	return menuCursor;
}

void runOptions()
{
	system("cls");
	int menuCursor = 0;
	string toPrint = "\n";
	do
	{
		string choices[OPTIONS_AMOUNT] = { "Szerokosc planszy:   " + to_string(width) + "    ", "Wysokosc planszy:   " + to_string(height) + "    ", " ", gamemode ? "Sciany wylaczone    " : "Sciany wlaczone    ", "Graj   " };
		choices[2] = getGameSpeed();
		showMenu(choices, OPTIONS_AMOUNT, toPrint, menuCursor);
		menuCursor = manageMenu(OPTIONS_AMOUNT, menuCursor, selectOption);
		clear();
	} while (menuCursor >= 0);
}

string getGameSpeed()
{
	switch (delay)
	{
	case 80: return "Predkosc gry: wolna    "; break;
	case 40: return "Predkosc gry: standardowa    "; break;
	}
	return "Predkosc gry: szybka    ";
}

int selectSpeed(int menuCursor)
{
	switch (menuCursor)
	{
	case 0: return -80; break;
	case 1: return -40; break;
	case 2:	return -20; break;
	}
	return -40;
}

int manageMenu(int optionsAmount, int menuCursor, int(*functionHandler)(int))
{
	char key = _getch();
	switch (key)
	{
	case KEY_DOWN2: return moveCursorDown(menuCursor, optionsAmount);
	case KEY_UP2: return moveCursorUp(menuCursor, optionsAmount);
	case KEY_ACCEPT2: return functionHandler(menuCursor);
	}
	key = tolower(key);
	switch (key)
	{
	case KEY_DOWN: return moveCursorDown(menuCursor, optionsAmount);
	case KEY_UP: return moveCursorUp(menuCursor, optionsAmount);
	case KEY_ACCEPT: return functionHandler(menuCursor);
	}
	return menuCursor;
}

void saveScore(char * nick, int score)
{
	FILE * file;
	file = fopen("highscores.txt", "r");
	char nickBuff[16];
	char nicks[10][16];
	char scoreBuff[10];
	int scoreArr[10];
	int times[10];
	char timeBuff[10];
	int i = 0;
	while (fgets(nickBuff, 15, file) && fgets(scoreBuff, 10, file) && fgets(timeBuff, 10, file))
	{
		strtok(nickBuff, "\n");
		strtok(scoreBuff, "\n");
		strtok(timeBuff, "\n");
		if (score > atoi(scoreBuff))
		{
			scoreArr[i] = score;
			score = atoi(scoreBuff);
			strcpy(nicks[i], nick);
			nick = nickBuff;
			times[i] = playTime;
			playTime = atoi(timeBuff);
		}
		else
		{
			scoreArr[i] = atoi(scoreBuff);
			strcpy(nicks[i], nickBuff);
			times[i] = atoi(timeBuff);
		}
		i++;
	}
	if (i < 10)
	{
		scoreArr[i] = score;
		strcpy(nicks[i], nick);
		times[i] = playTime;
		i++;
	}
	fclose(file);
	file = fopen("highscores.txt", "w");
	for (int j = 0; j < i; j++)
	{
		fprintf(file, "%s\n%d\n%d\n", nicks[j], scoreArr[j], times[j]);
	}

	fclose(file);
}

void loadOptions()
{
	FILE * file;
	file = fopen("options.txt", "r");
	char buffC[10];
	fgets(buffC, 10, file);
	height = atoi(buffC);	
	fgets(buffC, 10, file);
	width = atoi(buffC);	
	fgets(buffC, 10, file);
	delay = atoi(buffC);	
	fgets(buffC, 10, file);
	gamemode = atoi(buffC);
	fclose(file);
}

void saveOptions()
{
	FILE * file;
	file = fopen("options.txt", "w");
	fprintf(file, "%d\n%d\n%d\n%d\n", height, width, delay, gamemode);
	fclose(file);
}

void mainMenu()
{
	loadOptions();
	runMenu();
	saveOptions();
}