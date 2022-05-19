package com.example.ys.orbtest.entity;


import java.io.Serializable;

public class Parameter implements Serializable {
    private static final long serialVersionUID = 1L;

    private int screenHeight;
    private int screenWidth;

    public int getScreenHeight() {
        return screenHeight;
    }

    public void setScreenHeight(int screenHeight) {
        this.screenHeight = screenHeight;
    }

    public int getScreenWidth() {
        return screenWidth;
    }

    public void setScreenWidth(int screenWidth) {
        this.screenWidth = screenWidth;
    }

    @Override
    public String toString() {
        return "Parameter{" +
                "screenHeight=" + screenHeight +
                ", screenWidth=" + screenWidth +
                '}';
    }
}
