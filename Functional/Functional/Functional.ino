#include <BEURS_LEDS.h>
#include <Wire.h>
#include <SPI.h>
#include <WiFlyHQ.h>

WiFly wifly;
BEURS_LEDS LED;

#define TCSAdress 0x39
#define TMPAdress 0x40

//#define RTCAdress B1010001 //compiler set read/write
#define RTCAdress B1010001
#define EEPROMAdress B1010000
#define RTCControl_1 0x00
#define RTCControl_2 0x01
#define RTCClockoutControl 0x0D
#define RTCTimerControl 0x0E
#define RTCTimer 0x0F
#define RTCMonths 0x07

unsigned int Checkwaarde = 0;
unsigned int Checkkleur = 0;

int Button  = 30;

float Tambient;
unsigned int Seconds;
unsigned int Minutes;
unsigned int Hours;
unsigned int Greenhigh = 0;
unsigned int Greenlow = 0;
byte CheckTMP = 0;
byte CheckTCS = 0;
byte CheckEEPROM = 0;
byte CheckRTC = 0;
byte CountGood =0;
char Buf[15];
byte K = 0;

boolean Firsttime = true;
boolean UpdateIP = true;
boolean UpdateRgb = false;
boolean UpdateTemp = false;
boolean UpdateDelay = false;
boolean UpdateDelay2 = false;
boolean UpdateHeliosTime=false;

boolean SCount = false;

unsigned int SecCount = 0;

#define RESET_VAL 3035 //500ms
#define RESET_T3 TCNT3 = RESET_VAL //Timer3 reset value

ISR(TIMER3_OVF_vect){
  RESET_T3; //reset instellingen voor overflow elke ms
  if(SCount){
    SecCount++;
    if(SecCount == 12){ //6seconden
      LED.SetRGB(1,0,0);
    }
    if(SecCount == 13){
        LED.SetRGB(0,0,0);
        SecCount = 11;
    }
  }
};



void setup()
{
  //TIMER3 INSTELLINGEN
  TCCR3B = B00000011;
  TCCR3A = B00000000;
  TIMSK3 |= (1<<TOIE3) | (0<<OCIE3A); //disabel T3OVF - TOIE3 = 0-
  LED.Init();
  Serial.begin(9600);
  Serial1.begin(9600);
  Wire.begin();
  LED.ClearAll();
  pinMode(Button,INPUT);
  //RTC set alles op 0
  Wire.beginTransmission(RTCAdress);
  Wire.write(0x02);
  Wire.write(0x00);
  Wire.write(0x00);
  Wire.write(0x00);
  Wire.endTransmission();
}
  
void loop()
{   
  if(digitalRead(Button)){
    CheckWifly();
  }
  if(Firsttime){
    unsigned int millis1,millis2;
    millis1 = millis();
    millis2 = millis();
    while((millis2 - millis1)<1500){  
      millis2 = millis();
      Firsttime = false;
    }
    LED.ClearAll();
   
    CheckWifly();
  } 
}
 
void CheckWifly()
{
  Checkwaarde = B10000000;
  LED.SetRGB(0,1,1);
  LED.LedkrantCheckLed(Checkwaarde);
  SCount = true;
  if(wifly.begin(&Serial1,&Serial))
  {
    Serial.println("Wifly available and started"); 
    LED.SetRGB(0,1,0);  
    CountGood = CountGood + 1; 
    SCount = false;
  }
  else
  {
    Serial.println("Can't start Wifly");
    LED.SetRGB(1,0,0);
    CountGood = CountGood + 0;
    SCount = false;
  }
  delay(500);
}


