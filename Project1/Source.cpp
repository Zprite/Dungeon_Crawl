#include <iostream>
#include <string>
#include <windows.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include "Battle.h"
#include "common.h"
#define width 10
#define height 10
#define eCount 5
#define iCount 5
#define max_items 9
using namespace std;

int item_list[max_items];
int cItem = 0;
void draw_game();
void game();
void player();
void set_items();
int enemy();
void win();
void game_over();
void spawn_enemy();
void spawn_item();
void create_item();
int check_collision();
int check_collision_enemy();
void move_right();
void move_left();
void move_up();
void move_down();
void move_enemy_right();
void move_enemy_left();
void move_enemy_up();
void move_enemy_down();
char game_board[height][width];
int eRemove[eCount];
int remove_count = 0;
int direction;
int spawn_point_x[2+eCount+iCount]; // Store spawnpoint for Player, all enemies, all items, and exitpoint.
int spawn_point_y[2+eCount+iCount];
int prev_x[eCount+1]; // Stores all previous x values of player and all enemies
int prev_y[eCount+1]; // Stores all previous y values of player and all enemies
int collided = 0;
int counter;
int move_to_player = 0;

struct items {
	string name;
	int nItem;
} heal, power;

int main() {
	game();
	return 0;
}

void game() {

	srand(time(0));
	//Fills in the game board with dashes
	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			game_board[j][i] = '-';
		}
	}
	spawn_point_x[0] = 0; // Spawn player in top left corner
	spawn_point_y[0] = 0;
	spawn_point_x[1] = width - 1; //Spawn exit in bottom right corner
	spawn_point_y[1] = height - 1;
	spawn_enemy();
	spawn_item();
	game_board[height-1][width-1] = 'X'; //Spawn exit in bottom right corner
	game_board[0][0] = 'P'; //Spawn player in top left 
	prev_x[0] = 0; // prev(x/y) [0] is the previous/current position of player, the other indexes are for the enemy positions
	prev_y[0] = 0;

	do {
		system("CLS");
		draw_game();//Draw game board
		player();
		system("CLS");
		draw_game();//Draw game board
		enemy();

	} while (collided == 0); // When collided is 1, player has won the game.
}

void spawn_enemy() {
	int cSpawn = 0;
	// Generates  enemies in random locations
	//2-11
	for (int i = 2; i < eCount + 2; i++) {
		bool check;
		int x;
		int y;
		do {
			x = rand() % width;
			y = rand() % height;
			//check if number is already used:
			check = 1;
			for (int j = 0; j < i; j++)
				if (x == spawn_point_x[j] && y == spawn_point_y[j]) //if number is already used
				{
					check = 0; //set check to false
					break;
				}
		} while (check==0); //loop until new, unique number is found

		game_board[y][x] = 'E';
		prev_x[i - 1] = x; //Each enemy's X and Y position is stored in array
		prev_y[i - 1] = y;
		spawn_point_x[i] = x;//store the generated number in the array
		spawn_point_y[i] = y;
	}
}

void spawn_item() {
	int cSpawn = 0;
	//12-21
	for (int i = eCount+2; i < 2+eCount+iCount; i++) {
		bool check;
		int x;
		int y;
		do {
			x = rand() % width;
			y = rand() % height;
			//check if number is already used:
			check = 1;
			for (int j = 0; j < i; j++)
				if (x == spawn_point_x[j] && y == spawn_point_y[j]) //if number is already used
				{
					check = 0; //set check to false
					break;
				}
		} while (check==0); //loop until new, unique number is found
		game_board[y][x] = 'I';
		spawn_point_x[i] = x;//store the generated number in the array
		spawn_point_y[i] = y;
	}
}

