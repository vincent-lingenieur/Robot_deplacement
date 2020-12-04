#include <Arduino.h>
#include <LibRobus.h>
#include <Fonctions.h>
//0=moteur de gauche et 1=moteur de droite

#define Servo1 1
bool dessin;
String incomingByte = "";

int nbLettre = 0;


void setup()
{
  // put your setup code here, to run once:
  BoardInit();
  Serial.begin(9600);
  Serial1.begin(9600);

}

float Angle(float theta)
{
  // theta (entier): angle en degré à laquelle le robot doit virer

  float nbLecture = 0;
  nbLecture = (7979.002624 * (theta / 360));
  //Serial.println(nbLecture);
  return nbLecture;
}
// appeler virage pour tourner sur soi-même selon l'angle "angle". 0 = virage à gauche. 1 = virage à droite
void virage(int sens, float angle)
{
  // sens (entier): 0 pour virer à gauche et 1 pour virer à droite
  // angle (entier): angle du virage en degré
  float lecture = (float)ENCODER_Read(0);
  while ((float)ENCODER_Read(0) <= (lecture + Angle(angle)) && (float)ENCODER_Read(0) >= ((lecture - Angle(angle))))
  {
    // Serial.println(virageSelonAngle(angle));
    if (sens == 1)
    {
      MOTOR_SetSpeed(0, 0.20);
      MOTOR_SetSpeed(1, -0.20);
    }
    else if (sens == 0)
    {
      MOTOR_SetSpeed(0, -0.20);
      MOTOR_SetSpeed(1, 0.20);
    }
  }
  MOTOR_SetSpeed(0, 0);
  MOTOR_SetSpeed(1, 0);
  ENCODER_Reset(1);
  ENCODER_Reset(0);
  delay(100);
}
// appeler lignerDroite pour avancer selon la distance en mètre "distance".
void tourner(int sens, float angle)
{
  float distance,distancelu = 0;
  if (sens == 0)
  {
    distance = (19*3.1416*(angle/360))*300;
   while(distance > distancelu)
    {
      delay(10);
      MOTOR_SetSpeed(1,0.25);
      distancelu = ENCODER_Read(1);
    }
  }
   if (sens == 1)
  {
    distance = (19*3.1416*(angle/360))*300;
   while(distance  > distancelu)
    {
      delay(10);
      MOTOR_SetSpeed(0,0.25);
      distancelu = ENCODER_Read(0);
    }
  }
  MOTOR_SetSpeed(0, 0);
  MOTOR_SetSpeed(1, 0);
  ENCODER_Reset(1);
  ENCODER_Reset(0);
  delay(100);
} 

void tournerParEnArriere(int sens, float angle)
{
  float distance,distancelu = 0;
  if (sens == 0)
  {
    distance = (19*3.1416*(angle/360))*300;
   while(distance > distancelu)
    {
      delay(10);
      MOTOR_SetSpeed(1,-0.25);
      distancelu = ENCODER_Read(1);
    }
  }
   if (sens == 1)
  {
    distance = (19*3.1416*(angle/360))*300;
   while(distance < distancelu)
    {
      delay(10);
      MOTOR_SetSpeed(0,-0.25);
      distancelu = ENCODER_Read(0);
    }
  MOTOR_SetSpeed(0, 0);
  MOTOR_SetSpeed(1, 0);
  ENCODER_Reset(1);
  ENCODER_Reset(0);
  delay(100);
  }
}

void brasbouge()
{
  activerServo(Servo1, 116);
  activerServo(Servo1, 125);
}

