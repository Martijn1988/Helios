#include <Wire.h>
#include <Helios_Temperature_Sensor_TMP006.h>

Helios_Temperature_Sensor_TMP006 tsensor;
 
//font
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
 
/* Work around a bug with PROGMEM and PSTR where the compiler always
 * generates warnings.
 */
#undef PROGMEM 
#define PROGMEM __attribute__(( section(".progmem.data") )) 
#undef PSTR 
#define PSTR(s) (__extension__({static prog_char __c[] PROGMEM = (s); &__c[0];})) 

#include <WiFlyHQ.h>

WiFly wifly;

/* Change these to match your WiFi network */

const char mySSID[] = "xxxxxxxxx";
const char myPassword[] = "xxxxxxx";

void sendIndex();
void sendGreeting(char *name);
void send404();

char buf[80];
char username[16];

void setup()
{
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
  
    Serial.begin(9600);
    Serial.println(F("Starting"));
    Serial.print(F("Free memory: "));
    Serial.println(wifly.getFreeMemory(),DEC);

    Serial1.begin(9600);
    if (!wifly.begin(&Serial1, &Serial)) {
        Serial.println(F("Failed to start wifly"));
	wifly.terminal();
    }

    /* Join wifi network if not already associated */
    if (!wifly.isAssociated()) {
	/* Setup the WiFly to connect to a wifi network */
	Serial.println(F("Joining network"));
	wifly.setSSID(mySSID);
	wifly.setPassphrase(myPassword);
	wifly.enableDHCP();
	wifly.save();

	if (wifly.join()) {
	    Serial.println(F("Joined wifi network"));
	} else {
	    Serial.println(F("Failed to join wifi network"));
	    wifly.terminal();
	}
    } else {
        Serial.println(F("Already joined network"));
    }

    wifly.setBroadcastInterval(0);	// Turn off UPD broadcast

    //wifly.terminal();

    Serial.print(F("MAC: "));
    Serial.println(wifly.getMAC(buf, sizeof(buf)));
    Serial.print(F("IP: "));
    Serial.println(wifly.getIP(buf, sizeof(buf)));

    wifly.setDeviceID("Wifly-WebServer");

    if (wifly.isConnected()) {
        Serial.println(F("Old connection active. Closing"));
	wifly.close();
    }

    wifly.setProtocol(WIFLY_PROTOCOL_TCP);
    if (wifly.getPort() != 80) {
        wifly.setPort(80);
	/* local port does not take effect until the WiFly has rebooted (2.32) */
	wifly.save();
	Serial.println(F("Set port to 80, rebooting to make it work"));
	wifly.reboot();
	delay(3000);
    }
    Serial.println(F("Dotmatrix setup..."));
    
   
    
    //clearDotmatrix(); this makes it crash...
    
    Serial.println(F("Ready"));
}


void loop()
{
  if (wifly.available() > 0) {

        /* See if there is a request */
	if (wifly.gets(buf, sizeof(buf))) {
            if (strncmp_P(buf, PSTR("GET / "), 6) == 0) {
		/* GET request */
		Serial.println(F("Got GET request"));
		while (wifly.gets(buf, sizeof(buf)) > 0) {
		    /* Skip rest of request */
		}
		sendIndex();
		Serial.println(F("Sent index page"));
	    } else if (strncmp_P(buf, PSTR("POST"), 4) == 0) {
	        /* Form POST */
	        Serial.println(F("Got POST"));

		/* Get posted field value */
		if (wifly.match(F("user="))) {
		    wifly.gets(username, sizeof(username));
		    wifly.flushRx();		// discard rest of input
		    sendGreeting(username);
                    Serial.println(username);
                    //writeString(username,sizeof(username),0);
		}
	    } else {
	        /* Unexpected request */
		Serial.print(F("Unexpected: "));
		Serial.println(buf);
		wifly.flushRx();		// discard rest of input
		Serial.println(F("Sending 404"));
		send404();
	    }
	}
    }
    
    if(digitalRead(30)){
      Serial.println("button pressed");
    }
    
    //for(i=0; i<20; i++){
    //  if(i=0){
        //clearDotmatrix();
    //    Serial.println(F("Writing to dotmatrix buffer:"));
        //writeString("BANAAN",6,0);
    //  }
      //strobeDotmatrix();
  //  }
}

