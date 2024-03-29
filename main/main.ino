#include <SimpleTimer.h>

/////////////////////////////////////////////////////////
// pins
// Pin connected to ST_CP of 74HC595
int latchPin = 8;
// Pin connected to SH_CP of 74HC595
int clockPin = 12;
// Pin connected to DS of 74HC595
int dataPin = 11;
int analog_input_0 = A0;

/////////////////////////////////////////////////////////
// shift register outputs 
const int led_g_left = 1; // q0
const int led_y_left = 2; // q1
const int led_g_right = 4; // q2
const int led_y_right = 8; // q3
const int vibration_motor = 128; // q7

int button = 16; // q4 
int touch_left = 32; // q5 piezo sensor left
int touch_right = 64; // q6 piezo sensor right

/////////////////////////////////////////////////////////////////////////////////////////////////////
// shift register outputs states

// to shift out all needed registers (MAPPING FROM register_nums -> register_states)
const int NUM_STATES = 5;
int register_to_activate[NUM_STATES] = {vibration_motor, led_g_left, led_y_left, led_g_right, led_y_right};

// to hold track of what needs to be updated
// indices
const int state_motor_index = 0;
const int state_led_g_left_index = 1;
const int state_led_y_left_index = 2;
const int state_led_g_right_index = 3;
const int state_led_y_right_index = 4;

// initial states
int state_motor = 0;
int state_led_g_left = 0;
int state_led_y_left = 0;
int state_led_g_right = 0;
int state_led_y_right = 0;

int register_states[NUM_STATES] = {state_motor, state_led_g_left, state_led_y_left, state_led_g_right, state_led_y_right};
//////////////////////////////////////////////////////////////////////////////

// input values
const double threshold_low = 200;
const double threshold_high = 800;
double touch_val_left;
double touch_val_right;
bool button_val;

const bool VIBRATION_ACTIVE = true;

// timer object
SimpleTimer timer;
int timer_io_menu;
int timer_display;

int activate; // the registers to be activated

const int display_update_t = 50;
const int io_menu_update_t = 20;

void setup() {
  // set pins to output so you can control the shift register
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  Serial.begin(9600);

  initDisplay();
  // initialize( measure & set) the idle values of the touch sensors
  setTouchInputIdleValues();
  timer_io_menu = timer.setInterval(io_menu_update_t, update_io_menu);
  timer_display = timer.setInterval(display_update_t, update_display);
}

void loop() {
  timer.run();
}

void update_io_menu () {
//  if (VIBRATION_ACTIVE == false) {
//    register_states[state_motor_index] = 0;
//  }

  // touch read
  touch_val_right = analogReadTouchAndMap(touch_right);
  Serial.println("MAPPED TOUCH RIGHT: " + String(touch_val_right));
  touch_val_left = analogReadTouchAndMap(touch_left);
  Serial.println("MAPPED TOUCH LEFT: " + String(touch_val_left));

  // button read
  button_val = readButton(button);
  Serial.println("BUTTON: " + String(button_val));
  if (button_val) {
    setTouchInputIdleValues();
  }

  register_states[state_led_y_left_index] =
    (touch_val_left > threshold_low &&  touch_val_left < threshold_high) || button_val;
  register_states[state_led_y_right_index] =
    (touch_val_right > threshold_low &&  touch_val_right < threshold_high) || button_val;
  register_states[state_led_g_left_index] =
    (touch_val_left > threshold_high) || button_val;
  register_states[state_led_g_right_index] =
    (touch_val_right > threshold_high) || button_val;

  process();

  shiftOutC(dataPin, clockPin, 0);
}


//  if (VIBRATION_ACTIVE && (touch_val_left > 800 || touch_val_right > 800)){
//    register_states[state_led_g_left_index] = 1;
//    register_states[state_led_g_right_index] = 1;
//  }
//  else {
//    register_states[state_led_g_left_index] = 0;
//    register_states[state_led_g_right_index] = 0;
//  }
//  
//  //delay(20);
//  
//  if (button_val) {
//    //int timeout = 0;
//    //int timer_vibrate = timer.setTimeout(timeout, vibrateOnceFor);
//    register_states[state_led_y_left_index] = 1;
//    register_states[state_led_y_right_index] = 1;
//    register_states[state_motor_index] = 0;
//    shiftOutC(dataPin, clockPin, 0); 
//    //delay(20);
//  }
//}

/*
void addToBuffer(int digit){
  // clear buffer
  memset(dataBuffer, 0, sizeof(dataBuffer));
 
  for (int c = 0; c < sizeof(dataBuffer); c++) {
    dataBuffer[c] = state
  }
  
  writeBuffer();
}

void writeBuffer(){
  digitalWrite(latchPin, 0);
  
  for (int a = sizeof(dataBuffer) - 1; a >= 0  ; a--) {
    shiftOut(dataPin, clockPin, dataBuffer[a]);
  }
  
  digitalWrite(latchPin, 1);
}
*/
