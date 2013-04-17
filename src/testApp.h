#pragma once

#include "ofMain.h"
#include "ofEvents.h"

#define NUMSCENES 5

class Scene {
    
public:
    Scene();
    ~Scene();

    void start();
    void update();
    void draw();
    void transition();
    void loadVideo(string _path);
    bool isActive();
    bool isDone();
private:
    
    ofFbo fbo;
    bool bActive;
    bool bDone;
    int state; //0 intro, 1 loop, 2 outro
    
    string path;
    string pathToBegin;
    string pathToLoop;
    string pathToEnd;

    ofVideoPlayer begin;
    ofVideoPlayer loop;
    ofVideoPlayer end;
};

class SensorEvent : public ofEventArgs {
    
public:
    SensorEvent() {
        type = NULL;
        pin = NULL;
        payload = NULL;
    }
    
    int type; // 0 analog, 1 digital
    int pin;
    int payload;
    static ofEvent <SensorEvent> events;
};

class testApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    void changeScene(int sceneId);
    
    void keyPressed  (int key);
    
    void setupArduino(const int & version);
    void updateArduino();
    
    void digitalDelegate(const int & pinNum);
    void analogDelegate(const int & pinNum);
    
    void sensorControl(SensorEvent &e);
    
    // Serial var
    ofSerial serial;
    ofArduino firmata;
    bool bFirmataSetup;
    string buffer;
    long limitBuffer;
    bool transition;
    int activeScene, transScene, queuedScene;
    Scene * scenes[NUMSCENES];
    Scene * interface;
    
};
