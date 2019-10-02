// default idle values (calibrated by hand)
double touch_val_left_idle = 0;
double touch_val_right_idle = 0;

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

double analogReadTouch(int touch_pin) {
   // read touch input values 
    int led;
    if (touch_pin == touch_left) {
      led = led_g_left;
    } else if (touch_pin == touch_right) {
      led = led_g_right;
    }
    
    digitalWrite(latchPin, LOW);
    shiftOutC(dataPin, clockPin, touch_pin); //| state_motor);    
    digitalWrite(latchPin, HIGH);
    delay(10); // for shiftOut to shift
    
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
