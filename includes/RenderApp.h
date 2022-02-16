#pragma once
#include <Application.h>
#include <vector>
#include <Entity.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_vulkan.h>
#include <glm/glm.hpp>

class RenderApp final:
    public App::Managers::Application
{
public:
    RenderApp(void) : App::Managers::Application("AlGoMa Sample") { }
    RenderApp(std::string in_strAppName,
                  bool in_bFullScreen = false,
                  long in_lPosX = 0,
                  long in_lPosY = 0,
                  long in_lWidth = 0,
                  long in_lHeight = 0) : App::Managers::Application(in_strAppName.c_str(), in_lPosX, in_lPosY, in_lWidth, in_lHeight, in_bFullScreen, true),
                                    m_vMousePos(glm::vec4(0.1f)) { }
    ~RenderApp(void) { }

    enum class Events : uint64_t { Resize, Moving, MouseClick, MouseHover, KeyEscPressed };

private:
    virtual void                                         vInitialization(void);
    virtual void                                         vDeInitialization(void);
    virtual void                                         Draw(void);
    virtual void                                         LoadContent(void);
    virtual void                                         ProcessInput(void);
    virtual const App::Managers::Application::AppStatus  Update(void);
 
private:
    std::unique_ptr<Entity>                              m_pEntity;
    glm::vec4                                            m_vMousePos;
    ImGui_ImplVulkanH_Window                             m_pImGuiWindow;
};

