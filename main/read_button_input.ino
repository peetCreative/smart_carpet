bool readButton(int button_pin) {
  // read the button over the analog input
  digitalWrite(latchPin, LOW);
  shiftOutC(dataPin, clockPin, button_pin | led_g );//| state_motor);    
  digitalWrite(latchPin, HIGH);
  delay(20);
  double button_val = analogRead(analog_input_0);
  // Serial.println("BUTTON ANALOG VALUE: " + String(button_val));
  if (button_val > 500) {
    return 1;
  } else {
    return 0;
  }
}


  
