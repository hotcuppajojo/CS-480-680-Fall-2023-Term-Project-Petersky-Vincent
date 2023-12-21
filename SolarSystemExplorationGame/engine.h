/* Engine Class Header, Author: Dr. Tavakkoli*/

#ifndef ENGINE_H
#define ENGINE_H

#include <assert.h>
#include "window.h"
#include "graphics.h"
#include "camera.h"  
#include "starship.h" 

static void cursorPositionCallBack(GLFWwindow*, double xpos, double ypos);
static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

class Engine {
public:
    Engine(const char* name, int width, int height);
    ~Engine();
    bool Initialize();
    void Run();
    void ProcessInput();
    void ProcessFreeLook();
    void UpdateStarshipFromCamera();  // Method to update starship position based on camera
    unsigned int getDT();
    long long GetCurrentTimeMillis();
    void Display(GLFWwindow*, double);
    static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);

private:
    // Window related variables
    Window* m_window;
    const char* m_WINDOW_NAME;
    int m_WINDOW_WIDTH;
    int m_WINDOW_HEIGHT;
    bool m_FULLSCREEN;

    Graphics* m_graphics;  // Pointer to the Graphics class
    float fixedOffset = 20.0;  // Fixed offset for the camera
    bool m_running;
};

#endif // ENGINE_H
