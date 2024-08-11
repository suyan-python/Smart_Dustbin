#include <Servo.h>

#include <Servo.h>
#include <HCSR04.h>
HCSR04 man_sensor(6, 7);  //initialisation class HCSR04 (trig pin , echo pin)
HCSR04 garbage_sensor(4, 5);
Servo garbage_motor;  // create servo object to control a servo


void setup() {

  Serial.begin(9600);
  garbage_motor.attach(9);  // attaches the servo on pin 9 to the servo object
}

void rotate_motor(Servo* motor, int angle) {

  if (angle < 0) {
    for (int pos = 180; pos >= 0; pos -= 1) {  // goes from 0 degrees to 180 degrees
                                               // in steps of 1 degree
      motor->write(pos);                       // tell servo to go to position in variable 'pos'
      delay(15);                               // waits 15 ms for the servo to reach the position
    }

  } else {

    for (int pos = 0; pos <= 180; pos += 1) {  // goes from 0 degrees to 180 degrees
                                               // in steps of 1 degree
      motor->write(pos);                       // tell servo to go to position in variable 'pos'
      delay(15);                               // waits 15 ms for the servo to reach the position
    }
  }
}
int prev_angle = 0;
int new_angle = 0;

void loop() {
  while (Serial.available()) {
    String x = Serial.readString();
    if (x == "G-D\n") send_garbage_level();
  };
  float dis = man_sensor.dist();  // return curent distance in serial
  if (dis != 0.0 && dis < 30.0) new_angle = 180;
  else new_angle = 0;
  if (prev_angle != new_angle) {
    garbage_motor.write(prev_angle);
    prev_angle = new_angle;
  }

  //  Serial.println("Current Wastelevel :");
  //  Serial.println(garbage_sensor.dist());
  //Serial.println("Object in distance :");
  //Serial.println(dis);
  delay(1000);  // we suggest to use over 60ms measurement cycle, in order to prevent trigger signal to the echo signal.
                //garbagelevel();
}

void send_garbage_level() {
  float lev = garbage_sensor.dist();
  if (lev > 0 && lev < 30) {
    int per = (lev / 30) * 100;
    Serial.print(per);
    delay(1000);
  }
}

void garbagelevel() {
  float lev = garbage_sensor.dist();
  if (lev > 0 && lev < 30) {
    int per = (lev / 30) * 100;
    Serial.println("Space left in dustbin:");
    Serial.print(per);
    Serial.println("%");
    delay(1000);
  }
}