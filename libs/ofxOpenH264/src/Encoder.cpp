//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#include "ofx/OpenH264/Encoder.h"
#include <iostream>


namespace ofx {
namespace OpenH264 {


Encoder::Encoder()
{
//    CAMERA_VIDEO_REAL_TIME,      ///< camera video for real-time communication
//    SCREEN_CONTENT_REAL_TIME,    ///< screen content signal
//    CAMERA_VIDEO_NON_REAL_TIME


    int result = WelsCreateSVCEncoder(&pSvcEncoder);
    
    if (result != 0 || pSvcEncoder == nullptr)
        throw std::runtime_error("Unable to allocate encoder.");


//    typedef enum {
//        RC_QUALITY_MODE = 0,     ///< quality mode
//        RC_BITRATE_MODE = 1,     ///< bitrate mode
//        RC_BUFFERBASED_MODE = 2, ///< no bitrate control,only using buffer status,adjust the video quality
//        RC_TIMESTAMP_MODE = 3, //rate control based timestamp
//        RC_BITRATE_MODE_POST_SKIP = 4, ///< this is in-building RC MODE, WILL BE DELETED after algorithm tuning!
//        RC_OFF_MODE = -1,         ///< rate control off mode
//    } RC_MODES;

    SEncParamBase param;
    std::memset(&param, 0, sizeof(SEncParamBase));
    param.iUsageType = CAMERA_VIDEO_REAL_TIME;
    param.fMaxFrameRate = 30;
    param.iPicWidth = 640;
    param.iPicHeight = 480;
    param.iTargetBitrate = 5000000;
    pSvcEncoder->Initialize(&param);


    ///

//    pSvcEncoder->SetOption(ENCODER_OPTION_TRACE_LEVEL, &g_LevelSetting);
//    int videoFormat = videoFormatI420;
//    pSvcEncoder->SetOption(ENCODER_OPTION_DATAFORMAT, &videoFormat);

}


Encoder::~Encoder()
{
    if (pSvcEncoder != nullptr)
    {
        pSvcEncoder->Uninitialize();
        WelsDestroySVCEncoder(pSvcEncoder);
    }
}


std::string Encoder::name() const
{
    return "ofx::OpenH264::Encoder";
}


void Encoder::setLogLevel(ofLogLevel logLevel)
{
    pSvcEncoder->SetOption(ENCODER_OPTION_TRACE_LEVEL, &logLevel);
}


} }  // namespace ofx::H264