/** Send an index HTML page with an input box for a username */
void sendIndex()
{
    /* Send the header direclty with print */
    wifly.println(F("HTTP/1.1 200 OK"));
    wifly.println(F("Content-Type: text/html"));
    wifly.println(F("Transfer-Encoding: chunked"));
    wifly.println();

    /* Send the body using the chunked protocol so the client knows when
     * the message is finished.
     * Note: we're not simply doing a close() because in version 2.32
     * firmware the close() does not work for client TCP streams.
     */
    wifly.sendChunkln(F("<html>"));
    wifly.sendChunkln(F("<title>Dotmatrix server 2014</title>"));
    wifly.sendChunkln(F("<h1>"));
    wifly.sendChunkln(F("<p>Dotmatrix server 2014</p>"));
    wifly.sendChunkln(F("</h1>"));
    wifly.sendChunkln(F("<form name=\"input\" action=\"/\" method=\"post\">"));
    wifly.sendChunkln(F("Message:"));
    wifly.sendChunkln(F("<input type=\"text\" name=\"user\" />"));
    wifly.sendChunkln(F("<input type=\"submit\" value=\"Submit\" />"));
    
    /* Include temperature measurement */
    unsigned int ambient = tsensor.ReadAmbient();
    snprintf_P(buf, sizeof(buf), PSTR("<p>Temperatuur in Martijn zijn kamer is %d graden celcius </p>"), ambient);
    wifly.sendChunkln(buf);
    wifly.sendChunkln(F("<p>Martijn Bijwaard 2014</p>"));    
    wifly.sendChunkln(F("</form>")); 
    wifly.sendChunkln(F("</html>"));
    wifly.sendChunkln();
}

/** Send a greeting HTML page with the user's name and an analog reading */
void sendGreeting(char *name)
{
    /* Send the header directly with print */
    wifly.println(F("HTTP/1.1 200 OK"));
    wifly.println(F("Content-Type: text/html"));
    wifly.println(F("Transfer-Encoding: chunked"));
    wifly.println();

    /* Send the body using the chunked protocol so the client knows when
     * the message is finished.
     */
    wifly.sendChunkln(F("<html>"));
    wifly.sendChunkln(F("<title>Dotmatrix server 2014</title>"));
    /* No newlines on the next parts */
    wifly.sendChunk(F("<h1><p>Message: "));
    wifly.sendChunk(name);
    /* Finish the paragraph and heading */
    wifly.sendChunkln(F(", sent</p></h1>"));

    /* Include temperature measurement */
    unsigned int ambient = tsensor.ReadAmbient();
    snprintf_P(buf, sizeof(buf), PSTR("<p>Temperatuur in Martijn zijn kamer is %d graden celcius </p>"), ambient);
    wifly.sendChunkln(buf);

    wifly.sendChunkln(F("</html>"));
    wifly.sendChunkln();
}

/** Send a 404 error */
void send404()
{
    wifly.println(F("HTTP/1.1 404 Not Found"));
    wifly.println(F("Content-Type: text/html"));
    wifly.println(F("Transfer-Encoding: chunked"));
    wifly.println();
    wifly.sendChunkln(F("<html><head>"));
    wifly.sendChunkln(F("<title>404 Not Found</title>"));
    wifly.sendChunkln(F("</head><body>"));
    wifly.sendChunkln(F("<h1>Not Found</h1>"));
    wifly.sendChunkln(F("<hr>"));
    wifly.sendChunkln(F("</body></html>"));
    wifly.sendChunkln();
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
     // displayBuffer[i][pos] = font5x7[(cChar*7) + i];
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
