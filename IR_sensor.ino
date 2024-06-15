// File path: ir_sensor_object_detection.ino

// Define the analog pin connected to the IR sensor
const int irSensorPin = A3;

// Threshold to determine object detection (adjust based on your sensor's characteristics)
const int threshold = 290;

void setup() {
  // Start serial communication at 9600 baud rate
  Serial.begin(9600);
}

void loop() {
  // Read the value from the IR sensor
  int sensorValue = analogRead(irSensorPin);
  
  // Check if the sensor value is above or below the threshold
  if (sensorValue < threshold) {
    Serial.print("Object detected!");
    Serial.print(sensorValue);
    Serial.println();
  } else {
    Serial.print("No Object detected!");
    Serial.print(sensorValue);
    Serial.println();
  }
  
  // Wait for a short period before the next read
  delay(500);
}
