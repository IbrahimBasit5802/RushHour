/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*						Name: Muhammad Ibrahim Basit						 *
*						Roll NO: 21I-0669									 *
*                       Project: 2D Rush-Hour Game                           *
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  */

//============================================================================
// Name        : .cpp
// Author      : FAST CS Department
// Version     :
// Copyright   : (c) Reserved
// Description : Basic 2D game of Rush Hour...
//============================================================================

#ifndef RushHour_CPP_
#define RushHour_CPP_
#include "util.h"
#include <iostream>
#include<string>
#include <stdlib.h>
#include <time.h>
#include <fstream>
#include<cmath> // for basic math functions such as cos, sin, sqrt
using namespace std;
// Forward Declarations for functions representing different screens in the game:
void Menu();
void highScoreScreen();
void selectionScreen();
void inputNameScreen();
void gameOverScreen();
void drawBoard();
// Forward Declarations for other functions:
void writeHighScores();
void readHighScores();
void sortScores();
// Global Variables used:
char board[20][20], taxi_color; // The board is basically the whole map. The whole game logic is controlled
								// through its 2D array.			
string player_name = "", game_over_text;
int high_scores[11]; // 
string high_names[11];
int des_y, des_x; // The x and y of the desired destinations of the passengers
int passengers_dropped = 0, passengers_onscreen = 0;
int score = 0, mouseClickXPos = 0, mouseClickYPos = 0;
int timer, tracking_time = 0, number_of_scores = 0; // Timer to run the game for 3 minutes and tracking
													// is used to decrement the timer in timer function every sec

bool gameOver, file_written = true, sorted = true;
/* Function sets canvas size (drawing area) in pixels...
 *  that is what dimensions (x and y) your game will have
 *  Note that the bottom-left coordinate has value (0,0) and top-right coordinate has value (width-1,height-1)
 * */
void SetCanvasSize(int width, int height) {
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, width, 0, height, -1, 1); // set the screen size to given width and height.
	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity();
}

// One box on the grid is 50 pixels and one y is 42 pixels:
int one_grid_x = 50, one_grid_y = 42; 
int x_speed, y_speed; // For different speeds for red and yellow taxis
// Initialize the initial position of our car:
int xI = 0, yI = one_grid_y * 19;

bool pickedUp = false, playing = false, inputtingName = false, flagx = true, flagy = true;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*						Name: moveOtherCarsX								 *
*						Type: Logic											 *
*        Purpose: Ranomly decides whether to move npc cars at an empty space *
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  */

void moveOtherCarsX() {
	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 20; j++) {
			if (board[i][j] == 'C') {
				if (board[i][j - 1] == 'E' and flagx and j * one_grid_x > 0){
					board[i][j - 1] = 'C';
					board[i][j] = 'E';
				}
				else {
					flagx = false;
				}
				
				if (board[i][j + 1] == 'E' and !flagx and j * one_grid_x < 1000) {
					board[i][j + 1] = 'C';
					board[i][j] = 'E';
				}
				else {
					flagx = true;
				}
			}
		}
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*						Name: moveOtherCarsY								 *
*						Type: Logic											 *
*        Purpose: Ranomly decides whether to move npc cars at an empty space *
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  */

void moveOtherCarsY() {
	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 20; j++) {
			if (board[i][j] == 'C') {
				if (board[i - 1][j] == 'E' and flagy and i * one_grid_x > 0){
					board[i - 1][j] = 'C';
					board[i][j] = 'E';
				}
				else {
					flagy = false;
				}
				
				if (board[i + 1][j] == 'E' and !flagy and i * one_grid_x < 840) {
					board[i + 1][j] = 'C';
					board[i][j] = 'E';
				}
				else {
					flagy = true;
				}
			}
		}
	}
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*						Name: Menu											 *
*						Type: Screen										 *
*                       Purpose: Displays the game's main menu               *
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  */

