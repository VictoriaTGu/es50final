Variables of note: 
servoSpeed      // controls speed of servo


/* PPA: Ping Pong Assassins
* Alisa Nguyen and Vanessa Tan
* ES50 Spring 2012
*
* Arduino sketch controlling two motors and a Servo
* continuously
*/
#include <Servo.h>
/* Define pins */
int motorPin = 11; 
int motorPin2 = 3; 
int servoPin = 6;
/* Set up motor variables */
int onSpeed = 150; // define pulsewidth (0-255)
int rawSerial;
/* Set up servo variables */
Servo contServo;
/* WE’LL WANT TO BE ABLE TO SPEED THIS UP */
int servoSpeed = 20; // 0 = continous in one direction. 90 indicates stop, 
180 is other direction
void setup() {
pinMode(motorPin, OUTPUT); 
pinMode(motorPin2, OUTPUT); 
contServo.attach(servoPin);
// Open serial connection
Serial.begin(9600);
}9
void loop() {
// Get serial input from Processing GUI
if (Serial.available()) {
rawSerial = Serial.read();
// Convert serial input percentage (0-100) to speed (0-255)
onSpeed = map(rawSerial, 0,100, 0,255); 
}
// Set speed
analogWrite(motorPin, onSpeed);
analogWrite(motorPin2, onSpeed);
contServo.write(servoSpeed);
}
// CHANGE THIS FILE TO BE SOMETHING THAT WE COME UP WITH, TO CONTROL SPEED

SliderSerial_Processing.pde
/* PPA: Ping Pong Assassins
* Alisa Nguyen and Vanessa Tan
* ES50 Spring 2012
*
* Processing sketch - creates a GUI to control motor speed,
* frequency of launch, and launch angle 
*/
import controlP5.*;
import processing.serial.*;
ControlP5 controlP5;
Serial serial;
void setup() {
// Draw the GUI window
size(400,350);
String port = "/dev/ttyS42";
serial = new Serial(this, port, 9600);
controlP5 = new ControlP5(this); 
// Add a vertical slider control
//("MOTOR", min,max, startpos, xpos,ypos, width,height);
controlP5.addSlider("MOTOR", 0,100, 0, 190,50, 20,200);
// Configure the slider properties
Slider s1 = (Slider)controlP5.controller("MOTOR");
s1.setSliderMode(Slider.FLEXIBLE);
s1.setNumberOfTickMarks(21);
s1.showTickMarks(true);
s1.snapToTickMarks(true);
}10
void draw() {
background(0); 
}
void MOTOR(float motorValue) {
// Grab slider value (0-100) and send to Arduino
int speed = round(motorValue);
serial.write(speed);
}
