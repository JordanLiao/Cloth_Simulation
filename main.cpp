#include "main.h"


////////////////////////////////////////////////////////////////////////////////

void error_callback(int error, const char* description)
{
	// Print error.
	std::cerr << description << std::endl;
}

void setup_callbacks(GLFWwindow* window)
{
	// Set the error callback.
	glfwSetErrorCallback(error_callback);
	// Set the window resize callback.
	glfwSetWindowSizeCallback(window, Window::resizeCallback);
	
	// Set the key callback.
	glfwSetKeyCallback(window, Window::keyCallback);

	// Set the mouse and cursor callbacks
	glfwSetMouseButtonCallback(window, Window::mouse_callback);
	glfwSetCursorPosCallback(window, Window::cursor_callback);
}

void setup_opengl_settings()
{
	// Enable depth buffering.
	glEnable(GL_DEPTH_TEST);
	// Related to shaders and z value comparisons for the depth buffer.
	glDepthFunc(GL_LEQUAL);
	// Set polygon drawing mode to fill front and back of each polygon.
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	// Set clear color to black.
	glClearColor(0.0, 0.0, 0.0, 0.0);
}

void print_versions()
{
	// Get info of GPU and supported OpenGL version.
	std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "OpenGL version supported: " << glGetString(GL_VERSION) 
		<< std::endl;

	//If the shading language symbol is defined.
#ifdef GL_SHADING_LANGUAGE_VERSION
	std::cout << "Supported GLSL version is: " << 
		glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
#endif
}



////////////////////////////////////////////////////////////////////////////////

int main(int argc, char* argv[])
{
	const char* file = "nothing";
	const char* file1 = "nothing";
	const char* file2 = "nothing";
	if (argc == 1) {
		file = NULL;
		file1 = NULL;
	}
	else if (argc == 2) {
		string str = argv[1];
		if (str.find(".skel") != string::npos) {
			file = argv[1];
			file1 = NULL;
		}
		else {
			file1 = argv[1];
			file = NULL;
		}
	}
	else if (argc == 4) {
		file = argv[1];
		file1 = argv[2];
		file2 = argv[3];
	}
	// Create the GLFW window.
	GLFWwindow* window = Window::createWindow(800, 600);
	if (!window) exit(EXIT_FAILURE);

	// Print OpenGL and GLSL versions.
	print_versions();
	// Setup callbacks.
	setup_callbacks(window);
	// Setup OpenGL settings.
	setup_opengl_settings();

	// Initialize the shader program; exit if initialization fails.
	if (!Window::initializeProgram()) exit(EXIT_FAILURE);
	// Initialize objects/pointers for rendering; exit if initialization fails.
	//if (!Window::initializeObjects(file, file1, file2)) exit(EXIT_FAILURE);
	if (!Window::initializeObjects()) exit(EXIT_FAILURE);
	
	// Loop while GLFW window should stay open.
	while (!glfwWindowShouldClose(window))
	{
		// Main render display callback. Rendering of objects is done here.
		Window::displayCallback(window);

		// Idle callback. Updating objects, etc. can be done here.
		Window::idleCallback();
	}

	Window::cleanUp();
	// Destroy the window.
	glfwDestroyWindow(window);
	// Terminate GLFW.
	glfwTerminate();

	exit(EXIT_SUCCESS);
}

////////////////////////////////////////////////////////////////////////////////