void ligneDroite(float distance)
{
  //Pour aller en ligne droite sur une distance déterminée en mètre
  float borne = 0;
  borne = distance * 13367;
  float kp = 0.0007;
  float ki = 0.00003;
  float errorp = 0;
  float errori = 0;
  
  float speedright = 0.25;
  float speedleft = 0.25;
  float adjust = 0;
  
  float distancep = 0;
  int nbcycle = 0;
  MOTOR_SetSpeed(0, 0.10);
  MOTOR_SetSpeed(1, 0.10);
  delay(50);
  MOTOR_SetSpeed(0, 0.20);
  MOTOR_SetSpeed(1, 0.20);
  delay(50);

  MOTOR_SetSpeed(1, speedright);
  MOTOR_SetSpeed(0, speedleft);

  while (distancep < borne)
  {
    brasbouge();
    nbcycle++;
    float valeur_voulue = ENCODER_ReadReset(0);
    float valeur_reel = ENCODER_ReadReset(1);
    distancep += valeur_reel;
    errorp = valeur_voulue - valeur_reel;
    errori = ((nbcycle * valeur_voulue) - distancep);
    adjust = (errorp * kp) + (errori * ki);
    MOTOR_SetSpeed(1, (speedright + adjust));   
  }

  MOTOR_SetSpeed(0, 0.20);
  MOTOR_SetSpeed(1, 0.20);
  delay(50);
  MOTOR_SetSpeed(0, 0.10);
  MOTOR_SetSpeed(1, 0.10);
  delay(50);
  MOTOR_SetSpeed(0, 0);
  MOTOR_SetSpeed(1, 0);
  delay(100);
  ENCODER_Reset(0);
  ENCODER_Reset(1);
}

void Avancer(float distance)
{
  //Pour aller en ligne droite sans que le crayon fasse des pointiller
  float borne = 0;
  borne = distance * 13367;
  float kp = 0.0005;
  float ki = 0.00003;
  float errorp = 0;
  float errori = 0;
  
  float speedright = 0.25;
  float speedleft = 0.25;
  float adjust = 0;
  
  float distancep = 0;
  int nbcycle = 0;
  MOTOR_SetSpeed(0, 0.10);
  MOTOR_SetSpeed(1, 0.10);
  delay(50);
  MOTOR_SetSpeed(0, 0.20);
  MOTOR_SetSpeed(1, 0.20);
  delay(50);

  MOTOR_SetSpeed(1, speedright);
  MOTOR_SetSpeed(0, speedleft);

  while (distancep < borne)
  {
    nbcycle++;
    delay(200);
    float valeur_voulue = ENCODER_ReadReset(0);
    float valeur_reel = ENCODER_ReadReset(1);
    distancep += valeur_reel;
    errorp = valeur_voulue - valeur_reel;
    errori = ((nbcycle * valeur_voulue) - distancep);
    adjust = (errorp * kp) + (errori * ki);
    MOTOR_SetSpeed(1, (speedright + adjust));   
  }

  MOTOR_SetSpeed(0, 0.20);
  MOTOR_SetSpeed(1, 0.20);
  delay(50);
  MOTOR_SetSpeed(0, 0.10);
  MOTOR_SetSpeed(1, 0.10);
  delay(50);
  MOTOR_SetSpeed(0, 0);
  MOTOR_SetSpeed(1, 0);
  delay(100);
  ENCODER_Reset(0);
  ENCODER_Reset(1);
}

void courbe(float angle, float rayon, int sens)
// appeler courbe pour tourner selon un arc de cercle. distance à parcourir en degré "angle" d'un cercle de rayon "rayon" en cm.
// 0 = virage vers la gauche. 1 = virage vers la droite.
{
  float kp = 0.00055;
  float ki = 0.00003;
  float errorp;
  float errori;
  float speedright = 0.15;
  float speedleft = 0.15;
  float adjust;
  float distance = 0;
  int nbcycle = 0;
  float distancegauche = 0;
  float distancedroite = 0;
  if (sens == 1)
  {
    distancegauche = (2 * 3.1416 * (rayon + 19) * (angle / 360));
    distancedroite = (2 * 3.1416 * (rayon) * (angle / 360));
  }
  if (sens == 0)
  {
    distancegauche = (2 * 3.1416 * (rayon) * (angle / 360));
    distancedroite = (2 * 3.1416 * (rayon + 19) * (angle / 360));
  }
  while (distance < ((distancedroite*13367)/100))
  {
    brasbouge();
    MOTOR_SetSpeed(1, (distancedroite / distancegauche) * speedright);
    nbcycle++;
    MOTOR_SetSpeed(0, speedleft);
    float valeur_voulue = ((distancedroite / distancegauche) * ENCODER_Read(0));
    float valeur_reel = ENCODER_Read(1);
    distance += valeur_reel;
    errorp = valeur_voulue - valeur_reel;
    errori = ((nbcycle * valeur_voulue) - distance);
    adjust = (errorp * kp) + (errori * ki);
    MOTOR_SetSpeed(1, (speedright + adjust));
    ENCODER_Reset(0);
    ENCODER_Reset(1);
  }
  MOTOR_SetSpeed(0, 0.20);
  MOTOR_SetSpeed(1, 0.20);
  delay(50);
  MOTOR_SetSpeed(0, 0.10);
  MOTOR_SetSpeed(1, 0.10);
  delay(50);
  MOTOR_SetSpeed(0, 0);
  MOTOR_SetSpeed(1, 0);
  delay(50);
}

