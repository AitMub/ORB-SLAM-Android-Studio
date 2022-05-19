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

    private native void _init(AssetManager _mgr);
    private native void _draw();
    private native void _onResolutionChanged();

    // Used to load the 'BasicRenderer' library on application startup.
    static { System.loadLibrary("basic_renderer"); }
    static { System.loadLibrary("assimp"); }
}