int enemy() {
	// bug occurs when i=0 eRemove = 3, i=1 eRemove =2|
	if (remove_count >= eCount) {
		return 0;
	}
	for (counter = 1; counter <= eCount; counter++) {
		int skip = 0;
		move_to_player = 0;
		if (remove_count >= eCount) {
			return 0;
		}
		// Ignore enemy if it is defeated
		for (int i = 0; i < eCount; i++) {
			
			if (counter == eRemove[i]) {
				skip = 1;
			} 
			if (counter > eCount) {
				return 0;
			}
		}
		if (collided == 1) {
			break;
		}

		if (skip != 1) {
			direction = rand() % 4 + 1;
			switch (direction) {
			case 1:
				move_enemy_right();
				break;
			case 2:
				move_enemy_left();
				break;
			case 3:
				move_enemy_up();
				break;
			case 4:
				move_enemy_down();
				break;
			}
		}
	}

}

void move_enemy_right() {
	int chk = check_collision_enemy();
	if (prev_x[counter] < width-1 && chk == 0) {
		game_board[prev_y[counter]][prev_x[counter]] = '-';
		prev_x[counter] += 1;
		game_board[prev_y[counter]][prev_x[counter]] = 'E';
		if (move_to_player == 1) {
			Sleep(500);
			game_over();
			game_board[prev_y[counter]][prev_x[counter]] = 'P';
			eRemove[remove_count] = counter;  // eRemove adds an exception in check collision_enemy to not check for the specifed enemy
			remove_count++; // Remove count stores the value of each removed enemy;
			
		}
	}
}

void move_enemy_left() {

	int chk = check_collision_enemy();
	if (prev_x[counter]> 0 && chk == 0) {
		game_board[prev_y[counter]][prev_x[counter]] = '-';
		prev_x[counter] -= 1;
		game_board[prev_y[counter]][prev_x[counter]] = 'E';
		if (move_to_player == 1) {
			Sleep(500);
			game_over();
			game_board[prev_y[counter]][prev_x[counter]] = 'P';
			eRemove[remove_count] = counter;
			remove_count++;
		}
	}
}

void move_enemy_up() {

	int chk = check_collision_enemy();
	if (prev_y[counter]> 0 && chk == 0) {
		game_board[prev_y[counter]][prev_x[counter]] = '-';
		prev_y[counter] -= 1;
		game_board[prev_y[counter]][prev_x[counter]] = 'E';
		if (move_to_player == 1) {
			Sleep(500);
			game_over();
			game_board[prev_y[counter]][prev_x[counter]] = 'P';
			eRemove[remove_count] = counter;
			remove_count++;
			
		}
	}
}

void move_enemy_down() {
	int chk = check_collision_enemy();
	if (prev_y[counter] < height-1 && chk == 0) {
		game_board[prev_y[counter]][prev_x[counter]] = '-';
		prev_y[counter] += 1;
		game_board[prev_y[counter]][prev_x[counter]] = 'E';
		if (move_to_player == 1) {
			Sleep(500);
			game_over();
			game_board[prev_y[counter]][prev_x[counter]] = 'P';
			eRemove[remove_count] = counter; 
			remove_count++;
		}
	}
}

// Checks if next position is an invalid target
int check_collision_enemy() {

	char mark[3] = { 'E','X','I' };
	char collide_element = 'P';
		switch (direction) {
			//Move right
		case 1:
			for (int i = 0; i <= 2; i++) {
				if (game_board[prev_y[counter]][prev_x[counter] + 1] == mark[i]) {
					return -1;
				}
			}
			if (game_board[prev_y[counter]][prev_x[counter] + 1] == collide_element) {
				move_to_player = 1;
				return 0;
			}
			else {
				return 0;
			}
			//Move left
		case 2:
			for (int i = 0; i <= 2; i++) {
				if (game_board[prev_y[counter]][prev_x[counter] - 1] == mark[i]) {
					return -1;
				}
			}
			if (game_board[prev_y[counter]][prev_x[counter] - 1] == collide_element) {
				move_to_player = 1;
				return 0;
			}
			else {
				return 0;
			}
		case 3:
			for (int i = 0; i <= 2; i++) {
				if (game_board[prev_y[counter] - 1][prev_x[counter]] == mark[i]) {
					return -1;
				}
			}
			if (game_board[prev_y[counter] - 1][prev_x[counter]] == collide_element) {
				move_to_player = 1;
				return 0;
			}
			else {
				return 0;
			}
		case 4:
			for (int i = 0; i <= 2; i++) {
				if (game_board[prev_y[counter] + 1][prev_x[counter]] == mark[i]) {
					return -1;
				}
			}
			if (game_board[prev_y[counter] + 1][prev_x[counter]] == collide_element) {
				move_to_player = 1;
				return 0;
			}
			else {
				return 0;
			}
		}
}

