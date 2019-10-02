#include <Wire.h>
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"
Adafruit_AlphaNum4 alpha4 = Adafruit_AlphaNum4();

void initDisplay() {
  alpha4.begin(0x70);  // pass in the address
  alpha4.writeDigitRaw(3, 0x0);
  alpha4.writeDigitRaw(0, 0xFFFF);
  alpha4.writeDisplay();
  //delay(200);
  alpha4.writeDigitRaw(0, 0x0);
  alpha4.writeDigitRaw(1, 0xFFFF);
  alpha4.writeDisplay();
  //delay(200);
  alpha4.writeDigitRaw(1, 0x0);
  alpha4.writeDigitRaw(2, 0xFFFF);
  alpha4.writeDisplay();
  //delay(200);
  alpha4.writeDigitRaw(2, 0x0);
  alpha4.writeDigitRaw(3, 0xFFFF);
  alpha4.writeDisplay();
  //delay(200);
  
  alpha4.clear();
  alpha4.writeDisplay();
}

char displaybuffer[4] = {' ', ' ', ' ', ' '};

int pos = 0;
String text;
int disId;
void printChar() {

    char c = text.charAt(pos);
    Serial.println("!!!!!!!!!!!!!!!!!!CHAR: " + c);
    pos++;
    if (pos >= text.length()) {
      timer.deleteTimer(disId);
    }
    displaybuffer[0] = displaybuffer[1];
    displaybuffer[1] = displaybuffer[2];
    displaybuffer[2] = displaybuffer[3];
    displaybuffer[3] = c;    
    
    // set every digit to the buffer
    alpha4.writeDigitAscii(0, displaybuffer[0]);
    alpha4.writeDigitAscii(1, displaybuffer[1]);
    alpha4.writeDigitAscii(2, displaybuffer[2]);
    alpha4.writeDigitAscii(3, displaybuffer[3]);
    
    // write it out!
    alpha4.writeDisplay();
}

void displayText(String text_to_display) {
  pos = 0;
  text = text_to_display;
  // scroll down display
  disId = timer.setInterval(200, printChar);
}
