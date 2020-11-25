#include <MeMCore.h>
#include <Arduino.h>
#include <Wire.h>
#include <SoftwareSerial.h>

double currentTime = 0;
double lastTime = 0;

MeLEDMatrix ledMtx_1(1);
MeUltrasonicSensor ultrasonic_3(3);
MeDCMotor motor_9(9);
MeDCMotor motor_10(10);

// Measures relative time
double
getLastTime()
{
  currentTime = millis() / 1000.0 - lastTime;
  return currentTime;
}

// Defines int values for movement
void 
move(int direction, int speed)
{
  int leftSpeed = 0;
  int rightSpeed = 0;
  switch(direction)
  {
    case 1:
      leftSpeed = speed;
      rightSpeed = speed;
      break;

    case 2:
      leftSpeed = -speed;
      rightSpeed = -speed;
      break;

    case 3:
      leftSpeed = -speed;
      rightSpeed = speed;
      break;

    case 4:
      leftSpeed = speed;
      rightSpeed = -speed;
  }
  motor_9.run((9 == M1) ? -leftSpeed : leftSpeed);
  motor_10.run((10 == M1) ? -rightSpeed : rightSpeed);
}

// Makes the robot do absolutely nothing for x seconds
void 
_delay(float seconds)
{
  long endTime = millis() + seconds * 1000;
  while (millis() < endTime) _loop();
}

// The main program
// Tactic, ride as right as possible, if there is an object, turn left.
void 
setup()
{
  pinMode(A7, INPUT);
  ledMtx_1.setColorIndex(1);
  ledMtx_1.setBrightness(6);
  int readA7 = (analogRead(A7) > 10) ? 0 : 1;
  while (!(0 ^ readA7))
  {
    _loop();
  }
  lastTime = millis() / 1000.0;
  while(1)
  {
    ledMtx_1.drawStr(0, 0 + 7, String(getLastTime()).c_str());
    // Drive robot with slight curve to the right if there is no object within 
    // 20 cm or the robot has been circling for less than 9 seconds
    if (ultrasonic_3.distanceCm() >= 20 && getLastTime() < 9)
    {
      motor_9.run(-255);
      motor_10.run(153);
    }
    else
    {
      // Else, the robot rotates towards the left
      move(2, 127.5);
      _delay(0.5);
      move(2, 0);
          
      move(3, 127.5);
      _delay(0.5);
      move(3, 0);
      lastTime = millis() / 1000.0;
    }
    _loop();
  }
 }

void
_loop()
{
}

void
loop()
{
  _loop();
}
