#include "Battle.h"
#include <iostream>
#include <Windows.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <string>
#include <math.h>
#include "common.h"

using namespace std;


bool lvl_up;
int lvl_count = 2;
int lvl = 1;
int xp = 0;
int xp_ceil = 1000;
int xp_gain;
int hp = 200;
int curr_hp = hp;
int base_atk = 20;
int atk_buff;
double dex = 1.1;
int eHp;
int curr_eHp;
int base_eAtk;
double eDex;
int eSpawn;
int ePrint;
bool end_battle = 0;
char cursor[9] = { '>'};
int pos_cursor = 0;
int Battle_system();
int select_move();
int item_cursor();
void reset_cursor();
int cursormove(char mvcrsr, int max_val, bool trumov);
void list_items();
int hp_potion(int max_hpin);
int atk_potion(int atk_in);
int check_win(); 
int win = 0;
void ui();
int Attack();
void Flee();
int escape;
int eAttack();
void enemy1();
void enemy2();
void defeat();
void victory_screen();
void lvl_scaler();
int get_xp();
string eyes1;
string eyes2;

Battle::Battle()
{
	
	escape = 0;
	lvl_up = 0;
	win = 0;
	atk_buff = 0;
	eyes1 = "|     O        O     |";
	eyes2 = "|          00       00          |";
	srand(time(0));
	eSpawn = rand() % 2 + 1;
	Battle_system();
	int game_over = check_win();
	if (game_over == -1) {
	}
}

void lvl_scaler() {
	if (lvl > 1 && lvl_count == lvl) {
		hp += lvl * 15;
		hp = round(hp);
		curr_hp = hp;
		double newatk = lvl * 1.1;
		base_atk += round(newatk);
		lvl_count++;
	}
}

int get_xp() {
	xp_gain = 500;
	xp += xp_gain;
	if (xp >= xp_ceil) {
		lvl += 1;
		xp = xp_ceil - xp;
		xp_ceil *= 1.5;
		xp_ceil = round(xp_ceil);
		return 1;
	}
return 0;
}

int check_win() {
	if (curr_hp <= 0) {
		return -1;
	}
	else if (curr_eHp <= 0) {
		return 1;
	}
	else if (escape == 1) {
		return 2;
	}
	else {
		return 0;
	}
}

int Battle_system(){

	int selected = 0;
	int i = 1;

	//Update current enemy HP
	switch (eSpawn) {
	case 1:
		enemy1();
		curr_eHp = eHp;
		break;
	case 2:
		enemy2();
		curr_eHp = eHp;
		break;
	}
	do{
		do {
			ui();
			selected = select_move();
		} while (selected == 0);
		  win = check_win();
		if (win == 1) {		  // If enemy hp =0
			lvl_up = get_xp();
			victory_screen();
			lvl_scaler();
			return 0;
		} 
		else if (win == -1) { // If player hp =0
			defeat();
			return 0;
		}
		else if (win == 2) {
			system("CLS");
			cout << "ESCAPED FROM BATTLE!";
			Sleep(1000);
			return 0;
		}
		ui();
		eAttack();
		ui();
		win = check_win();
		if (win == -1) {
			defeat();
			return 0;
		}
	} while (i==1);
}

void ui() {

	system("CLS");
	//Print different enemy design
	switch (eSpawn) {
	case 1:
		enemy1();
		break;
	case 2:
		enemy2();
		break;
	}
	cout << "\n\n Enemy HP: " << curr_eHp << "\n\n";
	// If battle is won, life bars and move selection menu will not be shown.
	if (win != 1) {
	//Print enmy life bar
	if (curr_eHp > 0) {
		cout << "|";
	}
	for (int i = 1; i <= 25; i++) {
		if (curr_eHp >= i * 10) {
			cout << "|";
		}
		else {
			break;
		}
	}
	cout << "\n\n Current HP: " << curr_hp << "\n\n";
	//Print life bar
	for (int i = 1; i <= 50; i++) {
		if (curr_hp >= i * 2) {
			cout << "|";
		}
		else {
			break;
		}
	}
	cout << "\n\n";
	
		cout << cursor[0] << "Attack \n"
			<< cursor[1] << "Item \n"
			<< cursor[2] << "Flee \n";
	}
}

