//  sensorEvent.h
//  plumPrototype
//
//  Created by Andrew Lovett Barron on 2013-04-19.
//
//

#ifndef __plumPrototype__sensorEvent__
#define __plumPrototype__sensorEvent__

#include "ofMain.h"
#include "ofEvents.h"

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

#endif /* defined(__plumPrototype__sensorEvent__) */
