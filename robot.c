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


double //Measures relative time
getLastTime()
{
  return currentTime = millis() / 1000.0 - lastTime;
}
 
void //Defines int values for movement
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
    break;
  }

 motor_9.run((9) == M1 ? -(leftSpeed) : (leftSpeed));
  motor_10.run((10) == M1 ? -(rightSpeed) : (rightSpeed));
}

void //Makes the robot do absolutely nothing for x seconds
_delay(float seconds)
{
  long endTime = millis() + seconds * 1000;
  while(millis() < endTime) _loop();
}

void //THE program
setup()
{
  pinMode(A7, INPUT);
  ledMtx_1.setColorIndex(1);
  ledMtx_1.setBrightness(6);
  while(!((0 ^ (analogRead(A7) > 10 ? 0 : 1))))
  {
    _loop();
  }
  lastTime = millis() / 1000.0;
  while(1)
  {
      ledMtx_1.drawStr(0, 0 + 7, String(getLastTime()).c_str());
      /*
       If there is no object within 20 cm
       or the robot has been circling for less than 9 seconds…
      */
      if((!((ultrasonic_3.distanceCm() < 20)))  &&  (getLastTime() < 9))
      {
          //… the robot drives with a slight curve towards the right
          motor_9.run(-1 * 100/100.0*255);
          motor_10.run(60/100.0*255);

      }
      else
      {
          //… else, the robot rotates towards the left.
          move(2, 50 / 100.0 * 255);
          _delay(0.5);
          move(2, 0);
          
          move(3, 50 / 100.0 * 255);
          _delay(0.5);
          move(3, 0);
          lastTime = millis() / 1000.0;

      }

      _loop();
  }
/*
 In short, the "tactic" is to hug the rightmost barrier
 without making circles for an eternity.
 */
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
