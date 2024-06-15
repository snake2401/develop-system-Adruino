
// Including Libs
    #include <Adafruit_GFX.h>    // Core graphics library
    //#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
    #include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
    #include <SPI.h>

// Display settings 
    #define TFT_CS        10
    #define TFT_RST        -1 // Or set to -1 and connect to Arduino RESET pin
    #define TFT_DC         9

  Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

// Rotary settings temperature
    const int encoderPin1 = 2;
    const int encoderPin2 = 3;
    const int buttonPin = 4;
    int lastEncoded = 0;
    long encoderValue = 15.5;
    const int sensitivity = 1; // Adjust sensitivity here, higher values make it less sensitive


void setup(void) {

  Serial.begin(9600);
  Serial.print(F("Hello! ST77xx TFT Test"));
  tft.init(240, 320);           // Init ST7789 320x240
  tft.setRotation(1); // ROTATE TO LANDSCAPE
  tft.fillScreen(ST77XX_BLACK);

  // ROTARY TEMPERATURE INIT
  pinMode(encoderPin1, INPUT);
  pinMode(encoderPin2, INPUT);
  pinMode(buttonPin, INPUT_PULLUP); // Internal pull-up resistor for button pin

  uint16_t time = millis();
  time = millis() - time;

  Serial.println(time, DEC);
  delay(500);


// SETUP INTERFACE GRID
    
    tft.drawLine(0, 119, 320, 119, ST77XX_RED);
    tft.drawLine(0, 120, 320, 120, ST77XX_RED);
    tft.drawLine(159, 0, 159, 240, ST77XX_RED);
    tft.drawLine(160, 0, 160, 240, ST77XX_RED);

  // large block of text
  // tft.fillScreen(ST77XX_BLACK);
  // testdrawtext("Lorem ipsum dolor sit amet, consectetur adipiscing elit. Curabitur adipiscing ante sed nibh tincidunt feugiat. Maecenas enim massa, fringilla sed malesuada et, malesuada sit amet turpis. Sed porttitor neque ut ante pretium vitae malesuada nunc bibendum. Nullam aliquet ultrices massa eu hendrerit. Ut sed nisi lorem. In vestibulum purus a tortor imperdiet posuere. ", ST77XX_WHITE);
  // delay(1000);

  // // tft print function!
  // tftPrintTest();
  // delay(4000);

  // a single pixel
  // tft.drawPixel(tft.width()/2, tft.height()/2, ST77XX_GREEN);
  // delay(500);

  // // line draw test
  // testlines(ST77XX_YELLOW);
  // delay(500);

  // // optimized lines
  // testfastlines(ST77XX_RED, ST77XX_BLUE);
  // delay(500);

  // testdrawrects(ST77XX_GREEN);
  // delay(500);

  // testfillrects(ST77XX_YELLOW, ST77XX_MAGENTA);
  // delay(500);

  // tft.fillScreen(ST77XX_BLACK);
  // testfillcircles(10, ST77XX_BLUE);
  // testdrawcircles(10, ST77XX_WHITE);
  // delay(500);

  // testroundrects();
  // delay(500);

  // testtriangles();
  // delay(500);

  // mediabuttons();
  // delay(500);

  // Serial.println("done");
  // delay(1000);
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
       tft.setTextSize(6);
     tft.fillRect(0,0,158,118, ST77XX_BLACK);
    tft.setCursor(30,40);
      tft.print(encoderValue);
       tft.setTextSize(3);
      tft.print(" C");

  } else if (sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) {
      if (encoderValue > 15 ) {encoderValue -= 1;};
       tft.setTextSize(6);
     tft.fillRect(0,0,158,118, ST77XX_BLACK);
    tft.setCursor(30,40);
    
      tft.print(encoderValue);
       tft.setTextSize(3);
      tft.print(" C");
  }

  // Update the lastEncoded value for the next iteration
  lastEncoded = encoded;

  // Check if the button is pressed
  if (digitalRead(buttonPin) == LOW) {
    encoderValue = 15; 
      tft.setTextSize(3);
      tft.fillRect(0,0,158,118, ST77XX_BLACK);
     tft.setCursor(10,60);
      testdrawtext("reset", ST77XX_WHITE);
  }

}

void testlines(uint16_t color) {
  tft.fillScreen(ST77XX_BLACK);
  for (int16_t x=0; x < tft.width(); x+=6) {
    tft.drawLine(0, 0, x, tft.height()-1, color);
    delay(0);
  }
  for (int16_t y=0; y < tft.height(); y+=6) {
    tft.drawLine(0, 0, tft.width()-1, y, color);
    delay(0);
  }

  tft.fillScreen(ST77XX_BLACK);
  for (int16_t x=0; x < tft.width(); x+=6) {
    tft.drawLine(tft.width()-1, 0, x, tft.height()-1, color);
    delay(0);
  }
  for (int16_t y=0; y < tft.height(); y+=6) {
    tft.drawLine(tft.width()-1, 0, 0, y, color);
    delay(0);
  }

  tft.fillScreen(ST77XX_BLACK);
  for (int16_t x=0; x < tft.width(); x+=6) {
    tft.drawLine(0, tft.height()-1, x, 0, color);
    delay(0);
  }
  for (int16_t y=0; y < tft.height(); y+=6) {
    tft.drawLine(0, tft.height()-1, tft.width()-1, y, color);
    delay(0);
  }

  tft.fillScreen(ST77XX_BLACK);
  for (int16_t x=0; x < tft.width(); x+=6) {
    tft.drawLine(tft.width()-1, tft.height()-1, x, 0, color);
    delay(0);
  }
  for (int16_t y=0; y < tft.height(); y+=6) {
    tft.drawLine(tft.width()-1, tft.height()-1, 0, y, color);
    delay(0);
  }
}

