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


class Decoder: public Module
{
public:
    Decoder();
    virtual ~Decoder();

    std::string name() const override;

    /// \brief Set the internal log level for the decoder.
    void setLogLevel(ofLogLevel logLevel);

//    /// \brief Write arbitrary bytes to be decoded.
//    ///
//    /// Upon decoding a frame, the onPixels event will be called.
//    ///
//    /// \param buffer The bytes to be decoded.
//    void writeBytes(const ofBuffer& buffer);
//
//    /// \brief Write arbitrary bytes to be decoded.
//    ///
//    /// Upon decoding a frame, the onPixels event will be called.
//    ///
//    /// \param buffer The bytes to be decoded.
//    void writeBytes(const uint8_t* buffer, std::size_t size);

    /// \brief Get the size of the next slice.
    ///
    /// If no slice boundary is found, false is returned.
    ///
    /// \param buffer A pointer to the beginning of a buffer.
    /// \param offset The offset in the given buffer to begin looking for the next slice.
    /// \param sliceSize The size of the next slice found offset from \p position.
    /// \returns false if no boundary is found, or true if a slice is found.
    static bool findNextSlice(const uint8_t* buffer,
                              std::size_t size,
                              std::size_t position,
                              std::size_t& sliceSize);


    /// \brief Event called when a new frame is decoded.
//    ofEvent<const ofPixels> onPixels;

//protected:
    /// \brief Update the decoder with a new data slice.
    /// \param buffer A pointer to the beginning of the data slice.
    /// \param size The length of the data slice.
    /// \param pixels The pixels object to fill if possible.
    /// \param resultCode The result code.
    /// \returns true if a new frame is available.
    bool decode(const uint8_t* buffer,
                     std::size_t size,
                     ofPixels& pixels,
                     DECODING_STATE& state);

    /// \brief Decode the entire buffer into a vector of frames.
    /// \param buffer The raw h264 bitstream buffer to decode.
    /// \param size The size of the raw h264 bistream buffer to decode.
    /// \returns a vector of frames.
    std::vector<ofPixels> decode(const uint8_t* buffer, std::size_t size);


private:
    //decoder declaration
    ISVCDecoder* pSvcDecoder = nullptr;

};


} } // namespace ofx::OpenH264
