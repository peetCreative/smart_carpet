//Pin connected to ST_CP of 74HC595
int latchPin = 8;
//Pin connected to SH_CP of 74HC595
int clockPin = 12;
////Pin connected to DS of 74HC595
int dataPin = 11;

int piezoSensor = A0;

int led_r = 1;
int led_g = 2;
int led_y = 4;
int q3 = 8;
int q4 = 16;


bool clock = 0; 


void setup() {
  //set pins to output so you can control the shift register
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

  digitalWrite(clockPin, LOW);
  Serial.begin(9600);
 delay(50);
    // take the latchPin low so 
    // the LEDs don't change while you're sending in bits:
    digitalWrite(latchPin, LOW);
   /* // shift out the bits:
  digitalWrite(dataPin, HIGH);
  digitalWrite(clockPin, HIGH);
  delay(30);
  digitalWrite(clockPin, LOW);
  delay(30);
  digitalWrite(dataPin, HIGH);
  //digitalWrite(clockPin, HIGH);
  //delay(30);
  //digitalWrite(clockPin, LOW);
  //delay(30);
  */
  
//  shiftOut(dataPin, clockPin, MSBFIRST,255);  
  //shiftOut(dataPin, clockPin, LSBFIRST,0);  
    //shiftOut(dataPin, clockPin, LSBFIRST,1);  

    //take the latch pin high so the LEDs will light up:
  //  digitalWrite(latchPin, HIGH);
    //Serial.println(255);
    // pause before next value:

}

// the heart of the program
void shiftOutC(int myDataPin, int myClockPin, byte myDataOut) {
  // This shifts 8 bits out MSB first,
  //on the rising edge of the clock,
  //clock idles low

  //internal function setup
  int i=0;
  int pinState;

  //clear everything out just in case to
  //prepare shift register for bit shifting
  digitalWrite(myDataPin, 0);
  digitalWrite(myClockPin, clock);

  //for each bit in the byte myDataOut�
  //NOTICE THAT WE ARE COUNTING DOWN in our for loop
  //This means that %00000001 or "1" will go through such
  //that it will be pin Q0 that lights.
  for (i=7; i>=0; i--)  {
    digitalWrite(myClockPin, clock);

    //if the value passed to myDataOut and a bitmask result
    // true then... so if we are at i=6 and our value is
    // %11010100 it would the code compares it to %01000000
    // and proceeds to set pinState to 1.
    if ( myDataOut & (1<<i) ) {
      pinState= 1;
    }
    else { 
      pinState= 0;
    }

    //Sets the pin to HIGH or LOW depending on pinState
    digitalWrite(myDataPin, pinState);
    delay(40);
    //register shifts bits on upstroke of clock pin  
    digitalWrite(myClockPin, !clock);
    //zero the data pin after shift to prevent bleed through
    digitalWrite(myDataPin, 0);
    delay(40);
  }

  //stop shifting
  digitalWrite(myClockPin, clock);
}

void loop() {
  // count from 0 to 255 and display the number 
  // on the LEDs
  //for (int i = 0; i < 3; i++){
    //digitalWrite(latchPin, LOW);
    //shiftOutC(dataPin, clockPin, i);
    //digitalWrite(latchPin, HIGH);

    
    double sensor;
    digitalWrite(latchPin, LOW);
    shiftOutC(dataPin, clockPin, q3 );    
    digitalWrite(latchPin, HIGH);
    delay(50);
    sensor = analogRead(piezoSensor);
    Serial.println("links: " + String(sensor));
    digitalWrite(latchPin, LOW);
    shiftOutC(dataPin, clockPin, q4 );    
    digitalWrite(latchPin, HIGH);
    delay(50);
    sensor = analogRead(piezoSensor);
    Serial.println("rechts: " + String(sensor));
}
