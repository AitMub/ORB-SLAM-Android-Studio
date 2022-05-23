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

    public void setModelPath(String path){
        myGLRenderer.setModelPath(path);
    }

    public void setPattern(String pattern){
        myGLRenderer.setPattern(pattern);
    }

    public void setSunHeight(int progress, int max){
        myGLRenderer.setSunHeight(progress, max);
    }

    public void setSunRotation(int progress, int max){
        myGLRenderer.setSunRotation(progress, max);
    }

    public void setSunRadius(int progress, int max){
        myGLRenderer.setSunRadius(progress, max);
    }

    public void passVector(float x, float y) {
        myGLRenderer.passVector(x, y);
    }

    public void zoom(double ratio) {
        myGLRenderer.zoom(ratio);
    }
}
