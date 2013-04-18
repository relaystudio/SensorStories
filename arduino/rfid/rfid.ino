char val = 0; // variable to store the data from the serial port

void setup() {
Serial.begin(9600); // connect to the serial port
Serial1.begin(9600); // connect to the serial port
pinMode(13,OUTPUT);
digitalWrite(13,LOW);
}

void loop () {
// read the serial port

if(Serial.available() > 0) {
  String thisTag=readTag();
    if(thisTag > 0) {
        digitalWrite(13,HIGH);
    }
    val = Serial1.read();
    Serial.print(thisTag);

    }
}

//int RFIDResetPin = 6;
//
//void setup(){
//  Serial.begin(9600);
//  Serial1.begin(9600);
//
//  pinMode(RFIDResetPin, OUTPUT);
//  digitalWrite(RFIDResetPin, HIGH);
//}
//
//void loop(){
//
//  char tagString[13];
//  int index = 0;
//  boolean reading = false;
//
//  while(Serial1.available() && Serial.available()){
//    
//    Serial.print('1');
//    int readByte = Serial1.read(); //read next available byte
//
//    if(readByte == 2) reading = true; //begining of tag
//    if(readByte == 3) reading = false; //end of tag
//
//    if(reading && readByte != 2 && readByte != 10 && readByte != 13){
//      //store the tag
//      tagString[index] = readByte;
//      index ++;
//    }
//  }
//
//  checkTag(tagString); //Check if it is a match
//  clearTag(tagString); //Clear the char of all value
//  resetReader(); // reset the RFID reader
//}
//
//
//void resetReader(){
/////////////////////////////////////
////Reset the RFID reader to read again.
/////////////////////////////////////
//  digitalWrite(RFIDResetPin, LOW);
//  digitalWrite(RFIDResetPin, HIGH);
//  delay(150);
//}
//
//void clearTag(char one[]){
/////////////////////////////////////
////clear the char array by filling with null - ASCII 0
////Will think same tag has been read otherwise
/////////////////////////////////////
//  for(int i = 0; i < strlen(one); i++){
//    one[i] = 0;
//  }
//}
//
//

String readTag(){
  String thisTag;

  char b;
  if(Serial1.available()) {
    if(Serial1.read()==2){
      do{
        b=Serial1.read();
        if(b!=3 && b!=-1) thisTag+=b;
      }
      while(b!=3);
    }
  }
  thisTag.trim();

  if(thisTag.length()==0) thisTag="NOTAG";

  return(thisTag);
}
