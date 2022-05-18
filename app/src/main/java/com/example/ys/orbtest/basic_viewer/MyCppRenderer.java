package com.example.ys.orbtest.basic_viewer;

import android.content.res.AssetManager;

public class MyCppRenderer {
    private AssetManager mgr;

    MyCppRenderer(AssetManager _mgr) {
        mgr = _mgr ;
    }

    void draw() {
        _draw();
    }

    void init() { _init(mgr); }

    private native void _init(AssetManager _mgr);
    private native void _draw();

    // Used to load the 'BasicRenderer' library on application startup.
    static { System.loadLibrary("basic_renderer"); }
    static { System.loadLibrary("assimp"); }
}
