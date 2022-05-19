package com.example.ys.orbtest.basic_viewer;

import android.content.res.AssetManager;

import com.example.ys.orbtest.entity.Parameter;

public class MyCppRenderer {
    private AssetManager mgr;

    private Parameter parameter;

    MyCppRenderer(AssetManager _mgr, Parameter parameter) {
        mgr = _mgr ;
        this.parameter = parameter;
    }

    void draw() {
        _draw();
    }

    void init() { _init(mgr); }

    private native void _init(AssetManager _mgr);
    private native void _draw();
    native void onParameterChanged();
    // Used to load the 'basic_renderer' library on application startup.
    static {
        System.loadLibrary("basic_renderer");
    }
}
