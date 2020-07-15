//
// Created by 蒋诗曲 on 2020/7/13.
//

#include "sk_bitmap_decoder.h"
#include "log.h"

#include <stdio.h>
#include "include/codec/SkAndroidCodec.h"
#include <include/core/SkBitmap.h>
#include <include/core/SkData.h>
#include <include/core/SkImage.h>


SkBitmapDecoder *SkBitmapDecoder::create(std::unique_ptr<SkStream> stream) {

    auto codec = SkAndroidCodec::MakeFromStream(std::move(stream));
    if (nullptr == codec) {
        return nullptr;
    }
    switch (codec->getEncodedFormat()) {
        case SkEncodedImageFormat::kJPEG:
        case SkEncodedImageFormat::kPNG:
        case SkEncodedImageFormat::kWEBP:
        case SkEncodedImageFormat::kHEIF:
            break;
        default:
            return nullptr;
    }
    return new SkBitmapDecoder(codec.release());
}


SkBitmapDecoder::SkBitmapDecoder(SkAndroidCodec *codec) :
        codec_(codec),
        width_(codec->getInfo().width()),
        height_(codec->getInfo().height()) {
//    SkISize size = codec_->getSampledDimensions(1);
//    SkColorType decodeColorType = kN32_SkColorType;
//    SkAlphaType alphaType = kUnpremul_SkAlphaType;
//    sk_sp<SkColorSpace> decodeColorSpace = codec->computeOutputColorSpace(
//            decodeColorType, nullptr);
//    const SkImageInfo decodeInfo = SkImageInfo::Make(size.width(), size.height(),
//                                                     decodeColorType, alphaType, decodeColorSpace);

}


bool SkBitmapDecoder::decode(void *pix, int sampleSize) {
    SkBitmap bitmap;

    SkAndroidCodec::AndroidOptions codecOptions;
    codecOptions.fZeroInitialized = SkCodec::kYes_ZeroInitialized;
    codecOptions.fSampleSize = sampleSize;
    LOGE("codec_ colorType: %d", codec_->getInfo().colorType())
    SkColorType decodeColorType = codec_->computeOutputColorType(
            kRGB_565_SkColorType); //todo switch type
    LOGE("decode colorType: %d", decodeColorType);
    SkAlphaType alphaType = codec_->computeOutputAlphaType(false);  //todo alpha type
    LOGE("decode alphaType: %d", alphaType);
    sk_sp<SkColorSpace> decodeColorSpace = codec_->computeOutputColorSpace(
            decodeColorType, nullptr);

    SkISize size = codec_->getSampledDimensions(sampleSize);
    const SkImageInfo decodeInfo = SkImageInfo::Make(size.width(), size.height(),
                                                     decodeColorType, alphaType, decodeColorSpace);
    SkImageInfo bitmapInfo = decodeInfo;// decodeInfo;
    if (decodeColorType == kGray_8_SkColorType) {
        // The legacy implementation of BitmapFactory used kAlpha8 for
        // grayscale images (before kGray8 existed).  While the codec
        // recognizes kGray8, we need to decode into a kAlpha8 bitmap
        // in order to avoid a behavior change.
        bitmapInfo =
                bitmapInfo.makeColorType(kAlpha_8_SkColorType).makeAlphaType(kPremul_SkAlphaType);
    }
    bitmap.setInfo(bitmapInfo);
    bitmap.setPixels(pix);

    SkCodec::Result result = codec_->getAndroidPixels(bitmapInfo, bitmap.getPixels(),
                                                      bitmap.rowBytes(), &codecOptions);
    LOGE("result %d :", result)
    return result == SkCodec::kSuccess;
}

bool SkBitmapDecoder::decodeRegion(void *pixels, const SkIRect &desiredSubset, int sampleSize) {
    SkBitmap bitmap;
    bitmap.setPixels(pixels);

    SkIRect subset = desiredSubset;

    SkAndroidCodec::AndroidOptions codecOptions;
    codecOptions.fZeroInitialized = SkCodec::kYes_ZeroInitialized;
    codecOptions.fSampleSize = sampleSize;
    codecOptions.fSubset = &subset;
    SkColorType decodeColorType = codec_->computeOutputColorType(
            kRGB_565_SkColorType); //todo switch type
    SkAlphaType alphaType = codec_->computeOutputAlphaType(false);  //todo alpha type
    sk_sp<SkColorSpace> decodeColorSpace = codec_->computeOutputColorSpace(
            decodeColorType, nullptr);
    SkISize size = getSampledSubsetDimensions(sampleSize, subset);

    const SkImageInfo decodeInfo = SkImageInfo::Make(size.width(), size.height(),
                                                     decodeColorType, alphaType, decodeColorSpace);
    LOGD("decodeRegion size (%d ,%d) :", decodeInfo.width(), decodeInfo.height())
    SkImageInfo bitmapInfo = decodeInfo;// decodeInfo;
    if (decodeColorType == kGray_8_SkColorType) {
        // The legacy implementation of BitmapFactory used kAlpha8 for
        // grayscale images (before kGray8 existed).  While the codec
        // recognizes kGray8, we need to decode into a kAlpha8 bitmap
        // in order to avoid a behavior change.
        bitmapInfo =
                bitmapInfo.makeColorType(kAlpha_8_SkColorType).makeAlphaType(kPremul_SkAlphaType);
    }
    bitmap.setInfo(bitmapInfo);
    bitmap.setPixels(pixels);
    void *dst = bitmap.getAddr(subset.x(), subset.y());
    LOGD("before decode");
    SkCodec::Result result = codec_->getAndroidPixels(bitmapInfo, dst,
                                                      bitmap.rowBytes(), &codecOptions);
    LOGE("decodeRegion result %d :", result)
    return result == SkCodec::kSuccess;
}