void victory_screen() {

	system("CLS");
	cout << "Enemy defeated!\n";
	switch (eSpawn) {
	case 1:
		eyes1 = "|     X        X     |";
		enemy1();
		break;
	case 2:
		eyes2 = "|          XX       XX          |";
		enemy2();
		break;
	}
	ui();
	if (lvl_up == 1) {
		cout << "\nLEVEL UP!\n";
	}
	cout << "\n Gained " << xp_gain << " xp!\n Press any key to continue";
	bool i = 0;
	do {
		char resume ='@';
		resume = _getch();
		if (resume != '@') {
			i = 1;
		} 
	} while (i == 0);

}

void defeat() {
	system("CLS");
	cout << " GAME OVER ";
	Sleep(10000);
}

int select_move() {

	int max = 2;
	bool valid_move = 0;
	do {
		char move_cursor;
		move_cursor = _getch();
		int returnval = cursormove(move_cursor, max, valid_move);
		if (returnval == 0) {
			return 0;
		}
		else if (move_cursor == 'j' || move_cursor == 'J') {
			if (pos_cursor == 0) {
				Attack();
				return 1;
			}
			else if (pos_cursor == 1) {
				// Reset heal_for
				int heal_for = 0;
				// Reset cursor position to top item
				for (int i = 0; i <= cItem; i++) {
					cursor[i] = ' ';
				}
				cursor[0] = '>';
				pos_cursor = 0;
				do {
					system("CLS");
					list_items(); // List all available items
					returnval = item_cursor(); // MOVES CURSOR IN ITEM MENU
				} while (returnval == 0);
				// If J is pressed
				if (returnval==1) {
					for (int i = 0; i < cItem; i++) {
						if (cursor[i] == '>') {
							switch (item_list[i]) {
							case 1:
								heal_for += hp_potion(hp);
								curr_hp += heal_for;
								if (curr_hp > hp) { //Healing above maximum hp cap is not allowed
									curr_hp = hp;
								}
								cout << "Using Hp potion . . .\n You got +" << heal_for << " HP!\n";
								Sleep(1000);
								reset_cursor();
								return 1;
							case 2:
								atk_buff += atk_potion(base_atk);
								cout << "Using Attack potion . . .\n You got +" << atk_buff << " attack!\n";
								Sleep(1000);
								reset_cursor();
								return 1;
							}
							break;
						}
					}
				}
				//If K is pressed 
				else if (returnval==-1) {
					// Reset cursor position to top item
					for (int i = 0; i <= cItem; i++) {
						cursor[i] = ' ';
					}
					cursor[1] = '>';
					pos_cursor = 1;
					return 0;
				}
			}
			else if (pos_cursor == 2) {
				Flee();
				return 1;
			}
		}
	} while (valid_move == 0);
}

void reset_cursor() {
	for (int i = 0; i <= 2; i++) {
		cursor[i] = ' ';
	}
	cursor[0] = '>';
	pos_cursor = 0;
}
int hp_potion(int max_hpin) {
	//Function adds 50% of target's max hp
	int addhp;
	addhp = round(max_hpin / 2);
	return addhp;
}
int atk_potion(int atk_in) {
	//Function adds 25% of target ATK
	int addAtk;
	addAtk = round(atk_in / 4);
	return addAtk;
}
int cursormove(char mvcrsr, int max_val, bool trumov) {
	
	//Moves cursor. Press W/S to cycle through list of available moves
	switch (mvcrsr) {
	case 'w':
	case 'W':
		if (pos_cursor > 0) {
			cursor[pos_cursor] = ' ';
			pos_cursor -= 1;
			cursor[pos_cursor] = '>';
			return 0;
		}
		if (pos_cursor == 0) {
			cursor[pos_cursor] = ' ';
			pos_cursor = max_val;
			cursor[pos_cursor] = '>';
			return 0;
		}
	case 's':
	case 'S':
		cursor[pos_cursor] = ' ';
		pos_cursor += 1;
		pos_cursor %= max_val + 1; // +1 because of modulus
		cursor[pos_cursor] = '>';
		return 0;
	case 'j':
	case 'J':
		return 1;
	default:
		trumov = 0;
		return 0;
	}
}

