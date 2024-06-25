// Including Libs
    #include <Adafruit_GFX.h>    // Core graphics library
    #include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
    #include <SPI.h>
    #include <Servo.h>
    #include <Encoder.h>


/*************************************/
/*****      Display settings     *****/
/*************************************/
  #define TFT_CS        10
  #define TFT_RST        -1
  #define TFT_DC         9
  Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);
  #define DARKGRAY 0x528B       // Define custom colors

/*************************************/
/*****  Rotator servo settings.  *****/
/*************************************/
  Servo servoMotor; // Create a Servo object
  const int servoPin = 5; // Servo control pin
  unsigned long startTime; // Variable to store the start time for 30 seconds
  unsigned long directionChangeTime; // Variable to store the start time for direction change
  const long runDuration = 30000; // Duration for which the servo should run (30 seconds)
  const long directionInterval = 4000; // Interval for changing direction (4 seconds)
  boolean servoRunning = false; // Variable to track servo state
  boolean servoDirection = false; // Variable to track servo direction


/*************************************/
/**** Rotary temperature controle ****/
/*************************************/
  const int  temp_encoderPin1 = 3;
  const int  temp_encoderPin2 = 12;
  Encoder tempEnc(temp_encoderPin1, temp_encoderPin2);

  int temperature = 15; // Default temperatuur
  const int minTemp = 15; // Miniale temperatuur
  const int maxTemp = 50; // Maximale temperatuur
  long previousTempPosition = 0;   // Previous state of the encoder


/*************************************/
/*****    NTC sensor settings    *****/
/*************************************/
  const int TEMP_SENSOR_PIN = A1;          // Analog A1 pin
  const int seriesResistor = 10000;  // Resistor series (10k)
  const float supplyVoltage = 5.0;   
  const int nominalResistance = 10000; // Normal resistorvalue when 25°
  const int nominalTemperature = 20;   // Nominale temperatuur (25 graden Celsius)
  const int bCoefficient = 3950;       // Beta-coëfficiënt of the NTC
  int lasttemp; // Sertting var for saving last temperature to measure changes for display refresh;
  int previousRoundedTemp = -1;


/*************************************/
/***** TIMER ROTARY settings *****/
/*************************************/

  const int timer_encoderPin1 = 2;
  const int timer_encoderPin2 = 7;
  const int timer_buttonPin = 8;
  Encoder timerEncoder(timer_encoderPin1, timer_encoderPin2);
  // Variables for timer
  int minutes = 0;
  int seconds = 0;
  bool timerRunning = false;
  unsigned long lastUpdateTime = 0;

  // Previous state of the encoder
  long previousPosition = 0;
  bool lastButtonState = HIGH;
  bool currentButtonState = HIGH;
  long timerbackground;


/*************************************/
/*****      RELAIS settings      *****/
/*************************************/
  const int heaterRelayPin = 12;
  const int pumpRelayPin = 13;
  const unsigned long pumpExtraTime = 5000; // 5 seconden langer
  unsigned long heaterOffTime;
  bool heaterActive = false;
  bool pumpActive = false;
  bool pumpDelayActive = false;
  bool heater = false; // Deze variabele kun je ergens anders in de code wijzigen

/*************************************/
/*****      BUZZER settings      *****/
/*************************************/
  const int buzzerPin = 1;
  bool buzzerActive = false;
  bool buzzerState = false;
  const unsigned long buzzerDuration = 3000; // 3 seconden
  const unsigned long buzzerInterval = 500; // 0,5 seconde
  unsigned long buzzerStartTime;
  unsigned long lastBuzzerToggleTime;  


void setup(void) {
  pinMode(heaterRelayPin, OUTPUT);
  pinMode(pumpRelayPin, OUTPUT);
  digitalWrite(heaterRelayPin, LOW); // Zet het heater relais uit bij het opstarten
  digitalWrite(pumpRelayPin, LOW);   // Zet het pump relais uit bij het opstarten

  Serial.begin(9600); // Initialize serial communication for debugging (optional)

  servoMotor.attach(servoPin); // Attach the servo to the control pin
  pinMode(timer_buttonPin, INPUT_PULLUP); // Attach timer rotary to pin
  tempEnc.write(0); // Reset encoder position

  // SETUP INTERFACE GRID
  tft.init(240, 320);           // setting the resolution of display to 320x240
  tft.setRotation(1);           // ROTATE TO LANDSCAPE
  tft.fillScreen(ST77XX_BLACK); // Fill screen black
  tft.drawLine(0, 119, 320, 119, ST77XX_WHITE);
  tft.drawLine(0, 120, 320, 120, ST77XX_WHITE);
  tft.drawLine(0, 121, 320, 121, ST77XX_WHITE);
  tft.drawLine(0, 122, 320, 122, ST77XX_WHITE);
  tft.drawLine(159, 0, 159, 119, ST77XX_WHITE);
  tft.drawLine(160, 0, 160, 119, ST77XX_WHITE);
  tft.drawLine(161, 0, 161, 119, ST77XX_WHITE);
  tft.drawLine(162, 0, 162, 119, ST77XX_WHITE);

  // SETUP TEMPERATURE BLOCK
  tft.fillRect(0,94,159,24, DARKGRAY);
  tft.setCursor(49,100);
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(2);
  tft.print("timer");

  // SETUP TIMER BLOCK
  tft.fillRect(163,94,159,25, DARKGRAY);
  tft.setCursor(179,100);
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(2);
  tft.print("temperature"); 

   // SETUP START/STOP BLOCK
  tft.fillRect(0,215,320,25, DARKGRAY);
  tft.setCursor(100,220);
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(2);
  tft.print("start/stop");

  // Buzzer setup
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin, LOW);      // Zet de buzzer uit bij het opstarten

}

