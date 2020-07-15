//
// Created by 蒋诗曲 on 2020/7/13.
//

#ifndef SKIACODEC_SK_BITMAP_DECODER_H
#define SKIACODEC_SK_BITMAP_DECODER_H

#include <include/codec/SkAndroidCodec.h>

class SkBitmapDecoder {

public:
    static SkBitmapDecoder *create(std::unique_ptr<SkStream> stream);

    bool decode(void *pix, int sampleSize = 1);

    bool decodeRegion(void *pix, const SkIRect &desiredSubset,int sampleSize = 1);

    int width() const { return width_; }

    int height() const { return height_; }

    SkISize getSampledDimensions(int sampleSize) const {
        return codec_->getSampledDimensions(sampleSize);
    }

    SkISize getSampledSubsetDimensions(int sampleSize, const SkIRect &subset) const {
        return codec_->getSampledSubsetDimensions(sampleSize, subset);
    }


private:
    SkBitmapDecoder(SkAndroidCodec *codec);

    std::unique_ptr<SkAndroidCodec> codec_;
    const int width_;
    const int height_;

};


#endif //SKIACODEC_SK_BITMAP_DECODER_H
