//
//  scene.cpp
//  plumPrototype
//
//  Created by Andrew Lovett Barron on 2013-04-19.
//
//

#include "scene.h"


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
    vid->draw(0, 0, ofGetWidth(),ofGetHeight());
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
    
    if(!vid->isPlaying()) vid->play(); // Check
    
    vid->update(); // Update video
    
    // Is the movie done if it's an intro?
    if(vid->getIsMovieDone() && state == 0) {
        state = 1;
    } else
        
    // Is the movie done if it's an outro?
    if( vid->getIsMovieDone() && state == 2 ) {
//        begin.stop();
//        loop.stop();
//        end.stop();
//        
        state = 0;
        bDone = true;
        bActive = false;
    }
}