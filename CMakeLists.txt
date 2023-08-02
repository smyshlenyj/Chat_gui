cmake_minimum_required(VERSION 3.0.0)
project(chat_gui VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
find_package( OpenCV REQUIRED )

add_executable(chat_gui main.cpp LoadImage.cpp Message.cpp SHA256.cpp UI.cpp User.cpp Users.cpp Connection.cpp Constants.cpp)

add_library(libimgui STATIC IMPORTED)
set_property(TARGET libimgui PROPERTY
             IMPORTED_LOCATION libimgui.a)
target_link_libraries(chat_gui PRIVATE libimgui)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

#target_link_libraries( chat_gui ${OpenCV_LIBS} ${SDL2_LIBRARIES} )