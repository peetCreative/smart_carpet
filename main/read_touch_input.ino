int analogReadTouch(int touch_pin) {
    int led;
    if (touch_pin == touch_left) {
      led = led_r;
    } else if (touch_pin == touch_right) {
      led = led_g;
    }
    
    digitalWrite(latchPin, LOW);
    shiftOutC(dataPin, clockPin, led | touch_pin | state_motor);    
    digitalWrite(latchPin, HIGH);
    delay(40); // for shiftOut to shift
    
    double touch_val = analogRead(analog_input_0);
    return touch_val;
}
