
   /*************************************************************************************
    * MIT License
    *
    * Copyright (c) 2023 Crunchlabs LLC (IRTurret Control Code)
    * Copyright (c) 2020-2022 Armin Joachimsmeyer (IRremote Library)
   
    * Permission is hereby granted, free of charge, to any person obtaining a copy
    * of this software and associated documentation files (the "Software"), to deal
    * in the Software without restriction, including without limitation the rights
    * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    * copies of the Software, and to permit persons to whom the Software is furnished
    * to do so, subject to the following conditions:
    *
    * The above copyright notice and this permission notice shall be included in all
    * copies or substantial portions of the Software.
    *
    * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
    * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
    * PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
    * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
    * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
    * OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
    *
    ************************************************************************************
    */
   
   //////////////////////////////////////////////////
                  //  LIBRARIES  //
   //////////////////////////////////////////////////
   #include <Arduino.h>
   #include <Servo.h>
   #include "PinDefinitionsAndMore.h" // Define macros for input and output pin etc.
   #include <IRremote.hpp>
   
   
   #define DECODE_NEC  //defines the type of IR transmission to decode based on the remote. See IRremote library for examples on how to decode other types of remote
   
   //defines the specific command code for each button on the remote
   #define left 0x8
   #define right 0x5A
   #define up 0x52
   #define down 0x18
   #define ok 0x1C
   #define cmd1 0x45
   #define cmd2 0x46
   #define cmd3 0x47
   #define cmd4 0x44
   #define cmd5 0x40
   #define cmd6 0x43
   #define cmd7 0x7
   #define cmd8 0x15
   #define cmd9 0x9
   #define cmd0 0x19
   #define star 0x16
   #define hashtag 0xD
   
   //////////////////////////////////////////////////
             //  PINS AND PARAMETERS  //
   //////////////////////////////////////////////////
   Servo yawServo; //names the servo responsible for YAW rotation, 360 spin around the base
   Servo pitchServo; //names the servo responsible for PITCH rotation, up and down tilt
   Servo rollServo; //names the servo responsible for ROLL rotation, spins the barrel to fire darts
   
   int yawServoVal; //initialize variables to store the current value of each servo
   int pitchServoVal = 100;
   int rollServoVal;
   
   //////////////////////////////////////////////////
                  //  S E T U P  //
   //////////////////////////////////////////////////
   void setup() {
       Serial.begin(9600);
   
       yawServo.attach(10); //attach YAW servo to pin 3
       pitchServo.attach(11); //attach PITCH servo to pin 4
       rollServo.attach(12); //attach ROLL servo to pin 5
   
       // Just to know which program is running on my Arduino
       Serial.println(F("START " __FILE__ " from " __DATE__ "\r\nUsing library version " VERSION_IRREMOTE));
   
       // Start the receiver and if not 3. parameter specified, take LED_BUILTIN pin from the internal boards definition as default feedback LED
       IrReceiver.begin(9, ENABLE_LED_FEEDBACK);
   
       Serial.print(F("Ready to receive IR signals of protocols: "));
       printActiveIRProtocols(&Serial);
       Serial.println(F("at pin " STR(9)));
   
   
   
       yawServo.write(90); //setup YAW servo to be STOPPED (90)
       delay(20);
       rollServo.write(90); //setup ROLL servo to be STOPPED (90)
       delay(100);
       pitchServo.write(100); //set PITCH servo to 100 degree position
       delay(100);
       pitchServoVal = 100;
   }
   
   ////////////////////////////////////////////////
                  //  L O O P  //
   ////////////////////////////////////////////////
   
   void loop() {
   
       /*
        * Check if received data is available and if yes, try to decode it.
        * Decoded result is in the IrReceiver.decodedIRData structure.
        *
        * E.g. command is in IrReceiver.decodedIRData.command
        * address is in command is in IrReceiver.decodedIRData.address
        * and up to 32 bit raw data in IrReceiver.decodedIRData.decodedRawData
        */
       if (IrReceiver.decode()) {
   
           /*
            * Print a short summary of received data
            */
           IrReceiver.printIRResultShort(&Serial);
           IrReceiver.printIRSendUsage(&Serial);
           if (IrReceiver.decodedIRData.protocol == UNKNOWN) {
               Serial.println(F("Received noise or an unknown (or not yet enabled) protocol"));
               // We have an unknown protocol here, print more info
               IrReceiver.printIRResultRawFormatted(&Serial, true);
           }
           Serial.println();
   
           /*
            * !!!Important!!! Enable receiving of the next value,
            * since receiving has stopped after the end of the current received data packet.
            */
           IrReceiver.resume(); // Enable receiving of the next value
   
           /*
            * Finally, check the received data and perform actions according to the received command
            */
   
           switch(IrReceiver.decodedIRData.command){ //this is where the commands 
   
               case up://pitch up
                 if(pitchServoVal > 10){//make sure the servo is within rotation limits
                 pitchServoVal = pitchServoVal-8; //decrement the current angle and update
                 pitchServo.write(pitchServoVal);
                 delay(50);
                 //Serial.println("UP");
                 }
                 break;
               
               case down://pitch down
                 if(pitchServoVal < 175){ //make sure the servo is within rotation limits
                 pitchServoVal = pitchServoVal+8;//increment the current angle and update
                 pitchServo.write(pitchServoVal);
                 delay(50);
                 //Serial.println("DOWN");
                 }
                 break;
   
               case left://fast counterclockwise rotation
                 //yawServo.write(165);
                 yawServo.write(180);
                 delay(200);
                 yawServo.write(90);
                 delay(5);
                 //Serial.println("LEFT");
                 break;
               
               case right://fast clockwise rotation
                 //yawServo.write(15);
                 yawServo.write(0);
                 delay(200);
                 yawServo.write(90);
                 delay(5);
                 //Serial.println("RIGHT");
                 break;
               
               case ok: //firing routine 
                 fire();
                 //Serial.println("FIRE");
                 break;
                 
               case star:
                 fireAll();
                 delay(50);
                 break;
   
               case hashtag:
                 spinAndFire();
               break;
   
               default:
               
               break;
   
           }
       }
       delay(5);
   }
   
   
   
    void fire(){ //function for firing a single dart
        rollServo.write(180);//start rotating the servo
        delay(280);//time for approximately 60 degrees of rotation
        rollServo.write(90);//stop rotating the servo
        delay(5);
      }
   
     void fireAll(){
        rollServo.write(180);//start rotating the servo
        delay(1500);//time for 360 degrees of rotation
        rollServo.write(90);//stop rotating the servo
        delay(5);
      }
   
   void spinAndFire() {
     unsigned long startTime = millis();
     pitchServo.write(90);
     yawServoVal = 180;
     yawServo.write(yawServoVal);
     delay(20); // Adjust delay for smoother movement
     while (millis() - startTime < 10000){
       if (millis() % 1000 == 0 && yawServoVal > 90) {
         yawServoVal--;
         yawServo.write(yawServoVal);
       }
       // if (millis() % 2500 == 0 && random(6) == 0) {
       //  fire();
       // }
     }
     yawServo.write(90);
     fireAll();
   }   
