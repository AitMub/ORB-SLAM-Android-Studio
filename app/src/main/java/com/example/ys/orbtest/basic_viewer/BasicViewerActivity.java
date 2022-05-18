package com.example.ys.orbtest.basic_viewer;
import android.Manifest;
import android.app.Activity;
import android.content.pm.PackageManager;
import android.os.Build;
import android.os.Bundle;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;

import android.content.res.AssetManager;

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
        try {
            FileInputStream fileInputStream = new FileInputStream(new File("/storage/emulated/0/Models/bunny/bunny.obj"));
            byte[] bytes=new byte[fileInputStream.available()];
            fileInputStream.read(bytes);
            String content=new String(bytes);
            fileInputStream.close();
        }catch (Exception e) {
            e.printStackTrace();
        }

        myGLSurfaceView = new MyGLSurfaceView(this,mgr);
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

