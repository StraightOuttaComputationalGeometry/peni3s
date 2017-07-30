#ifndef PENI3S_GRAPHICS_H
#define PENI3S_GRAPHICS_H

#include <jni.h>
#include <string>

namespace graphics {

    bool initialize(JNIEnv* env);
    void on_surface_changed(int width, int height);
    void on_draw_frame();
    void load_model_from_string(std::string source);
    void on_pan(float x, float y);
    void on_rotate(float x, float y);
    void on_scale(float amount);

}

#endif //PENI3S_GRAPHICS_H
