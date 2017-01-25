//
// Copyright (c) 2016 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#pragma once


#include "ofMain.h"
#include "ofxOpenH264.h"


class ofApp: public ofBaseApp
{
public:
    void setup();
    void draw();

    ofTexture frame;
    ofTexture yFrame;
    ofTexture uFrame;
    ofTexture vFrame;
    std::vector<ofPixels> frames;
    std::size_t currentFrame = 0;

    ofVideoGrabber grabber;

};
