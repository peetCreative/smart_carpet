#include <Wire.h>
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"
Adafruit_AlphaNum4 alpha4 = Adafruit_AlphaNum4();

void initDisplay() {
  int d = 100;
  alpha4.begin(0x70);  // pass in the address
  alpha4.writeDigitRaw(3, 0x0);
  alpha4.writeDigitRaw(0, 0xFFFF);
  alpha4.writeDisplay();
  delay(d);
  alpha4.writeDigitRaw(0, 0x0);
  alpha4.writeDigitRaw(1, 0xFFFF);
  alpha4.writeDisplay();
  delay(d);
  alpha4.writeDigitRaw(1, 0x0);
  alpha4.writeDigitRaw(2, 0xFFFF);
  alpha4.writeDisplay();
  delay(d);
  alpha4.writeDigitRaw(2, 0x0);
  alpha4.writeDigitRaw(3, 0xFFFF);
  alpha4.writeDisplay();
  delay(d);
  
  alpha4.clear();
  alpha4.writeDisplay();
}

char displaybuffer[4] = {' ', ' ', ' ', ' '};

int pos = 0;
String text;
int number = -1;
int animation_t = 0;
int disId;

void update_display () {
  Serial.println("Display");
//  if (animation_t > 0) {
//    animation_t -= display_update_t;
//    return;
//  }
  Serial.println("printText: " + text +  String(number));
  if (number != -1) {
    text = String(number);
  }
//    if (text.length() > 4) {
//      displayTextRunning();
//    } else {
//      displayText();
//    }
//  } else {
//    displayNumber();
//  }
  alpha4.writeDigitAscii(0, text.charAt(0));
  alpha4.writeDigitAscii(1, text.charAt(1));
  alpha4.writeDigitAscii(2, text.charAt(2));
  alpha4.writeDigitAscii(3, text.charAt(3));

 // alpha4.writeDigitRaw(0, 0x0);
 // alpha4.writeDigitRaw(1, 0xFFFF);
  alpha4.writeDisplay();

  //delay(100);
}

void printChar() {

    char c = text.charAt(pos);
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

void displayTextRunning() {
  pos = 0;
  // scroll down display
  disId = timer.setInterval(200, printChar);
}

void displayText() {
    // set every digit to the buffer
    alpha4.writeDigitAscii(0, text[0]);
    alpha4.writeDigitAscii(1, text[1]);
    alpha4.writeDigitAscii(2, text[2]);
    alpha4.writeDigitAscii(3, text[3]);
    
    // write it out!
    alpha4.writeDisplay();
}

void displayNumber () {

  text = String(number);
}
