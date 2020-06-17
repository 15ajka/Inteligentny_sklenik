/* 
 *  
 *  Demonstration of Controlling Continous Servo (360 servo)
 *  this code allows you to control 360 degree servo by a command from Serial Monitor
 *  
 * Modified by Ahmad Shamshiri for Robojax.com
 * on Sunday July 01, 2018 at 11:09 in Ajax, Ontario, Canada
 * Watch video instruction of this video:https://youtu.be/b_xvu6wWafA
 * Get this code from Robojax.com
 * 
 Original code by BARRAGAN <http://barraganstudio.com>
 This example code is in the public domain.
 modified 8 Nov 2013
 by Scott Fitzgerald
 http://www.arduino.cc/en/Tutorial/Sweep
*/

#include <Servo.h>

Servo myservo;  // create servo object to control a servo
Servo myservo2;
// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position
int incomingByte = 0;   // for incoming serial data

int STOP = 90;
int FRONT = 115;
int BACK = 65;
int FRONT2 = 110;
int BACK2 = 63;
int poc = 0;
int current = FRONT;
int current2 = FRONT2;

void setup() {
  Serial.begin(115200);
  myservo.attach(4);  // attaches the servo on pin 9 to the servo object
  myservo2.attach(6);
  current = FRONT;
  Serial.write(current);
}


void loop() {
  poc++;
  Serial.print(poc);
  if((poc  > 2000 && poc < 3500) || poc > 5500){
    current = STOP;
    current2 = STOP;
  }
  if(poc > 3500 && poc < 5000){
    current = BACK;
  }
  if(poc > 3800 && poc < 5300){
    current2 = BACK2;
  }
  Serial.println(" moving Random"); 
  myservo.write(current);
  myservo2.write(current2);
  if(poc > 10000) poc = 0;
                  
} 
