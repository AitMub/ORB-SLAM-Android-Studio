package com.example.ys.orbtest.basic_viewer;

import android.content.res.AssetManager;
import android.opengl.GLES30;
import android.opengl.GLSurfaceView;

import com.example.ys.orbtest.entity.Parameter;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class MyGLRenderer implements GLSurfaceView.Renderer {
    final private MyCppRenderer mCppRenderer;

    MyGLRenderer(AssetManager mgr, Parameter parameter) {
        mCppRenderer = new MyCppRenderer(mgr, parameter);
    }

    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig eglConfig) {
        gl.glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
        mCppRenderer.init();
    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height) {
        gl.glViewport(0, 0, width, height);
        // change value in parameter
        mCppRenderer.parameter.setScreenWidth(width);
        mCppRenderer.parameter.setScreenHeight(height);
        // pass change to cpp
        mCppRenderer.onResolutionChanged();
    }

    @Override
    public void onDrawFrame(GL10 gl) {
        gl.glClear(GL10.GL_COLOR_BUFFER_BIT | GL10.GL_DEPTH_BUFFER_BIT);
        mCppRenderer.draw();
    }

    static int loadShader(int type, String shaderCode) {
        int shader = GLES30.glCreateShader(type);

        GLES30.glShaderSource(shader, shaderCode);
        GLES30.glCompileShader(shader);

        return shader;
    }


    private static class Triangle {
        private FloatBuffer vertexBuffer;
        private int mProgram;
        private int mPositionHandle;
        private int mColorHandle;

        private final int vertexCount = triangleCoords.length / COORDS_PER_VERTEX;
        private final int vertexStride = COORDS_PER_VERTEX * 4; // 4 bytes per vertex

        private final String vertexShaderCode =
                "attribute vec4 vPosition;" + "void main() {" + "gl_Position = vPosition;" + "}";
        private final String fragmentShaderCode =
                "precision mediump float;" + "uniform vec4 vColor;" + "void main() {" + "gl_FragColor = vColor;" + "}";

        static final int COORDS_PER_VERTEX = 3;
        static  float triangleCoords[] = {
                0.0f, 0.122008459f, 0.0f,
                -0.7f, -0.711004243f, 0.0f,
                0.1f, -0.711004243f, 0.0f
        };
        float color[] = {0.63671875f, 0.76953125f, 0.22265625f, 1.0f};

        public Triangle() {
            ByteBuffer bb = ByteBuffer.allocateDirect(
                    triangleCoords.length *4);
            bb.order(ByteOrder.nativeOrder());

            vertexBuffer = bb.asFloatBuffer();
            vertexBuffer.put(triangleCoords);
            vertexBuffer.position(0);
        }

        public void init() {
            int vertexShader = MyGLRenderer.loadShader(GLES30.GL_VERTEX_SHADER, vertexShaderCode);
            int fragmentShader = MyGLRenderer.loadShader(GLES30.GL_FRAGMENT_SHADER, fragmentShaderCode);

            mProgram = GLES30.glCreateProgram();
            GLES30.glAttachShader(mProgram, vertexShader);
            GLES30.glAttachShader(mProgram, fragmentShader);
            GLES30.glLinkProgram(mProgram);
        }

        public void draw() {
            GLES30.glUseProgram(mProgram);
            mPositionHandle = GLES30.glGetAttribLocation(mProgram, "vPosition");
            GLES30.glEnableVertexAttribArray(mPositionHandle);
            GLES30.glVertexAttribPointer(mPositionHandle, COORDS_PER_VERTEX,
                    GLES30.GL_FLOAT, false,
                    vertexStride, vertexBuffer);

            mColorHandle = GLES30.glGetUniformLocation(mProgram, "vColor");
            GLES30.glUniform4fv(mColorHandle, 1, color, 0);
            GLES30.glDrawArrays(GLES30.GL_TRIANGLES, 0, vertexCount);
            GLES30.glDisableVertexAttribArray(mPositionHandle);
        }
    }
}


