package com.example.ys.orbtest.basic_viewer;
import android.os.Bundle;
import androidx.appcompat.app.AppCompatActivity;
import android.content.res.AssetManager;
import android.util.Log;

import com.example.ys.orbtest.entity.Parameter;

import java.io.Serializable;


public class BasicViewerActivity extends AppCompatActivity {

    public MyGLSurfaceView myGLSurfaceView;

    public AssetManager mgr;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        mgr = getResources().getAssets();

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

