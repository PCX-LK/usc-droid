# Graphics library

set(INCROOT ${CMAKE_CURRENT_SOURCE_DIR}/include/Graphics/)
set(SRCROOT ${CMAKE_CURRENT_SOURCE_DIR}/src/)
set(PCHROOT ${CMAKE_CURRENT_SOURCE_DIR}/)

file(GLOB INC "${INCROOT}/*.hpp")
source_group("Public Headers" FILES ${INC})

file(GLOB SRC "${SRCROOT}/*.cpp" "${SRCROOT}/*.hpp")
source_group("Sources" FILES ${SRC})

# Find files used for project
set(GRAPHICS_SRC ${SRC} ${INC})

set(PCH_SRC ${PCHROOT}/stdafx.cpp)
set(PCH_INC ${PCHROOT}/stdafx.h)
set(PCH_FILES ${PCH_SRC} ${PCH_INC})
source_group("" FILES ${PCH_FILES})

# Compiler stuff
enable_precompiled_headers("${GRAPHICS_SRC}" ${PCH_SRC})

add_library(Graphics ${GRAPHICS_SRC} ${PCH_FILES})
target_compile_features(Graphics PUBLIC cxx_std_17)
target_include_directories(Graphics PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(Graphics PRIVATE
    ${INCROOT}
    ${SRCROOT}
    ${PCHROOT}
)

# Dependencies
# # Local Dependencies
target_link_libraries(Graphics Shared)

# # 3rd Party dependencies
target_include_directories(Graphics PUBLIC ${SDL_INCLUDE_DIR})
target_include_directories(Graphics PRIVATE SYSTEM ${FREETYPE_INCLUDE_DIRS})
target_link_libraries(Graphics ${FREETYPE_LIBRARIES})
target_link_libraries(Graphics ${SDL_LIBRARY})
target_link_libraries(Graphics ${JPEG_LIBRARY})
target_link_libraries(Graphics ${PNG_LIBRARIES})
target_link_libraries(Graphics GLEW)

if(UNIX)
    target_link_libraries(Graphics dl)

    if(APPLE)
        find_library(OPEN_GL OpenGL)
        target_link_libraries(Graphics ${OPEN_GL})
    else(APPLE)
        target_link_libraries(Graphics ${ANDROID_NDK_PLATFORM}/libGLESv2.so)

        # find_package(X11 REQUIRED)
        # include_directories(${X11_INCLUDE_DIR})
    endif(APPLE)
endif(UNIX)

set_werror(Graphics)

target_link_libraries(Graphics cc-common)

# Enable multiprocess compiling
if(MSVC)
    target_compile_options(Graphics PRIVATE /MP)
endif(MSVC)