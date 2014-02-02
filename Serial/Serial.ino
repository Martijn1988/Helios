#include <Wire.h>
#include <Helios_Temperature_Sensor_TMP006.h>

Helios_Temperature_Sensor_TMP006 tsensor;

char Temp[100];

void setup()
{
    //Wire.begin();
    Serial.begin(9600);
}

void loop(){

  //temperature
  unsigned int ambient = tsensor.ReadAmbient();
  sprintf(Temp,"External temperature = %d C",ambient);
  
   //serial loop

   Serial.println(Temp);

   delay(1000);
   
}
