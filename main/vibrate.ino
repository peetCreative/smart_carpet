void vibrateOnceFor() { //(int vibration_duration) {
 
  int vibration_duration = 3000;
  Serial.println("-----------------Vibration activated for " + String(vibration_duration) + "---------------------");
 
  const int d = 1; // ms delay between calls
  
  // vibration_duration = d * n;
  int n = vibration_duration / d;
  n = 5;
  // Call function f every d milliseconds for n times. 
  timer.setTimer(d, vibrate, n);
}

void vibrate() {
  Serial.println("---------------VIBRATION!-----------------------");
  if (!VIBRATION_ACTIVE) {
    return;
  }

 
  register_states[state_motor_index] = 1;
  register_states[state_led_y_left_index] = 1;
  register_states[state_led_y_right_index] = 1;
  
  shiftOutC(dataPin, clockPin, activate);
  //delay(20);
}
