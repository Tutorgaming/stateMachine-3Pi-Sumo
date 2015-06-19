/* 3pi_app2 - an application for the Pololu 3pi Robot
 *
 * This application uses the Pololu AVR C/C++ Library.  For help, see:
 * -User's guide: http://www.pololu.com/docs/0J20
 * -Command reference: http://www.pololu.com/docs/0J18
 *
 * Created: 6/19/2015 4:02:05 PM
 *  Author: student
 */

#include <pololu/3pi.h>

unsigned int current_state;
unsigned int nextstate;
unsigned int toggle;
int black_found = 0;
int SPEED = 20;
int IR_VALUE = 400;


void forward(unsigned int time){
	set_motors(SPEED,SPEED);
	delay(time);
}

void backward(unsigned int time){
	set_motors(-SPEED,-SPEED);
	delay(time);
}

void turn_left(unsigned int time){
	set_motors(-SPEED,SPEED);
	delay(time);
}
void turn_right(unsigned int time){
	set_motors(SPEED,-SPEED);
	delay(time);
}

void slow_down(){
	unsigned int init_speed = SPEED;
	while(1){
		init_speed--;
		set_motors(init_speed,init_speed);
		delay(8);
		if(init_speed == 0)break;
	}
}

void action(){
	switch (current_state) {
		case 0://NORMAL STATE
		forward(20);
		break;
		case 1://BLACK LINE FOUND
		slow_down();
		break;
		case 2:
		backward(10);
		break;
		case 3://Recover after Black line
		if(toggle%2==0)turn_left(1566);
		else turn_right(1111);
		break;
	}
}

void nextState(){

	switch (current_state) {
		case 0://NORMAL STATE
		if(black_found == 1){
			nextstate = 1;
			}else{
			nextstate = 0;
		}
		break;
		case 1://BLACK LINE FOUND = SLOW 
		nextstate = 2;
		break;
		case 2://Backward
		if(black_found == 0){
			nextstate = 3;
			toggle++;
			}else{
			nextstate = 2;
		}
		break;
		case 3://Recover after Black line
		nextstate = 0;
		break;
	}
	//Update Current State
	current_state = nextstate;
}


int main(){

	unsigned int sensors[5];
	pololu_3pi_init(2000);

	while(1){
		read_line_sensors(sensors,IR_EMITTERS_ON);
		if(sensors[0]>IR_VALUE ||
		sensors[1]>IR_VALUE ||
		sensors[2]>IR_VALUE ||
		sensors[3]>IR_VALUE ||
		sensors[4]>IR_VALUE ){
			black_found = 1;
			}else{
			black_found = 0;
		}
		action();
		nextState();
	}
}
