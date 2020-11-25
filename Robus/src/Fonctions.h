#include <Arduino.h>
#include <LibRobus.h>


//=======================================================DÉCLARATIONS========================================================//
uint32_t soundLenght = 0;
int longif(int moyennea, int moyenneb, int moyennec);

//=========================================================FONCTIONS=========================================================//

//Fonction du ServoMoteur
void activerServo(int id_Servo,int angle_Servo){
    SERVO_Enable(id_Servo);
    SERVO_SetAngle(id_Servo, angle_Servo);
    delay(100);
    SERVO_Disable(id_Servo);
}


//Fonction de détection du son 5kHz
int sound_detection()
{
  if(analogRead(A0) > 520)
    {
      soundLenght += 25;
    }
  else
    {
      soundLenght = 0;
    }
  delay(25);
  if(soundLenght > 1200)
    {
      Serial.println("Son detecte");
      return 0;
    }
  return 1;
}