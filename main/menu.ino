bool in_main_menu = 1;
int state_main_menu = 0;
//not so nice var name:
//state to go back if we are not going back to Mainmenu
int next_state = 0;

double state_left = 0;
double state_right = 0;
int left_down = 0;
int right_down = 0;
int left_up = 0;
int right_up = 0;

#define MAIN_MENU_TIMER 0
#define MAIN_MENU_STOP_WATCH 1
#define MAIN_MENU_GOTO 2

#define STATE_TIMER_START 0
#define STATE_TIMER_INCREMENT_TIME 1
#define STATE_TIMER_RESET 2
#define STATE_TIMER_READY 3
#define STATE_TIMER_RUNNING 4
#define STATE_TIMER_RUNNING_START 5
#define STATE_TIMER_RING 6
#define STATE_TIMER_PAUSE 7
#define STATE_TIMER_BACK_MM 8
#define STATE_TIMER_BACK_MMW 9
int state_timer = STATE_TIMER_START;
// t of timer in secs
int timer_t = 0;
// timer id of the TimeOut
int timer_timer_ring_off;
int timer_timer_count_down;

void process() {
  left_down = touch_val_left > threshold_low && state_left < threshold_low;
  right_down = touch_val_right > threshold_low && state_right < threshold_low;
  left_up = touch_val_left < threshold_low && state_left > threshold_low;
  right_up = touch_val_right < threshold_low && state_right > threshold_low;
  state_left = touch_val_left;
  state_right = touch_val_right;

  register_states[state_motor_index] = left_down || right_down || (state_timer == STATE_TIMER_RING);

  if (in_main_menu) {
    switch (state_main_menu) {
      case MAIN_MENU_TIMER:
        text = "HOME";
        if (right_down) {
          state_main_menu = MAIN_MENU_GOTO;
        }
        //show timer on board
        break;
      case MAIN_MENU_STOP_WATCH:
        text = "STOP WATCH";
        //show timer on board
        break;
      case MAIN_MENU_GOTO:
        text = "GOTO";
        if (right_up) {
          in_main_menu = 0;
          state_main_menu = MAIN_MENU_TIMER;
        }
    }
    if (left_down) {
      // we don't have more functionallity than
      state_main_menu = (state_main_menu + 1) % MAIN_MENU_GOTO;
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
      text = "START";
      if (right_down) {
        next_state = STATE_TIMER_START;
      }
      if (left_down) {
        timer_t = 0;
        process_timer_increment();
        state_timer = STATE_TIMER_INCREMENT_TIME;
      }
      break;
    case STATE_TIMER_INCREMENT_TIME:
      if (left_up) {
        state_timer = STATE_TIMER_READY;
      }
      else {
        process_timer_increment();
        number = timer_t;
      }
      if (right_down) {
        state_timer = STATE_TIMER_RESET;
        number = -1;
      }
      break;
    case STATE_TIMER_RESET:
      text = "RESET";
      if (touch_val_left < threshold_low && 
          touch_val_right < threshold_low) {
        timer_t = 0;
        state_timer = STATE_TIMER_START;      
      }
      break;
    case STATE_TIMER_READY:
      number = timer_t;
      if (left_down) {
        state_timer = STATE_TIMER_INCREMENT_TIME;
        process_timer_increment();
      }
      if (right_down) {
        state_timer = STATE_TIMER_BACK_MM;
        next_state = STATE_TIMER_RUNNING_START;
      }
      break;
    case STATE_TIMER_RUNNING_START:
      timer_timer_count_down = timer.setInterval(1000, timer_count_down);
      state_timer = STATE_TIMER_RUNNING;
      break;
    case STATE_TIMER_RUNNING:
      number = timer_t;
      if (right_down) {
        timer.deleteTimer(timer_timer_count_down);
        next_state = STATE_TIMER_PAUSE;
        state_timer = STATE_TIMER_BACK_MM;
      }
      break;
    case STATE_TIMER_RING:
      text = "RING";
      if (right_down) {
        register_states[state_motor_index] = 0;    
        next_state = STATE_TIMER_START;
        state_timer = STATE_TIMER_BACK_MM;
      }
      if (left_down) {
        register_states[state_motor_index] = 0;    
        timer_t = 0;
        process_timer_increment();
        state_timer = STATE_TIMER_INCREMENT_TIME;
      }
      break;
    case STATE_TIMER_PAUSE:
      number = -1;
      text = "PAUS";
      if (right_down) {
        next_state = STATE_TIMER_RUNNING_START;
        state_timer = STATE_TIMER_BACK_MM;
      }
      if (left_down) {
        process_timer_increment();
        state_timer = STATE_TIMER_INCREMENT_TIME;
      }
      break;
    case STATE_TIMER_BACK_MM:
      // state that goes back to main menu 
      //or to a next state after right is up
      if (right_up) {
        state_timer = next_state;
      }
      if (left_down) {
        state_timer = STATE_TIMER_BACK_MMW;     
      }
      break;
    case STATE_TIMER_BACK_MMW:
      text = "MENU";
      number = -1;
      // just wait that left and right is up and go back to main menu
      if (touch_val_left < threshold_low && 
          touch_val_right < threshold_low) {
        state_timer = STATE_TIMER_START;
        in_main_menu = 1;
        state_main_menu = MAIN_MENU_TIMER;
      }
      break;
  }
}

void timer_stop_ring() {
  register_states[state_motor_index] = 0;    
  state_timer = STATE_TIMER_START;
}

void timer_count_down() {
  timer_t--;
  if (timer_t == 0) {
    state_timer = STATE_TIMER_RING;
    register_states[state_motor_index] = 1;
    timer_timer_ring_off = timer.setTimeout(10 * 1000, timer_stop_ring);
    timer.deleteTimer(timer_timer_count_down);
    number = -1;
    text = "RING";
  }
}

void process_stop_watch() {
  //TODO: do whatever
}


void process_timer_increment() {
  //possibly make logarthmic scale
  //just implement this by hand don't overingeneer!
//  timer_t += map(touch_val_left, 0, 800, 5, 60);
  if (touch_val_left < threshold_high)
    timer_t += 1;
  else
    timer_t += 10;
}
