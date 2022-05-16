package com.example.ys.orbtest.basic_viewer;

import android.content.Context;
import android.content.res.AssetManager;
import android.opengl.GLSurfaceView;

public class MyGLSurfaceView extends GLSurfaceView{
    private final MyGLRenderer myGLRenderer;
    public MyGLSurfaceView(Context context, AssetManager mgr) {
        super(context);
        setEGLContextClientVersion(3);
        myGLRenderer = new MyGLRenderer(mgr);
        setRenderer(myGLRenderer);
    }
}
