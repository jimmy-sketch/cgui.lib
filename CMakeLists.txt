cmake_minimum_required(VERSION 3.12)

set(CMAKE_CXX_STANDARD 20)

project(cgui
    LANGUAGES CXX C
    VERSION 1.2.0
)

add_library(CGUI
    include/cgui/components/basicImage.h
    include/cgui/components/basicProgressBar.h
    include/cgui/components/basicText.h
    include/cgui/components/canvas.h
    include/cgui/components/component.h
    include/cgui/components/hContainer.h
    include/cgui/components/multiLineText.h
    include/cgui/components/scrollText.h
    include/cgui/components/tContainer.h
    include/cgui/components/vContainer.h
    include/cgui/utils/config.h
    include/cgui/utils/pos.h
    include/cgui/utils/string.h
    include/cgui/page.h
    include/cgui/cgui.h
    thirdparty/src/wtswidth.c
    src/basicImage.cpp
    src/basicProgressBar.cpp
    src/basicText.cpp
    src/canvas.cpp
    src/config.cpp
    src/hContainer.cpp
    src/multiLineText.cpp
    src/page.cpp
    src/tContainer.cpp
    src/vContainer.cpp
    src/scrollText.cpp
    src/string.cpp
)

target_include_directories(CGUI PUBLIC
    include/
    thirdparty/
)

add_subdirectory(sandbox)