package com.example.ys.orbtest;

import android.content.Intent;
import android.os.Bundle;
import android.util.DisplayMetrics;
import android.view.View;

import androidx.appcompat.app.AppCompatActivity;

import com.example.ys.orbtest.basic_viewer.BasicViewerActivity;
import com.example.ys.orbtest.entity.Parameter;

import java.util.Locale;

public class ThreeDActivity extends AppCompatActivity implements View.OnClickListener {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_three_dactivity);

        findViewById(R.id.ThreeDButton).setOnClickListener(this);
        findViewById(R.id.ARButton).setOnClickListener(this);
    }

    @Override
    public void onClick(View view) {
        switch (view.getId()){
            case  R.id.ThreeDButton:
                DisplayMetrics dm = getResources().getDisplayMetrics();
                int screenWidth = dm.widthPixels;
                int screenHeight = dm.heightPixels;
                Intent intent = new Intent(this, BasicViewerActivity.class);
                Parameter parameter = new Parameter();
                parameter.setScreenHeight(screenHeight);
                parameter.setScreenWidth(screenWidth);
                intent.putExtra("data", parameter);
                startActivity(intent);
                break;

            case R.id.ARButton:
                Intent intent2 = new Intent(this, OrbTest.class);
                startActivity(intent2);
                break;
        }
    }
}