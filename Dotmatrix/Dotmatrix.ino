//#include <Wire.h>
//#include <Helios_Temperature_Sensor_TMP006.h>

//Helios_Temperature_Sensor_TMP006 tsensor;

static const char font5x7[] = {
0x0E, 0x11, 0x11, 0x1F, 0x11, 0x11, 0x11,  //A
0x0E, 0x11, 0x1E, 0x11, 0x11, 0x11, 0x1E,  //B
0x0E, 0x11, 0x10, 0x10, 0x10, 0x11, 0x0E,  //C
0x1C, 0x12, 0x11, 0x11, 0x11, 0x12, 0x1C,  //D
0x1F, 0x10, 0x10, 0x1E, 0x10, 0x10, 0x1F,  //E
0x1F, 0x10, 0x10, 0x1C, 0x10, 0x10, 0x10,  //F
0x0E, 0x11, 0x10, 0x10, 0x13, 0x11, 0x0E,  //G
0x11, 0x11, 0x11, 0x1F, 0x11, 0x11, 0x11,  //H
0x0E, 0x04, 0x04, 0x04, 0x04, 0x04, 0x0E,  //I
0x07, 0x02, 0x02, 0x02, 0x02, 0x12, 0x0C,  //J
0x11, 0x12, 0x14, 0x18, 0x14, 0x12, 0x11,  //K
0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x1F,  //L
0x11, 0x1B, 0x15, 0x11, 0x11, 0x11, 0x11,  //M
0x11, 0x11, 0x19, 0x15, 0x13, 0x11, 0x11,  //N
0x0E, 0x11, 0x11, 0x11, 0x11, 0x11, 0x0E,  //O
0x1E, 0x11, 0x11, 0x1E, 0x10, 0x10, 0x10,  //P
0x0E, 0x11, 0x11, 0x11, 0x15, 0x12, 0x0D,  //Q
0x1E, 0x11, 0x11, 0x1E, 0x14, 0x12, 0x11,  //R
0x0F, 0x10, 0x10, 0x0E, 0x01, 0x01, 0x1E,  //S
0x1F, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04,  //T
0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x0E,  //U
0x11, 0x11, 0x11, 0x11, 0x11, 0x0A, 0x04,  //V
0x11, 0x11, 0x11, 0x15, 0x15, 0x1B, 0x11,  //W  
0x11, 0x11, 0x0A, 0x04, 0x0A, 0x11, 0x11,  //X
0x11, 0x11, 0x0A, 0x04, 0x04, 0x04, 0x04,  //Y
0x1F, 0x01, 0x02, 0x04, 0x08, 0x10, 0x1F,  //Z
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00   //spacja 
};


/*buffer, same size as display*/
char displayBuffer[7][16];
int i;

//DOTMATRIX PIN SETTINGS
#define CLK  2
#define DATA 3
#define STR  4

#define R7   9   
#define R6   8   
#define R5   5   
#define R4   6   
#define R3   7   
#define R2   11  
#define R1   10




void setup()
{
    //serial port to pc
    Serial.begin(9600);
  
    pinMode(CLK,  OUTPUT);
    pinMode(DATA, OUTPUT);
    pinMode(STR,  OUTPUT);  
    pinMode(R1,   OUTPUT);
    pinMode(R2,   OUTPUT);
    pinMode(R3,   OUTPUT);
    pinMode(R4,   OUTPUT);
    pinMode(R5,   OUTPUT);
    pinMode(R6,   OUTPUT);
    pinMode(R7,   OUTPUT);
   
    digitalWrite(CLK, LOW);
    digitalWrite(DATA, LOW);
    digitalWrite(STR, HIGH);
    digitalWrite(R1, HIGH);
    digitalWrite(R2, HIGH);
    digitalWrite(R3, HIGH);
    digitalWrite(R4, HIGH);
    digitalWrite(R5, HIGH);
    digitalWrite(R6, HIGH);  
    digitalWrite(R7, HIGH);
    
    clearDotmatrix();
    writeString("TEST",4,5); 
}

void loop(){

  //temperature
  //unsigned int object = tsensor.ReadObject();
  //unsigned int ambient = tsensor.ReadAmbient();
  //sprintf(Temp,"De omgevings temperatuur = %d C De object temperatuur = %d C",ambient,object);
  
  
  //Serial.println("test");
   //while(Serial.available()){
   //    writeString("SERIAL DATA",11,2);  
   //    if(Serial.available() > 10){
   //        writeString("TEN CHARS",9,3);
   //        while(Serial.available()){
   //            Serial.read();
   //            strobeDotmatrix(); 
   //        }
           //writeString("NO DATA",7,4);     
   //    }
   //    strobeDotmatrix();
   //}   
   strobeDotmatrix(); 
 }


