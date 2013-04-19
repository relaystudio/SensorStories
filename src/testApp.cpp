#include "testApp.h"

ofEvent <SensorEvent> SensorEvent::events;

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
    }
}

void testApp::changeScene(int sceneId) {
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

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    if(key <= 9) {
        if(key < NUMSCENES-1)
            changeScene(key-1);
    }
    
    switch(key) {
        case '1':
            changeScene(0);
            break;
        case '2':
            changeScene(1);
            break;
        case '3':
            changeScene(2);
            break;
        case '4':
            changeScene(3);
            break;
        case '5':
            changeScene(4);
            break;
        case '6':
            changeScene(5);
            break;
        case ' ':
            debug = !debug;
    }
    
}

void testApp::setupArduino(const int & version) {
    //serial.listDevices();
	//serial.setup(9,9600);
    
    // Setting pin modes
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
    event.payload = firmata.getAnalog(pinNum);
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
            case 0:
                break;
            case 1:
                break;
            default:
                ofLog() << "No pin indicated";
                break;
        }
    } else if( e.type == 2) { // rfid msg
        if(e.msg == "67005893784")
            ofLog() << "User 1 is active";
        else if(e.msg == "420044162131")
            ofLog() << "User 2 is active";
        else if(e.msg == "6700726172")
            ofLog() << "Book 1 is active";
        else if(e.msg == "67007402")
            ofLog() << "Book 2 is active";
        else
            ofLog() << "Unidentified id:" << e.msg;
    }
}

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

Scene::Scene() {
    
    // Don't reallllly need an fbo here...
    //fbo.allocate(loop.getWidth(), loop.getHeight());
}

Scene::~Scene() {
    
}

bool Scene::isActive() {
    return bActive;
}

bool Scene::isDone() {
    return bDone;
}

void Scene::transition() {
    state = 2;
}



void Scene::draw() {
    ofVideoPlayer * vid;
    switch(state) {
        case 0:
            vid = &begin;
            break;
        case 1:
            vid = &loop;
            break;
        case 2:
            vid = &end;
            break;
        default:
            vid = &loop;
            break;
    }
    ofPushMatrix();
    ofSetColor(255);
    vid->draw(0, 0);
    ofPopMatrix();
    ofPushMatrix();
    ofTranslate(20,40);
    ofSetColor(255,120,120);
    switch(state) {
        case 0:
            ofDrawBitmapString("Beginning Scene",0,0);
            break;
        case 1:
            ofDrawBitmapString("Loop state",0,0);
            break;
        case 2:
            ofDrawBitmapString("Exiting scene",0,0);
            break;
    }
    ofPopMatrix();
}

void Scene::loadVideo(string _path) {
    ofLog() << "Loading video at " << _path;
    path = _path;
    pathToBegin = path + "/begin.mov";
    pathToLoop = path + "/loop.mov";
    pathToEnd = path + "/end.mov";
    
    // Load in start
    begin.loadMovie(pathToBegin);
    begin.setLoopState(OF_LOOP_NONE);

    loop.loadMovie(pathToLoop);
    loop.setLoopState(OF_LOOP_NORMAL);
    
    end.loadMovie(pathToEnd );
    end.setLoopState(OF_LOOP_NONE);
    
    bActive = false;
    bDone = false;
    state = 0;
}

void Scene::start() {
    if(!bActive) {
        begin.setFrame(0);
        loop.setFrame(0);
        end.setFrame(0);
        
        begin.play();
        bActive = true;
        bDone = false;
        state = 0;
    }
}

void Scene::update() {
    ofVideoPlayer * vid;
    switch(state) {
        case 0:
            vid = &begin;
            break;
        case 1:
            vid = &loop;
            break;
        case 2:
            vid = &end;
            break;
        default:
            vid = &loop;
            break;
    }
    
    if(!vid->isPlaying()) vid->play();
    
    vid->update();

    if(vid->getIsMovieDone() && state == 0) {
        state = 1;
    } else if( vid->getIsMovieDone() && state == 2 ) {
        begin.stop();
        loop.stop();
        end.stop();
        
        state = 0;
        bDone = true;
        bActive = false;
    }
}