//
// Copyright (c) 2016 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#pragma once


#include "ofLog.h"


namespace ofx {
namespace OpenH264 {

class Module
{
public:
    virtual ~Module()
    {
    }


    /// \returns the name of the module.
    virtual std::string name() const = 0;

};


class Common
{
public:
    static ofLogLevel toOfLogLevel(int welsLogLevel);

    typedef void (*LoggerCallback)(void*, int, const char*);
    static void logger_cb(void* context, int level, const char* message);

};


} } // namespace ofx::OpenH264
