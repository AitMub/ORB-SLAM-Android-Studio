package com.example.ys.orbtest.basic_viewer;
import android.Manifest;
import android.app.Activity;
import android.content.pm.PackageManager;
import android.os.Build;
import android.os.Bundle;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;

import android.content.res.AssetManager;
import android.util.Log;

import com.example.ys.orbtest.entity.Parameter;

import java.io.Serializable;

import java.io.File;
import java.io.FileInputStream;
import java.util.ArrayList;
import java.util.List;


public class BasicViewerActivity extends AppCompatActivity {

    public MyGLSurfaceView myGLSurfaceView;

    public AssetManager mgr;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        mgr = getResources().getAssets();
        
        // read file test
//        try {
//            FileInputStream fileInputStream = new FileInputStream(new File("/data/data/com.example.ys.orbtest.debug/files/b.txt"));
//            byte[] bytes=new byte[fileInputStream.available()];
//            fileInputStream.read(bytes);
//            String content=new String(bytes);
//            Log.d("tag", content);
//            fileInputStream.close();
//        }catch (Exception e) {
//            e.printStackTrace();
//        }

        Parameter parameter = (Parameter) getIntent().getSerializableExtra("data");
        
        myGLSurfaceView = new MyGLSurfaceView(this, mgr, parameter);
        setContentView(myGLSurfaceView);
    }

    @Override
    protected void onPause() {
        super.onPause();
        myGLSurfaceView.onPause();
    }

    @Override
    protected void onResume() {
        super.onResume();
        myGLSurfaceView.onResume();
    }

}

