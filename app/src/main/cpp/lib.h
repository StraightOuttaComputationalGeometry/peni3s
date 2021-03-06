/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class com_eugeneyche_peni3s_GraphicsLib */

#ifndef _Included_com_eugeneyche_peni3s_GraphicsLib
#define _Included_com_eugeneyche_peni3s_GraphicsLib
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     com_eugeneyche_peni3s_GraphicsLib
 * Method:    onSurfaceCreated
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_com_eugeneyche_peni3s_GraphicsLib_onSurfaceCreated
  (JNIEnv *, jclass);

/*
 * Class:     com_eugeneyche_peni3s_GraphicsLib
 * Method:    onSurfaceChanged
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_com_eugeneyche_peni3s_GraphicsLib_onSurfaceChanged
  (JNIEnv *, jclass, jint, jint);

/*
 * Class:     com_eugeneyche_peni3s_GraphicsLib
 * Method:    onDrawFrame
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_eugeneyche_peni3s_GraphicsLib_onDrawFrame
  (JNIEnv *, jclass);

/*
 * Class:     com_eugeneyche_peni3s_GraphicsLib
 * Method:    loadModelFromString
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_eugeneyche_peni3s_GraphicsLib_loadModelFromString
  (JNIEnv *, jclass, jstring);

/*
 * Class:     com_eugeneyche_peni3s_GraphicsLib
 * Method:    onPan
 * Signature: (FF)V
 */
JNIEXPORT void JNICALL Java_com_eugeneyche_peni3s_GraphicsLib_onPan
  (JNIEnv *, jclass, jfloat, jfloat);

/*
 * Class:     com_eugeneyche_peni3s_GraphicsLib
 * Method:    onRotate
 * Signature: (FF)V
 */
JNIEXPORT void JNICALL Java_com_eugeneyche_peni3s_GraphicsLib_onRotate
  (JNIEnv *, jclass, jfloat, jfloat);

/*
 * Class:     com_eugeneyche_peni3s_GraphicsLib
 * Method:    onScale
 * Signature: (F)V
 */
JNIEXPORT void JNICALL Java_com_eugeneyche_peni3s_GraphicsLib_onScale
  (JNIEnv *, jclass, jfloat);

#ifdef __cplusplus
}
#endif
#endif