void loop() {
  unsigned long currentMillis = millis(); // Get the current time
  
  handleButtonPress();
  if (!timerRunning) {
    readTimerEncoder();
  }
  if (timerRunning) {
    updateTime();
  }
  
  updateTimerBackground();
  printTime();

  readTemperatureEncoder();


  if (timerRunning == 0) {
        servoMotor.writeMicroseconds(1475); // Full speed in one direction
        servoMotor.detach();
  }
  
    float actualTemperature = round(readTemperatureSensor());
  printActualTemperature(actualTemperature);
  checkTemperatureAndTriggerRelay(actualTemperature);

// resetDisplay('temp');
  // Start the servo and record the start time if it's not already running
  if (!servoRunning && timerRunning == 1) {
    servoMotor.attach(servoPin); // Attach the servo to the control pin
    servoRunning = true;
    startTime = currentMillis;
    directionChangeTime = currentMillis; // Initialize direction change timer
    servoDirection = true; // Start with one direction
    servoMotor.write(180); // Full speed in one direction
    Serial.println("Servo started");
  }


  // Change servo direction every 4 seconds
  if (servoRunning && (currentMillis - directionChangeTime >= directionInterval)) {
    servoDirection = !servoDirection; // Toggle the servo direction
    directionChangeTime = currentMillis; // Reset direction change timer
    
    if (servoDirection) {
      servoMotor.write(180); // Full speed in one direction
      Serial.println("Servo direction: Forward");
    } else {
      servoMotor.write(0); // Full speed in the opposite direction
      Serial.println("Servo direction: Reverse");
    }
  }

  manageRelays();

}

// Set timer function using rotary encoder
void readTimerEncoder() {
  long newPosition = timerEncoder.read() / 4;  // Dividing by 4 to slow down changes

  if (newPosition < 0) {
    newPosition = 0;  // Prevent negative timer values
    timerEncoder.write(0);   // Reset encoder position to 0
  }

  if (newPosition != previousPosition) {
    previousPosition = newPosition;

    // Convert encoder position to minutes and seconds
    minutes = (newPosition / 60) % 60;
    seconds = newPosition % 60;
  }
}

// Start - stop timer using rotary timer click
void handleButtonPress() {
  currentButtonState = digitalRead(timer_buttonPin);
  if (lastButtonState == HIGH && currentButtonState == LOW) {
    // Button was just pressed
    timerRunning = !timerRunning;
    // servoRunning = !servoRunning;
    
  }
  lastButtonState = currentButtonState;
}

// update running time function
void updateTime() {
  unsigned long currentTime = millis();
  if (currentTime - lastUpdateTime >= 1000) {
    lastUpdateTime = currentTime;
    if (seconds > 0) {
      seconds--;
    } else if (minutes > 0) {
      minutes--;
      seconds = 59;
    } else {
      // Timer finished
      timerRunning = false;
      servoRunning = false;
      Serial.println("Timer Finished!");
    }
  }
}


// print time to display function
void printTime() {
  static int lastPrintedMinutes = -1;
  static int lastPrintedSeconds = -1;

  if (minutes != lastPrintedMinutes || seconds != lastPrintedSeconds) {
    lastPrintedMinutes = minutes;
    lastPrintedSeconds = seconds;
  if (timerRunning) {
  resetDisplay('start');

    tft.setCursor(20,146);
    tft.setTextSize(7);   
    if (minutes < 10) tft.print("0");
    tft.print(minutes);
    tft.print(":");
    if (seconds < 10) tft.print("0");
    tft.print(seconds);

    tft.fillCircle(273, 175, 18, ST77XX_RED);


  }
  else {
      resetDisplay('timer');

    tft.setCursor(22,45);
    tft.setTextSize(4);   
    if (minutes < 10) tft.print("0");
    tft.print(minutes);
    tft.print(":");
    if (seconds < 10) tft.print("0");
    tft.print(seconds);
    }
  }
}


