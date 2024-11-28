#include "Stepper_28BYJ_48.h"
#include "IRremote.h"
//#include "RTClib.h"
Stepper_28BYJ_48 motor_rot(11,10,9,8);
IRrecv receiver(2);
unsigned short int photo_val,led_val=0,prev_p_val,mot_val=0,prev_mot_val=0;
bool man=true;

//Intervale de valori:
//fotorezistor:0-1023
//led:0-255
//motor in trepte:0-5100

void setup() {
    Serial.begin(9600); 
    pinMode(2,INPUT);
    receiver.enableIRIn();
    receiver.blink13(true);
}

void loop() {
    //citire semnale telecomanda
    if(receiver.decode() ){
      //mod manual 
      if(receiver.decodedIRData.decodedRawData == 0xB847FF00 ){
        man=true;
        mot_val=0;
        led_val=0;
      }
      if(man){

      //modificare luminozitate LED
        if(receiver.decodedIRData.decodedRawData==0xF807FF00 && led_val-10>=0){
          led_val-=10;
          analogWrite(6,led_val);
      }
        if(receiver.decodedIRData.decodedRawData==0xEA15FF00 && led_val+10<=255){
          led_val+=10;
          analogWrite(6,led_val);
        }
  
      //modificare pozitie jaluzele 
        if(receiver.decodedIRData.decodedRawData==0xE916FF00 && mot_val-500>=0){
          mot_val-=200;
          motor_rot.step(mot_val);
          }
        if(receiver.decodedIRData.decodedRawData==0xE619FF00 && mot_val+500<=5100 ){
          mot_val+=200;
          motor_rot.step(mot_val);
        }
        

      //valori
        Serial.print("Valoare led: ");
        Serial.println(led_val);
        
        Serial.print("Valoare motor: ");
        Serial.println(mot_val);

        Serial.print("\n");
        
      }
     
      
      //mod automat 
      if(receiver.decodedIRData.decodedRawData == 0xBC43FF00){
        man=false;
        prev_mot_val=0;
      }
       
      receiver.resume();
     }
      if(!man){
        //valorile sa fie citite din 5 in 5 secunde pentru simulare(in utilizare practica din 2 in 2 ore )
        delay(5000);

        mot_val=map(photo_val,0,1023,0,5100);
        analogWrite(6,led_val);

        if(photo_val>=700)
          led_val=0;
        else if(photo_val<=100)
          led_val=255;
        else 
          led_val=map(photo_val,800,100,0,255);

        //citire valori fotorezisotr
        photo_val=analogRead(A1);
      
        if(prev_mot_val<mot_val)
          motor_rot.step(mot_val - prev_mot_val);
         else 
           motor_rot.step(-(prev_mot_val - mot_val));
        
        //parte de valori==============================
        Serial.print("Valoare fotorezistor:");
        Serial.println(photo_val);

        Serial.print("Valoare led: ");
        Serial.println(led_val);
        
        Serial.print("Valoare anterioara motor: ");
        Serial.println(prev_mot_val);

        Serial.print("Valoare motor: ");
        Serial.println(mot_val);

        Serial.print("\n");
         //============================================

          prev_mot_val=mot_val;
        }
      
    
} 