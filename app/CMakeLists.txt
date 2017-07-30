cmake_minimum_required(VERSION 3.4.1)

include_directories( src/main/cpp/glm )

set(GLM_FORCE_CXX98 ON)

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
                       )