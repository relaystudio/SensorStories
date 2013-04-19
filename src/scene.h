//
//  scene.h
//  plumPrototype
//
//  Created by Andrew Lovett Barron on 2013-04-19.
//
//

#ifndef __plumPrototype__scene__
#define __plumPrototype__scene__

#include "ofMain.h"

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

#endif /* defined(__plumPrototype__scene__) */
