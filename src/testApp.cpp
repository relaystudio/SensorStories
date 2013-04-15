#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    int baud = 9600;
	serial.setup(0, baud);
    
    for(int i=0;i<NUMSCENES;i++) {
        scenes[i] = new Scene();
        scenes[i]->loadVideo("mov" + ofToString(i));
    }
    transition = false;
    activeScene = 0;
    transScene = 0;

    interface = scenes[0];
    interface->start();
}

//--------------------------------------------------------------
void testApp::update(){
    if(transition) {
        if(scenes[transScene]->isDone()) {
            transition = false;
            interface = scenes[activeScene];
            interface->start();
        }
    }
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
    }
    
}

void testApp::setupArduino() {
    serial.listDevices();
	serial.setup(9,9600);
    
}

void testApp::writeArduino() {
//    if(limitBuffer <= ofGetElapsedTimeMillis()) {
//        limitBuffer = ofGetElapsedTimeMillis() + panel.getValueI("TimeBetweenUpdate");
        buffer = "";
//        for(int l = 0; l<lights.size();l++) {
//            lights[l]->update();
//            
//            serial.writeByte((unsigned char)floor(lights[l]->getStrength() * 127));
//            
//            buffer += ofToString((int)(lights[l]->getStrength() * 127));
//            
//            if(l!=lights.size()-1) buffer += ",";
//            
//            serial.writeByte(',');
//        }
//        buffer += "\n";
//        serial.writeByte('a');
//        //        serial.writeBytes((unsigned char*) buffer.c_str(),buffer.size());
    //}
    ofLog() << buffer;
}

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
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




////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

bool PlumEvent::isPlumPresent() {
    return plumPresent;
}


bool PlumEvent::isBookPresent() {
    return bookPresent;
}

bool PlumEvent::isBookOpen() {
    return bookOpen;
}

string PlumEvent::getPlumId() {
    return plumId;
}

void PlumEvent::update() {
    
}


void PlumEvent::renderSensors() {    
    ofPushMatrix();
    ofTranslate(20,ofGetHeight()-200);
/*    string Msg;
    Msg << "photosensor0: " << photo0 << endl
    << "photosensor1: " << photo1 << endl
    << "photosensor2: " << photo2 << endl
    << "photosensor3: " << photo3 << endl
    << "plumPresent: " << plumPresent << endl
    << "plumId: " << plumId << endl
    << "bookPresent: " << bookPresent << endl
    << "bookId: " << bookId << endl
    ofDrawBitmapString(Msg,0,0); */
    ofPopMatrix();
}