void Menu() {
	glClearColor(1.0f, 0.0f, 1.0f, 0.0f);
	glClear (GL_COLOR_BUFFER_BIT);
	DrawString(325, 750, "Rush Hour by Muhammad Ibrahim Basit", colors[BLACK]);
	DrawRoundRect(410,500,200,100,colors[LIME_GREEN],20);
	DrawString(488, 535, "Play", colors[BLACK]);
	DrawRoundRect(410,300,200,100,colors[LIME_GREEN],20);
	DrawString(480, 340, "Scores", colors[BLACK]);
	if((mouseClickXPos >= 410 and mouseClickXPos <= 610) and (mouseClickYPos >= 280 and mouseClickYPos <= 380)) {
		glutDisplayFunc(selectionScreen);
	}
	else if((mouseClickXPos >= 410 and mouseClickXPos <= 610) and (mouseClickYPos >= 481 and mouseClickYPos <= 578)) {
		glutDisplayFunc(highScoreScreen);
		readHighScores();
	}
	glutSwapBuffers();
	glutPostRedisplay();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*						Name: highScoreScreen								 *
*						Type: Screen										 *
*                       Purpose: Displays the game's high scores screen      *
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  */

void highScoreScreen() {
	glClearColor(1.0f, 0.0f, 1.0f, 0.0f);
	glClear (GL_COLOR_BUFFER_BIT);
	DrawRoundRect(250, 230, 500, 550, colors[BLACK], 20);
	DrawString(450, 740, "Highscores",colors[WHITE]);
	DrawRoundRect(410, 100, 200, 100, colors[LIME_GREEN], 20);
	int temp = 700;
	readHighScores();
	sortScores();
	cout << number_of_scores << endl;
	for (int i = 0; i < number_of_scores; i++) {
		DrawString(270, temp, high_names[i], colors[WHITE]);
		DrawString(680, temp, to_string(high_scores[i]), colors[WHITE]);
		temp -= 50;

	}
	
	DrawString(488, 140, "Back", colors[BLACK]);
	if ((mouseClickXPos >= 410 and mouseClickXPos <= 610) and (mouseClickYPos >= 680 and mouseClickYPos <= 776)) {
		glutDisplayFunc(Menu);
	}
	glutSwapBuffers();
	glutPostRedisplay();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*						Name: selectionScreen								 				   *
*						Type: Screen														   *
*         Purpose: Displays the options which allow the user to select their taxi color        *
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  */

void selectionScreen() {
	glClearColor(1.0f, 0.0f, 1.0f, 0.0f);
	glClear (GL_COLOR_BUFFER_BIT);
	DrawString(375, 500, "Select the color of your taxi: ", colors[BLACK]);
	DrawRoundRect(410,350,200,100,colors[LIME_GREEN],20);
	DrawString(475, 385, "Yellow", colors[BLACK]);
	DrawRoundRect(410,200,200,100,colors[LIME_GREEN],20);
	DrawString(490, 240, "Red", colors[BLACK]);
	DrawRoundRect(410,50,200,100,colors[LIME_GREEN],20);
	DrawString(470, 90, "Random", colors[BLACK]);
	if ((mouseClickXPos >= 417 and mouseClickXPos <= 603) and (mouseClickYPos >= 432 and mouseClickYPos <= 526)) {
		taxi_color = 'Y';
		glutDisplayFunc(inputNameScreen);

	}
	else if((mouseClickXPos >= 417 and mouseClickXPos <= 602) and (mouseClickYPos >= 582 and mouseClickYPos <= 677)) {
		taxi_color = 'R';
		glutDisplayFunc(inputNameScreen);
	}
	else if((mouseClickXPos >= 415 and mouseClickXPos <= 603) and (mouseClickYPos >= 733 and mouseClickYPos <= 828)) {
		srand(time(0));
		int color_rand = rand() % 2;
		if (color_rand == 1) {
			taxi_color = 'Y';
			glutDisplayFunc(inputNameScreen);
		}
		else if(color_rand == 0) {
			taxi_color = 'R';
			glutDisplayFunc(inputNameScreen);

		} 
	}
	glutSwapBuffers();
	glutPostRedisplay();

}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*						Name: inputNameScreen								 *
*						Type: Screen										 *
*           Purpose: Displays the screen for user to enter their name        *
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  */

void inputNameScreen() {
	inputtingName = true;
	glClearColor(1.0f, 0.0f, 1.0f, 0.0f);
	glClear (GL_COLOR_BUFFER_BIT);
	DrawString(420, 700, "Enter your name:", colors[BLACK]);
	DrawRoundRect(300, 600, 400, 75, colors[BLACK], 20);
	DrawString(320, 632, player_name, colors[WHITE]);
	DrawRoundRect(400, 450, 200, 100, colors[LIME_GREEN], 20);
	DrawString(480, 490, "Start", colors[BLACK]);
	if ((mouseClickXPos >= 411 and mouseClickXPos <= 595) and (mouseClickYPos >= 334 and mouseClickYPos <= 425)) {
		glutDisplayFunc(drawBoard);
		timer = 180;
		playing = true;
		file_written = false;
		inputtingName = false;
	}
	glutSwapBuffers();
	glutPostRedisplay();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*						Name: gameOverScreen								 *
*						Type: Drawing										 *
*          	Purpose: Displays the game over screen when the game ends        *
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  */

void gameOverScreen() {
	glClearColor(1.0f, 0.0f, 1.0f, 0.0f);
	glClear (GL_COLOR_BUFFER_BIT);
	if (score >= 100) {
		game_over_text = "You Win!";
	}
	else {
		game_over_text = "You Lost";
	}
	if (file_written == false) {
	 	writeHighScores();
		cout << "written" << endl;
	 	file_written = true;
	}
	DrawRoundRect(350, 440, 300, 150, colors[BLACK], 20);
	DrawString(450, 550, game_over_text, colors[WHITE]);
	DrawString(423, 460, "Your Score: " + to_string(score), colors[WHITE]);
	DrawRoundRect(415, 300, 180, 100, colors[LIME_GREEN], 20);
	DrawString(475, 340, "Menu", colors[BLACK]);

	if ((mouseClickXPos >= 428 and mouseClickXPos <= 588) and (mouseClickYPos >= 482 and mouseClickYPos <= 577)) {
		glutDisplayFunc(Menu);
		mouseClickXPos = 0;
		// if (sorted == false) {
		//  	sortScores();
		//  }

		high_scores[10] = score;
		high_names[10] = player_name;
		if (sorted == false) {
			sortScores();
		}
		mouseClickYPos = 0;
		score = 0;
		player_name = "";
		sorted = true;
	}
	glutSwapBuffers();
	glutPostRedisplay();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*						Name: fillBoard																	 *
*						Type: Logic											 							 *
*          Purpose: Fills the indexes of the board with buildings, obstacles, cars and passengers      	 *
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  */

void fillBoard() {
	cout << "board started" << endl;
	// Init with character E:
	for (int i = 0; i < 20; i++) {
		for(int j = 0; j < 20; j++) {
			board[i][j] = 'E';
		}
	}
	srand(time(0));
	// For Buildings:
	board[17][2] = 'B';
	board[17][3] = 'B';
	board[17][4] = 'B';
	for (int i = 9; i <= 19; i++) {
		board[17][i] = 'B';
	}
	for (int i = 13; i <= 16; i++) {
		board[i][7] = 'B';
	}
	board[13][5] = 'B';
	board[13][6] = 'B';
	board[12][9] = 'B';
	for (int i = 10; i <= 15; i++) {
		board[i][12] = 'B';
	}
	for (int i = 13; i <= 15; i++) {
		board[13][i] = 'B';
	}
	for (int i = 11; i <= 15; i++) {
		board[i][18] = 'B';
	}
	board[9][9] = 'B';
	board[10][9] = 'B';
	board[9][7] = 'B';
	board[9][8] = 'B';
	for (int i = 2; i <= 8; i++) {
		board[i][5] = 'B';
	}
	for (int i = 7; i <= 10; i++) {
		board[5][i] = 'B';
	}
	board[5][11] = 'B';
	board[6][11] = 'B';
	board[7][11] = 'B';

	board[0][9] = 'B';
	board[1][9] = 'B';
	board[2][9] = 'B';

	board[1][17] = 'B';
	board[1][18] = 'B';

	for (int i = 3; i <= 6; i++) {
		board[i][14] = 'B';
	}
	for (int i = 14; i <= 18; i++) {
		board[6][i] = 'B';
	}
	board[4][2] = 'B';
	board[4][3] = 'B';

	board[5][2] = 'B';
	board[5][3] = 'B';

	board[6][2] = 'B';
	board[6][3] = 'B';
	cout << "b done" << endl;
	// For Passengers:
	int pass_rand_one = rand() % 20;
	int pass_rand_two = rand() % 20;
	for(int i = 1; i <= 5; i++) {
			while(board[pass_rand_one][pass_rand_two] != 'E' or (pass_rand_one == 19 and pass_rand_two == 0)) {
				pass_rand_one = rand() % 20;
				pass_rand_two = rand() % 20;
			}
			board[pass_rand_one][pass_rand_two] = 'P';
			passengers_onscreen += 1;
	}

	cout << "passengers placed" << endl;

	// For Trees:
	int tree_rand = rand() % 20;
	int tree_rand_two = rand() % 20;
	for (int i = 1; i <= 4; i++) {

		while (board[tree_rand][tree_rand_two] != 'E' or (tree_rand == 19 and tree_rand_two == 0)) {
			tree_rand = rand() % 20;
			tree_rand_two = rand() % 20;
		}
		board[tree_rand][tree_rand_two] = 'T';
	}
	cout << "trees placed" << endl;
	// For Boxes:
	int box_rand_one = rand() % 20;
	int box_rand_two = rand() % 20;
	for (int i = 1; i <= 3; i++) {
		while (board[box_rand_one][box_rand_two] != 'E' or (box_rand_one == 19 and box_rand_two == 0)) 
		{
			box_rand_one = rand() % 20;
			box_rand_two = rand() % 20;
		}
		board[box_rand_one][box_rand_two] = 'X';
		
	}
	cout << "boxes placed" << endl;
	// for other cars:
	int other_cars_rand_one = rand() % 20;
	int other_cars_rand_two = rand() % 20;
	for (int i = 0; i < 5; i++) {
		while (board[other_cars_rand_one][other_cars_rand_two] != 'E' or (other_cars_rand_one == 19 and other_cars_rand_two == 0)) {
			other_cars_rand_one = rand() % 20;
			other_cars_rand_two = rand() % 20;
		}
		board[other_cars_rand_one][other_cars_rand_two] = 'C';
	}
	cout << "other cars placed" << endl;
	cout << "board filled" << endl;
 
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*						Name: readHighScores								 *
*						Type: FileHandling									 *
*    Purpose: Reads the high scores from scores file and stored in the array *
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  */

void readHighScores() {
	ofstream fout;
	fout.open("highscores.txt", ios::app);
	fout.close();

	ifstream fin;
	fin.open("highscores.txt");

	string num;

	if(fin){

		while(fin){

			getline(fin, high_names[number_of_scores], ',');

			high_scores[number_of_scores] = stoi(num);
			number_of_scores++;
		

		}
	}

	else{

		cout << "File could not be opened" << endl;

	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*						Name: writeHighScores								 *
*						Type: FileHandling									 *
*       	 Purpose: Writes the scores from array to the text file			 *
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  */

void writeHighScores() {
	if (!playing) {
		ofstream fout("highscores.txt");
		if (fout) {
			for (int i = 0; i < number_of_scores; i++) {
				fout << high_names[i];
				fout << ",";
				fout << high_scores[i];
				cout << "File written" << endl;
				fout.close();
			}

		}
		else {
			cout << "Error opening file" << endl;
		}
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*						Name: sortScores									 *
*						Type: Logic											 *
*      		  Purpose: Sorts the scores and names array in parallel 		 *
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  */

void sortScores() {
	int temp;
	string tmp;
	for (int i = 0; i < number_of_scores; i++) {
		for (int j = i + 1; j < number_of_scores; j++) {
			if (high_scores[i] > high_scores[j]) {
				temp = high_scores[i];
				high_scores[i] = high_scores[j];
				high_scores[j] = temp;

				tmp = high_names[i];
				high_names[i] = high_names[j];
				high_names[j] = tmp;
			}
		}
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*						Name: drawDestination								         		   *
*						Type: Drawing										 		 		   *
*  			Purpose: Draws the green destination on the board where board's index is 'D'       *
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  */

void drawDestination() {
		
		DrawRectangle(des_x * 50, des_y * 42, 50, 42, colors[GREEN]);
		glutPostRedisplay();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*						Name: drawBoxes								   			      		   *
*						Type: Drawing										 		 		   *
*  			Purpose: Draws the boxes on the board where board's index is 'X'   			       *
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  */

void drawBoxes() {
	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 20; j++) {
			if (board[i][j] == 'X') {
				DrawRectangle((j * one_grid_x), (i * one_grid_y), 50, 42, colors[SANDY_BROWN]);
				DrawRectangle(j * one_grid_x + 17, i * one_grid_y + 22, 20, 21, colors[SLATE_GRAY]);
			}
		}
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*						Name: drawTrees										         		   *
*						Type: Drawing										 		 		   *
*  			Purpose: Draws the trees on the board where board's index is 'T'     			   *
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  */

void drawTrees() {
	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 20; j++) {
			if(board[i][j] == 'T') {
				DrawRectangle((j * one_grid_x) + 25.5, (i * one_grid_y), 5, 20, colors[BROWN]);
				DrawCircle((j * one_grid_x) + 27, (i * one_grid_y) + 20, 10, colors[GREEN]);
			}
		}
	}
	glutPostRedisplay();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*						Name: drawOtherCars									         		   *
*						Type: Drawing										 		 		   *
*  			Purpose: Draws the npc cars on the board where board's index is 'C'    			   *
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  */

void drawOtherCars() {
	// int random_color = rand() % 139;
	for (int i = 0; i < 20; i++) {
		for(int j = 0; j < 20; j++) {
			// while(random_color == 23 or random_color == 73 or random_color == 5 or random_color == 1) {
			// 	random_color = rand() % 139;
			// }
			if (board[i][j] == 'C') {
				DrawRoundRect(j * one_grid_x + 10,i * one_grid_y + 5,25,20,colors[DARK_GOLDEN_ROD],15);
				DrawCircle(j * one_grid_x + 10,i * one_grid_y + 5,5,colors[BLACK]);
				DrawCircle(j * one_grid_x + 35,i * one_grid_y + 5,5,colors[BLACK]);
			}
			// random_color = rand() % 139;
		}
	}
	
	glutPostRedisplay();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*						Name: drawCar										         		   *
*						Type: Drawing										 		 		   *
*  			Purpose: Draws the player taxi on the board   							   		   *
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  */

void drawCar() {
	if (taxi_color == 'Y') {
		// BASE OF CAR
		DrawSquare(xI, yI, 7, colors[YELLOW]);
		DrawSquare(xI+6, yI, 7, colors[YELLOW]);
		DrawSquare(xI+12, yI, 7, colors[YELLOW]);
		DrawSquare(xI+18, yI, 7, colors[YELLOW]);
		DrawSquare(xI+24, yI, 7, colors[YELLOW]);
		DrawSquare(xI+30, yI, 7, colors[YELLOW]);
		DrawSquare(xI+36, yI, 7, colors[YELLOW]);
		DrawSquare(xI+42, yI, 7, colors[YELLOW]);
		DrawTriangle(xI+2, yI, xI+20, yI+16, xI+42, yI, colors[YELLOW]);
		
		//BODY
		DrawSquare(xI+30, yI+5, 6, colors[YELLOW]);
		DrawSquare(xI+20, yI+5, 6, colors[YELLOW]);
		DrawSquare(xI+25, yI+5, 6, colors[YELLOW]);
		
	
		//SECOND LEVEL
		DrawSquare(xI+20, yI+10, 6, colors[YELLOW]);
		DrawSquare(xI+25, yI+10, 6, colors[YELLOW]);
	
		// Wheels
	
		DrawCircle(xI+10 , yI , 5 ,colors[BLACK]);
		DrawCircle(xI+35 , yI , 5 ,colors[BLACK]);
	}

	else if(taxi_color == 'R') {
		// BASE OF CAR
		DrawSquare(xI, yI, 7, colors[RED]);
		DrawSquare(xI+6, yI, 7, colors[RED]);
		DrawSquare(xI+12, yI, 7, colors[RED]);
		DrawSquare(xI+18, yI, 7, colors[RED]);
		DrawSquare(xI+24, yI, 7, colors[RED]);
		DrawSquare(xI+30, yI, 7, colors[RED]);
		DrawSquare(xI+36, yI, 7, colors[RED]);
		DrawSquare(xI+42, yI, 7, colors[RED]);
		DrawTriangle(xI+2, yI, xI+20, yI+16, xI+42, yI, colors[RED]);

		//BODY
		DrawSquare(xI+30, yI+5, 6, colors[RED]);
		DrawSquare(xI+20, yI+5, 6, colors[RED]);
		DrawSquare(xI+25, yI+5, 6, colors[RED]);
		
	
		//SECOND LEVEL
		DrawSquare(xI+20, yI+10, 6, colors[RED]);
		DrawSquare(xI+25, yI+10, 6, colors[RED]);
	
		// Wheels	
		DrawCircle(xI+10 , yI , 5 ,colors[BLACK]);
		DrawCircle(xI+35 , yI , 5 ,colors[BLACK]);

	}
	glutPostRedisplay();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*						Name: drawBuildings									         		   *
*						Type: Drawing										 		 		   *
*  			Purpose: Draws the buildings on the board where board's index is 'B'    		   *
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  */

void drawBuildings() {
	for (int i = 0; i <= 19; i++) {
		for(int j = 0; j <= 19; j++) {
			if (board[i][j] == 'B') {
				DrawRectangle(j * one_grid_x, i * one_grid_y, one_grid_x, one_grid_y, colors[BLACK]);
			}

		}
	}
	glutPostRedisplay();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*						Name: drawPassengers								         		   *
*						Type: Drawing										 		 		   *
*  			Purpose: Draws the passengers on the board where board's index is 'P'     		   *
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  */

void drawPassengers() {
	for(int i = 0; i < 20; i++) {
		for(int j = 0; j < 20; j++) {
			if(board[i][j] == 'P') {
				// head:
				DrawCircle((j * one_grid_x) + 25.5, (i * one_grid_y) + 30, 5, colors[BLACK]);

				// body:
				DrawLine((j * one_grid_x) + 26, (i * one_grid_y) + 10, (j * one_grid_x) + 26, (i * one_grid_y) + 26, 3, colors[BLACK]);

				// legs:
				DrawLine((j * one_grid_x) + 15, (i * one_grid_y), (j * one_grid_x) + 26, (i * one_grid_y) + 10, 3, colors[BLACK]);
				DrawLine((j * one_grid_x) + 35, (i * one_grid_y), (j * one_grid_x) + 26, (i * one_grid_y) + 10, 3, colors[BLACK]);

				// arms:
				DrawLine((j * one_grid_x) + 14, (i * one_grid_y) + 12, (j * one_grid_x) + 26, (i * one_grid_y) + 18, 3, colors[BLACK]);
				DrawLine((j * one_grid_x) + 36, (i * one_grid_y) + 12, (j * one_grid_x) + 26, (i * one_grid_y) + 18, 3, colors[BLACK]);

			}
		}
	}
		
	glutPostRedisplay();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*						Name: drawTrees										         		   *
*						Type: Drawing										 		 		   *
*  			Purpose: Draws the grid and uses other drawing functions to make the game's map    *
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  */

void drawBoard() {

	glClearColor( 1.0f, 1.0f, 1.0f, 1.0f);
	glClear (GL_COLOR_BUFFER_BIT);
	DrawString(10, 852, "Score = " + to_string(score), colors[BLACK]);
	DrawString(830, 852, "Time Left: " + to_string(timer) + "s", colors[BLACK]);
	for(int i = 0; i < 20; i++) {
		for (int j = 0; j < 20; j++) {
			DrawRectangle(i * one_grid_x, j * one_grid_y, one_grid_x, one_grid_y, colors[WHITE]); // change values according to screen
			// width and height.
		}
	}
	drawBuildings();
	drawPassengers();
	drawTrees();
	drawBoxes();
	drawOtherCars();
	
	if(pickedUp) {
		drawDestination();
	}
	drawCar();
	if (!playing) {
		mouseClickYPos = 0;
		mouseClickXPos = 0;

		glutDisplayFunc(gameOverScreen);
		
	}
	glutSwapBuffers();
}


/*This function is called (automatically) whenever any non-printable key (such as up-arrow, down-arraw)
 * is pressed from the keyboard
 *
 * You will have to add the necessary code here when the arrow keys are pressed or any other key is pressed...
 *
 * This function has three argument variable key contains the ASCII of the key pressed, while x and y tells the
 * program coordinates of mouse pointer when key was pressed.
 *
 * */

void NonPrintableKeys(int key, int x, int y) {
	if(playing) {
		if (taxi_color == 'R') {
			x_speed = 25; 
			y_speed = 21;
		}
		else {
			x_speed = 50;
			y_speed = 42;
		}
		// x_speed = 50;
		// y_speed = 42;
		// Checking Collision logic with diff objects and handling score accordingly:
		if (key == GLUT_KEY_RIGHT) { 
				
				xI += x_speed;
				if (xI >= 1000) {
					xI -= x_speed;
				}
				if (board[yI / 42][xI / 50] == 'B' or board[yI / 42][xI / 50] == 'T' or board[yI / 42][xI / 50] == 'X' or board[yI / 42][xI / 50] == 'P' or board[yI / 42][xI / 50] == 'C') {
					
					if ((board[yI / 42] [xI / 50] == 'T' or board[yI / 42][xI / 50] == 'X') and taxi_color == 'Y') {
						score -= 4;
					}
					else if((board[yI / 42] [xI / 50] == 'T' or board[yI / 42][xI / 50] == 'X') and taxi_color == 'R') {
						score -= 2;
					}
					else if(board[yI / 42][xI / 50] == 'P') {
						score -= 5;
					}
					else if (board[yI / 42][xI / 50] == 'C' and taxi_color == 'R') {
						score -= 3;
					}
					else if (board[yI / 42][xI / 50] == 'C' and taxi_color == 'Y') {
						score -= 2;
					}

					xI -= (x_speed);
				}

			}
			else if (key == GLUT_KEY_LEFT) { 

				xI -= x_speed;
				if (xI < 0) {
					xI += x_speed;
				}
				if (board[yI / 42][xI / 50] == 'B' or board[yI / 42][xI / 50] == 'T' or board[yI / 42][xI / 50] == 'X' or board[yI / 42][xI / 50] == 'P' or board[yI / 42][xI / 50] == 'C') {
					
					if ((board[yI / 42] [xI / 50] == 'T' or board[yI / 42][xI / 50] == 'X') and taxi_color == 'Y') {
						score -= 4;
					}
					else if((board[yI / 42] [xI / 50] == 'T' or board[yI / 42][xI / 50] == 'X') and taxi_color == 'R') {
						score -= 2;
					}
					else if(board[yI / 42][xI / 50] == 'P') {
						score -= 5;
					}
					else if (board[yI / 42][xI / 50] == 'C' and taxi_color == 'R') {
						score -= 3;
					}
					else if (board[yI / 42][xI / 50] == 'C' and taxi_color == 'Y') {
						score -= 2;
					}
					xI += x_speed;
				}

			}
			else if (key == GLUT_KEY_DOWN) { 

				yI -= y_speed;
				if (yI < 0) {
					yI += y_speed;
				}
				if (board[yI / 42][xI / 50] == 'B' or board[yI / 42][xI / 50] == 'T' or board[yI / 42][xI / 50] == 'X' or board[yI / 42][xI / 50] == 'P' or board[yI / 42][xI / 50] == 'C') {
					
					if ((board[yI / 42] [xI / 50] == 'T' or board[yI / 42][xI / 50] == 'X') and taxi_color == 'Y') {
						score -= 4;
					}
					else if((board[yI / 42] [xI / 50] == 'T' or board[yI / 42][xI / 50] == 'X') and taxi_color == 'R') {
						score -= 2;
					}
					else if(board[yI / 42][xI / 50] == 'P') {
						score -= 5;
					}
					else if (board[yI / 42][xI / 50] == 'C' and taxi_color == 'R') {
						score -= 3;
					}
					else if (board[yI / 42][xI / 50] == 'C' and taxi_color == 'Y') {
						score -= 2;
					}
					yI += y_speed;
				}

			}
			else if (key == GLUT_KEY_UP) { 

				yI += y_speed;
				if (yI > 798) {
					yI -= y_speed;
				}

				if (board[yI / 42][xI / 50] == 'B' or board[yI / 42][xI / 50] == 'T' or board[yI / 42][xI / 50] == 'X' or board[yI / 42][xI / 50] == 'P' or board[yI / 42][xI / 50] == 'C') {
					
					if ((board[yI / 42] [xI / 50] == 'T' or board[yI / 42][xI / 50] == 'X') and taxi_color == 'Y') {
						score -= 4;
					}
					else if((board[yI / 42] [xI / 50] == 'T' or board[yI / 42][xI / 50] == 'X') and taxi_color == 'R') {
						score -= 2;
					}
					else if(board[yI / 42][xI / 50] == 'P') {
						score -= 5;
					}
					else if (board[yI / 42][xI / 50] == 'C' and taxi_color == 'R') {
						score -= 3;
					}
					else if (board[yI / 42][xI / 50] == 'C' and taxi_color == 'Y') {
						score -= 2;
					}
					yI -= y_speed;
				}

			}
		// Setting the board index to 'A' to signify our taxi is present there with each movement.
		// General Movement on empty spaces:
			else if (key == GLUT_KEY_LEFT) {/*GLUT_KEY_LEFT is constant and contains ASCII for left arrow key*/
				// what to do when left key is pressed...
				xI -= x_speed;
				board[yI / 42][xI / 50] = 'A';
			} 
			else if (key == GLUT_KEY_RIGHT) { // GLUT_KEY_RIGHT is constant and contains ASCII for right arrow key 
				xI += x_speed;
				board[yI / 42][xI / 50] = 'A';
			} 
			else if (key == GLUT_KEY_UP/*GLUT_KEY_UP is constant and contains ASCII for up arrow key*/) {
				yI += y_speed;
				board[yI / 42][xI / 50] = 'A';
			}
			else if (key == GLUT_KEY_DOWN/*GLUT_KEY_DOWN is constant and contains ASCII for down arrow key*/) {
				yI -= y_speed;
				board[yI / 42][xI / 50] = 'A';
			}
			// Checking overtaking:
			if ((key == GLUT_KEY_RIGHT or key == GLUT_KEY_LEFT) and (board[(yI / 42) + 1][xI / 50] == 'C' or board[(yI / 42) - 1][xI / 50] == 'C')) {
				score += 1;
			}
			else if((key == GLUT_KEY_UP or key == GLUT_KEY_DOWN) and (board[(yI / 42)][(xI / 50) + 1] == 'C' or board[(yI / 42)][(xI / 50) - 1] == 'C')) {
				score += 1;
			}
			

		/* This function calls the Display function to redo the drawing. Whenever you need to redraw just call
		* this function*/

			glutPostRedisplay();
	}

}

/*This function is called (automatically) whenever any printable key (such as x,b, enter, etc.)
 * is pressed from the keyboard
 * This function has three argument variable key contains the ASCII of the key pressed, while x and y tells the
 * program coordinates of mouse pointer when key was pressed.
 * */
void PrintableKeys(unsigned char key, int x, int y) {
	if (key == 27/* Escape key ASCII*/) {
		exit(1); // exit the program when escape key is pressed.
	}
	// Check to make sure no segmentation fault when inputting name: 
	if (inputtingName and key == 8 and player_name == "") {
		5 + 4;
	}
	else if (inputtingName and (isalpha(key) or key == ' ')) {
		player_name += char(key);
		//cout << char(key) << endl;
	}
	else if(inputtingName and key == 8) {
		player_name.pop_back();
	}

	if (key == ' ' and board[(yI / 42) + 1][(xI / 50)] == 'P' and pickedUp == false) {
		pickedUp = true;

		srand(time(0));
		des_x = rand() % 19;
		des_y = rand() % 19;
		passengers_onscreen--;
		if (passengers_onscreen < 3) {
			int num_two = rand() % 20;
			int num_three = rand() % 20;
			while(board[num_two][num_three] != 'E') {
				num_two = rand() % 20;
				num_three = rand() % 20;
			}
			board[num_two][num_three] = 'P';
			passengers_onscreen += 1;
			drawPassengers();
		}

		while(board[des_y][des_x] != 'E') {
			des_y = rand() % 19;
			des_x = rand() % 19;
		}
		board[(yI / 42) + 1][xI / 50] = 'E';
		drawPassengers();
		board[des_y][des_x] = 'D';
		// cout << "picked up passenger" << endl;
	}
	else if (key == ' ' and board[(yI / 42) - 1][(xI / 50)] == 'P' and pickedUp == false) {
		pickedUp = true;

		srand(time(0));
		des_x = rand() % 19;
		des_y = rand() % 19;
		passengers_onscreen--;
		if (passengers_onscreen < 3) {
			int num_two = rand() % 20;
			int num_three = rand() % 20;
			while(board[num_two][num_three] != 'E') {
				
				num_two = rand() % 20;
				num_three = rand() % 20;
			}
			board[num_two][num_three] = 'P';
			passengers_onscreen += 1;
			drawPassengers();
		}

		while(board[des_y][des_x] != 'E') {
			des_y = rand() % 19;
			des_x = rand() % 19;
		}
		board[(yI / 42) - 1][xI / 50] = 'E';
		drawPassengers();
		board[des_y][des_x] = 'D';
		// cout << "picked up passenger" << endl;
	}
	else if (key == ' ' and board[(yI / 42)][(xI / 50) + 1] == 'P' and pickedUp == false) {
		pickedUp = true;

		srand(time(0));
		des_x = rand() % 19;
		des_y = rand() % 19;
		passengers_onscreen--;
		if (passengers_onscreen < 3) {
			int num_two = rand() % 20;
			int num_three = rand() % 20;
			while(board[num_two][num_three] != 'E') {
				num_two = rand() % 20;
				num_three = rand() % 20;
			}
			board[num_two][num_three] = 'P';
			passengers_onscreen += 1;
			drawPassengers();
		}

		while(board[des_y][des_x] != 'E') {
			des_y = rand() % 19;
			des_x = rand() % 19;
		}
		board[(yI / 42)][(xI / 50) + 1] = 'E';
		drawPassengers();
		board[des_y][des_x] = 'D';
		// cout << "picked up passenger" << endl;
	}
	else if (key == ' ' and board[(yI / 42)][(xI / 50) - 1] == 'P' and pickedUp == false) {
		pickedUp = true;

		srand(time(0));
		des_x = rand() % 19;
		des_y = rand() % 19;
		passengers_onscreen--;
		if (passengers_onscreen < 3) {
			int num_two = rand() % 20;
			int num_three = rand() % 20;
			while(board[num_two][num_three] != 'E') {
				num_two = rand() % 20;
				num_three = rand() % 20;
			}
			board[num_two][num_three] = 'P';
			passengers_onscreen += 1;
			drawPassengers();
		}

		while(board[des_y][des_x] != 'E') {
			des_y = rand() % 19;
			des_x = rand() % 19;
		}
		board[(yI / 42)][(xI / 50) - 1] = 'E';
		drawPassengers();
		board[des_y][des_x] = 'D';
		// cout << "picked up passenger" << endl;
	}
	else if (key == ' ' and board[yI / 42][xI / 50] == 'D') {
		pickedUp = false;
		passengers_dropped++;
		int rand_x = rand() % 20;
		int rand_y = rand() % 20;

		if (passengers_dropped % 2 == 0) {
			while(board[rand_x][rand_y] != 'E') {
				rand_x = rand() % 20;
				rand_y = rand() % 20;
			}
			board[rand_x][rand_y] = 'C';
		}

		score += 10;
		board[des_y][des_x] == 'E';
	}
	glutPostRedisplay();
}


/*
 * This function is called after every 1000.0/FPS milliseconds
 * (FPS is defined on in the beginning).
 * You can use this function to animate objects and control the
 * speed of different moving objects by varying the constant FPS.
 *
 * */
int movement_decider;
void Timer(int m) {
	tracking_time += 100;
	if (playing) {
		movement_decider = rand() % 2;
		if (tracking_time % 1000 == 0) {
			timer -= 1;
			if (movement_decider == 0) {
				moveOtherCarsX();
			}
			else {
				moveOtherCarsY();
			}
			
		}
	}
	if (timer == 0) {
		playing = false;
	}
	// implement your functionality here
	// moveCar();

	// once again we tell the library to call our Timer function after next 1000/FPS
	glutTimerFunc(100, Timer, 0);
}

/*This function is called (automatically) whenever your mouse moves witin inside the game window
 *
 * You will have to add the necessary code here for finding the direction of shooting
 *
 * This function has two arguments: x & y that tells the coordinate of current position of move mouse
 *
 * */
void MousePressedAndMoved(int x, int y) {
	// cout << x << " " << y << endl;
	glutPostRedisplay();
}
void MouseMoved(int x, int y) {
	//cout << x << " " << y << endl;
	glutPostRedisplay();
}

/*This function is called (automatically) whenever your mouse button is clicked witin inside the game window
 *
 * You will have to add the necessary code here for shooting, etc.
 *
 * This function has four arguments: button (Left, Middle or Right), state (button is pressed or released),
 * x & y that tells the coordinate of current position of move mouse
 *
 * */
void MouseClicked(int button, int state, int x, int y) {
	// 	DrawRoundRect(410,500,200,100,colors[LIME_GREEN],20);

	if (button == GLUT_LEFT_BUTTON) {
		mouseClickXPos = x;
		mouseClickYPos = y;
		
		cout << x << " " << y << endl;
		// cout << GLUT_DOWN << " " << GLUT_UP << endl;
	}
		
		

	else if (button == GLUT_RIGHT_BUTTON) // dealing with right button
			{
			// cout<<"Right Button Pressed"<<endl;

	}
	glutPostRedisplay();
}
/*
 * our gateway main function
 * */
int main(int argc, char*argv[]) {

	int width = 1000, height = 880; // i have set my window size to be 800 x 600
	
	InitRandomizer(); // seed the random number generator...
	glutInit(&argc, argv); // initialize the graphics library...
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // we will be using color display mode
	glutInitWindowPosition(50, 50); // set the initial position of our window
	glutInitWindowSize(width, height); // set the size of our window
	glutCreateWindow("Rush Hour by Muhammad Ibrahim Basit"); // set the title of our game window
	SetCanvasSize(width, height); // set the number of pixels...
	fillBoard();
	// Register your functions to the library,
	// you are telling the library names of function to call for different tasks.
	//glutDisplayFunc(display); // tell library which function to call for drawing Canvas.

	glutDisplayFunc(Menu); // tell library which function to call for drawing Canvas.
	glutSpecialFunc(NonPrintableKeys); // tell library which function to call for non-printable ASCII characters
	glutKeyboardFunc(PrintableKeys);
	 // tell library which function to call for printable ASCII characters
	// This function tells the library to call our Timer function after 1000.0/FPS milliseconds...
	glutTimerFunc(1000.0, Timer, 0);

	glutMouseFunc(MouseClicked);
	glutPassiveMotionFunc(MouseMoved); // Mouse
	glutMotionFunc(MousePressedAndMoved); // Mouse

	// now handle the control to library and it will call our registered functions when
	// it deems necessary...
	glutMainLoop();
	return 1;
}
#endif /* RushHour_CPP_ */