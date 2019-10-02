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

  state.state_motor = 1;
  state.state_led_y_left = 1;
  state.state_led_y_right = 1;
  for (int i = 0; i < NUM_STATES; i++) {
    if (states[i] == 1) {
      activate_registers = activate_registers | active_registers[i];
    }
  }
  shiftOutC(dataPin, clockPin, active_registers);
  //delay(20);
}
