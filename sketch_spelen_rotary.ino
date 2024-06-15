// Define the pins for the rotary encoder and LED
const int encoderPin1 = 10;
const int encoderPin2 = 11;
const int buttonPin = 12;
const int ledPin = 2;
const int redPin = 13;

// Variables to store the current and previous state of the encoder
int lastEncoded = 0;
long encoderValue = 0;
const int sensitivity = 4; // Adjust sensitivity here, higher values make it less sensitive

// Variables for LED flashing
unsigned long previousMillis = 0;
unsigned long interval = 1000; // Initial interval for LED flashing
boolean ledState = HIGH;
boolean redState = LOW;

// Setup function runs once when you press reset or power the board
void setup() {
  // Initialize serial communication at 9600 baud
  Serial.begin(9600);

  // Set the encoder pins as inputs
  pinMode(encoderPin1, INPUT);
  pinMode(encoderPin2, INPUT);
  pinMode(buttonPin, INPUT_PULLUP); // Internal pull-up resistor for button pin
}

// Main loop function
void loop() {
  // Read the state of the encoder pins
  int MSB = digitalRead(encoderPin1);
  int LSB = digitalRead(encoderPin2);

  // Combine the bits into a single value
  int encoded = (MSB << 1) | LSB;
  int sum = (lastEncoded << 2) | encoded;

  // Check if the encoder moved clockwise or counterclockwise
  if (sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) {
    encoderValue += 10;
    Serial.println("Clockwise");
      // Output the encoder value to the serial monitor
  Serial.print("Encoder Value: ");
  Serial.println(encoderValue);
  } else if (sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) {
    encoderValue -= 10;
    Serial.println("Counterclockwise");
      // Output the encoder value to the serial monitor
  Serial.print("Encoder Value: ");
  Serial.println(encoderValue);
  }

  // Update the lastEncoded value for the next iteration
  lastEncoded = encoded;

  // Check if the button is pressed
  if (digitalRead(buttonPin) == LOW) {
    Serial.println("Button pressed");
    delay(500); // Debouncing delay
  }

  // Adjust the flashing interval based on the encoder value
  interval = map(encoderValue, 0, 100, 100, 1000);

  // Flash the LED
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= encoderValue) {
    previousMillis = currentMillis;

    // Toggle LED state
    if (ledState == LOW) {
      ledState = HIGH;
      redState = LOW;
    } else {
      ledState = LOW;
      redState = HIGH;

    }
    digitalWrite(ledPin, ledState);
    digitalWrite(redPin, redState);
  }



  // Add a small delay to debounce the encoder
  delay(sensitivity);
}