int item_cursor() {
	int max = cItem - 1; // -1 because of array
	bool valid_move = 0;
	do {
		char move_cursor;
		move_cursor = _getch();
		int returnval = cursormove(move_cursor, max, valid_move);
		// Specific to item selection, pressing K will return 1. This will cause the player to exit the item menu.
		if (move_cursor == 'K' || move_cursor == 'k') {
			return -1;
		}
		else {
			return returnval;
		}
	} while (valid_move == 0);
}

//Player attack
int Attack() {

	int atk = base_atk+atk_buff; // Current attack power is equal to base attack. If ATK potion is used, then add buff to current ATK.
	int rn_atk;
	int crit;
	int miss;
	miss = rand() % 100 / eDex;
	if (miss == 1) {
		cout << "You missed!\n";
		Sleep(500);
		return 0;
	}
	rn_atk = rand() % 5 + 1; // Set random number to add to base attack
	crit = rand() % 20 + 1;  // Set random number to decide if attack is critical
	atk += rn_atk;
	// Critical hit scales
	if (crit == 20) {
		atk *= 1.5;
		atk = round(atk); // Rounds int atk, because it is an integer. Decimal numbers not wanted
		cout << "A Critical hit!!\n";
	}
	cout << "You deal " << atk << " damage to enemy.\n";
	curr_eHp -= atk;
	Sleep(500);
}
//Enemy attack
int eAttack() {
	int eAtk = base_eAtk;
	int rn_atk;
	int crit;
	int miss;
	miss = rand() % 100 / dex;
	if (miss == 1) {
		cout << "Enemy missed!\n";
		Sleep(500);
		return 0;
	}
	rn_atk = rand() % 5 + 1; // Set random number to add to base attack
	crit = rand() % 20 + 1;  // Set random number to decide if attack is critical
	eAtk += rn_atk;
	if (crit == 20) {
		eAtk *= 1.5;
		eAtk = round(eAtk);
		cout << "A Critical hit!!\n";
	}
	cout << "Damaged for " << eAtk << " hp.\n";
	curr_hp -= eAtk;
	Sleep(500);
}

void Flee() {
	// Has 25% chance to flee from battle
	int esc= rand() % 4 + 1;
	if (esc == 1) {
		escape = 1;
	}
	else {
		cout << "Failed to escape.\n";
		Sleep(500);
	}
}

void list_items() {
	
	string item_name;
	for (int i=0; i < cItem; i++) {
		switch (item_list[i]) {
		case 1 :
			item_name = "HP potion";
				break;
		case 2 :
			item_name = "ATK potion";
			break;
		}
		cout << cursor[i] << "item number " << item_list[i] << "  item name: "<< item_name <<"\n";
		
	}
}

void enemy1() {

		eHp = 100;
		base_eAtk = 7;
		eDex = 1.1;
		cout << "\n" << "ENEMY TYPE: Negroidian\n\n";
		const string open = "|                    |";
		cout << "|--------------------|\n"
			<< open << "\n"
			<< eyes1 << "\n"
			<< "|         **         |\n";
		for (int i = 0; i <= 2; i++) {
			cout << "|     ||||||||||     |\n";
		}
		cout << open << "\n" << "|--------------------|\n";
	}

void enemy2() {

		eHp = 150;
		base_eAtk = 5;
		eDex = 1;
		cout << "\n" << "ENEMY TYPE: NiBBatarian\n\n";
		const string open = "|                               |";
		cout << "   ____________________________ \n"
			<< "  /                            \\" << endl
			<< " /                              \\" << endl
			<< eyes2 << "\n"
			<< open << "\n" << open << "\n"
			<< "|               F               |\n"
			<< open << "\n";
		for (int i = 0; i <= 2; i++) {
			cout << "|         **************        | \n";
		}
		cout << open << "\n" << open << "\n"
			<< "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n"
			<< "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n";
	}
