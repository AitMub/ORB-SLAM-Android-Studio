package com.example.ys.orbtest.basic_viewer;

import android.content.res.AssetManager;

import com.example.ys.orbtest.entity.Parameter;

public class MyCppRenderer {
    private AssetManager mgr;

    public Parameter parameter;

    MyCppRenderer(AssetManager _mgr, Parameter parameter) {
        mgr = _mgr ;
        this.parameter = parameter;
    }

    void draw() {
        _draw();
    }

    void init() { _init(mgr); }

    void onResolutionChanged(){ _onResolutionChanged();}

    public void setModelPath(String path){
        _setModelPath(path);
    }

    public void setPattern(String pattern){
        _setPattern(pattern);
    }

    public void setSunX(int progress, int max){
        _setSunX(progress, max);
    }

    public void setSunY(int progress, int max){
        _setSunY(progress, max);
    }

    public void setSunZ(int progress, int max){
        _setSunY(progress, max);
    }

    private native void _init(AssetManager _mgr);
    private native void _draw();

    private native void _onResolutionChanged();

    private native void _setModelPath(String path);

    private native void _setPattern(String pattern);

    private native void _setSunX(int progress, int max);
    private native void _setSunY(int progress, int max);
    private native void _setSunZ(int progress, int max);

    // Used to load the 'BasicRenderer' library on application startup.
    static { System.loadLibrary("basic_renderer"); }
    static { System.loadLibrary("assimp"); }

}
