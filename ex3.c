/******************************************************************************

							Online C Compiler.
				Code, Compile, Run and Debug C program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/

#include<stdio.h>
#include <stdlib.h>

#define MAX_HEIGHT_SIZE 70
#define MIN_HEIGHT_SIZE 10
#define MAX_WIDTH_SIZE 70
#define MIN_WIDTH_SIZE 10
#define PLAYER_COLOR 'R'
#define COMPUTER_COLOR 'G'
#define PLAYER_COLOR_LOWERCASE 'r'
#define COMPUTER_COLOR_LOWERCASE 'g'
#define DEAD_COLOR '-'

void printBoard(const char board[MAX_HEIGHT_SIZE][MAX_WIDTH_SIZE], int width, int height)
{
	int i;
	int j;
	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			printf("%c", board[i][j]);
		}
		printf("\n");
	}
}

void getDimentions(int* width, int* height) {
	printf("Enter width (10-70):\n");
	scanf("%d", width);
	while (*width < 10 || *width > 70)
	{
		printf("Enter width (10-70):\n");
		scanf("%d", width);
	}
	printf("Enter height (10-70):\n");
	scanf("%d", height);
	while (*height < 10 || *height > 70)
	{
		printf("Enter height (10-70):\n");
		scanf("%d", height);
	}
}

void initLivingCells(char board[MAX_HEIGHT_SIZE][MAX_WIDTH_SIZE], int width, int height)
{
	int number_of_living_cells;
	printf("Enter number of living cells (0-%d):\n", width*height);
	scanf("%d", &number_of_living_cells);
	while (number_of_living_cells<0 || number_of_living_cells > width*height) {
		printf("Enter number of living cells (0-%d):\n", width*height);
		scanf("%d", &number_of_living_cells);
	}
	int i;
	int j;
	for (i = 0; i < number_of_living_cells; i++) {
		int x, y;
		char color;
		printf("Enter x y and color (R/G):\n");
		scanf("%d %d %c", &x, &y, &color);

		while (x < 0 || x >= height || y < 0 || y >= width || (color != 'r' && color != 'R' && color != 'g' && color != 'G'))
		{
			printf("Enter x y and color (R/G):\n");
			scanf("%d %d %c", &x, &y, &color);
		}

		if (color == 'r')
		{
			color = 'R';
		}
		if (color == 'g')
		{
			color = 'G';
		}
		board[x][y] = color;
	}
}

void initBoard(char board[MAX_HEIGHT_SIZE][MAX_WIDTH_SIZE], int* width, int* height)
{
	getDimentions(width, height);
	int i;
	int j;
	for (i = 0; i < *height; i++)
	{
		for (j = 0; j < *width; j++)
		{
			board[i][j] = '-';
		}
	}

	initLivingCells(board, *width, *height);
}

long int getNumOfGenerations()
{
	long int number_of_generations;
	printf("Enter number of generations (>0):\n");
	scanf("%ld", &number_of_generations);
	while (number_of_generations < 0)
	{
		printf("Enter number of generations (>0):\n");
		scanf("%ld", &number_of_generations);
	}
	return number_of_generations;
}

void initGame(char board[MAX_HEIGHT_SIZE][MAX_WIDTH_SIZE], int* width, int* height, long int* generations)
{
	printf("Welcome to the game of life!\nSettings:\n");
	initBoard(board, width, height);
	printBoard(board, *width, *height);
	*generations = getNumOfGenerations();
}

char getCurrentPlayer(int gen_index)
{
	if (gen_index % 2 == 1)
	{
		return 'R';
	}
	else
	{
		return 'G';
	}
}

void CountNeighbours(char board[MAX_HEIGHT_SIZE][MAX_WIDTH_SIZE], int width, int height, int i, int j, int* playerNeighbours, int* computerNeighbours)
{
	int living_counter_R = 0;
	int living_counter_G = 0;
	int k;
	int l;

	for (k = i - 1; k <= i + 1; k++)
	{
		for (l = j - 1; l <= j + 1; l++)
		{
			if (k < 0 || k > height || l < 0 || l > height)
			{
				continue;
			}

			if (k == i && l == j)
			{
				continue;
			}

			if (board[k][l] == 'R')
			{
				living_counter_R++;
			}

			if (board[k][l] == 'G')
			{
				living_counter_G++;
			}
		}
	}

	*playerNeighbours = living_counter_R;
	*computerNeighbours = living_counter_G;

}

void processBoard(char board[MAX_HEIGHT_SIZE][MAX_WIDTH_SIZE], int width, int height)
{
	char new_board[MAX_HEIGHT_SIZE][MAX_WIDTH_SIZE] = { {0} };
	int i;
	int j;
	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			new_board[i][j] = board[i][j];
		}
	}
	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			int living_counter_R = 0;
			int living_counter_G = 0;

			CountNeighbours(board, width, height, i, j, &living_counter_R, &living_counter_G);

			int living_counter = living_counter_R + living_counter_G;

			if (living_counter <= 1 || living_counter >= 4)
			{
				new_board[i][j] = '-';
			}

			if (board[i][j] == '-' && living_counter == 3)
			{
				if (living_counter_R > living_counter_G)
				{
					new_board[i][j] = 'R';
				}
				else
				{
					new_board[i][j] = 'G';
				}
			}

		}
	}
	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			board[i][j] = new_board[i][j];
		}
	}
}

/***************************************************************************************
Function name: ComputerTurn
Input: the board, the height and the width, and pointers to the computer next move location (x,y).
Output: None (puts valid values into x and y).
The function operation: calculate the next move the computer will make.
****************************************************************************************/
void ComputerTurn(char board[MAX_HEIGHT_SIZE][MAX_WIDTH_SIZE], int width, int height, int* x, int* y) {
	int i, j;
	int localX[2] = { 0,0 }, localY[2] = { 0,0 };
	int flag[2] = { 0,0 };
	int playerNeighbours = 0, computerNeighbours = 0;

	//computer scans the board from the top left to the bottom right
	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			// read the commnets above about CountNeighbours
			CountNeighbours(board, width, height, i, j, &playerNeighbours, &computerNeighbours);
			//look for the first player cell with 2 or 3 neighbours, and return the location
			if (board[i][j] == PLAYER_COLOR &&
				(playerNeighbours + computerNeighbours == 2 || playerNeighbours + computerNeighbours == 3)) {
				*x = i;
				*y = j;
				return;
			}
			//look for the first dead cell with 2 or 3 neighbours that are mostly belong to the player, and remember it
			if (board[i][j] == DEAD_COLOR && playerNeighbours > computerNeighbours && !flag[0] &&
				(playerNeighbours + computerNeighbours == 2 || playerNeighbours + computerNeighbours == 3)) {
				localX[0] = i;
				localY[0] = j;
				flag[0] = 1;
			}
			//look for the first player cell, and remember it
			if (board[i][j] == PLAYER_COLOR && !flag[1]) {
				localX[1] = i;
				localY[1] = j;
				flag[1] = 1;
			}

		}
	}
	// if we are here, we didn't found player cell with 2 or 3 neighbours

	// if we found dead cell with 2 or 3 neighbours that are mostly belong to the player, we return the location
	if (flag[0]) {
		*x = localX[0];
		*y = localY[0];
		return;
	}
	// if we are here, we didn't found dead cell with 2 or 3 neighbours that are mostly belong to the player

	// we return the location of the first player cell
	*x = localX[1];
	*y = localY[1];
}

