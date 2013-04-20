#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    firmata.connect("/dev/tty.usbmodem411", 57600);
    ofAddListener(firmata.EInitialized, this, &testApp::setupArduino);
    bFirmataSetup = false;
    
    for(int i=0;i<NUMSCENES;i++) {
        scenes[i] = new Scene();
        scenes[i]->loadVideo("mov" + ofToString(i));
    }
    transition = false;
    activeScene = 0;
    transScene = 0;
    debug = false;
    
    book1.loadImage("debug/book1.jpg");
    book2.loadImage("debug/book2.png");
    user1.loadImage("debug/user1.jpg");
    user2.loadImage("debug/user2.jpg");
    
    
    interface = scenes[0];
    interface->start();
    ofAddListener(SensorEvent::events, this, &testApp::sensorControl);

}

//--------------------------------------------------------------
void testApp::update(){
    updateArduino();
    
    // Transitions scene
    if(transition) {
        if(scenes[transScene]->isDone()) {
            transition = false;
            interface = scenes[activeScene];
            interface->start();
        }
    }
    // Triggers a queued scene if present
    if(queuedScene >= 0) {
        changeScene(queuedScene);
        queuedScene = -1;
    }
    
////    if(ofGetElapsedTimeMillis() - time1 > TIMEOUT) {
//        isUser = false;
//        isObject = false;
//    }
    
    sceneControl(checkScene);
    
    
    interface->update();
}

//--------------------------------------------------------------
void testApp::draw(){
    
    interface->draw();
    ofPushMatrix();
    ofTranslate(20,20);
    ofSetColor(255,120,120);
    if(!transition) {
        ofDrawBitmapString("Playing Scene " + ofToString(activeScene),0,0);
    } else {
        ofDrawBitmapString("Transfering from Scene " + ofToString(transScene) + " to " + ofToString(activeScene),0,0);
    }
    ofPopMatrix();
    drawDebug(debug);
}

void testApp::drawDebug(bool draw) {
    if(draw) {
        ofPushMatrix();
        ofTranslate(20,ofGetHeight() - 100);
        for(int i=0;i<NUMPHOTO;i++) {
            ofPushMatrix();
            ofSetColor((int)(photo[i] * 255));
            ofTranslate(20 * i,0);
            ofRect(0,0,20,100 * photo[i]);
            ofPopMatrix();
        }

        ofPopMatrix();

        // Draw books and users
        ofPushMatrix();
            ofTranslate(ofGetWidth()-100, 20);
        
            ofSetColor(255);
            ofDrawBitmapString("User",0,-20);
//            if(isUser) ofSetColor(255,200,200);
//            else ofSetColor(120,120,120);
            ofSetColor((int)(rfidphoto[0] * 255));
            ofRect(0,0,50,(int)(rfidphoto[0 ] * 50));
            if(isUser && user == "user1") user1.draw(0,50,50,50);
            else if(isUser && user == "user2") user2.draw(0,50,50,50);
        
            ofSetColor(255);
            ofDrawBitmapString("Object",50,-20);
            //if(isObject) ofSetColor(255,200,200);
            ofSetColor((int)(rfidphoto[1] * 255));
            ofRect(50,0,50,(int)(rfidphoto[1] * 50));
            if(isObject && book == "book1") book1.draw(50,50,50,50);
            else if(isObject && book == "book2") book2.draw(50,50,50,50);
        
        ofPopMatrix();
    }
}

void testApp::changeScene(int sceneId) {
    if( activeScene == sceneId) return;
    if(!transition) {
        ofLog() << "Transition to scene " << ofToString(sceneId);
        scenes[activeScene]->transition();
        transScene = activeScene;
        activeScene = sceneId;
        transition = true;
    } else {
        ofLog() << "Queued scene " << ofToString(sceneId);
        queuedScene = sceneId;
    }
}

void testApp::sceneControl(bool check) {
    if(check) {
        
        // First, check to see if it's attract loop
        if(!isUser && !isObject) {
            changeScene(0);
        } else
            
        // if user1 is present, no object
        if(isUser && user == "user1" && !isObject ) {
            changeScene(1);
        } else
        
        // if user2 is present, no object
        if(isUser && user == "user2" && !isObject ) {
            changeScene(2);
        } else
            
        // if user1 is present, object present
        if(isUser && user == "user1" && isObject ) {
            changeScene(1);
        }
        
        
        
        
        
    }
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    if(key <= 9) {
        if(key < NUMSCENES-1)
            changeScene(key-1);
    }
    
    switch(key) {
        case '1':
            changeScene(0); // Intro
            break;
        case '2':
            changeScene(1); // User 1
            break;
        case '3':
            changeScene(2); // User 2
            break;
        case '4':
            changeScene(3); // User 2 w/ Book 1
            break;
        case '5':
            changeScene(4); // User 2 w/ Book open
            break;
        case '6':
            changeScene(5); // Book only
            break;
        case ' ':
            debug = !debug;
    }
    
}

