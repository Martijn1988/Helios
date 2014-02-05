//#include <Wire.h>
//#include <Helios_Temperature_Sensor_TMP006.h>

//Helios_Temperature_Sensor_TMP006 tsensor;


//TODO 0 - 9
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
}

void loop(){

  //temperature
  //unsigned int object = tsensor.ReadObject();
  //unsigned int ambient = tsensor.ReadAmbient();
  //sprintf(Temp,"De omgevings temperatuur = %d C De object temperatuur = %d C",ambient,object);
  
   writeToDotmatrix("    BANAAN     ",5,0);

 }


/**
 * Clears dotmatrix display.
 */ 
 int clearDotmatrix(void){
      writeToDotmatrix("                ",16,0);  //16x spacja                   
 }
 
 
/**
 * Writes a single char to the dotmatrix display buffer.
 * 
 * @param text to write to display.
 * @param length of the text on the display(0-15).
 * @param pos starting posistion of the char on the display. 
 *
 * TODO: Position and length!
 */ 
int writeToDotmatrix(char* text, int length, int pos){
    int i,j,k;
    char cTemp;
      
    for(i=0; i<7; i++){                   //7 rows

        for(j=0; j<16; j++){              //16 5x7 displays
            //temporary code or space
            if(text[j] <= 48){
              text[j] = 26+65;
            }
            //end of temporary code
  
            cTemp = font5x7[((text[j]-65)*7) + i];
            for(k=0; k<5; k++){           //write one line of 5x7 display       
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
    delayMicroseconds(50); 
    digitalWrite(STR, LOW);
    delayMicroseconds(50);
    }
    
}



/**
 * Row selection.
 * 
 */ 
 inline int rowSelect(char row){ 
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
 inline void one(void){
    digitalWrite(CLK, LOW);
    digitalWrite(DATA, HIGH);
    digitalWrite(CLK, HIGH);
    digitalWrite(CLK, LOW); 
}
 /**
 * not set led of dotmatrix.
 */  
inline void zero(void){
    digitalWrite(CLK, LOW);
    digitalWrite(DATA, LOW);
    digitalWrite(CLK, HIGH);
    digitalWrite(CLK, LOW); 
}


