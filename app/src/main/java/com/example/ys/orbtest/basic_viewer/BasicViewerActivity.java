package com.example.ys.orbtest.basic_viewer;
import android.Manifest;
import android.app.Activity;
import android.content.ContentUris;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.database.Cursor;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;

import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;

import android.content.res.AssetManager;
import android.os.Environment;
import android.provider.DocumentsContract;
import android.provider.MediaStore;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.LinearLayout;
import android.widget.SeekBar;

import com.example.ys.orbtest.R;
import com.example.ys.orbtest.entity.Parameter;

import java.io.Serializable;

import java.io.File;
import java.io.FileInputStream;
import java.util.ArrayList;
import java.util.List;


public class BasicViewerActivity extends AppCompatActivity implements View.OnClickListener, SeekBar.OnSeekBarChangeListener {

    public MyGLSurfaceView myGLSurfaceView;

    public AssetManager mgr;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        mgr = getResources().getAssets();
        
        // read file test
//        try {
//            FileInputStream fileInputStream = new FileInputStream(new File("/data/data/com.example.ys.orbtest.debug/files/b.txt"));
//            byte[] bytes=new byte[fileInputStream.available()];
//            fileInputStream.read(bytes);
//            String content=new String(bytes);
//            Log.d("tag", content);
//            fileInputStream.close();
//        }catch (Exception e) {
//            e.printStackTrace();
//        }

        Parameter parameter = (Parameter) getIntent().getSerializableExtra("data");
        
        myGLSurfaceView = new MyGLSurfaceView(this, mgr, parameter);
        setContentView(R.layout.activity_basic_viewer);

        LinearLayout threeDViewArea = findViewById(R.id.ThreeDViewArea);
        threeDViewArea.addView(myGLSurfaceView);

        findViewById(R.id.pickFIle).setOnClickListener(this);

