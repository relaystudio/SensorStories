///////////////////////////////////
// 
// Hackled together from various sketches
// Firmata base for analog read
// Logic managed in openframeworks
// Use ID12 Innovations rfid readers
// Uses Arduino mega 2560
//
// By Andrew Lovett-Barron
// andrew@relaystudio.com
//
// https://github.com/davidchatting/SmartCityLab/blob/master/demos/RFID_ID12/RFID_ID12.ino
// And FirmataAnalog.ino
//
///////////////////////////////////
#include <Firmata.h>

#define SYSEX_READER0       0x0D
#define SYSEX_READER1       0x1D


int analogInputsToReport = 0;
int analogPin = 0;

/* timer variables */
unsigned long currentMillis;
unsigned long previousMillis;


void setup() {
    // Analog reading
    firmataSetup();
    
    //rfid
//    Serial.begin(9600); // Usb serial
    Serial2.begin(9600); // Primary reader
    Serial3.begin(9600); // Secondary reader
    
    //Test
    pinMode(13,OUTPUT);
    digitalWrite(13,LOW);
}

void loop () {
// read the serial port

    if(Serial2.available() > 0) {
    
        digitalWrite(13,HIGH); // Testing to make sure this is available
        
        byte tag1 = readTag(&Serial2);
        byte tag2 = readTag(&Serial3);

        Firmata.sendSysex(SYSEX_READER0, 1, &tag1);
        Firmata.sendSysex(SYSEX_READER1, 1, &tag2);
  //      Serial.write(tag1);
//        Serial.write(tag2);
        
    }
    
    firmataLoop();

}

                    
byte readTag(HardwareSerial * ser) {
    byte thisTag = 0;
//
//  char b;
//  if(serial->available()) {
//    if(serial->read()==2){
//      do{
//        b=serial->read();
//        if(b!=3 && b!=-1) thisTag+=b;
//      }
//      while(b!=3);
//    }
//  }
//  thisTag.trim();
//
//  if(thisTag.length()==0) thisTag="NOTAG";
//  byte[] theTag;
//  thisTag.getBytes(theTag,thisTag.length());

    if (ser->available() > 0) {
        thisTag = ser->read();
    }

    return thisTag;
}

void firmataSetup() {
    Firmata.setFirmwareVersion(0, 2);
    Firmata.printFirmwareVersion();
    Firmata.attach(ANALOG_MESSAGE, analogWriteCallback);
    Firmata.attach(REPORT_ANALOG, reportAnalogCallback);
    Firmata.attach(START_SYSEX, sysexCallback);  
    Firmata.begin(57600);
}

void firmataLoop() {
    while(Firmata.available())
        Firmata.processInput();
    currentMillis = millis();
    if(currentMillis - previousMillis > 20) {
        previousMillis += 99; // run this every 20ms
        for(analogPin=0;analogPin<TOTAL_ANALOG_PINS;analogPin++) {
            if( analogInputsToReport & (1 << analogPin) ) 
                Firmata.sendAnalog(analogPin, analogRead(analogPin));
        }
    }
}

void reportAnalogCallback(byte pin, int value)
{
    if(value == 0) {
        analogInputsToReport = analogInputsToReport &~ (1 << pin);
    }
    else { // everything but 0 enables reporting of that pin
        analogInputsToReport = analogInputsToReport | (1 << pin);
    }
    // TODO: save status to EEPROM here, if changed
}

void analogWriteCallback(byte pin, int value)
{
    switch(pin) {
    case 9: 
    case 10:
    case 3: 
    case 5: 
    case 6: 
    case 11: // PWM pins
        analogWrite(pin, value); 
        break;
    }
}
void sysexCallback(byte command, byte argc, byte *argv)
{
    byte mode;
    
    mode = argv[0];
}