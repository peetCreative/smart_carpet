double analogReadTouch(int touch_pin) {
   // read touch input values 
    int led;
    if (touch_pin == touch_left) {
      led = led_r;
    } else if (touch_pin == touch_right) {
      led = led_g;
    }
    
    digitalWrite(latchPin, LOW);
    shiftOutC(dataPin, clockPin, led | touch_pin | state_motor);    
    digitalWrite(latchPin, HIGH);
    delay(20); // for shiftOut to shift
    
    double touch_val = analogRead(analog_input_0);

    return touch_val;
}

double analogReadTouchAndMap(int touch_pin) {

  double touch_val = analogReadTouch(touch_pin);

  // constrain and map
  // set the minimum constrain value to the lower idle bound
  int min_mappable_value = 0;
  if (touch_pin == touch_left) {
    min_mappable_value = touch_val_left_idle;
    Serial.println("TOUCH LEFT: " + String(touch_val));
  } else if (touch_pin == touch_right) {
    min_mappable_value = touch_val_right_idle;
    Serial.println("TOUCH RIGHT: " + String(touch_val));
  }

  const int max_mappable_value = 860; // set the maximum constrain value to manually measured value
  
  double constrained_touch_val = constrain(touch_val, min_mappable_value, max_mappable_value);
  int fromLow = min_mappable_value;
  int fromHigh = max_mappable_value;
  const int toLow = 0;
  const int toHigh = 1023;
  double mapped_touch_val = map(constrained_touch_val, fromLow, fromHigh, toLow, toHigh);  

  return mapped_touch_val;
}