void testdrawtext(char *text, uint16_t color) {
  tft.setCursor(0, 0);
  tft.setTextColor(color);
  tft.setTextWrap(true);
  tft.print(text);
}

void testfastlines(uint16_t color1, uint16_t color2) {
  tft.fillScreen(ST77XX_BLACK);
  for (int16_t y=0; y < tft.height(); y+=5) {
    tft.drawFastHLine(0, y, tft.width(), color1);
  }
  for (int16_t x=0; x < tft.width(); x+=5) {
    tft.drawFastVLine(x, 0, tft.height(), color2);
  }
}

void testdrawrects(uint16_t color) {
  tft.fillScreen(ST77XX_BLACK);
  for (int16_t x=0; x < tft.width(); x+=6) {
    tft.drawRect(tft.width()/2 -x/2, tft.height()/2 -x/2 , x, x, color);
  }
}

void testfillrects(uint16_t color1, uint16_t color2) {
  tft.fillScreen(ST77XX_BLACK);
  for (int16_t x=tft.width()-1; x > 6; x-=6) {
    tft.fillRect(tft.width()/2 -x/2, tft.height()/2 -x/2 , x, x, color1);
    tft.drawRect(tft.width()/2 -x/2, tft.height()/2 -x/2 , x, x, color2);
  }
}

void testfillcircles(uint8_t radius, uint16_t color) {
  for (int16_t x=radius; x < tft.width(); x+=radius*2) {
    for (int16_t y=radius; y < tft.height(); y+=radius*2) {
      tft.fillCircle(x, y, radius, color);
    }
  }
}

void testdrawcircles(uint8_t radius, uint16_t color) {
  for (int16_t x=0; x < tft.width()+radius; x+=radius*2) {
    for (int16_t y=0; y < tft.height()+radius; y+=radius*2) {
      tft.drawCircle(x, y, radius, color);
    }
  }
}

void testtriangles() {
  tft.fillScreen(ST77XX_BLACK);
  uint16_t color = 0xF800;
  int t;
  int w = tft.width()/2;
  int x = tft.height()-1;
  int y = 0;
  int z = tft.width();
  for(t = 0 ; t <= 15; t++) {
    tft.drawTriangle(w, y, y, x, z, x, color);
    x-=4;
    y+=4;
    z-=4;
    color+=100;
  }
}

void testroundrects() {
  tft.fillScreen(ST77XX_BLACK);
  uint16_t color = 100;
  int i;
  int t;
  for(t = 0 ; t <= 4; t+=1) {
    int x = 0;
    int y = 0;
    int w = tft.width()-2;
    int h = tft.height()-2;
    for(i = 0 ; i <= 16; i+=1) {
      tft.drawRoundRect(x, y, w, h, 5, color);
      x+=2;
      y+=3;
      w-=4;
      h-=6;
      color+=1100;
    }
    color+=100;
  }
}

void tftPrintTest() {
  tft.setTextWrap(false);
  tft.fillScreen(ST77XX_BLACK);
  tft.setCursor(0, 30);
  tft.setTextColor(ST77XX_RED);
  tft.setTextSize(1);
  tft.println("Hello World!");
  tft.setTextColor(ST77XX_YELLOW);
  tft.setTextSize(2);
  tft.println("Hello World!");
  tft.setTextColor(ST77XX_GREEN);
  tft.setTextSize(3);
  tft.println("Hello World!");
  tft.setTextColor(ST77XX_BLUE);
  tft.setTextSize(4);
  tft.print(1234.567);
  delay(1500);
  tft.setCursor(0, 0);
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(0);
  tft.println("Hello World!");
  tft.setTextSize(1);
  tft.setTextColor(ST77XX_GREEN);
  tft.println(" Want pi?");
  tft.println(" ");
  tft.print(8675309, HEX); // print 8,675,309 out in HEX!
  tft.println(" Print HEX!");
  tft.println(" ");
  tft.setTextColor(ST77XX_WHITE);
  tft.println("Sketch has been");
  tft.println("running for: ");
  tft.setTextColor(ST77XX_MAGENTA);
  tft.print(millis() / 1000);
  tft.setTextColor(ST77XX_WHITE);
  tft.print(" seconds.");
}

void mediabuttons() {
  // play
  tft.fillScreen(ST77XX_BLACK);
  tft.fillRoundRect(25, 10, 78, 60, 8, ST77XX_WHITE);
  tft.fillTriangle(42, 20, 42, 60, 90, 40, ST77XX_RED);
  delay(500);
  // pause
  tft.fillRoundRect(25, 90, 78, 60, 8, ST77XX_WHITE);
  tft.fillRoundRect(39, 98, 20, 45, 5, ST77XX_GREEN);
  tft.fillRoundRect(69, 98, 20, 45, 5, ST77XX_GREEN);
  delay(500);
  // play color
  tft.fillTriangle(42, 20, 42, 60, 90, 40, ST77XX_BLUE);
  delay(50);
  // pause color
  tft.fillRoundRect(39, 98, 20, 45, 5, ST77XX_RED);
  tft.fillRoundRect(69, 98, 20, 45, 5, ST77XX_RED);
  // play color
  tft.fillTriangle(42, 20, 42, 60, 90, 40, ST77XX_GREEN);
}