void testApp::setupArduino(const int & version) {
    //serial.listDevices();
	//serial.setup(9,9600);
    
    // Setting pin modes
    firmata.sendAnalogPinReporting(0, ARD_ANALOG);
    firmata.sendAnalogPinReporting(1, ARD_ANALOG);
    firmata.sendAnalogPinReporting(12, ARD_ANALOG);
    firmata.sendAnalogPinReporting(13, ARD_ANALOG);
    firmata.sendAnalogPinReporting(14, ARD_ANALOG);
    firmata.sendAnalogPinReporting(15, ARD_ANALOG);
    
    // Setting Listeners
    ofAddListener(firmata.EDigitalPinChanged, this, &testApp::digitalDelegate);
    ofAddListener(firmata.EAnalogPinChanged, this, &testApp::analogDelegate);
//    ofAddListener(firmata.ESysExReceived, this, &testApp::sysexDelegate);
    ofAddListener(firmata.EStringReceived, this, &testApp::stringDelegate);
}

void testApp::updateArduino() { 
	firmata.update();
}

void testApp::stringDelegate(const string & str) {
//    vector<string> s = ofSplitString(str, ":");
//    ofLog() << str;
    if(!stringComplete) {
        if (str == "\r") {
            stringComplete = true;
//        } else if(str != "OF" && str != "" && str !=) {
        } else if( std::isdigit(str[0]) ){
            stringBuffer += str;
        }
    } else {
        ofLog() << stringBuffer;
        static SensorEvent event;
        event.type    = 2;
        event.pin     = 0;
        event.payload = 0;
        event.msg = stringBuffer;
        ofNotifyEvent(SensorEvent::events, event);
        stringBuffer = ""; stringComplete = false;
    }
}

void testApp::sysexDelegate(const vector<unsigned char> & sysex) {
    ofLog() << ofToString(sysex);
    int source;
    // Magic doth lie here:
    // http://stackoverflow.com/questions/4031459/how-to-convert-vectorunsigned-char-to-int
    vector<unsigned char> v = firmata.getSysEx();//(v[0] << 8) | v[1];
//    ofLog() << ofToString((int)v);
    static SensorEvent event;
    event.type    = 2;
    event.pin     = source;
    event.payload = (v[0] << 8) | v[1];
    ofNotifyEvent(SensorEvent::events, event);
};

void testApp::digitalDelegate(const int & pinNum) {
    static SensorEvent event;
    event.type    = 1;
    event.pin     = pinNum;
    event.payload = firmata.getDigital(pinNum);
    ofNotifyEvent(SensorEvent::events, event);
};

void testApp::analogDelegate(const int & pinNum) {
    static SensorEvent event;
    event.type    = 0;
    event.pin     = pinNum;
    event.payload = firmata.getAnalog(pinNum);
    ofNotifyEvent(SensorEvent::events, event);
};

void testApp::sensorControl(SensorEvent &e) {
    if(e.type == 0) { // Analog signal
        switch(e.pin) {
            case 0:
                //ofLog() << "a12: " << ofToString(e.payload);
                rfidphoto[0] = ofMap(e.payload, 700,1000,0,1);
                if(rfidphoto[0] > .7) isUser = true;
                else isUser = false;
                break;
            case 1:
                //ofLog() << "a12: " << ofToString(e.payload);
                rfidphoto[1] = ofMap(e.payload, 700,1000,0,1);
                if(rfidphoto[1] > .7) isObject = true;
                else isObject = false;
                break;
                
            case 12:
                //ofLog() << "a12: " << ofToString(e.payload);
                photo[0] = ofMap(e.payload, 700,1000,0,1);
                break;
            case 13:
                //ofLog() << "a13: " << ofToString(e.payload);
                photo[1] = ofMap(e.payload, 700,1000,0,1);
                break;
            case 14:
                //ofLog() << "a14: " << ofToString(e.payload);
                photo[2] = ofMap(e.payload, 700,1000,0,1);
                break;
            case 15:
                //ofLog() << "a15: " << ofToString(e.payload);
                photo[3] = ofMap(e.payload, 700,1000,0,1);
                break;
            default:
                ofLog() << "No pin indicated";
                break;
        }
        
    } else if( e.type == 1) { // Digital signal
        switch(e.pin) {
            case 6:
                isUser = e.payload;
                break;
            case 7:
                isObject = e.payload;
                break;
            default:
                ofLog() << "No pin indicated";
                break;
        }
    } else if( e.type == 2) { // rfid msg
        
        if(e.msg == "67005893784") {
            ofLog() << "User 1 is active";
            user = "user1";
            
        } else if(e.msg == "420044162131") {
            ofLog() << "User 2 is active";
            user = "user2";
            
        } else if(e.msg == "6700726172") {
            ofLog() << "Book 1 is active";
            book = "book1";

            
        } else if(e.msg == "67007402") {
            ofLog() << "Book 2 is active";
            book = "book2";
            
        } else {
            ofLog() << "Unidentified id:" << e.msg;
        }
        
        time1 = ofGetElapsedTimeMillis(); 
    }
}
