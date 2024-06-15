#include <Servo.h>

Servo servoMotor;  // Create a servo object

int servoPin = 5;  // Change this to match the pin you're using for the servo
int speed = 85;   // Change this to adjust the speed of the servo (0 to 180)
int ease = speed;
int easestrength = 2;
void setup() {
  servoMotor.attach(servoPin);  // Attach the servo to the pin
}

void loop() {
  // Move counterclockwise
  servoMotor.writeMicroseconds(1465 + speed); // Adjust the speed with speed variable
  delay(3000);  // Adjust the delay as needed
  int i = 0;
while (ease > 5) {
  ease=ease-easestrength;
    servoMotor.writeMicroseconds(1465 + ease); // Adjust the speed with speed variable
  delay(20);  // Adjust the delay as needed
i++;
};
  ease = speed;

  // Move clockwise
  servoMotor.writeMicroseconds(1465 - speed); // Adjust the speed with speed variable
  delay(3000);  // Adjust the delay as needed
   i = 0;
while (ease > 5) {
  ease=ease-easestrength;
    servoMotor.writeMicroseconds(1465 - ease); // Adjust the speed with speed variable
  delay(20);  // Adjust the delay as needed
i++;
};
  ease = speed;



}
