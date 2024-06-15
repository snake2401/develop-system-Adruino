// File: ntc_temperature_sensor_with_pump.ino

const int sensorPin = A1;          // Analoge pin verbonden met de sensor
const int pumpPin = 2;             // Digitale pin verbonden met het pompje
const int seriesResistor = 10000;  // Weerstand van de serieweerstand (10k)
const float supplyVoltage = 5.0;   // Voedingsspanning van de Arduino (5V)

const int nominalResistance = 10000; // Nominale weerstand van de NTC bij 25 graden Celsius
const int nominalTemperature = 25;   // Nominale temperatuur (25 graden Celsius)
const int bCoefficient = 3950;       // Beta-coëfficiënt van de NTC

void setup() {
  Serial.begin(9600);              // Start seriële communicatie op 9600 baud
  pinMode(pumpPin, OUTPUT);        // Stel de pomp pin in als output
  digitalWrite(pumpPin, LOW);      // Zet de pomp uit bij het opstarten
}

void loop() {
  int analogValue = analogRead(sensorPin);  // Lees de analoge waarde van de sensor
  float voltage = analogValue * (supplyVoltage / 1023.0); // Converteer analoge waarde naar spanning

  // Bereken de weerstand van de thermistor
  float resistance = seriesResistor / (1023.0 / analogValue - 1.0);

  // Bereken de stroom door de weerstand
  float current = supplyVoltage / (seriesResistor + resistance);

  // Converteer de weerstand naar temperatuur met de Steinhart-Hart vergelijking
  float steinhart;
  steinhart = resistance / nominalResistance;  // (R/Ro)
  steinhart = log(steinhart);                  // ln(R/Ro)
  steinhart /= bCoefficient;                   // 1/B * ln(R/Ro)
  steinhart += 1.0 / (nominalTemperature + 273.15); // + (1/To)
  steinhart = 1.0 / steinhart;                 // Inverteer
  steinhart -= 273.15;                         // Converteer naar Celsius

  // Print de weerstand, de stroom en de temperatuur naar de seriële monitor
  Serial.print("Weerstand: ");
  Serial.print(resistance);
  Serial.print(" ohm, Stroom: ");
  Serial.print(current * 1000);  // Converteer naar mA
  Serial.print(" mA, Temperatuur: ");
  Serial.print(steinhart);
  Serial.println(" °C");

  // Controleer of de temperatuur hoger is dan 25 graden
  if (steinhart > 25.0) {
    digitalWrite(pumpPin, HIGH); // Zet de pomp aan
  } else {
    digitalWrite(pumpPin, LOW);  // Zet de pomp uit
  }

  delay(1000);  // Wacht 1 seconde voor de volgende meting
}
