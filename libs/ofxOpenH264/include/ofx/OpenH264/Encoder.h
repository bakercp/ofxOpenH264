//
// Copyright (c) 2016 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#pragma once


#include <cstddef>
#include <cstdint>
#include "wels/codec_api.h"
#include "ofPixels.h"
#include "ofLog.h"
#include "ofx/OpenH264/Common.h"
#include "ofEvent.h"



namespace ofx {
namespace OpenH264 {


class Encoder: public Module
{
public:
    Encoder();
    virtual ~Encoder();

    std::string name() const override;

    /// \brief Set the internal log level for the decoder.
    void setLogLevel(ofLogLevel logLevel);

    
private:
    /// \brief Encoder declaration.
    ISVCEncoder* pSvcEncoder = nullptr;
    
};


} } // namespace ofx::OpenH264