//set temperature function using rotary encoder
void readTemperatureEncoder() {
  long newTempPosition = tempEnc.read() / 4;  // Dividing by 4 to slow down changes

  if (newTempPosition != previousTempPosition) {
    if (newTempPosition > previousTempPosition) {
      temperature++; // Increase temperature
    } else {
      temperature--; // Decrease temperature
    }

    // Constrain the temperature within the min and max limits
    temperature = constrain(temperature, minTemp, maxTemp);

    previousTempPosition = newTempPosition; // Update previous position

    // Print the updated temperature
  resetDisplay('temp');
    tft.setCursor(190,40);
  tft.setTextSize(6);   
  tft.print(temperature);
  tft.setTextSize(3);
  tft.print(" C");

  }
}



void updateTimerBackground() {
  if (timerRunning) {
    timerbackground = ST77XX_RED;   // Turn on red LED
  } else {
    timerbackground = ST77XX_BLACK;   // Turn on red LED
  }
}


// FCUNTION TO RESET AREAS ON THE HMI DISLPAT
void resetDisplay(long area) {
  switch(area) {
  case 'timer':
    tft.fillRect(0,30,158,60, ST77XX_BLACK);
    break;
  case 'temp':
    tft.fillRect(163,30,320,60, ST77XX_BLACK);
    break;

  case 'start':
    tft.fillRect(0,124,320,90, timerbackground);
    break;

  }
}

float readTemperatureSensor() {
  int analogValue = analogRead(TEMP_SENSOR_PIN);
  float resistance = (1023.0 / analogValue - 1.0) * seriesResistor;
  float steinhart;
  steinhart = resistance / nominalResistance;           // (R/Ro)
  steinhart = log(steinhart);                           // ln(R/Ro)
  steinhart /= bCoefficient;                            // 1/B * ln(R/Ro)
  steinhart += 1.0 / (nominalTemperature + 273.15);     // + (1/To)
  steinhart = 1.0 / steinhart;                          // Invert
  steinhart -= 273.15;                                  // convert to Celsius

  return steinhart;
}

void printActualTemperature(float temperature) {
  int roundedTemp = round(temperature);

  // Only print if the rounded temperature has changed
  if (roundedTemp != previousRoundedTemp) {
    previousRoundedTemp = roundedTemp;

       tft.setCursor(190,10);
  tft.setTextSize(2);   
  tft.print(roundedTemp);
  tft.print(" C");

  }



}

void checkTemperatureAndTriggerRelay(float actualTemperature) {
  if (actualTemperature > temperature) {
        tft.fillCircle(300, 15, 7, ST77XX_BLUE);
        heater = false;

  } else if (actualTemperature < temperature) {
        tft.fillCircle(300, 15, 7, ST77XX_RED);
        heater = true;
        


  } else {tft.fillCircle(300, 15, 7, ST77XX_GREEN);
          heater = false;
}

 Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(actualTemperature);
  Serial.println("°C");

}

void triggerRelay(int relayPin, bool state) {
  digitalWrite(relayPin, state ? HIGH : LOW);
}

void manageRelays() {
  unsigned long currentMillis = millis();

  if (heater) {
    if (!heaterActive) {
      // Zet het heater relais aan
      triggerRelay(heaterRelayPin, true);
      heaterActive = true;
    }

    if (!pumpActive) {
      // Zet het pump relais aan
      triggerRelay(pumpRelayPin, true);
      pumpActive = true;
    }
  } else {
    if (heaterActive) {
      // Zet het heater relais uit
      triggerRelay(heaterRelayPin, false);
      heaterActive = false;
      // Noteer de tijd wanneer het heater relais is uitgezet
      heaterOffTime = currentMillis;
      // Start de 5 seconden vertraging voor het pump relais
      pumpDelayActive = true;
    }
  }

  // Controleer of het tijd is om het pump relais uit te schakelen
  if (pumpDelayActive && (currentMillis - heaterOffTime >= pumpExtraTime)) {
    triggerRelay(pumpRelayPin, false);
    pumpActive = false;
    pumpDelayActive = false;
  }
}


// trigger the buzzer pin
void toggleBuzzer() {
  buzzerState = !buzzerState;
  digitalWrite(buzzerPin, buzzerState ? HIGH : LOW);
}

// manager buzzertrigger and timer to set for 3s
void manageBuzzer() {
  unsigned long currentMillis = millis();

  if (buzzerActive) {
    if (currentMillis - buzzerStartTime >= buzzerDuration) {
      digitalWrite(buzzerPin, LOW); // Zet de buzzer uit
      buzzerActive = false;
    } else if (currentMillis - lastBuzzerToggleTime >= buzzerInterval) {
      // Toggle de buzzer om de 0,5 seconde
      toggleBuzzer();
      lastBuzzerToggleTime = currentMillis;
    }
  }
}
