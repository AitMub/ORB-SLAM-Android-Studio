package com.example.ys.orbtest;


import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import android.Manifest;
import android.os.Bundle;
import android.os.Environment;
import android.util.Log;

import com.example.ys.orbtest.util.PermissionUtil;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;

public class TestActivity extends AppCompatActivity {

    private static final String[] PERMISSIONS_FILE = new String[]{
            Manifest.permission.READ_EXTERNAL_STORAGE,
            Manifest.permission.WRITE_EXTERNAL_STORAGE
    };

    private static final int REQUEST_CODE_FILE = 1;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_test);
        String fileName = "b.txt";
        //外部存储（SD卡）的公共空间
        String directory = Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_DOWNLOADS).toString();

        String path = directory + File.separatorChar + fileName;
        Log.d("zjh", path);

        PermissionUtil.checkPermission(this, PERMISSIONS_FILE, REQUEST_CODE_FILE);

        try {

//            FileOutputStream fileOutputStream = new FileOutputStream(path);
//            OutputStreamWriter outputStreamWriter = new OutputStreamWriter(fileOutputStream);
//            BufferedWriter bufferedWriter = new BufferedWriter(outputStreamWriter);
//            bufferedWriter.write("1111111111111111111\n");
//            bufferedWriter.close();

            FileInputStream fileInputStream = new FileInputStream(path);
            InputStreamReader inputStreamReader = new InputStreamReader(fileInputStream);
            BufferedReader bufferedReader = new BufferedReader(inputStreamReader);
            String s = bufferedReader.readLine();
            bufferedReader.close();
            Log.d("zjh", s);
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }

    }

    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
        switch (requestCode){
            case REQUEST_CODE_FILE:
                if (PermissionUtil.checkGrant(grantResults)){
                    Log.d("zjh", "外部储存读写权限授权成功");
                }
                break;
        }
    }
}