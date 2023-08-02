cmake_minimum_required(VERSION 3.0.0)
project(chat_gui VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

add_executable(chat_gui main.cpp Chat.cpp LoadImage.cpp Message.cpp SHA256.cpp UI.cpp User.cpp Users.cpp Connection.cpp Constants.cpp 
imgui.cpp imgui_demo.cpp imgui_draw.cpp imgui_tables.cpp imgui_widgets.cpp imgui_impl_glfw.cpp imgui_impl_opengl3.cpp)

find_package(OpenGL REQUIRED COMPONENTS OpenGL)
find_package(GLUT REQUIRED)
find_package(glfw3 REQUIRED)

add_dependencies(chat_gui OpenGL::OpenGL)
include_directories(${GLUT_INCLUDE_DIRS} )

target_link_libraries(chat_gui OpenGL::OpenGL ${GLUT_LIBRARY} )
#target_link_libraries(${PROJECT_NAME} libstdc++)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

target_link_libraries(chat_gui glfw)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
