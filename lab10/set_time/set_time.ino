/*
  LiquidCrystal Library - Serial Input

  Demonstrates the use a 16x2 LCD display.  The LiquidCrystal
  library works with all LCD displays that are compatible with the
  Hitachi HD44780 driver. There are many of them out there, and you
  can usually tell them by the 16-pin interface.

  This sketch displays text sent over the serial port
  (e.g. from the Serial Monitor) on an attached LCD.

  The circuit:
   LCD RS pin to digital pin 12
   LCD Enable pin to digital pin 11
   LCD D4 pin to digital pin 5
   LCD D5 pin to digital pin 4
   LCD D6 pin to digital pin 3
   LCD D7 pin to digital pin 2
   LCD R/W pin to ground
   10K resistor:
   ends to +5V and ground
   wiper to LCD VO pin (pin 3)

  Library originally added 18 Apr 2008
  by David A. Mellis
  library modified 5 Jul 2009
  by Limor Fried (http://www.ladyada.net)
  example added 9 Jul 2009
  by Tom Igoe
  modified 22 Nov 2010
  by Tom Igoe

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/LiquidCrystalSerial
*/

// include the library code:
#include <LiquidCrystal.h>
#include <avr/interrupt.h>
// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(48, 46, 44, 42, 40, 38, 36, 34, 32, 30, 28);
int __counter = 0;



int t = -1;
int m = -1;
int s = -1;
int h = -1;
bool count = false;

//arduino runs the setup function first, then the loop function below
void setup() {
  pinMode(24, OUTPUT); //K
  pinMode(26, OUTPUT); //A
  pinMode(54, OUTPUT); //VSS
  pinMode(52, OUTPUT); //VDD
  pinMode(50, OUTPUT); //Contrasty pin

  // digitalWrite(50, LOW);
  digitalWrite(24, LOW); //Backlight
  digitalWrite(26, HIGH); //Backlight
  digitalWrite(54, LOW); //GND
  digitalWrite(52, HIGH); //VDD
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // initialize the serial communications:
  Serial.begin(9600);
  // Timer0 is used for millis() - we'll just interrupt
  // in the middle and call the compA
  OCR0A = 0x01;
  TIMSK0 |= _BV(OCIE0A);
}

SIGNAL(TIMER0_COMPA_vect) 
{
   __counter++;
   if (__counter > 14){
      digitalWrite(50,HIGH);
      __counter = 0;
   }
   else if (__counter > 3){
      digitalWrite(50, LOW);
   }
}

//Here is where your code goes. Arduino runs this function in an infinite loop after running the setup function
void loop() {

  String str;
  String message = Serial.readString();
  if (message!="")
  {

    if (message.charAt(0) == 't')
    {
     str = message.charAt(1);
     str.concat(message.charAt(2));
     h = str.toInt();

     str = message.charAt(3);
     str.concat(message.charAt(4));
     m = str.toInt();

     s = 0;
    }
    else if (message == "start")
    {
      count = true;
    }
    else if (message == "stop")
    {
      count = false;
    }
  }
  int r;
  if (count)
  {
    if (millis()>t)
    {
      r = millis()-t;
      s+=(r/1000);
    }
    if (s>=60)
    {
      s = 0;
      m+=1;
    }
    if (m>=60)
    {
      m = 0;
      h+=1;
    }
    if (h>=24)
    {
      h=0;
    }
  }



  //displays a String on the lcd screen. You can also print a String object
  if (h==-1)
  {
       
      //return cursor to starting position
      lcd.home();

      //clear the lcd screen
      lcd.clear();
  
     lcd.print("NOT SET"); 
  }
  else
  {
     lcd.clear();
     lcd.print(t);
     //setTimer(h,m,s);
  }
    t = millis();
}
void setTimer(int h, int m, int s)
{
  lcd.home();
  lcd.clear();
  lcd.print(String(h)+":"+String(m)+"::"+String(s));
}



