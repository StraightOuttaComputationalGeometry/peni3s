#include <string>
#include <vector>
#include "lib.h"
#include "graphics.h"

JNIEXPORT jboolean JNICALL Java_com_eugeneyche_peni3s_GraphicsLib_onSurfaceCreated(JNIEnv* env, jclass)
{
    return graphics::initialize(env);
}

static std::string jstring2string(JNIEnv *env, jstring jStr) {
    if (!jStr)
        return "";

    std::vector<char> charsCode;
    const jchar *chars = env->GetStringChars(jStr, NULL);
    jsize len = env->GetStringLength(jStr);
    jsize i;

    for( i=0;i<len; i++){
        int code = (int)chars[i];
        charsCode.push_back( code );
    }
    env->ReleaseStringChars(jStr, chars);

    return  std::string(charsCode.begin(), charsCode.end());
}

JNIEXPORT void JNICALL Java_com_eugeneyche_peni3s_GraphicsLib_loadModelFromString
        (JNIEnv* env, jclass, jstring str)
{
    graphics::load_model_from_string(jstring2string(env, str));
}

JNIEXPORT void JNICALL Java_com_eugeneyche_peni3s_GraphicsLib_onSurfaceChanged(JNIEnv* env, jclass, jint width, jint height)
{
    graphics::on_surface_changed(width, height);
}


JNIEXPORT void JNICALL Java_com_eugeneyche_peni3s_GraphicsLib_onDrawFrame(JNIEnv* env, jclass)
{
    graphics::on_draw_frame();
}

JNIEXPORT void JNICALL Java_com_eugeneyche_peni3s_GraphicsLib_onPan
        (JNIEnv *, jclass, jfloat x, jfloat y)
{
    graphics::on_pan(x, y);
}

JNIEXPORT void JNICALL Java_com_eugeneyche_peni3s_GraphicsLib_onRotate
        (JNIEnv *, jclass, jfloat x, jfloat y)
{
    graphics::on_rotate(x, y);
}

JNIEXPORT void JNICALL Java_com_eugeneyche_peni3s_GraphicsLib_onScale
        (JNIEnv *, jclass, jfloat amount)
{
    graphics::on_scale(amount);
}