        SeekBar barX = findViewById(R.id.barX);
        SeekBar barY = findViewById(R.id.barY);
        SeekBar barZ = findViewById(R.id.barZ);
        barX.setOnSeekBarChangeListener(this);
        barY.setOnSeekBarChangeListener(this);
        barZ.setOnSeekBarChangeListener(this);
    }

    @Override
    protected void onPause() {
        super.onPause();
        myGLSurfaceView.onPause();
    }

    @Override
    protected void onResume() {
        super.onResume();
        myGLSurfaceView.onResume();
    }

    @Override
    public void onClick(View v) {
        switch (v.getId()){
            case R.id.pickFIle:
                Intent intent = new Intent(Intent.ACTION_GET_CONTENT);
                intent.setType("*/*");//设置类型，我这里是任意类型，任意后缀的可以这样写。
                intent.addCategory(Intent.CATEGORY_OPENABLE);
                startActivityForResult(intent,1);
                break;

            case R.id.mode1:
                String pattern1 = ((Button) v).getText().toString();
                myGLSurfaceView.setPattern(pattern1);
                break;

            case R.id.mode2:
                String pattern2 = ((Button) v).getText().toString();
                myGLSurfaceView.setPattern(pattern2);
                break;

            case R.id.mode3:
                String pattern3 = ((Button) v).getText().toString();
                myGLSurfaceView.setPattern(pattern3);
                break;
        }
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, @Nullable Intent data) {
        super.onActivityResult(requestCode, resultCode, data);

        switch (requestCode){
            case 1:
                if (resultCode == RESULT_OK) {
                    Uri uri = data.getData();
                    if (uri != null) {
                        String path = getPath(this, uri);
                        if (path != null) {
                            Log.d("zjh", path);
                            myGLSurfaceView.setModelPath(path);
                        }
                    }
                }
                break;
        }
    }

    @Override
    public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
        switch (seekBar.getId()){
            case R.id.barX:
                myGLSurfaceView.setSunX(progress, seekBar.getMax());
                break;
            case R.id.barY:
                myGLSurfaceView.setSunY(progress, seekBar.getMax());
                break;
            case R.id.barZ:
                myGLSurfaceView.setSunZ(progress, seekBar.getMax());
                break;
        }
    }

    public String getPath(final Context context, final Uri uri) {

        final boolean isKitKat = Build.VERSION.SDK_INT >= Build.VERSION_CODES.KITKAT;

        // DocumentProvider
        if (isKitKat && DocumentsContract.isDocumentUri(context, uri)) {
            // ExternalStorageProvider
            if (isExternalStorageDocument(uri)) {
                final String docId = DocumentsContract.getDocumentId(uri);
//                Log.d("zjh","isExternalStorageDocument***"+uri.toString());
//                Log.d("zjh","docId***"+docId);
//                以下是打印示例：
//                isExternalStorageDocument***content://com.android.externalstorage.documents/document/primary%3ATset%2FROC2018421103253.wav
//                docId***primary:Test/ROC2018421103253.wav
                final String[] split = docId.split(":");
                final String type = split[0];

                if ("primary".equalsIgnoreCase(type)) {
                    return Environment.getExternalStorageDirectory() + "/" + split[1];
                }
            }
            // DownloadsProvider
            else if (isDownloadsDocument(uri)) {
//                Log.i("zjh","isDownloadsDocument***"+uri.toString());
                final String id = DocumentsContract.getDocumentId(uri);
//                final Uri contentUri = ContentUris.withAppendedId(
//                        Uri.parse("content://downloads/public_downloads"), Long.valueOf(id) );

//                return getDataColumn(context, contentUri, null, null);
                return id.split(":")[1];
            }
            // MediaProvider
            else if (isMediaDocument(uri)) {
//                Log.d("zjh","isMediaDocument***"+uri.toString());
                final String docId = DocumentsContract.getDocumentId(uri);
                final String[] split = docId.split(":");
                final String type = split[0];

                Uri contentUri = null;
                if ("image".equals(type)) {
                    contentUri = MediaStore.Images.Media.EXTERNAL_CONTENT_URI;
                } else if ("video".equals(type)) {
                    contentUri = MediaStore.Video.Media.EXTERNAL_CONTENT_URI;
                } else if ("audio".equals(type)) {
                    contentUri = MediaStore.Audio.Media.EXTERNAL_CONTENT_URI;
                }

                final String selection = "_id=?";
                final String[] selectionArgs = new String[]{split[1]};

                return getDataColumn(context, contentUri, selection, selectionArgs);
            }
        }
        // MediaStore (and general)
        else if ("content".equalsIgnoreCase(uri.getScheme())) {
//            Log.d("zjh","content***"+uri.toString());
            return getDataColumn(context, uri, null, null);
        }
        // File
        else if ("file".equalsIgnoreCase(uri.getScheme())) {
//            Log.d("zjh","file***"+uri.toString());
            return uri.getPath();
        }
        return null;
    }

    /**
     * Get the value of the data column for this Uri. This is useful for
     * MediaStore Uris, and other file-based ContentProviders.
     *
     * @param context       The context.
     * @param uri           The Uri to query.
     * @param selection     (Optional) Filter used in the query.
     * @param selectionArgs (Optional) Selection arguments used in the query.
     * @return The value of the _data column, which is typically a file path.
     */
    public String getDataColumn(Context context, Uri uri, String selection,
                                String[] selectionArgs) {

        Cursor cursor = null;
        final String column = "_data";
        final String[] projection = {column};

        try {
            cursor = context.getContentResolver().query(uri, projection, selection, selectionArgs,
                    null);
            if (cursor != null && cursor.moveToFirst()) {
                final int column_index = cursor.getColumnIndexOrThrow(column);
                return cursor.getString(column_index);
            }
        } finally {
            if (cursor != null)
                cursor.close();
        }
        return null;
    }


    public boolean isExternalStorageDocument(Uri uri) {
        return "com.android.externalstorage.documents".equals(uri.getAuthority());
    }

    public boolean isDownloadsDocument(Uri uri) {
        return "com.android.providers.downloads.documents".equals(uri.getAuthority());
    }

    public boolean isMediaDocument(Uri uri) {
        return "com.android.providers.media.documents".equals(uri.getAuthority());
    }



    @Override
    public void onStartTrackingTouch(SeekBar seekBar) {

    }

    @Override
    public void onStopTrackingTouch(SeekBar seekBar) {

    }
}

