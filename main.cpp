#define STB_IMAGE_IMPLEMENTATION

#include <iostream>
#include "vars.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

static void glfw_error_callback(int error, const char* description)
{
	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}
#define IMGUI_DEMO_MARKER(section)  do { if (GImGuiDemoMarkerCallback != NULL) GImGuiDemoMarkerCallback(__FILE__, __LINE__, section, GImGuiDemoMarkerCallbackUserData); } while (0)


int main(int, char**)
{
	// Setup window
	glfwSetErrorCallback(glfw_error_callback);
	if (!glfwInit())
		return 1;

	// Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
	// GL ES 2.0 + GLSL 100
	const char* glsl_version = "#version 100";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
	// GL 3.2 + GLSL 150
	const char* glsl_version = "#version 150";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
	// GL 3.0 + GLSL 130
	const char* glsl_version = "#version 130";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

	// Create window with graphics context
	GLFWwindow* window = glfwCreateWindow(1280, 720, "Stack - past generation messenger", NULL, NULL);
	if (window == NULL)
		return 1;
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // Enable vsync

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);



	static char login[64] = "";
	static char password[64] = "";
	static char userName[64] = "";
	static char message[256] = "";

	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	ImVec2 windowSize = ImVec2(400.0f, 220.0f);
	ImVec2 usersWindowSize = ImVec2(500.0f, 720.0f);
	ImVec2 messagesWindowSize = ImVec2(500.0f, 720.0f);
	User user;
	User selectedRecepient;
	Users usersDB = Users();
	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_NoCollapse;
	window_flags |= ImGuiWindowFlags_NoResize;
	window_flags |= ImGuiWindowFlags_NoMove;

	ImGuiWindowFlags message_window_flags = 0;
	message_window_flags |= ImGuiWindowFlags_NoCollapse;
	message_window_flags |= ImGuiWindowFlags_NoResize;
	message_window_flags |= ImGuiWindowFlags_NoMove;

	int avatar_image_width = 60;
	int avatar_image_height = 60;
	GLuint my_image_texture = 0;
	bool ret = LoadTextureFromFile("avatar.jpg", &my_image_texture, &avatar_image_width, &avatar_image_height);
	IM_ASSERT(ret);

	int background_image_width = 1280;
	int background_image_height = 720;
	GLuint background_texture = 0;
	ret = LoadTextureFromFile("background.jpg", &background_texture, &background_image_width, &background_image_height);
	IM_ASSERT(ret);

	const ImGuiViewport* main_viewport = ImGui::GetMainViewport();

	ImVec2 topLeft = ImVec2(0.0f, 0.0f);
	ImVec2 center = ImGui::GetMainViewport()->GetCenter();

	// Main loop
	while (!glfwWindowShouldClose(window) && programAlive)
	{

		// Poll and handle events (inputs, window resize, etc.)
		// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
		// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
		// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
		// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
		glfwPollEvents();

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
		if (show_demo_window)
			ImGui::ShowDemoWindow(&show_demo_window);

		// 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.

		auto MAIN_WINDOW = ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoTitleBar |
			ImGuiWindowFlags_NoBringToFrontOnFocus |
			ImGuiWindowFlags_NoInputs |
			ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoScrollbar;

		glfwGetWindowSize(window, &background_image_width, &background_image_height);
		ImVec2 size(background_image_width, background_image_height);
		ImGui::SetNextWindowPos(ImVec2());
		ImGui::SetNextWindowSize(size);
		if (ImGui::Begin("1", nullptr, MAIN_WINDOW)) {
			ImGui::Image((void*)(intptr_t)background_texture, ImVec2(background_image_width, background_image_height));
			ImGui::End();
		}


		if (show_main_menu_window)
		{
			ImGui::SetNextWindowPos(topLeft, ImGuiCond_Appearing, ImVec2(0.0f, 0.0f));
			ImGui::SetNextWindowSize(windowSize, ImGuiCond_None);
			ImGui::Begin("Main menu", NULL, window_flags); // Create a window called "Hello, world!" and append into it.
			ImGui::Text("Welcome to Stack, past generation messenger!");
			ImGui::Text("");

			if (loggedIn == true)
			{
				ImGui::Text("You are logged in as:");
				ImGui::SameLine();
				ImGui::Text(user.getLogin().c_str());
				//ImGui::Text(user.getPassword().c_str());
				//ImGui::Text(user.getUserName().c_str());

				if (ImGui::Button("Back to messages"))   // Buttons return true when clicked (most widgets return true when edited/activated)
				{
					show_signIn_window = false;
					show_main_menu_window = false;
					show_users_window = true;
					show_message_window = true;
				}

				if (ImGui::Button("Sign out"))   // Buttons return true when clicked (most widgets return true when edited/activated)
				{
					loggedIn = false;
					show_signIn_window = false;
					show_main_menu_window = true;
					show_users_window = false;
					show_message_window = false;
				}
			}

			if (loggedIn == false)
			{
				ImGui::Text("Please sign in or sign up:");
				ImGui::Text("");

				if (ImGui::Button("Sign in"))   // Buttons return true when clicked (most widgets return true when edited/activated)
				{
					show_signIn_window = true;
					show_main_menu_window = false;
				}

				if (ImGui::Button("Sign up"))   // Buttons return true when clicked (most widgets return true when edited/activated)
				{
					show_signUp_window = true;
					show_main_menu_window = false;
				}
			}
			ImGui::Text("");

			if (ImGui::Button("Exit"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
				programAlive = false;

			ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state

			//	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::End();
		}

		if (show_signUp_window)
		{
			if (signUpModalWindow)
				ImGui::OpenPopup("Warning!");

			// Always center this window when appearin
			ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

			if (ImGui::BeginPopupModal("Warning!", NULL, ImGuiWindowFlags_AlwaysAutoResize))
			{
				if (strcmp(login, "_all") == 0)
					ImGui::Text("Please don't use \"_all\" as login, it is hardcoded!");
				if (strcmp(password, "") == 0)
					ImGui::Text("Password cannot be empty!");
				if (strcmp(login, "") == 0)
					ImGui::Text("Login cannot be empty!");
				if (strcmp(userName, "") == 0)
					ImGui::Text("User name cannot be empty!");
				if (!usersDB.uniqueLogin(std::string(login)))
					ImGui::Text("User %s already exists!", login);

				ImGui::Separator();

				if (ImGui::Button("I understand", ImVec2(120, 0)))
				{
					signUpModalWindow = false;
					ImGui::CloseCurrentPopup();
				}

				ImGui::SetItemDefaultFocus();
				ImGui::EndPopup();
			}

			ImGui::SetNextWindowPos(topLeft, ImGuiCond_Appearing, ImVec2(0.0f, 0.0f));
			ImGui::SetNextWindowSize(windowSize, ImGuiCond_None);

			ImGui::Begin("Sign up", NULL, window_flags);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)

			ImGui::Text("Login:    ");
			ImGui::SameLine();
			ImGui::InputText("##login", login, 64);

			ImGui::Text("Password: ");
			ImGui::SameLine();
			ImGui::InputText("##password", password, IM_ARRAYSIZE(password), ImGuiInputTextFlags_Password);

			ImGui::Text("User name:");
			ImGui::SameLine();
			ImGui::InputText("##userName", userName, 64);

			ImGui::Text("");

			if (ImGui::Button("Create user"))
			{
				if (strcmp(login, "_all") == 0 ||
					strcmp(login, "") == 0 ||
					strcmp(password, "") == 0 ||
					strcmp(userName, "") == 0 ||
					!usersDB.uniqueLogin(std::string(login)))
					signUpModalWindow = true;
				else
				{
					user.setLogin(login);
					user.setPassword(password);
					user.setUserName(userName);
					loggedIn = true;

					std::ofstream out("users.mdf", std::ios::app); // add user record to data base
					if (out.is_open())
						out << user.getLogin() + "\t" + user.getPassword() + "\t" + user.getUserName() << std::endl;  //add new user to file

					show_users_window = true;
					//show_message_window = true;
					show_signUp_window = false;

					// Clear the buffer
					strncpy(login, "", 64);
					strncpy(password, "", 64);
					strncpy(userName, "", 64);
				}
			}

			if (ImGui::Button("Exit to main menu"))
			{
				show_signUp_window = false;
				show_main_menu_window = true;
			}

			ImGui::End();
		}

		if (show_signIn_window)
		{
			if (signInModalWindow)
				ImGui::OpenPopup("Warning!");

			// Always center this window when appearing
			//ImGui::SetNextWindowPos(main_viewport->Pos);
			ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
			if (ImGui::BeginPopupModal("Warning!", NULL, ImGuiWindowFlags_AlwaysAutoResize))
			{
				if (strcmp(password, "") == 0)
					ImGui::Text("Password cannot be empty!");
				if (strcmp(login, "") == 0)
					ImGui::Text("Login cannot be empty!");
				if (!usersDB.loginAndPasswordMatch(login, password))
					ImGui::Text("Error. No such login + password combination!");

				ImGui::Separator();

				if (ImGui::Button("I understand", ImVec2(120, 0)))
				{
					signInModalWindow = false;
					ImGui::CloseCurrentPopup();
				}

				ImGui::SetItemDefaultFocus();
				ImGui::EndPopup();
			}

			ImGui::SetNextWindowPos(topLeft, ImGuiCond_Appearing, ImVec2(0.0f, 0.0f));
			ImGui::SetNextWindowSize(windowSize, ImGuiCond_None);

			ImGui::Begin("Sign in", NULL, window_flags);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)

			ImGui::Text("Login:    ");
			ImGui::SameLine();
			ImGui::InputText("##login", login, 64);

			ImGui::Text("Password: ");
			ImGui::SameLine();
			ImGui::InputText("##password", password, IM_ARRAYSIZE(password), ImGuiInputTextFlags_Password);

			ImGui::Text("");

			if (ImGui::Button("Sign in"))
			{
				if (strcmp(login, "_all") == 0 ||
					strcmp(login, "") == 0 ||
					strcmp(password, "") == 0 ||
					!usersDB.loginAndPasswordMatch(login, password))
					signInModalWindow = true;
				else
				{
					user.setLogin(login);
					user.setPassword(password);
					user.setUserName(usersDB.findUserNameByLogin(login));
					loggedIn = true;
					show_signIn_window = false;
					show_users_window = true;

					// Clear the buffer
					strncpy(login, "", 64);
					strncpy(password, "", 64);
				}
			}

			if (ImGui::Button("Exit to main menu"))
			{
				show_signIn_window = false;
				show_main_menu_window = true;
			}

			ImGui::End();
		}

		if (show_users_window)
		{
			ImGui::SetNextWindowSize(usersWindowSize, ImGuiCond_None);
			ImGui::SetNextWindowPos(topLeft, ImGuiCond_Appearing, ImVec2(0.0f, 0.0f));
			ImGui::Begin("Users", NULL, window_flags);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
			ImGui::Text("Registered users:");
			ImGui::Text("");

			Users usersDB = Users();

			ImGui::PushID(0);
			ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor(22, 21, 23));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor(200, 200, 200));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor(200, 200, 200));
			if (ImGui::Button("PUBLIC GROUP"))
			{
				User groupUser("_all");
				selectedRecepient = groupUser;
				show_message_window = true;
			}
			ImGui::PopStyleColor(3);
			ImGui::PopID();

			int i = 0;
			for (auto element : usersDB.listOfUsers())
			{
				if (user.getLogin() != element.getLogin())
				{
					ImGui::PushID(i);
					ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor(22, 21, 23));
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor(200, 200, 200));
					ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor(200, 200, 200));
					if (ImGui::Button(element.getLogin().c_str()))
					{
						selectedRecepient = element;
						show_message_window = true;
					}
					ImGui::PopStyleColor(3);
					ImGui::PopID();
					++i;
				}
			}


			ImGui::Text("");
			if (ImGui::Button("Exit to main menu"))
			{
				//selectedRecepient = User();
				show_users_window = false;
				show_message_window = false;
				show_main_menu_window = true;
			}

			ImGui::End();
		}

		if (show_message_window)
		{
			ImGui::SetNextWindowSize(usersWindowSize, ImGuiCond_None);
			ImGui::SetNextWindowPos(ImVec2(500.0f, 0.0f), ImGuiCond_Appearing, ImVec2(0.0f, 0.0f));
			//ImGui::Image((void*)(intptr_t)background_texture, ImVec2(520, 300));
			ImGui::Begin("Messages", NULL, window_flags);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)

			ImGui::Image((void*)(intptr_t)my_image_texture, ImVec2(avatar_image_width, avatar_image_height));
			ImGui::SameLine();
			ImGui::Text(selectedRecepient.getLogin().c_str());
			ImGui::Text("");
			ImGui::Separator();
			ImGui::BeginChild("ChildL", ImVec2(ImGui::GetContentRegionAvail().x * 1.0f, 500), false, window_flags);

			Chat currentChat = Chat(user.getLogin(), selectedRecepient.getLogin());
			for (auto const& i : currentChat.listOfMessages())
			{
				ImGui::TextWrapped(i.c_str());
			}


			ImGui::EndChild();
			ImGui::Separator();
			ImGui::Text("");
			ImGui::Text("Input your message: ");
			ImGui::SameLine();
			ImGui::InputText("##message", message, 256, ImGuiInputTextFlags_AlwaysOverwrite);
			ImGui::Text("");
			if (ImGui::Button("Send msg"))
			{
				if (strcmp(message, "") != 0)
				{
					Message reply = Message(user.getLogin(), selectedRecepient.getLogin(), message);
					reply.sendMessage();
					//user.setLogin(login);

					// Clear the buffer
					strncpy(message, "", 256);
				}
			}

			ImGui::End();
		}


		// Rendering
		ImGui::Render();
		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
	}

	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}