/**
 * Clears dotmatrix display.
 */ 
 int clearDotmatrix(void){
    int i;
    for(i=0; i<80; i++){
         displayBuffer[0][i] = 0;
         displayBuffer[1][i] = 0;
         displayBuffer[2][i] = 0;
         displayBuffer[3][i] = 0;
         displayBuffer[4][i] = 0;
         displayBuffer[5][i] = 0;
         displayBuffer[6][i] = 0;
    }
    return 1;                       
 }
 
 
 /**
 * Writes a string of chars to the dotmatrix display buffer.
 * 
 * @param sString string to wirte to display.
 * @param length  Length of the string.
 * @param pos     start position of the string.
 */ 
 int writeString(char* text, int length, int pos){
    int i;
    
    clearDotmatrix(); // clear the display
    //if(pos+length>15) return 0;            //There are only 16 positions available    
    for(i = 0; i < length; i++){
      if(text[i] <= 48){
        writeChar(26        , i+pos); //spacja
      }else{
        writeChar(text[i]-65, i+pos); //49
      }
    }
    return 1;                       
 }

/**
 * Writes a single char to the dotmatrix display buffer.
 * 
 * @param cChar char to wirte to display.
 * @param pos posistion of the char on the display(0-15).
 */ 
 int writeChar(char cChar, int pos){
   int i=0; 
   if(pos>15) return 0;            //There are only 16 positions available    
    for(i = 0; i < 7; i++){ 
      displayBuffer[i][pos] = font5x7[(cChar*7) + i];
    }  
    strobeDotmatrix();
    return 1;                       
 }
 
 
/**
 * Write local buffer to dotmatrix display.
 * 
 */ 
 int strobeDotmatrix(void){
    int i,j,k;
    char cTemp;
    for(i=0; i<7; i++){                    //7 rows
        for(j=0; j<16; j++){              //16 5x7 displays
            cTemp = displayBuffer[i][j];
            for(k=0; k<5; k++){          
                if(cTemp & 0x10){
                    one();
                } else{
                    zero();
                }
                cTemp = cTemp << 1;
            }
        }
    rowSelect(i);    
    digitalWrite(STR, HIGH);  //strobe every row
    digitalWrite(STR, LOW);
    //delay();
    }
}


/**
 * Write local buffer to dotmatrix display.
 * 
 */ 
 int rowSelect(char row){ 
    switch(row){
      case 0: 
        digitalWrite(R1  , LOW);
        digitalWrite(R2  , HIGH);
        digitalWrite(R3  , HIGH);
        digitalWrite(R4  , HIGH);
        digitalWrite(R5  , HIGH);
        digitalWrite(R6  , HIGH);
        digitalWrite(R7  , HIGH);
        break;
      case 1:
        digitalWrite(R1  , HIGH);
        digitalWrite(R2  , LOW);
        digitalWrite(R3  , HIGH);
        digitalWrite(R4  , HIGH);
        digitalWrite(R5  , HIGH);
        digitalWrite(R6  , HIGH);
        digitalWrite(R7  , HIGH);
        break;
      case 2:
        digitalWrite(R1  , HIGH);
        digitalWrite(R2  , HIGH);
        digitalWrite(R3  , LOW);
        digitalWrite(R4  , HIGH);
        digitalWrite(R5  , HIGH);
        digitalWrite(R6  , HIGH);
        digitalWrite(R7  , HIGH);
        break;
      case 3:
        digitalWrite(R1  , HIGH);
        digitalWrite(R2  , HIGH);
        digitalWrite(R3  , HIGH);
        digitalWrite(R4  , LOW);
        digitalWrite(R5  , HIGH);
        digitalWrite(R6  , HIGH);
        digitalWrite(R7  , HIGH);
        break;
      case 4:
        digitalWrite(R1  , HIGH);
        digitalWrite(R2  , HIGH);
        digitalWrite(R3  , HIGH);
        digitalWrite(R4  , HIGH);
        digitalWrite(R5  , LOW);
        digitalWrite(R6  , HIGH);
        digitalWrite(R7  , HIGH);
        break;
      case 5:
        digitalWrite(R1  , HIGH);
        digitalWrite(R2  , HIGH);
        digitalWrite(R3  , HIGH);
        digitalWrite(R4  , HIGH);
        digitalWrite(R5  , HIGH);
        digitalWrite(R6  , LOW);
        digitalWrite(R7  , HIGH);
        break;
      case 6:
        digitalWrite(R1  , HIGH);
        digitalWrite(R2  , HIGH);
        digitalWrite(R3  , HIGH);
        digitalWrite(R4  , HIGH);
        digitalWrite(R5  , HIGH);
        digitalWrite(R6  , HIGH);
        digitalWrite(R7  , LOW);
        break;
      default:
        digitalWrite(R1  , LOW);
        digitalWrite(R2  , LOW);
        digitalWrite(R3  , LOW);
        digitalWrite(R4  , LOW);
        digitalWrite(R5  , LOW);
        digitalWrite(R6  , LOW);
        digitalWrite(R7  , LOW);
        break;      
      }
 } 

 /**
 * set led of dotmatrix.
 */  
 void one(void){
    digitalWrite(CLK, LOW);
    digitalWrite(DATA, HIGH);
    digitalWrite(CLK, HIGH);
    digitalWrite(CLK, LOW); 
}
 /**
 * not set led of dotmatrix.
 */  
void zero(void){
    digitalWrite(CLK, LOW);
    digitalWrite(DATA, LOW);
    digitalWrite(CLK, HIGH);
    digitalWrite(CLK, LOW); 
}


