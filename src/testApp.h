#pragma once

#include "ofMain.h"
#include "ofEvents.h"

#define NUMSCENES 5
#define NUMPHOTO 4
#define FIRMATA TRUE
#define TIMEOUT 800

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
//        msg = '';
    }
    
    int type; // 0 analog, 1 digital, 2 sysex
    int pin;
    int payload;
    string msg;
    static ofEvent <SensorEvent> events;
};

class testApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    void changeScene(int sceneId);
    void drawDebug(bool draw);
    
    void keyPressed  (int key);
    
    void setupArduino(const int & version);
    void updateArduino();
    
    void digitalDelegate(const int & pinNum);
    void analogDelegate(const int & pinNum);
    void sysexDelegate(const vector<unsigned char> & sysex);
    void stringDelegate(const string & str);
    
    void sensorControl(SensorEvent &e);
    
    // Serial var
    ofSerial serial;
    ofArduino firmata;
    bool bFirmataSetup;
    string buffer;
    long limitBuffer;
    bool transition, debug;
    int activeScene, transScene, queuedScene;
    Scene * scenes[NUMSCENES];
    Scene * interface;
    
    // Sensor buffers
    float photo[NUMPHOTO];
    float rfidphoto[2];
//    string rfid1, rfid2;
    string user;
    string book;
    bool isUser, isObject;
    ofImage user1, user2, book1, book2;
    
    // checks for rfids
    string stringBuffer;
    bool stringComplete;
    int time1,time2;
};
