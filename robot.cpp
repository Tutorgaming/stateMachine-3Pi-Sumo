/* SIMPLE STATE MACHINE ROBOT V0.1
*  Platform : 3pi popolu
*  By C3MX .
*  http://www.itutor.name
*/
unsigned int current_state;
unsigned int nextstate;
unsigned int toggle;
bool black_found = false;
int SPEED = 20;

void forward(unsigned int time){
  set_motors(SPEED,SPEED);
  delay(time);
}

void backward(unsigned int time){
  set_motors(-SPEED,-SPEED);
  delay(time);
}

void turn_left(unsigned int time){
  set_motors(-SPEED,-SPEED);
  delay(time);
}
void turn_left(unsigned int time){
  set_motors(-SPEED,-SPEED);
  delay(time);
}

void slow_down(){
  unsigned int init_speed = SPEED;
  while(1){
    init_speed--;
    set_motors(init_speed,init_speed);
    delay(10);
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
      backward();
    break;
    case 2://Recover after Black line
      if(toggle%2==0)turn_left(200);
      else turn_right(200);
    break;
  }
}

void nextState(){

  switch (current_state) {
    case 0://NORMAL STATE
      if(black_found){
          nextstate = 1;
      }else{
          nextstate = 0;
      }
    break;
    case 1://BLACK LINE FOUND
      if(!black_found){
        nextstate = 2;
        toggle++;
      }else{
        nextstate = 1;
      }
    break;
    case 2://Recover after Black line
      nextstate = 0;
    break;
  }
  //Update Current State
  current_state = nextstate;
}


void main(){

  unsigned int sensors[5];
  pololu_3pi_init(2000);

  while(1){
    read_line_sensors(sensors,IR_EMITTED_ON);
    if(sensors[0]>IR_VALUE ||
      sensors[1]>IR_VALUE ||
      sensors[2]>IR_VALUE ||
      sensors[3]>IR_VALUE ||
      sensors[4]>IR_VALUE ){
        black_found = true;
      }else{
        black_found = false;
      }
    action();
    nextState();
  }
}
