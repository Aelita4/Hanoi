#include <iostream>
#include <windows.h>
#include <conio.h>

//COLORS
#define BLACK 0
#define BLUE 1
#define GREEN 2
#define AQUA 3
#define RED 4
#define PURPLE 5
#define YELLOW 6
#define WHITE 7
#define GRAY 8
#define LIGHT_BLUE 9
#define LIGHT_GREEN 10
#define LIGHT_AQUA 11
#define LIGHT_RED 12
#define LIGHT_PURPLE 13
#define LIGHT_YELLOW 14
#define BRIGHT_WHITE 15

unsigned int counter = 0;
unsigned int maxx = 0;

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

void gotoxy(int x, int y)
{
	COORD c;
	c.X = x;
	c.Y = y;
	SetConsoleCursorPosition(hConsole, c);
}

char getCursorChar()
{
	char c = '\0';
	CONSOLE_SCREEN_BUFFER_INFO con;
	if (hConsole != INVALID_HANDLE_VALUE &&
		GetConsoleScreenBufferInfo(hConsole, &con))
	{
		DWORD read = 0;
		if (!ReadConsoleOutputCharacterA(hConsole, &c, 1,
			con.dwCursorPosition, &read) || read != 1
			)
			c = '\0';
	}
	return c;
}

char readChar(int x, int y)
{
	gotoxy(x, y);
	char c = getCursorChar();
	return c;
}

void vanishCursor()
{
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 100;
	info.bVisible = FALSE;
	SetConsoleCursorInfo(hConsole, &info);
}

void color(int foreground, int background)
{
	int color = (background * 16) + foreground;
	SetConsoleTextAttribute(hConsole, color);
}

int getColor(int n)
{
	switch (n)
	{
	case 1:
		return LIGHT_GREEN;
	case 2:
		return YELLOW;
	case 3:
		return BLUE;
	case 4:
		return LIGHT_RED;
	case 5:
		return RED;
	default:
		return 0;
	}
}

void generatePoles()
{
	color(WHITE, BLACK);
	gotoxy(15, 20);
	std::cout << "-----+-----";
	gotoxy(30, 20);
	std::cout << "-----+-----";
	gotoxy(45, 20);
	std::cout << "-----+-----";

	for (int i = 1; i <= 5; i++)
	{
		gotoxy(20, 20 - i);
		std::cout << "|";
		gotoxy(35, 20 - i);
		std::cout << "|";
		gotoxy(50, 20 - i);
		std::cout << "|";
	}
}

void generateRing(int n)
{
	for (int j = n; j > 0; j--)
	{
		color(getColor(j), BLACK);
		for (int i = 0; i < j; i++)
		{
			gotoxy(21 + i, (19 - n) + j);
			std::cout << "_";
			gotoxy(19 - i, (19 - n) + j);
			std::cout << "_";
		}
	}

}

void editRing(char action, int n, char pole, int total)
{
	int x = 0;
	switch (pole)
	{
	case 'A':
		x = 20;
		break;
	case 'B':
		x = 35;
		break;
	case 'C':
		x = 50;
		break;
	}

	switch (action)
	{
	case 'P':
	case 'p':
		for (int i = 0; i < total; i++)
		{
			if (readChar(x + 1, 19 - i) == ' ')
			{
				for (int j = 0; j < n; j++)
				{
					color(getColor(n), BLACK);
					gotoxy(x + 1 + j, 19 - i);
					std::cout << "_";
					gotoxy(x - 1 - j, 19 - i);
					std::cout << "_";
				}
				break;
			}
		}
		break;
	case 'r':
	case 'R':
		for (int i = 0; i < total + 1; i++)
		{
			if (readChar(x + 1, 19 - i) == ' ')
			{
				for (int j = 0; j < total; j++)
				{
					gotoxy(x + 1 + j, 19 - i + 1);
					std::cout << " ";
					gotoxy(x - 1 - j, 19 - i + 1);
					std::cout << " ";
				}
			}
		}
		break;
	}
}

void hanoi(int n, char x, char y, char z)
{
	if (n > maxx) maxx = n;
	if (n == 1)
	{
		editRing('R', n, x, maxx);
		editRing('P', n, y, maxx);
		Sleep(1000);
	}
	else
	{
		hanoi(n - 1, x, z, y);
		editRing('R', n, x, maxx);
		editRing('P', n, y, maxx);
		Sleep(1000);
		hanoi(n - 1, z, y, x);
	}
}

void hanoiMain(int n)
{
	generatePoles();
	generateRing(n);
	Sleep(1000);
	hanoi(n, 'A', 'B', 'C');
}

int main()
{
	vanishCursor();
	int n;
	while (1)
	{
		std::cout << "How many rings? ";
		std::cin >> n;
		if (n > 5 || n < 3)
		{
			std::cout << "Select a number between 3 and 5" << std::endl;
			n = 0;
		}
		else
		{
			system("cls");
			hanoiMain(n);
			break;
		}
	}
	color(WHITE, BLACK);
	gotoxy(0, 0);
	return 0;
}