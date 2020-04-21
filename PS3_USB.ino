/*
 Example sketch for the PS3 USB library - developed by Kristian Lauszus
 For more information visit my blog: http://blog.tkjelectronics.dk/ or
 send me an e-mail:  kristianl@tkjelectronics.com
 */

#include <PS3USB.h>
#include <Servo.h>
// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#include <SPI.h>
#endif

USB Usb;
/* You can create the instance of the class in two ways */
//PS3USB PS3(&Usb); // This will just create the instance
PS3USB PS3(&Usb,0x00,0x15,0x83,0x3D,0x0A,0x57); // This will also store the bluetooth address - this can be obtained from the dongle when running the sketch

bool printAngle;
uint8_t state = 0;
int stepper1PWM = 2;
int stepper1DIR = 3;
int stepper2PWM = 4;
int stepper2DIR = 5;

boolean movingForward = true;
boolean movingBackwards = false;
boolean turningRight = false;
boolean turningLeft = false;
boolean worstCaseBreak = false;
Servo myservo;  // create servo object to control a servo

void setup() {
  Serial.begin(115200);
#if !defined(__MIPSEL__)
  while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
#endif
  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
    while (1); //halt
  }
  Serial.print(F("\r\nPS3 USB Library Started"));
  myservo.attach(9);
  //delay(1000);
}
void loop() {
  Usb.Task();

  Serial.println("HALT");
  //myservo.write(0);
  analogWrite(stepper1PWM,0);
  analogWrite(stepper2PWM,0);
  analogWrite(stepper1DIR,0);
  analogWrite(stepper2DIR,0);
  movingForward = false;
  movingBackwards = false;
  turningRight = false;
  turningLeft = false;
  worstCaseBreak = false;
  //delay(200);

  if (PS3.PS3Connected || PS3.PS3NavigationConnected) {
    if (PS3.getAnalogHat(RightHatX) > 137 || PS3.getAnalogHat(RightHatX) < 117 || PS3.getAnalogHat(RightHatY) > 137 || PS3.getAnalogHat(RightHatY) < 117) {
      //Serial.print(F("\r\nLeftHatX: "));
      //Serial.print(PS3.getAnalogHat(LeftHatX));
      //Serial.print(F("\tLeftHatY: "));
      //Serial.print(255 - PS3.getAnalogHat(LeftHatY));

      if (PS3.PS3Connected) { // The Navigation controller only have one joystick

        if(PS3.getAnalogHat(RightHatY) > ((int)(255/2) +20) && PS3.getAnalogHat(RightHatY) < ((int)(255)+5) && PS3.getAnalogHat(RightHatX) < 177 && PS3.getAnalogHat(RightHatX) > 90){
          //if(PS3.getAnalogHat(RightHatX) < 3){
          if(turningLeft == false && turningRight == false && movingBackwards == false){
            movingBackwards = true;
            analogWrite(stepper1DIR,-1);
            analogWrite(stepper2DIR,-1);
            analogWrite(stepper1PWM,(PS3.getAnalogHat(RightHatY) - 3));
            analogWrite(stepper2PWM,(PS3.getAnalogHat(RightHatY) - 3));
            
            Serial.print("Reverse:  ");
            Serial.print(PS3.getAnalogHat(RightHatY));
          //}
          } 
       }
       else if(PS3.getAnalogHat(RightHatY) < ((int)(255/2)-20) && PS3.getAnalogHat(RightHatY) > ((int)(0)-5) && PS3.getAnalogHat(RightHatX) < 177 && PS3.getAnalogHat(RightHatX) > 90){
        //if(PS3.getAnalogHat(RightHatX) < 2){
        if(turningLeft == false && turningRight == false && movingForward == false){
          movingForward = true;
          analogWrite(stepper1DIR,0);
          analogWrite(stepper2DIR,0);
          analogWrite(stepper1PWM,(255 - abs(PS3.getAnalogHat(RightHatY))) );
          analogWrite(stepper2PWM,(255 - abs(PS3.getAnalogHat(RightHatY))) );
          Serial.print("Forward:  ");
          Serial.print(PS3.getAnalogHat(RightHatY));
          //delay(50);
        }
        //}

      }

      //delay(20);
      //+10
      else if(PS3.getAnalogHat(RightHatX) > ((int)(145)+20) && PS3.getAnalogHat(RightHatX) < ((int)(255+5)) && PS3.getAnalogHat(RightHatY) < 170 && PS3.getAnalogHat(RightHatY) > 90){        
       if(movingForward == false && movingBackwards == false && turningLeft == false){
          turningLeft = true;
          analogWrite(stepper1DIR,0);
          analogWrite(stepper2DIR,-1);
          analogWrite(stepper1PWM,(PS3.getAnalogHat(RightHatX) - 3));
          analogWrite(stepper2PWM,(155 - ((PS3.getAnalogHat(RightHatX))+ 100)));
          Serial.print("Turning Right:  ");
          Serial.print("\n");
          Serial.print((155 - (PS3.getAnalogHat(RightHatX))) + 100);
          Serial.print("\t RAW ANALOG_RIGHT_HAT_X: ");
          Serial.print(PS3.getAnalogHat(RightHatX));
        }
       }
       //-10
       else if(PS3.getAnalogHat(RightHatX) < ((int)(100)) && PS3.getAnalogHat(RightHatX) > ((int)(0-5)) && PS3.getAnalogHat(RightHatY) < 170 && PS3.getAnalogHat(RightHatY) > 90){
        if(movingForward == false && movingBackwards == false && turningRight == false){
          turningRight = true;
          analogWrite(stepper1DIR,-1);
          analogWrite(stepper2DIR,0);
          analogWrite(stepper1PWM,(155 - ((PS3.getAnalogHat(RightHatX))- 100)));
          analogWrite(stepper2PWM,(PS3.getAnalogHat(RightHatX)) + 25);
          Serial.print("Turning Left:  ");
          //Serial.print("\n");
          Serial.print((155 - (PS3.getAnalogHat(RightHatX) )) + 100);
          Serial.print("\t RAW ANALOG_RIGHT_HAT_X: ");
          Serial.print(PS3.getAnalogHat(RightHatX)+25);
        }
       }
       else{
              Serial.println("HALT");
              //myservo.write(0);
              analogWrite(stepper1PWM,0);
              analogWrite(stepper2PWM,0);
              analogWrite(stepper1DIR,0);
              analogWrite(stepper2DIR,0);
              movingForward = false;
              movingBackwards = false;
              turningRight = false;
              turningLeft = false;
              worstCaseBreak = false;

        
       }
        Serial.println();
        
        
      }
    }
    if (PS3.getButtonClick(PS))
      Serial.print(F("\r\nPS"));

    if (PS3.getButtonClick(TRIANGLE)){
      Serial.print(F("\r\nTraingle"));
      if(movingForward == false && movingBackwards == false && turningRight == false && turningLeft == false){
        myservo.write(90);
        worstCaseBreak = true;
        delay(100);
      
     }
     if(worstCaseBreak == true){
        myservo.write(0);
        delay(100);
     }
     
     
    }
    if (PS3.getButtonClick(CIRCLE))
      Serial.print(F("\r\nCircle"));
    if (PS3.getButtonClick(CROSS))
      Serial.print(F("\r\nCross"));
    if (PS3.getButtonClick(SQUARE))
      Serial.print(F("\r\nSquare"));

    if (PS3.getButtonClick(UP)) {
      Serial.print(F("\r\nUp"));
      PS3.setLedOff();
      PS3.setLedOn(LED4);
    }
    if (PS3.getButtonClick(RIGHT)) {
      Serial.print(F("\r\nRight"));
      PS3.setLedOff();
      PS3.setLedOn(LED1);
    }
    if (PS3.getButtonClick(DOWN)) {
      Serial.print(F("\r\nDown"));
      PS3.setLedOff();
      PS3.setLedOn(LED2);
    }
    if (PS3.getButtonClick(LEFT)) {
      Serial.print(F("\r\nLeft"));
      PS3.setLedOff();
      PS3.setLedOn(LED3);
    }

  }

}
