void vibrateOnceFor() { //(int vibration_duration) {
 
  int vibration_duration = 3000;
  Serial.println("Vibration activated for " + String(vibration_duration));
 
  const int d = 50; // ms delay between calls
  // n = 10;
  // vibration_duration = d * n;
  n = vibration_duration / d;
  
  // Call function f every d milliseconds for n times. 
  timer.setTimer(d, vibrate, n);
}

void vibrate() {
  if (!vibration_active) {
    return;
  }
  
  Serial.println("VIBRATION!");
  shiftOutC(dataPin, clockPin, vibration_motor);
  delay(20);
}
