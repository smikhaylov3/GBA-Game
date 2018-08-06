
#include "library.h"
#include "start.h"
#include "tank.h"
#include "lane.h"
#include "hedgehog.h"
#include "gameover.h"
#include <stdlib.h>
#include <stdio.h>

enum gameState {START,START_NODRAW, SETUP, PLAY};

int main() {
	REG_DISPCNT = MODE_3 | BG2_ENABLE;
	u32 health = GREEN;
	enum gameState state = START;
	//Initialize game structs representing vehicle, road lane markers and obstacles.
	gameObj vehicle;
	vehicle.col = 120;
	vehicle.oldRow = 80;
	vehicle.oldCol = 120;
	vehicle.row  = 80;
	vehicle.width = TANK_WIDTH;
	vehicle.height = TANK_HEIGHT;

	gameObj lanes;
	lanes.col = 120;
	lanes.oldRow =0 ;
	lanes.oldCol =120 ;
	lanes.row  = 0;
	lanes.width = LANE_WIDTH;
	lanes.height = LANE_HEIGHT;

	gameObj hedgehogs;
	hedgehogs.col = random_between(30,220);
	hedgehogs.oldRow = 0 ;
	hedgehogs.oldCol = 0 ;
	hedgehogs.row  = 0;
	hedgehogs.width = HEDGEHOG_WIDTH;
	hedgehogs.height = HEDGEHOG_HEIGHT;

	gameObj bullet;
	bullet.col = vehicle.col + 10;
	bullet.oldRow = 0 ;
	bullet.oldCol = 0 ;
	bullet.row  = vehicle.row;
	bullet.width = 5;
	bullet.height = 5;
	//initialize game variables.
	int bulletDisplay = 0;
	int random = 0;
	int loopCount = 0;
	int lives  = 3;
	int score = 0;
	while(1){
		loopCount++;
		waitForVblank();
		switch(state) {
		case START:
			drawImage3(0,0,240,160,start);
			state = START_NODRAW;
			break;
		case START_NODRAW:
			while(!KEY_DOWN_NOW(BUTTON_START)) {
			}
			state = SETUP;
			break;
		//SETUP case for resetting game state.
		case SETUP:
			fillScreen3(BLACK);

			lives = 3;
			hedgehogs.col = random_between(30,220);
			hedgehogs.oldRow = 0 ;
			hedgehogs.oldCol = 0 ;
			hedgehogs.row  = 0;

			health = GREEN;

			vehicle.oldRow = 0;
			vehicle.oldCol = 0;
			vehicle.row = 100;
			vehicle.col = 120;
			drawImage3(vehicle.row,vehicle.col,35,50,tank);

			score = 0;

			state = PLAY;
			break;
		case PLAY:
			random = random_between(0, 100);
			if(KEY_DOWN_NOW(BUTTON_SELECT)) {
				state = START;
				break;
			}
			//Collision detection!
			if(bulletDisplay) {
				if (abs(bullet.row - hedgehogs.row) <= 5 && abs(bullet.col - hedgehogs.col)  <= 5) {
					bulletDisplay = 0;
					bullet.oldRow = bullet.row ;
					bullet.oldCol = bullet.col ;
					bullet.col = vehicle.col + 10;
					bullet.row  = vehicle.row;
					drawRect3(bullet.oldRow,bullet.oldCol,5,5,BLACK);
					hedgehogs.oldRow = hedgehogs.row;
					hedgehogs.oldCol = hedgehogs.col;
					hedgehogs.row = 0;
					hedgehogs.col = 0;
					drawImage3(hedgehogs.oldRow,hedgehogs.oldCol,hedgehogs.height,hedgehogs.width,BLACK);
				} else {
					//Advances bullet each frame to simulate motion.
					if (bullet.row > 5) {
						bullet.oldRow = bullet.row;
						bullet.oldCol = bullet.col;
						bullet.row -= 5;
						drawRect3(bullet.oldRow,bullet.oldCol,5,5,BLACK);
						drawRect3(bullet.row,bullet.col,5,5,RED);
					} else {
						bulletDisplay = 0;
						bullet.oldRow = bullet.row ;
						bullet.oldCol = bullet.col ;
						bullet.col = vehicle.col + 10;
						bullet.row  = vehicle.row;
						drawRect3(bullet.oldRow,bullet.oldCol,5,5,BLACK);
					}
				}
			}
			if (abs(vehicle.row - hedgehogs.row) <= 25 && abs(vehicle.col - hedgehogs.col)  <= 25) {
				lives--;
				hedgehogs.oldRow = hedgehogs.row;
				hedgehogs.oldCol = hedgehogs.col;
				hedgehogs.row = 0;
				hedgehogs.col = 0;
				drawImage3(hedgehogs.oldRow,hedgehogs.oldCol,hedgehogs.height,hedgehogs.width,BLACK);
				if (lives == 2) {
					health = YELLOW;
				}
				if(lives == 1) {
					health = RED;
				}
				//End of game all lives lost.
				if (lives <= 0) {
					drawImage3(0,0,240,160,gameover);
					char str [1];
					sprintf(str,"%d", score);
					//Display player score on screen
					//short colorStr = random_between(0,85)*3;
					drawString(75,75,"YOUR SCORE IS: ",10);
					drawString(95,120,str,0);
					drawString(115,65,"THANKS FOR PLAYING! ",20);
					//int ct = 0;
					//Strobes player score
					while(!KEY_DOWN_NOW(BUTTON_START)){
						/**if(ct % 10 == 0){
							short colorStr = random_between(0,255);
							drawString(95,120,str,colorStr);
						}*/
						//ct++;
					}
					state = START;
					break;
				}
			}
			//Simulate tank approaching hedgehogs
			if(hedgehogs.row >= 150) {
				hedgehogs.oldRow = hedgehogs.row;
				hedgehogs.oldCol = hedgehogs.col;
				hedgehogs.row = 10;
				hedgehogs.col = random_between(30,227);
				drawImage3(hedgehogs.oldRow,hedgehogs.oldCol,hedgehogs.height,hedgehogs.width,BLACK);
				score++;
			}
			//Resets the lane marker's position
			if(lanes.row + 5 >= 160) {
				lanes.row = 0;
			}
			drawRect3(3,207,30,30,health);
			drawRect3(lanes.oldRow,lanes.col,4,12,BLACK);
			drawRect3(lanes.row,lanes.col,4,12,YELLOW);

			//Advances lane marker each frame to simulate motion.
			lanes.oldRow = lanes.row;
			lanes.row += 3;

			hedgehogs.oldRow = hedgehogs.row;
			hedgehogs.row += 3;

			drawImage3(vehicle.row,vehicle.col,35,50,tank);
			drawImage3(hedgehogs.oldRow,hedgehogs.col,hedgehogs.height,hedgehogs.width,BLACK);
			drawImage3(hedgehogs.row,hedgehogs.col,hedgehogs.height,hedgehogs.width,hedgehog);
			//tank movement code below
			if (KEY_DOWN_NOW(BUTTON_RIGHT) && vehicle.col <= 200) {
				//fillScreen3(GRAY);
				drawImage3(vehicle.row,vehicle.col,35,50,tank);
				drawRect3(hedgehogs.oldRow,hedgehogs.col,hedgehogs.height,hedgehogs.width,BLACK);
				drawImage3(hedgehogs.row,hedgehogs.col,hedgehogs.height,hedgehogs.width,hedgehog);
				drawRect3(lanes.row,lanes.col,4,12,YELLOW);
				drawRect3(3,207,30,30,health);
				vehicle.oldRow = vehicle.row;
				vehicle.oldCol = vehicle.col;
				vehicle.col = vehicle.oldCol + 1;
				drawImage3(vehicle.row,vehicle.col,35,50,tank);
			}
			if (KEY_DOWN_NOW(BUTTON_LEFT) && vehicle.col >= 10) {
				//fillScreen3(GRAY);
				drawImage3(vehicle.row,vehicle.col,35,50,tank);
				drawImage3(hedgehogs.oldRow,hedgehogs.col,hedgehogs.height,hedgehogs.width,BLACK);
				drawImage3(hedgehogs.row,hedgehogs.col,hedgehogs.height,hedgehogs.width,hedgehog);
				drawRect3(lanes.row,lanes.col,4,12,YELLOW);
				drawRect3(3,207,30,30,health);
				vehicle.oldRow = vehicle.row;
				vehicle.oldCol = vehicle.col;
				vehicle.col = vehicle.oldCol - 1;
				drawRect3(vehicle.oldRow,vehicle.oldCol,35,50,BLACK);
				drawImage3(vehicle.row,vehicle.col,35,50,tank);
			}
			if (KEY_DOWN_NOW(BUTTON_DOWN) && vehicle.row <= 100) {
				//fillScreen3(GRAY);
				drawImage3(vehicle.row,vehicle.col,35,50,tank);
				drawImage3(hedgehogs.oldRow,hedgehogs.col,hedgehogs.height,hedgehogs.width,BLACK);
				drawImage3(hedgehogs.row,hedgehogs.col,hedgehogs.height,hedgehogs.width,hedgehog);
				drawRect3(lanes.row,lanes.col,4,12,YELLOW);
				drawRect3(3,207,30,30,health);
				vehicle.oldRow = vehicle.row;
				vehicle.oldCol = vehicle.col;
				vehicle.row = vehicle.oldRow + 1;
				drawImage3(vehicle.row,vehicle.col,35,50,tank);
			}
		}
			if (KEY_DOWN_NOW(BUTTON_UP) && vehicle.row >= 20) {
				//fillScreen3(GRAY);
				drawImage3(vehicle.row,vehicle.col,35,50,tank);
				drawRect3(hedgehogs.oldRow,hedgehogs.col,hedgehogs.height,hedgehogs.width,BLACK);
				drawImage3(hedgehogs.row,hedgehogs.col,hedgehogs.height,hedgehogs.width,hedgehog);
				drawRect3(lanes.row,lanes.col,4,12,YELLOW);
				drawRect3(3,207,30,30,health);
				vehicle.oldRow = vehicle.row;
				vehicle.oldCol = vehicle.col;
				vehicle.row = vehicle.oldRow - 1;
				drawImage3(vehicle.row,vehicle.col,35,50,tank);
			}
			if (KEY_DOWN_NOW(BUTTON_A)) {
				if(!bulletDisplay) {
					bulletDisplay = 1;
					bullet.oldRow = bullet.row ;
					bullet.oldCol = bullet.col ;
					bullet.col = vehicle.col + 10;
					bullet.col = vehicle.col + 10;
					bullet.row  = vehicle.row;
					drawRect3(bullet.row,bullet.col,5,5,RED);
				}
			}

	}
	return 0;
}

