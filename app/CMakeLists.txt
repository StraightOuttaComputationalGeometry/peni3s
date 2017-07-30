cmake_minimum_required(VERSION 3.4.1)


include_directories( /opt/opencv-android-sdk/sdk/native/jni/include )
add_library( lib_opencv SHARED IMPORTED )
set_target_properties(lib_opencv PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/src/main/jniLibs/${ANDROID_ABI}/libopencv_java3.so)

add_library( graphics
             SHARED
             src/main/cpp/graphics.cpp
             src/main/cpp/lib.cpp
             src/main/cpp/mesh.cpp
             src/main/cpp/technique.cpp
             src/main/cpp/tiny_obj_loader.cpp
             )

find_library( log-lib
              log
              )

target_link_libraries( graphics
                       ${log-lib}
                       GLESv3
                       lib_opencv
                       )