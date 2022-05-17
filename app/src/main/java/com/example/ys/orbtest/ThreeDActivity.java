package com.example.ys.orbtest;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;

import androidx.appcompat.app.AppCompatActivity;

import com.example.ys.orbtest.basic_viewer.BasicViewerActivity;

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
                Intent intent = new Intent(this, BasicViewerActivity.class);
                startActivity(intent);
                break;

            case R.id.ARButton:
                Intent intent2 = new Intent(this, OrbTest.class);
                startActivity(intent2);
                break;
        }
    }
}