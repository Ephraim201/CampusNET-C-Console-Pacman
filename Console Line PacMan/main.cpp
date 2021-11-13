#include <iostream>
#include <conio.h>
#include <chrono>
#include <thread>

using namespace std;

using namespace std::this_thread;     // sleep_for, sleep_until
using namespace std::chrono_literals; // ns, us, ms, s, h, etc.
using std::chrono::system_clock;

#define SCREEN_WIDTH 119
#define SCREEN_HEIGHT 29

#define RESET   "\033[0m"
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[43m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */
#define BOLDBLACK   "\033[1m\033[30m"      /* Bold Black */
#define BOLDRED     "\033[1m\033[31m"      /* Bold Red */
#define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
#define BOLDYELLOW  "\033[1m\033[33m"      /* Bold Yellow */
#define BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */
#define BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */
#define BOLDWHITE   "\033[1m\033[37m"      /* Bold White */
#define REVERSEBLACK   "\033[40m"      /* Black */
#define REVERSERED     "\033[41m"      /* Red */
#define REVERSEGREEN   "\033[42m"      /* Green */
#define REVERSEYELLOW  "\033[43m"      /* Yellow */
#define REVERSEBLUE    "\033[44m"      /* Blue */
#define REVERSEMAGENTA "\033[45m"      /* Magenta */
#define REVERSECYAN    "\033[46m"      /* Cyan */
#define REVERSEWHITE   "\033[47m"      /* White */

enum MazeTiles
{
	EMPTY,
	WALL,
	DOT,
	KEY,
	LOCK
};

MazeTiles rawGrid[SCREEN_WIDTH][SCREEN_HEIGHT];

int xPos, yPos, collectedDots;

bool isRunning = true;

void UpdateDisplay();

void FillGrid()
{

	for (int y = 0; y < SCREEN_HEIGHT; y++) { for (int x = 0; x < SCREEN_WIDTH; x++) { rawGrid[x][y] = (((y == 0 || y == (SCREEN_HEIGHT - 1) || x < 2 || x >(SCREEN_WIDTH - 3)) && (y < 10 || y > 19)) ? MazeTiles::WALL : MazeTiles::EMPTY); } }

	rawGrid[15][5] = MazeTiles::DOT;
	rawGrid[50][5] = MazeTiles::DOT;
	rawGrid[10][20] = MazeTiles::DOT;
	rawGrid[30][20] = MazeTiles::DOT;

}

int ReturnInput() { return (int)_getch(); }

int TrueMod(int n, int ModVal) { return (n % ModVal + ModVal) % ModVal; }

void MovePlayer()
{

	int xPosNew = xPos;

	int yPosNew = yPos;

	int receivedInput = ReturnInput();

	bool validInput = false;

	switch (receivedInput)
	{

		// Up
		case 119:
		case 72:
		case 87:

		yPosNew--;

		yPosNew = TrueMod(yPosNew, SCREEN_HEIGHT);

		validInput = true;

		break;

		// Down
		case 115:
		case 80:
		case 83:

		yPosNew++;

		yPosNew = TrueMod(yPosNew, SCREEN_HEIGHT);

		validInput = true;

		break;

		// Left
		case 97:
		case 75:
		case 65:

		xPosNew--;

		xPosNew = TrueMod(xPosNew, SCREEN_WIDTH);

		validInput = true;

		break;

		// Right
		case 100:
		case 77:
		case 68:

		xPosNew++;

		xPosNew = TrueMod(xPosNew, SCREEN_WIDTH);

		validInput = true;

		break;

		// Quit
		case 27:
		case 113:
		case 81:

		isRunning = false;

		break;

	}

	if (validInput)
	{

		bool MovedPlayer = false;

		switch (rawGrid[xPosNew][yPosNew])
		{

			case MazeTiles::EMPTY:

				MovedPlayer = true;

				break;

			case MazeTiles::DOT:

				collectedDots++;

				rawGrid[xPosNew][yPosNew] = MazeTiles::EMPTY;

				MovedPlayer = true;

				break;

			case MazeTiles::KEY:

				rawGrid[xPosNew][yPosNew] = MazeTiles::EMPTY;

				for (int y = 0; y < SCREEN_HEIGHT; y++) { for (int x = 0; x < SCREEN_WIDTH; x++) { if (rawGrid[x][y] = MazeTiles::LOCK) { rawGrid[x][y] = MazeTiles::EMPTY; } } }

				MovedPlayer = true;

				break;

		}

		if (MovedPlayer)
		{

			xPos = xPosNew;

			yPos = yPosNew;

			UpdateDisplay();			

		}

	}

}

string ReturnCharColor(MazeTiles TileType)
{

	switch (TileType)
	{

		case MazeTiles::EMPTY: return RESET;

		case MazeTiles::WALL: return BLUE;

		case MazeTiles::DOT: return BOLDYELLOW;

		case MazeTiles::KEY: return GREEN;

	}

	return 0;

}

char ReturnMazeTileChar(MazeTiles TileType)
{

	switch (TileType)
	{

		case MazeTiles::EMPTY: return ' ';

		case MazeTiles::WALL: return 219;

		case MazeTiles::DOT: return 15;

		case MazeTiles::KEY: return 4;

	}

	return 0;

}

void UpdateDisplay()
{

	system("CLS");

	for (int y = 0; y < SCREEN_HEIGHT; y++)
	{

		for (int x = 0; x < SCREEN_WIDTH; x++)
		{

			// Print Player
			if ((x == xPos) && (y == yPos))
			{

				cout << YELLOW << "@";

			}

			// Print Tile
			else
			{

				cout << ReturnCharColor(rawGrid[x][y]) << ReturnMazeTileChar(rawGrid[x][y]);

			}

			// Reset Color
			cout << RESET;

		}

		// Print New Line (Row End)
		cout << "\n";

	}

	cout << "Collected '" << ReturnCharColor(MazeTiles::DOT) << ReturnMazeTileChar(MazeTiles::DOT) << WHITE << "': " << collectedDots;

}

int main()
{

	xPos = 10;

	yPos = 15;

	collectedDots = 0;

	FillGrid();

	UpdateDisplay();

	while (isRunning) { MovePlayer(); }

	// for (int i = 0; i < 256; i++) { std::cout << i << ". " << (char)i << std::endl; }

}