void petiteCourbe(float angle, float rayon, int sens)
{
  float kp = 0.0006;
  float ki = 0.00006;
  float errorp;
  float errori;
  float speedright = 0.15;
  float speedleft = 0.15;
  float adjust;
  float distance = 0;
  int nbcycle = 0;
  float distancegauche = 0;
  float distancedroite = 0;
  if (sens == 1)
  {
    distancegauche = (2 * 3.1416 * (rayon + 19) * (angle / 360));
    distancedroite = (2 * 3.1416 * (rayon) * (angle / 360));
  }
  if (sens == 0)
  {
    distancegauche = (2 * 3.1416 * (rayon) * (angle / 360));
    distancedroite = (2 * 3.1416 * (rayon + 19) * (angle / 360));
  }
  while (distance < ((distancedroite*13367)/100))
  {
    brasbouge();
    MOTOR_SetSpeed(1, (distancedroite / distancegauche) * speedright);
    nbcycle++;
    MOTOR_SetSpeed(0, speedleft);
    float valeur_voulue = ((distancedroite / distancegauche) * ENCODER_Read(0));
    float valeur_reel = ENCODER_Read(1);
    distance += valeur_reel;
    errorp = valeur_voulue - valeur_reel;
    errori = ((nbcycle * valeur_voulue) - distance);
    adjust = (errorp * kp) + (errori * ki);
    MOTOR_SetSpeed(1, (speedright + adjust));
    ENCODER_Reset(0);
    ENCODER_Reset(1);
  }
  MOTOR_SetSpeed(0, 0.20);
  MOTOR_SetSpeed(1, 0.20);
  delay(50);
  MOTOR_SetSpeed(0, 0.10);
  MOTOR_SetSpeed(1, 0.10);
  delay(50);
  MOTOR_SetSpeed(0, 0);
  MOTOR_SetSpeed(1, 0);
  delay(50);
}

