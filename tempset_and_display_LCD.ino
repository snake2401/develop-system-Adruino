
// include the library code:
#include <LiquidCrystal.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 3, en = 4, d4 = 6, d5 = 7, d6 =8, d7 = 9;
const int encoderPin1 = 2;
const int encoderPin2 = 3;
const int buttonPin = 4;


// Variables to store the current and previous state of the encoder
int lastEncoded = 0;
long encoderValue = 15.5;
const int sensitivity = 1; // Adjust sensitivity here, higher values make it less sensitive

// Variables for LED flashing
unsigned long previousMillis = 0;
unsigned long interval = 1000; // Initial interval for LED flashing


LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
// ROTARY SETUP
  pinMode(encoderPin1, INPUT);
  pinMode(encoderPin2, INPUT);
  pinMode(buttonPin, INPUT_PULLUP); // Internal pull-up resistor for button pin


// LCD SETUP
  lcd.clear();
  lcd.begin(16, 2);
  lcd.setCursor(13, 2);
  lcd.print(encoderValue);
  lcd.print("C");
  lcd.setCursor(13, 2);

}

void loop() {
 // Read the state of the encoder pins
  int MSB = digitalRead(encoderPin1);
  int LSB = digitalRead(encoderPin2);

  // Combine the bits into a single value
  int encoded = (MSB << 5) | LSB;
  int sum = (lastEncoded << 2) | encoded;

  // Check if the encoder moved clockwise or counterclockwise
  if (sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011 ) {
      if (encoderValue < 50 ) {encoderValue += 1;};
      lcd.setCursor(0, 0);
      lcd.setCursor(13, 2);
      if (encoderValue < 10)
      {
        lcd.print(" ");
      }
      lcd.print (encoderValue);
      lcd.setCursor(15, 2);
      lcd.print ("C");

  } else if (sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) {
      if (encoderValue > 0 ) {encoderValue -= 1;};
      lcd.setCursor(0, 0);
      lcd.setCursor(13, 2);
      if (encoderValue < 10)
      {
        lcd.print(" ");
      }
      lcd.print (encoderValue);
      lcd.setCursor(15, 2);
      lcd.print ("C");
  }

  // Update the lastEncoded value for the next iteration
  lastEncoded = encoded;

  // Check if the button is pressed
  if (digitalRead(buttonPin) == LOW) {
    encoderValue = 15;

    for (int i = 0; i < 3; i++) {
    delay(200); // Debouncing delay
      lcd.setCursor(13, 2);
      lcd.print ("   ");
    delay(200); // Debouncing delay
      lcd.setCursor(13, 2);
      if (encoderValue < 10)
      {
        lcd.print(" ");
      }
      lcd.print (encoderValue);
      lcd.setCursor(15, 2);
      lcd.print ("C");
    }
 
  }



}

