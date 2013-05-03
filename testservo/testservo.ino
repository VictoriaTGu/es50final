#include <Servo.h>
int motorpin = 9;
int servopin = 6;

Servo myservo;
int num_lower = 0;
int num_raise = 0;

void servospin(int angle){
                  myservo.attach(servopin);
                  myservo.write(angle);
                  delay(100);
                  myservo.detach();
                }



void setup()
{
  /*pinMode(motorpin, OUTPUT);   // sets the pin as output
  pinMode(motorpin2, OUTPUT);*/
  Serial.begin(57600);
  //servospin(120);
  analogWrite(motorpin, 255);
  
  //servospin(100);
  //servospin(100);
  while(true){
    loop();
  }
  
}
void loop()
{
  //servospin(90);
  
  delay(1000);
  
  if(num_raise < 4){
       servospin(80);
       num_raise += 1;
       Serial.print("Raise ");
       Serial.print(num_raise);
       Serial.println();
       Serial.flush();
       
       if(num_lower > 0){
         num_lower -= 1;
       }
     }
     
  delay(1000);
  
  if(num_lower < 4){
       servospin(100);
       num_lower += 1;
       Serial.print("Lower ");
       Serial.print(num_lower);
       Serial.println();
       Serial.flush();
       
       if(num_raise > 0){
         num_raise -= 1;
       }
     }
  //analogWrite(motorpin, 100);
 // analogWrite(motorpin2, 100);
  /*if(num_lower < 3){
                       
     servospin(95);
     num_lower += 1;
   }
   else{
     // nothing
   }
   
  delay(100);*/
}
