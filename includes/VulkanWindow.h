#pragma once
#include "VulkanManager.h"
#include <Application.h>
#include <vector>
#include "Entity.h"

class VulkanWindow final:
    public App::Managers::Application
{
public:
    VulkanWindow(void) : App::Managers::Application("AlGoMa Sample") { }
    VulkanWindow(std::string in_strAppName,
                  long in_lPosX,
                  long in_lPosY,
                  long in_lWidth,
                  long in_lHeight) : App::Managers::Application(in_strAppName.c_str(), in_lPosX, in_lPosY, in_lWidth, in_lHeight, false, true),
                                    m_vMousePos(glm::vec4(0.1f)) { }
    ~VulkanWindow(void) { }

    enum class Events : uint64_t { Resize, Moving, MouseClick, MouseHover };

private:
    virtual void                                         vInitialization(void);
    virtual void                                         vDeInitialization(void);
    virtual void                                         Draw(void);
    virtual void                                         LoadContent(void);
    virtual void                                         ProcessInput(void);
    virtual const App::Managers::Application::AppStatus  Update(void);

    /* Callback events */
    static void                                          FramebufferResizeCallback(GLFWwindow* window, int width, int height);
    static void                                          Cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
    static void                                          Cursor_enter_callback(GLFWwindow* window, int entered);
 
private:
    std::unique_ptr<Entity>                              m_pEntity;
    glm::vec4                                            m_vMousePos;
};

