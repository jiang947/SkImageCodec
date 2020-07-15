package com.jsq.skbitmapcodec;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.BitmapRegionDecoder;
import android.graphics.Rect;

import androidx.annotation.Nullable;

import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;

public class SkBitmapDecoder {

    static {
        System.loadLibrary("skimagecodec");
    }

    private long mNativeSkBitmapDecoder;


    public static SkBitmapDecoder newInstance(String filepath) throws IOException {
        try (InputStream stream = new FileInputStream(filepath)) {
            byte[] tempStorage = new byte[16 * 1024];
            return nativeNewInstance(stream, tempStorage);
        }
    }

    private SkBitmapDecoder(long nativeSkBitmapDecoder) {
        this.mNativeSkBitmapDecoder = nativeSkBitmapDecoder;
    }

    //BitmapRegionDecoder
    public Bitmap decode(BitmapFactory.Options options, @Nullable Bitmap.Config config) {
        return nativeDecode(mNativeSkBitmapDecoder, options, config);
    }

    public Bitmap decodeDecode(Rect rect, BitmapFactory.Options options, Bitmap.Config config) {
        return nativeDecodeRegion(mNativeSkBitmapDecoder, rect, options, config);
    }


    public int getWidth(){
        return nativeGetWidth(mNativeSkBitmapDecoder);
    }

    public int getHeight(){
        return nativeGetHeight(mNativeSkBitmapDecoder);
    }

    public void destroy() {
        nativeDestroy(mNativeSkBitmapDecoder);
    }


    private static native SkBitmapDecoder nativeNewInstance(InputStream filepath, byte[] tempStorage);


    private native Bitmap nativeDecode(long nativeInstance, BitmapFactory.Options options, Bitmap.Config config);

    private native Bitmap nativeDecodeRegion(long nativeInstance, Rect rect, BitmapFactory.Options options, Bitmap.Config config);

    private native int nativeGetWidth(long nativeInstance);

    private native int nativeGetHeight(long nativeInstance);


    private native void nativeDestroy(long nativeInstance);

}
