package com.example.ys.orbtest.basic_viewer;

import android.content.Context;
import android.content.res.AssetManager;
import android.opengl.GLSurfaceView;

import com.example.ys.orbtest.entity.Parameter;

public class MyGLSurfaceView extends GLSurfaceView{
    private final MyGLRenderer myGLRenderer;
    public MyGLSurfaceView(Context context, AssetManager mgr, Parameter parameter) {
        super(context);
        setEGLContextClientVersion(3);
        myGLRenderer = new MyGLRenderer(mgr, parameter);
        setRenderer(myGLRenderer);
    }
}