void player() {
	
	int true_move;
	do {
		true_move = 1;
		char ch;
		ch = _getch();
		switch (ch) {
		case 'd':
		case 'D':
			move_right();
			break;
		case'w':
		case'W':
			move_up();
			break;
		case 'a':
		case 'A':
			move_left();
			break;
		case's':
		case'S':
			move_down();
			break;
		default:
			//If input is invalid
			true_move = 0;
		}
	} while (true_move == 0);
}
// Check collision for player before moving
int check_collision() {
	if (game_board[prev_y[0]][prev_x[0]] == 'E') {
		game_over();
		// Removes enemy after encounter
		for (int i = 1; i <= eCount; i++) {
			if (prev_y[i] == prev_y[0] && prev_x[i] == prev_x[0]) {
				eRemove[remove_count] = i;
				remove_count++;
				break;
			}
		}
		game_board[prev_y[0]][prev_x[0]] = 'P';
		return 0;
	}
	else if (game_board[prev_y[0]][prev_x[0]] == 'X') {
		win();
		return 1;
	}
	else if (game_board[prev_y[0]][prev_x[0]] == 'I') {
		create_item();
		game_board[prev_y[0]][prev_x[0]] = 'P';
		return 0;
	}
	else{
		game_board[prev_y[0]][prev_x[0]] = 'P';
		return 0;
	}
}

void move_right() {
	if (prev_x[0] < width-1) {
		game_board[prev_y[0]][prev_x[0]] = '-';
		prev_x[0] += 1;
		collided = check_collision();
	}
}

void move_left() {
	if (prev_x[0]>0) {
		game_board[prev_y[0]][prev_x[0]] = '-';
		prev_x[0] -= 1;
		collided = check_collision();
	}
}

void move_up() {
	if (prev_y[0]>0) {
		game_board[prev_y[0]][prev_x[0]] = '-';
		prev_y[0] -= 1;
		collided = check_collision();
	}
}

void move_down() {
	if (prev_y[0] < height-1) {
		game_board[prev_y[0]][prev_x[0]] = '-';
		prev_y[0] += 1;
		collided = check_collision();
	}
}
void game_over() {
	system("CLS");
	draw_game();
	cout << "ENEMY ENCOUNTER!!" << endl
		<< "Prepare for battle!" << endl;
	Sleep(500);
	Battle();
}

void win() {
	system("CLS");
	cout << "YOU WIN!!" << endl;
	Sleep(1000);
}

void draw_game() {
	int draw_column = 0;
	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			cout << " " << game_board[j][i] << " ";
		}
		cout << "\n";
	}
	//Print player stats
	cout << "\n\nHP: " << curr_hp << "/" << hp << "   ATK: " << base_atk <<"\n\n"
		 << "Lvl: " << lvl << "  XP: " << xp << "/" << xp_ceil << "\n";
}

inline void set_items(){
	
	heal.name = "Healing potion";
	heal.nItem = 1;
	power.name = "Attack+ potion";
	power.nItem = 2;

}

void create_item() {

	set_items();
	// if there is less than 10 items in array item_list[9], add random item.
	if (cItem <= 9) {
		int iSpawn = rand() % 2;
		switch (iSpawn) {
		case 0:
			item_list[cItem] = heal.nItem;
			cout << "You got a " << heal.name << "\n";
			Sleep(1000);
			cItem++;
			break;
		case 1:
			item_list[cItem] = power.nItem;
			cout << "You got an " << power.name << "\n";
			Sleep(1000);
			cItem++;
			break;
		}
	}
}
