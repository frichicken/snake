#include <string>
#include <conio.h>
#include <vector>
#include <windows.h>
#include <iostream>
using namespace std;

const int width = 30;
const int height = 10;
const char boundaryHorizontalSprite = '#';
const char boundaryVerticalSprite = '#';
const char foodSprite = 'o';
const char snakeSprite = '*';
const char tailSprite = '*';

int score;
bool screenWrapping = false;

enum class Direction
{
	IDLE,
	UP,
	LEFT,
	RIGHT,
	DOWN
};

enum class ScreenWrapping {
	NONE,
	TOP,
	BOTTOM,
	LEFT,
	RIGHT
};

class Position
{
public:
	int x, y;
	Position() {}
	Position(int a, int b) : x(a), y(b) {}
};

class Snake
{

public:
	Position position;
	Direction direction;
	vector<Position> tails;
	Snake() {}
} snake;

class Food
{
public:
	Position position;
	Food() {};
} food;

void init();

void showConsoleCursor(bool showFlag);
void clearScreen();

int main()
{
	char choice = 0;
	cout << "Snake" << endl;
	cout << "Do you want turn on the \"Screen Wrapping\" feature? [Y/N]: ";
	cin >> choice;


	if (choice == 'Y' || choice == 'y') {
		screenWrapping = true;
	}

	system("cls");

	srand(time(0));
	showConsoleCursor(false);
	init();

	char key = 0;
	while (key != 27)
	{
		int tailLength = snake.tails.size();
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				if (i == 0 || i == height - 1)
				{
					cout << boundaryHorizontalSprite;
				}
				else if (j == snake.position.x && i == snake.position.y)
				{
					cout << snakeSprite;
				}
				else if (j == food.position.x && i == food.position.y)
				{
					cout << foodSprite;
				}
				else if (j == 0 || j == width - 1)
				{
					cout << boundaryVerticalSprite;
				}
				else
				{
					bool tail = false;
					for (int n = 0; n < tailLength; n++)
					{
						if (i == snake.tails.at(n).y && j == snake.tails.at(n).x)
						{
							tail = true;
							cout << tailSprite;
						}
					}

					if (tail == false)
					{
						cout << " ";
					}
				}
			}
			cout << endl;
		}

		cout << "Score: " << score << endl;
		cout << "Press R to restart" << endl;
		cout << "Press ESC to exit" << endl;

		cout << "[HEAD]" << "[" << snake.position.x << "]"
			<< "[" << snake.position.y << "]" << endl;

		/*for (int i = 0; i < tailLength; i++)
		{
			cout << "[" << i << "]" << "[" << snake.tails[i].x << "]"
					<< "[" << snake.tails[i].y << "]" << endl;
		}*/

		clearScreen();

		if (_kbhit())
		{
			key = _getch();
			if (snake.position.x > 0 && snake.position.x < width - 1 && snake.position.y > 0 && snake.position.y < height - 1) {
				if ((key == 'W' || key == 'w') && snake.direction != Direction::DOWN)
				{
					snake.direction = Direction::UP;
				}
				else if ((key == 'A' || key == 'a') && snake.direction != Direction::RIGHT)
				{
					snake.direction = Direction::LEFT;
				}
				else if ((key == 'D' || key == 'd') && snake.direction != Direction::LEFT)
				{
					snake.direction = Direction::RIGHT;
				}
				else if ((key == 'S' || key == 's') && snake.direction != Direction::UP)
				{
					snake.direction = Direction::DOWN;
				}
			}

			if (key == 'R' || key == 'r')
			{
				init();
			}
			else if (key == 'P' || key == 'p')
			{
				snake.direction = Direction::IDLE;
			}
		}

		if (snake.position.x == food.position.x && snake.position.y == food.position.y)
		{
			score += 10;

			food.position.x = rand() % (width - 2) + 1;
			food.position.y = rand() % (height - 2) + 1;

			Position postion;
			if (tailLength == 0)
			{
				postion = snake.position;
			}
			else
			{
				postion = snake.tails[tailLength - 1];
			}
			snake.tails.push_back(postion);
		}

		Position preservedTail;

		if (tailLength > 0)
		{
			preservedTail = snake.tails.at(0);
			snake.tails.at(0) = snake.position;
		}

		for (int i = 1; i < tailLength; i++)
		{
			Position currentTail = snake.tails[i];
			snake.tails[i] = preservedTail;
			preservedTail = currentTail;
		}

		switch (snake.direction)
		{
		case Direction::UP:
			snake.position.y--;
			break;
		case Direction::DOWN:
			snake.position.y++;
			break;
		case Direction::LEFT:
			snake.position.x--;
			break;
		case Direction::RIGHT:
			snake.position.x++;
			break;
		default:
			break;
		}

		for (auto tail : snake.tails)
		{
			if (tail.x == snake.position.x && tail.y == snake.position.y)
			{
				init();
			}
		}

		if (screenWrapping) {
			if (snake.position.x == 0) {
				snake.position.x = width;
			}
			else if (snake.position.x == width) {
				snake.position.x = 0;
			}
			else if (snake.position.y == 0) {
				snake.position.y = height;
			}
			else  if (snake.position.y == height) {
				snake.position.y = 0;
			}
		}
		else {
			if (snake.position.x == width - 1 || snake.position.y == height - 1 ||
				snake.position.x == 0 || snake.position.y == 0)
			{
				init();
			}
		}

		Sleep(60);
	}
}

void init()
{
	snake.tails.clear();

	clearScreen();
	food.position.x = rand() % (width - 2) + 1;
	food.position.y = rand() % (height - 2) + 1;

	snake.direction = Direction::IDLE;
	snake.position.x = rand() % (width - 2) + 1;
	snake.position.y = rand() % (height - 2) + 1;

	score = 0;
}

void showConsoleCursor(bool showFlag)
{
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_CURSOR_INFO cursorInfo;

	GetConsoleCursorInfo(out, &cursorInfo);
	cursorInfo.bVisible = showFlag;
	cursorInfo.dwSize = 20;
	SetConsoleCursorInfo(out, &cursorInfo);
}

void clearScreen()
{
	HANDLE hConsoleOutput;
	COORD dwCursorPosition;

	hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	dwCursorPosition.X = 0;
	dwCursorPosition.Y = 0;
	SetConsoleCursorPosition(hConsoleOutput, dwCursorPosition);
}