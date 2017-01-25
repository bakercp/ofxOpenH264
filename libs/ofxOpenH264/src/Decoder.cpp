//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#include "ofx/OpenH264/Decoder.h"
#include "ofx/OpenH264/Common.h"
#include <iostream>


namespace ofx {
namespace OpenH264 {


Decoder::Decoder()
{
    auto result = WelsCreateDecoder(&pSvcDecoder);

    if (result != 0 || pSvcDecoder == nullptr)
        throw std::runtime_error("Unable to allocate decoder.");

    SDecodingParam sDecParam = { 0 };
    sDecParam.sVideoProperty.size = sizeof(sDecParam.sVideoProperty);
    sDecParam.sVideoProperty.eVideoBsType = VIDEO_BITSTREAM_DEFAULT;
    sDecParam.bParseOnly = false;

    pSvcDecoder->Initialize(&sDecParam);

    pSvcDecoder->SetOption(DECODER_OPTION_TRACE_CALLBACK, reinterpret_cast<Common::LoggerCallback*>(&Common::logger_cb));
    pSvcDecoder->SetOption(DECODER_OPTION_TRACE_CALLBACK_CONTEXT, this);

    // TODO: Allow configuration of this option.
    int32_t iErrorConMethod = int32_t(ERROR_CON_SLICE_MV_COPY_CROSS_IDR_FREEZE_RES_CHANGE);
    pSvcDecoder->SetOption(DECODER_OPTION_ERROR_CON_IDC, &iErrorConMethod);

}


Decoder::~Decoder()
{
    if (pSvcDecoder != nullptr)
    {
        pSvcDecoder->Uninitialize();
        WelsDestroyDecoder(pSvcDecoder);
    }
}


bool Decoder::decode(const uint8_t* buffer,
                     std::size_t size,
                     ofPixels& pixels,
                     DECODING_STATE& state)
{
    uint8_t* pData[3] = { nullptr, nullptr, nullptr };

    SBufferInfo sDstBufInfo;
    std::memset(&sDstBufInfo, 0, sizeof(SBufferInfo));

    state = pSvcDecoder->DecodeFrameNoDelay(buffer,
                                            size,
                                            pData,
                                            &sDstBufInfo);

    if (sDstBufInfo.iBufferStatus == 1 && pData[0] != nullptr && pData[1] != nullptr && pData[2] != nullptr)
    {
        if (videoFormatI420 == sDstBufInfo.UsrData.sSystemBuffer.iFormat)
        {
            // N.B. This will only reallocate memory if enough is not
            // already allocated.
            pixels.allocate(sDstBufInfo.UsrData.sSystemBuffer.iWidth,
                            sDstBufInfo.UsrData.sSystemBuffer.iHeight,
                            OF_PIXELS_I420);

            std::vector<std::size_t> strides = {
                std::size_t(sDstBufInfo.UsrData.sSystemBuffer.iStride[0]),
                std::size_t(sDstBufInfo.UsrData.sSystemBuffer.iStride[1]),
                std::size_t(sDstBufInfo.UsrData.sSystemBuffer.iStride[1])
            };

            std::size_t width = sDstBufInfo.UsrData.sSystemBuffer.iWidth;
            std::size_t height = sDstBufInfo.UsrData.sSystemBuffer.iHeight;

            std::size_t halfWidth = width / 2;
            std::size_t halfHeight = height / 2;

            unsigned char* destination = reinterpret_cast<unsigned char*>(pixels.getData());

            unsigned char* source = nullptr;

            source = pData[0];

            // Y Plane
            for (size_t i = 0; i < height; ++i)
            {
                std::memcpy(destination, source, width);
                source += strides[0];
                destination += width;
            }

            source = pData[1];

            // U Plane
            for (size_t i = 0; i < halfHeight; i++)
            {
                std::memcpy(destination,source,halfWidth);
                source += strides[1];
                destination += halfWidth;
            }

            source = pData[2];

            // V Plane
            for (size_t i = 0; i < halfHeight; ++i)
            {
                std::memcpy(destination, source, halfWidth);
                source += strides[2];
                destination += halfWidth;
            }

            // This could replace a lot of the above once we confirm that
            // pData[0], pData[1] and pData[2] are always contiguous in memory.
            // pixels.setFromAlignedPixels(pData[0], width, height, OF_PIXELS_I420, strides);

            return true;

        }
        else
        {
            ofLogError("ofx::OpenH264::Decoder") << "Unhandled decoded format: " << sDstBufInfo.UsrData.sSystemBuffer.iFormat;
        }
    }

    return false;
}


std::vector<ofPixels> Decoder::decode(const uint8_t* pBuffer, std::size_t bufferSize)
{
    std::vector<ofPixels> frames;

    std::size_t currentPosition = 0;
    std::size_t sliceSize = 0;
    DECODING_STATE state = dsErrorFree;

    ofPixels pixels;

    while (findNextSlice(pBuffer, bufferSize, currentPosition, sliceSize))
    {
        if (sliceSize < 4)
        {
            currentPosition += sliceSize;
            continue;
        }

        if (decode(pBuffer + currentPosition, sliceSize, pixels, state))
        {
            frames.push_back(pixels);
        }

        currentPosition += sliceSize;
    }

    return frames;
}



std::string Decoder::name() const
{
    return "ofx::OpenH264::Decoder";
}


void Decoder::setLogLevel(ofLogLevel logLevel)
{
    pSvcDecoder->SetOption(DECODER_OPTION_TRACE_LEVEL, &logLevel);
}


bool Decoder::findNextSlice(const uint8_t* buffer,
                            std::size_t size,
                            std::size_t position,
                            std::size_t& sliceSize)
{
    for (std::size_t i = (position + 1); i < size; ++i)
    {
        if ((buffer[i + 0] == 0 &&
             buffer[i + 1] == 0 &&
             buffer[i + 2] == 0 &&
             buffer[i + 3] == 1 && i > 0) ||
            (buffer[i + 0] == 0 &&
             buffer[i + 1] == 0 &&
             buffer[i + 2] == 1 && i > 0))
        {
            sliceSize = i - position;
            return true;
        }
    }

    return false;
}


} }  // namespace ofx::H264
