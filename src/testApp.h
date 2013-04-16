#pragma once

#include "ofMain.h"

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
    
    //Plum
    
    bool isRfidPresent();
    string getRfid();

    // Book
    bool isObjectPresent();
    bool isObjectOpen();
    
    // self
    void update();
    void renderSensors();
private:
    bool rfidPresent, objectPresent, objectOpen;
    unsigned int photo0, photo1, photo2, photo3;
    string rfidId, objId;
    
     static ofEvent <SensorEvent> events;
};

class testApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    void changeScene(int sceneId);
    
    void keyPressed  (int key);
    
    void setupArduino();
    void writeArduino();
    
    // Serial var
    ofSerial serial;
    string buffer;
    long limitBuffer;
    bool transition;
    int activeScene, transScene, queuedScene;
    Scene * scenes[NUMSCENES];
    Scene * interface;
    
};
