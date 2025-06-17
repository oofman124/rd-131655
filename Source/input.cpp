#include "Input.h"

GLFWwindow* g_window = nullptr;

bool isKeyDown(int key) {
    return glfwGetKey(g_window, key) == GLFW_PRESS;
}