void checkWin(char board[MAX_HEIGHT_SIZE][MAX_WIDTH_SIZE], int width, int height) {
	int living_counter_R = 0; int living_counter_G = 0;
	int i;
	int j;
	for (i = 0; i < height; i++) {
		for (j = 0; j < width; j++) {
			if (board[i][j] == 'R') {
				living_counter_R++;
			}
			if (board[i][j] == 'G') {
				living_counter_G++;
			}
		}
	}

	if (living_counter_R > 0 && living_counter_G == 0) {
		printf("Game over! R is the winner :)\n");
		exit(0);
	}

	if (living_counter_G > 0 && living_counter_R == 0) {
		printf("Game over! G is the winner :(\n");
		exit(0);
	}

	if (living_counter_G == 0 && living_counter_R == 0) {
		printf("Game over! There is no winner :|\n");
		exit(0);
	}
}
void checkFinalWin(char board[MAX_HEIGHT_SIZE][MAX_WIDTH_SIZE], int width, int height) {
	int living_counter_R = 0; int living_counter_G = 0;
	int i;
	int j;
	for (i = 0; i < height; i++) {
		for (j = 0; j < width; j++) {
			if (board[i][j] == 'R') {
				living_counter_R++;
			}
			if (board[i][j] == 'G') {
				living_counter_G++;
			}
		}
	}

	if (living_counter_R > living_counter_G) {
		printf("Game over! R is the winner :)\n");
		exit(0);
	}

	if (living_counter_G > living_counter_R) {
		printf("Game over! G is the winner :(\n");
		exit(0);
	}

	if (living_counter_G == living_counter_R) {
		printf("Game over! There is no winner :|\n");
		exit(0);
	}
}

void playGame(char board[MAX_HEIGHT_SIZE][MAX_WIDTH_SIZE], int width, int height, long int generations)
{
	printf("Welcome to the game of life!\nThis is the initial board:\n");
	processBoard(board, width, height);
	printBoard(board, width, height);

	for (int gen_index = 1; gen_index <= generations; gen_index++)
	{
		char currentPlayer = getCurrentPlayer(gen_index);
		printf("%c is playing\nx y:\n", currentPlayer);
		int x, y;

		if (currentPlayer == 'R') {
			scanf("%d%d", &x, &y);
		}
		else {
			ComputerTurn(board, width, height, &x, &y);
			printf("%d %d\n", x, y);
		}

		if (board[x][y] == '-')
		{
			board[x][y] = currentPlayer;
		}
		if (currentPlayer == 'R' && board[x][y] == 'G')
		{
			board[x][y] = 'R';
		}
		if (currentPlayer == 'G' && board[x][y] == 'R')
		{
			board[x][y] = 'G';
		}
		processBoard(board, width, height);
		printBoard(board, width, height);
		checkWin(board, width, height);
	}
	checkFinalWin(board, width, height);
}

int main() {
	char board[MAX_HEIGHT_SIZE][MAX_WIDTH_SIZE];
	int width, height;
	long int generations;

	initGame(board, &width, &height, &generations);
	playGame(board, width, height, generations);

	return 0;
}