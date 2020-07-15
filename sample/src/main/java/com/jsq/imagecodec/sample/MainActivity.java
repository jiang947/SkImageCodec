package com.jsq.imagecodec.sample;

import androidx.appcompat.app.AppCompatActivity;

import android.Manifest;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.ColorSpace;
import android.graphics.Rect;
import android.os.Bundle;
import android.util.Log;
import android.view.ViewGroup;
import android.widget.ImageView;

import com.jsq.skbitmapcodec.SkBitmapDecoder;
import com.tbruyelle.rxpermissions2.RxPermissions;

import java.io.IOException;

import io.reactivex.disposables.Disposable;

public class MainActivity extends AppCompatActivity {

    private static final String TAG = "MainActivity";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
//        RxPermissions permissions = new RxPermissions(this);
//        Disposable disposable = permissions.
//                request(Manifest.permission.READ_EXTERNAL_STORAGE)
//                .filter(isGranted -> isGranted)
//                .subscribe(isGranted -> {
//                    renderSimpleDecode();
//                }, Throwable::printStackTrace);

        ViewGroup buttonPanel = (ViewGroup) findViewById(R.id.buttonPanel);
        for (int i = 0; i < buttonPanel.getChildCount(); i++) {
            buttonPanel.getChildAt(i).setOnClickListener(view -> {
                int id = view.getId();
                switch (id) {
                    case R.id.btn1:
                        renderSimpleDecode();
                        break;
                    case R.id.btn2:
                        renderDecodeRegion();
                        break;

                }
            });
        }
        BitmapFactory.Options options = new BitmapFactory.Options();
        options.inPreferredConfig = Bitmap.Config.RGB_565;

        Bitmap bitmap = BitmapFactory.decodeFile("/storage/emulated/0/DCIM/apple.png", options);
        Log.e(TAG, "onCreate: RGB565 bitmap size:" + bitmap.getAllocationByteCount());
        bitmap.recycle();
        bitmap = null;
        bitmap = BitmapFactory.decodeFile("/storage/emulated/0/DCIM/apple.png");

//        Log.e(TAG, "onCreate: default bitmap size:" + bitmap.getAllocationByteCount());
//        bitmap.recycle();
//        bitmap = null;

        //1048576


    }

    private void renderSimpleDecode() {
        ImageView imageView = findViewById(R.id.image_view);
        try {
            SkBitmapDecoder decoder = SkBitmapDecoder.newInstance("/storage/emulated/0/DCIM/apple.png");
            BitmapFactory.Options options = new BitmapFactory.Options();
            Bitmap bitmap = decoder.decode(options, Bitmap.Config.RGB_565);
            if (bitmap != null) {
                imageView.setImageBitmap(bitmap);

            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    private void renderDecodeRegion() {
        ImageView imageView = findViewById(R.id.image_view);
        try {
            SkBitmapDecoder decoder = SkBitmapDecoder.newInstance("/storage/emulated/0/DCIM/big.png");
            BitmapFactory.Options options = new BitmapFactory.Options();
            Bitmap bitmap = decoder.decodeDecode(new Rect(0, 0, decoder.getWidth(), decoder.getHeight()), options, Bitmap.Config.ARGB_8888);
            if (bitmap != null) {
                imageView.setImageBitmap(bitmap);
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }


}
