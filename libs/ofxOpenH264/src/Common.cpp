//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#include "ofx/OpenH264/Common.h"
#include "wels/codec_api.h"


namespace ofx {
namespace OpenH264 {


ofLogLevel Common::toOfLogLevel(int welsLogLevel)
{
    if (welsLogLevel <= WELS_LOG_QUIET)
    {
        return OF_LOG_SILENT;
    }
    else if (welsLogLevel <= WELS_LOG_ERROR)
    {
        return OF_LOG_ERROR;
    }
    else if (welsLogLevel <= WELS_LOG_WARNING)
    {
        return OF_LOG_WARNING;
    }
    else if (welsLogLevel <= WELS_LOG_INFO)
    {
        return OF_LOG_NOTICE;
    }
    else
    {
        return OF_LOG_VERBOSE;
    }
}


void Common::logger_cb(void* context, int level, const char* message)
{
    Module* caller = reinterpret_cast<Module*>(context);
    std::string module = (caller != nullptr) ? caller->name() : "ofx::OpenH264::Common::logger_cb";
    ofLogLevel logLevel = toOfLogLevel(level);

    switch (logLevel)
    {
        case OF_LOG_VERBOSE:
            ofLogVerbose(module) << message;
            return;
        case OF_LOG_NOTICE:
            ofLogNotice(module) << message;
            return;
        case OF_LOG_WARNING:
            ofLogWarning(module) << message;
            return;
        case OF_LOG_ERROR:
            ofLogError(module) << message;
            return;
        case OF_LOG_FATAL_ERROR:
            ofLogFatalError(module) << message;
            return;
        case OF_LOG_SILENT:
            return;
    }
}


} }  // namespace ofx::OpenH264
