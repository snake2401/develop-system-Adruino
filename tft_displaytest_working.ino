#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <SPI.h>

// Definieer de pinnen voor het display
#define TFT_CS        10
#define TFT_RST       9
#define TFT_DC        8

// Maak een display object aan
Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

void setup() {
  // Initialiseer seriële communicatie
  Serial.begin(9600);

  // Initialiseer het display
  tft.init(240, 320); // Breedte en hoogte in pixels
  tft.setRotation(1); // Pas de oriëntatie van het scherm aan (0-3)
  tft.fillScreen(ST77XX_BLACK); // Vul het scherm met zwart

  // Toon een eenvoudige boodschap
  tft.setCursor(1, 1);
  tft.setTextColor(ST77XX_YELLOW);
  tft.setTextSize(4);
  tft.println("Hello, RAF!");
}

void loop() {
  // Hier kun je verder gaan met je code om het display te gebruiken
  tft.setCursor(1, 1);
  tft.println("Hello, World!");
    delay(1000);
    tft.fillScreen(ST77XX_BLACK); // Vul het scherm met zwart

}
