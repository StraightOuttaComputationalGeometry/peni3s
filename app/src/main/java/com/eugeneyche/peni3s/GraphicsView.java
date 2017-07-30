package com.eugeneyche.peni3s;

import android.content.Context;
import android.content.res.Resources;
import android.opengl.GLSurfaceView;
import android.view.MotionEvent;
import android.view.ScaleGestureDetector;

import java.util.Scanner;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;


public class GraphicsView extends GLSurfaceView {

    private float startX;
    private float startY;
    private Renderer mRenderer;
    private Resources mResources;
    private ScaleGestureDetector detector;

    public GraphicsView(Context context) {
        super(context);
        setEGLContextClientVersion(3);
        mRenderer = new Renderer();
        mResources = context.getResources();
        setRenderer(mRenderer);
        loadModel(R.raw.dino);
        detector = new ScaleGestureDetector(getContext(), new ScaleListener());
    }

    void loadModel(int modelId) {
        Scanner s = new Scanner(mResources.openRawResource(modelId)).useDelimiter("\\A");
        String modelSource = s.hasNext() ? s.next() : "";
        mRenderer.setModelSource(modelSource);
    }

    @Override
    public boolean onTouchEvent(MotionEvent e) {
        switch (e.getAction() & MotionEvent.ACTION_MASK) {
            case MotionEvent.ACTION_DOWN:
                startX = e.getX();
                startY = e.getY();
                break;
            case MotionEvent.ACTION_POINTER_DOWN:
            case MotionEvent.ACTION_MOVE:
                float dx = (e.getX() - startX) / 100.f;
                float dy = -(e.getY() - startY) / 100.f;
                startX = e.getX();
                startY = e.getY();
                if (e.getPointerCount() > 1) {
                    GraphicsLib.onPan(dx, dy);
                } else {
                    GraphicsLib.onRotate(dx, dy);
                }
                break;
        }
        detector.onTouchEvent(e);
        return true;
    }

    public static class Renderer implements GLSurfaceView.Renderer {

        String loadedSource = null;
        String source = null;

        void setModelSource(String src) {
            source = src;
        }

        @Override
        public void onSurfaceCreated(GL10 gl, EGLConfig config) {
            GraphicsLib.onSurfaceCreated();
        }

        @Override
        public void onSurfaceChanged(GL10 gl, int width, int height) {
            GraphicsLib.onSurfaceChanged(width, height);
        }

        @Override
        public void onDrawFrame(GL10 gl) {
            if (source != loadedSource) {
                GraphicsLib.loadModelFromString(source);
                loadedSource = source;
            }
            GraphicsLib.onDrawFrame();
        }
    }

    private class ScaleListener extends ScaleGestureDetector.SimpleOnScaleGestureListener {
        @Override
        public boolean onScale(ScaleGestureDetector detector) {
            GraphicsLib.onScale(detector.getScaleFactor());
            return true;
        }
    }
}
