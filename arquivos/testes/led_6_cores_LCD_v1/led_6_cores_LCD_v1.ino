// include the library code:
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(53, 52, 51, 50, 49, 48);

int redPin = 8;
int greenPin = 7;
int bluePin = 6;


void setup()
{
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("hello, world!");
  
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
}

//Before we take a look at the ‘loop’ function, lets look at the last function in the sketch.

void setColor(int red, int green, int blue)
{
analogWrite(redPin, red);
analogWrite(greenPin, green);
analogWrite(bluePin, blue);
}


void setLCD(int c1, int c2, int c3 , char t1)
{
lcd.setCursor(0, 1);
lcd.print("                ");
lcd.setCursor(0, 1);
lcd.print(t1);
}

/* This function takes three arguments, one for the brightness of the red, green and blue LEDs. In each case the number will be in the range 0 to 255. Where 0 means off and 255 means maximum brightness. The function then calls ‘analogWrite’ to set the brightness of each LED.
If you look at the ‘loop’ function you can see that we are setting the amount of red, green and blue light that we want to display and the pausing for a second before moving on to the next color. */

void loop()
{
setColor(255, 0, 0); // red
setLCD(0, 1, 0,'vermelho');
delay(1000);
setColor(0, 255, 0); // green
lcd.setCursor(0, 1);
lcd.print("                ");
lcd.setCursor(0, 1);
lcd.print("verde");
delay(1000);
setColor(0, 0, 255); // blue
lcd.setCursor(0, 1);
lcd.print("                ");
lcd.setCursor(0, 1);
lcd.print("azul");
delay(1000);
setColor(255, 255, 0);// yellow
lcd.setCursor(0, 1);
lcd.print("                ");
lcd.setCursor(0, 1);
lcd.print("amarelo");
delay(1000);
setColor(80, 0, 80); // purple
lcd.setCursor(0, 1);
lcd.print("                ");
lcd.setCursor(0, 1);
lcd.print("magenta");
delay(1000);
setColor(0, 255, 255);// aqua
lcd.setCursor(0, 1);
lcd.print("                ");
lcd.setCursor(0, 1);
lcd.print("ciano");
delay(1000);
}
//Try adding a few colors of your own to the sketch and watch the effect on your LED.
