package com.eugeneyche.peni3s;

public class GraphicsLib {
    static {
        System.loadLibrary("graphics");
    }
    public static native boolean onSurfaceCreated();
    public static native void onSurfaceChanged(int width, int height);
    public static native void onDrawFrame();
    public static native void loadModelFromString(String str);
    public static native void onPan(float x, float y);
    public static native void onRotate(float x, float y);
    public static native void onScale(float amount);
}
