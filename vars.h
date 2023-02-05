#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>

#include "LoadImage.h"
#include "Chat.h"
#include "Message.h"
#include "User.h"
#include "Users.h"
#include "UI.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h> // Will drag system OpenGL headers

extern bool programAlive;
extern bool show_main_menu_window;
extern bool show_demo_window;
extern bool show_signUp_window;
extern bool show_signIn_window;
extern bool show_users_window;
extern bool show_message_window;
extern bool signUpModalWindow;
extern bool signInModalWindow;
extern bool loggedIn;

//extern ImVec4 clear_color;
//extern ImVec2 windowSize;
//extern ImVec2 usersWindowSize;
//extern ImVec2 messagesWindowSize;
//extern User user;
//extern User selectedRecepient;
//extern Users usersDB;
//extern ImGuiWindowFlags window_flags;
//
//extern ImVec2 topLeft;
//extern ImVec2 center;

//extern int avatar_image_width;
//extern int avatar_image_height;
//extern GLuint my_image_texture;
//extern bool ret;
//
//extern int background_image_width;
//extern int background_image_height;
//extern GLuint background_texture;
//extern bool ret2;