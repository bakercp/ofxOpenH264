//
// Copyright (c) 2016 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#include "ofApp.h"


void ofApp::setup()
{
    grabber.setup(640, 480);

    auto buffer = ofBufferFromFile("half.264");

    ofxOpenH264::Decoder decoder;

    frames = decoder.decode(reinterpret_cast<const uint8_t*>(buffer.getData()), buffer.size());

}


void ofApp::draw()
{
    ofSetColor(255);

    if (!frames.empty())
    {
        frame.loadData(frames[currentFrame]);
        int x = 0;
        frame.draw(x, 0);
        currentFrame = (currentFrame + 1) % frames.size();
    }
}
