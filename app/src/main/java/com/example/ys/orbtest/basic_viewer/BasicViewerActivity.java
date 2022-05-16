package com.example.ys.orbtest.basic_viewer;
import android.os.Bundle;
import androidx.appcompat.app.AppCompatActivity;
import android.content.res.AssetManager;


public class BasicViewerActivity extends AppCompatActivity {

    public MyGLSurfaceView myGLSurfaceView;

    public AssetManager mgr;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        mgr = getResources().getAssets();

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

