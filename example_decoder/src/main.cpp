//
// Copyright (c) 2016 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#include "ofApp.h"


int main()
{
    ofGLWindowSettings settings;
    settings.setGLVersion(4, 1);
    settings.width = 720;
    settings.height = 576;
    ofCreateWindow(settings);
    return ofRunApp(std::make_shared<ofApp>());
}
