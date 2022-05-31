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
import android.view.MotionEvent;
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


public class BasicViewerActivity extends AppCompatActivity implements View.OnClickListener, SeekBar.OnSeekBarChangeListener, View.OnTouchListener {

    public MyGLSurfaceView myGLSurfaceView;

    public AssetManager mgr;

    /** 记录按下的坐标点（起始点）**/
    private float mPosX = 0;
    private float mPosY = 0;
    /** 记录移动后抬起坐标点（终点）**/
    private float mCurPosX = 0;
    private float mCurPosY = 0;
    //第一根手指的点
    private float mFirstTouchX;
    private float mFirstTouchY;
    //第二根手指的点
    private float mSecondTouchX;
    private float mSecondTouchY;
    private double firstPointerLength;


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
        myGLSurfaceView.setOnTouchListener(this);
        threeDViewArea.addView(myGLSurfaceView);

        findViewById(R.id.pickFIle).setOnClickListener(this);

        SeekBar barX = findViewById(R.id.barHeight);
        SeekBar barY = findViewById(R.id.barRotation);
        SeekBar barZ = findViewById(R.id.barRadius);
        barX.setOnSeekBarChangeListener(this);
        barY.setOnSeekBarChangeListener(this);
        barZ.setOnSeekBarChangeListener(this);
        findViewById(R.id.mode1).setOnClickListener(this);
        findViewById(R.id.mode2).setOnClickListener(this);
        findViewById(R.id.mode3).setOnClickListener(this);
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
                myGLSurfaceView.setPattern("1");
                break;

            case R.id.mode2:
                String pattern2 = ((Button) v).getText().toString();
                myGLSurfaceView.setPattern("2");
                break;

            case R.id.mode3:
                String pattern3 = ((Button) v).getText().toString();
                myGLSurfaceView.setPattern("3");
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
            case R.id.barHeight:
                myGLSurfaceView.setSunHeight(progress, seekBar.getMax());
                break;
            case R.id.barRotation:
                myGLSurfaceView.setSunRotation(progress, seekBar.getMax());
                break;
            case R.id.barRadius:
                myGLSurfaceView.setSunRadius(progress, seekBar.getMax());
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

    @Override
    public boolean onTouch(View v, MotionEvent event) {

        //滑动
        switch (event.getAction()){
            case MotionEvent.ACTION_DOWN:
                mPosX = event.getX();
                mPosY = event.getY();
                break;
            case MotionEvent.ACTION_MOVE:
                mCurPosX = event.getX();
                mCurPosY = event.getY();
                if(Math.abs(mCurPosX - mPosX)< 1 && Math.abs(mCurPosY - mPosY) < 1){
                    return false;
                }
                myGLSurfaceView.passVector(mCurPosX - mPosX, mCurPosY - mPosY);
                mPosX = mCurPosX;
                mPosY = mCurPosY;
                Log.d("zjh",  "(" + (mCurPosX - mPosX) + ", " + (mCurPosY - mPosY) + ")");
                break;
            case MotionEvent.ACTION_UP:
                mCurPosX = event.getX();
                mCurPosY = event.getY();
                if(Math.abs(mCurPosX - mPosX)< 1 && Math.abs(mCurPosY - mPosY) < 1){
                    return false;
                }
                break;
        }

        //缩放
        switch (event.getActionMasked()){
                case MotionEvent.ACTION_DOWN:
                    //获取第一个点（A点）的位置
                    mFirstTouchX = event.getX();
                    mFirstTouchY = event.getY();
                    break;
                case MotionEvent.ACTION_POINTER_DOWN:
                    if(event.getActionIndex() == 1){
                        //获取第二个点（B点）的位置
                        mSecondTouchX = event.getX(1);
                        mSecondTouchY = event.getY(1);
                        //根据两点的位置获取两个触摸点之间的距离（AB）
                        float firstLengthX = Math.abs(mFirstTouchX - mSecondTouchX);
                        float firstLengthY = Math.abs(mFirstTouchY - mSecondTouchY);
                        firstPointerLength = Math.sqrt(Math.pow(firstLengthX, 2) + Math.pow(firstLengthY, 2));
                    }
                    break;
                case MotionEvent.ACTION_MOVE:
                    if(event.getPointerCount() >= 2){
                        //获取第一个点（A‘）的位置
                        float firstX = event.getX(0);
                        float firstY = event.getY(0);

                        //获取第二个点（B‘）的位置
                        float secondX = event.getX(1);
                        float secondY = event.getY(1);

                        //计算两点之间的距离（A'B'）
                        float secondLengthX = Math.abs(firstX - secondX);
                        float secondLengthY = Math.abs(firstY - secondY);
                        double secondPointerLength = Math.sqrt(Math.pow(secondLengthX, 2) + Math.pow(secondLengthY, 2));
                        //缩放
//                        Log.d("zjh",  " "+ (secondPointerLength / firstPointerLength));
                        myGLSurfaceView.zoom(secondPointerLength / firstPointerLength);
                    }
                    break;
            }

        return true;
    }
}

