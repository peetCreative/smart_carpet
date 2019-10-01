// pins
// Pin connected to ST_CP of 74HC595
int latchPin = 8;
// Pin connected to SH_CP of 74HC595
int clockPin = 12;
// Pin connected to DS of 74HC595
int dataPin = 11;

int led_r = 1;
int led_g = 2;
int led_y = 4;
int touch_right = 8; // piezo sensor right
int button = 16;
int q5 = 32;
int touch_left = 64; // piezo sensor left
int analog_input_0 = A0;

// other
double touch_val_left;
double touch_val_right;
// default idle values (calibrated by hand)
double touch_val_left_idle = 0;
double touch_val_right_idle = 0;

bool button_val;

int state_motor = 0;
bool vibration_on = false;

void setup() {
  // set pins to output so you can control the shift register
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  Serial.begin(9600);

  // measure & set the idle values of the touch sensors
  setTouchInputIdleValues();
}


void setTouchInputIdleValues() {
  delay(100);
  // get the lower bounds of the idle states (no touch applied)
  touch_val_right_idle = analogReadTouch(touch_right);
  Serial.println("-----------------SET TOUCH INPUT IDLE VALUES-----------------");
  Serial.println("TOUCH RIGHT IDLE: " + String(touch_val_right_idle));
  touch_val_left_idle = analogReadTouch(touch_left);
  Serial.println("TOUCH LEFT IDLE: " + String(touch_val_left_idle));
  Serial.println("-------------------------------------------------------------");
}

void loop() {
  if (vibration_on == false) {
    state_motor = 0;
  }
  // TODO map values
  // touch read
  touch_val_right = analogReadTouchAndMap(touch_right);
  Serial.println("MAPPED TOUCH RIGHT: " + String(touch_val_right));
  touch_val_left = analogReadTouchAndMap(touch_left);
  Serial.println("MAPPED TOUCH LEFT: " + String(touch_val_left));

  // button read
  button_val = readButton(button);
  Serial.println("BUTTON: " + String(button_val));
  if (vibration_on && (touch_val_left > 800 || touch_val_right > 800 || button_val)){
    state_motor = q5 | led_y;
  }
  else {
    state_motor = 0;
  }
  shiftOutC(dataPin, clockPin, state_motor);
  delay(20);
}

void restart() {
  // reload the main program (e.g. reset)
  // TODO
}