void deplacer (char charByte)
{
  switch (charByte)
  {
    case 'L':
      activerServo(Servo1, 125);
      delay(1000);
      ligneDroite(0.45);
      delay(100);
      tournerParEnArriere(1,-80);
      delay(100);
      ligneDroite(0.30);
      delay(1000);
      nbLettre++;
      if (nbLettre < 2)
      {
      tournerParEnArriere(1,-80);
      Avancer(0.45);
      tourner(1,80);
      Avancer(0.355);
      tourner(1,80);
      delay(2000);
      }
      else
      {
      tournerParEnArriere(1,-80);
      Avancer(0.45);
      tourner(1,80);
      Avancer(0.10);
      tourner(1,80);
      delay(2000);
      }
      break;

    case 'Z':
      activerServo(Servo1, 125);
      delay(1000);
      virage(0, 90);
      delay(100);
      ligneDroite(0.30);
      delay(100);
      tourner(1, 115);
      delay(100);
      Avancer(0.03);
      delay(100);
      ligneDroite(0.50);
      delay(100);
      tournerParEnArriere(1, -116);
      delay(100);
      ligneDroite(0.30);
      delay(1000);
      nbLettre++;
      if (nbLettre < 2)
      {
      tournerParEnArriere(1,-25);
      Avancer(0.20);
      tourner(1,80);
      Avancer(0.355);
      tourner(1,80);
      delay(2000);
      }
      else
      {
      tournerParEnArriere(1,-25);
      Avancer(0.20);
      tourner(1,80);
      Avancer(0.10);
      tourner(1,80);
      delay(2000);
      }
      break;

    case 'C':
      activerServo(Servo1, 125);
      delay(1000);
      virage(1, 90);
      delay(100);
      courbe(240, 4, 0);
      delay(1000);
      nbLettre++;
      if (nbLettre < 2)
      {
      tournerParEnArriere(1,-25);
      Avancer(0.20);
      tourner(1,80);
      Avancer(0.355);
      tourner(1,80);
      delay(2000);
      }
      else
      {
      tournerParEnArriere(1,-25);
      Avancer(0.20);
      tourner(1,80);
      Avancer(0.10);
      tourner(1,80);
      delay(2000);
      }
      break;

    case 'U':
      activerServo(Servo1, 125);
      delay(1000);
      ligneDroite(0.30);
      delay(500);
      petiteCourbe(180, 4, 0);
      delay(500);
      ligneDroite(0.40);
      delay(1000);
      nbLettre++;
      if (nbLettre < 2)
      {
       //code
      }
      else
      {
        //code
      }
      break;

      case 'T':
      activerServo(Servo1, 125);
      delay(1000);
      tournerParEnArriere(1,-80);
      ligneDroite(0.26);
      delay(100);
      tourner(1,80);
      delay(100);
      Avancer(0.13);
      delay(100);
      tournerParEnArriere(1,-80);
      delay(100);
      ligneDroite(0.45);
      delay(1000);
      nbLettre++;
      if (nbLettre < 2)
      {
      tournerParEnArriere(1,-80);
      Avancer(0.15);
      tournerParEnArriere(1,-80);
      Avancer(0.45);
      tourner(1,80);
      Avancer(0.355);
      tourner(1,80);
      }
      else
      {
      tournerParEnArriere(1,-80);
      Avancer(0.15);
      tournerParEnArriere(1,-80);
      Avancer(0.45);
      tourner(1,80);
      Avancer(0.10);
      tourner(1,80);
      }
      break;

    case 'D':
      activerServo(Servo1, 125);
      delay(1000);
      ligneDroite(0.45);
      delay(100);
      tournerParEnArriere(1,-80);
      delay(100);
      courbe(240, 6, 0);
      delay(1000);
      nbLettre++;
      if (nbLettre < 2)
      {
      tourner(1,170);
      Avancer(0.655);
      tourner(1,80);
      delay(2000);
      }
      else
      {
      tourner(1,170);
      Avancer(0.185);
      tourner(1,80);
      delay(2000);
      }
      break;

    case 'J':
      activerServo(Servo1, 125);
      delay(1000);
      ligneDroite(0.30);
      delay(100);
      petiteCourbe(320, 7, 1);
      delay(1000);
      nbLettre++;
      if (nbLettre < 2)
      {
       //code
      }
      else
      {
        //code
      }
      break;

    case 'V':
      activerServo(Servo1, 125);
      delay(1000); 
      virage(0, 30);
      delay(100);
      ligneDroite(0.40);
      delay(100);
      tournerParEnArriere(1, -120);
      delay(100);
      ligneDroite(0.45);
      delay(1000);
      nbLettre++;
      if (nbLettre < 2)
      {
      tourner(1,55);
      Avancer(0.355);
      tourner(1,79);
      delay(2000);
      }
      else
      {
      tourner(1,55);
      Avancer(0.10);
      tourner(1,79);
      delay(2000);
      }
      break;

    default:

      break;   
  }
}

void loop()
{
  //VARIABLE
  // put your main code here, to run repeatedly:

  activerServo(Servo1, 125);

  if(nbLettre ==2)
  {
     //code
  }
  else
  if (Serial1.available() > 0) {
      // read the incoming byte:
      incomingByte = Serial1.readString();
      const char charByte = incomingByte[0];
      deplacer(charByte);
    }
}