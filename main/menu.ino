bool in_main_menu = 1;
int state_main_menu = 0;
//not so nice var name:
//state to go back if we are not going back to Mainmenu
int back_prev_state = 0;

double zero_threshold = 20;
double prev_state_left = 0;
double prev_state_right = 0;
int left_down = 0;
int right_down = 0;
int left_up = 0;
int right_up = 0;

#define MAIN_MENU_TIMER = 0;
#define MAIN_MENU_STOP_WATCH = 1;
#define MAIN_MENU_ITMES = 2;

#define STATE_TIMER_START 0;
#define STATE_TIMER_INCREMENT_TIME 1;
#define STATE_TIMER_RESET 2;
#define STATE_TIMER_READY 3;
#define STATE_TIMER_RUNNING 4;
#define STATE_TIMER_RUNNING_START 5;
#define STATE_TIMER_RING 6;
#define STATE_TIMER_STOP 7;
#define STATE_TIMER_BACK_MM 8;
int state_timer = STATE_TIMER_START;
int timer_t = 0;

void process() {
  left_down = touch_val_left > zero_threshold && state_left < zero_threshold;
  right_down = touch_val_right > zero_threshold && state_right < zero_threshold;
  left_up = touch_val_left < zero_threshold && state_left > zero_threshold;
  right_up = touch_val_right < zero_threshold && state_right > zero_threshold;
  prev_state_left = touch_val_left;
  prev_state_right = touch_val_right;

  register_states[state_motor_index] = left_down || right_down;

  if (in_main_menu) {
    switch (state_main_menu) {
      case MAIN_MENU_TIMER:
        //show timer on board
        break;
      case MAIN_MENU_STOP_WATCH:
        //show timer on board
        break;
    }
    if (left_down) {
      // we don't have more functionallity than
      state_main_menu = (state_main_menu + 1) % MENU_ITEMS;
    }
    
  }
  else {
    // over enigineered but we can extend it this way elagantly
    switch (state_main_menu) {
      case MAIN_MENU_TIMER:
        process_timer();
        break;
      case MAIN_MENU_STOP_WATCH:
        process_stop_watch();
        break;
    }
  }
}

void process_timer() {
  switch (state_timer) {
    case STATE_TIMER_START:
      if (right_down) {
        back_prev_state = STATE_TIMER_START;
      }
      if (left_down) {
        timer_t = 0;
        process_timer_increment();
        state_timer = STATE_TIMER_INCREMENT;
      }
      break;
    case STATE_TIMER_INCREMENT_TIME:
      if (left_up) {
        state_timer = STATE_TIMER_READY;
      }
      else {
        process_timer_incremement();
      }
      if (right_down) {
        state_timer = STATE_TIMER_RESET;
      }
      break;
    case STATE_TIMER_RESET:
      if (touch_val_left < zero_threshold && 
          touch_val_right < zero_threshold) {
        timer_t = 0;
        state_timer = STATE_TIMER_START;      
      }
      break;
    case STATE_TIMER_READY:
      if (left_down) {
        state_timer = STATE_TIMER_INCREMENT_TIME;
        process_timer_increment();
      }
      if (right_down) {
        STATE_TIMER_BACK_MM
        back_prev_state = STATE_TIMER_RUNNING_START;
      }
    case STATE_TIMER_RUNNING_START:
      //TODO: look for simple timer
      timer = 
      state_timer = STATE_TIMER_RUNNING;
    case STATE_TIMER_RUNNING:
      if (right_down) {
        //TODO: stop the timer
        back_prev_state = STATE_TIMER_STOP;
        state_timer = STATE_TIMER_BACK_MM;
      }
      //TIMER abgelaufen
      if (timer) {
        state_timer = STATE_TIMER_RING;
        register_states[state_motor_index] = 1;
      }
    case STATE_TIMER_RING:
      if (right_down) {
        register_states[state_motor_index] = 0;    
        back_prev_state = STATE_TIMER_START;
        state_timer = STATE_TIMER_BACK_MM;
      }
      if (left_down) {
        register_states[state_motor_index] = 0;    
        state_timer = STATE_TIMER_INCREMENT;
        
      }
    case STATE_TIMER_STOP:
      if (right_down) {
        back_prev_state = STATE_TIMER_RUNNING;
        state_timer = STATE_TIMER_BACK_MM;
      }
      if (left_down) {
        process_timer_increment();
        state_timer = STATE_TIMER_INCREMENT;
      }
      break;
    case STATE_TIMER_BACK_MM:
      // state that goes back to main menu 
      //or to a next state after right is up
      if (right_up) {
        state_timer = back_prev_state;
      }
      if (left_down) {
        timer_t = 0;
        process_timer_increment();
        state_timer = STATE_TIMER_INCREMENT;
      }
    case STATE_TIMER_BACK_MMW:
      // just wait that left and right is up and go back to main menu
      if (touch_val_left < zero_threshold && 
          touch_val_right < zero_threshold) {
        state_timer = STATE_TIMER_START;
        in_main_menu = 1;
        state_main_menu = MAIN_MENU_TIMER;
      }
      break;
  }
}

void process_stop_watch() {
  //TODO: do whatever
}


void process_timer_increment {
  //possibly make logarthmic scale
  //just implement this by hand don't overingeneer!
  timer_t += map(timer_val_left, 0, 800, 5, 60);
}
