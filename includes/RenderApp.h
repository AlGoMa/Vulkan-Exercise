#pragma once
#include <Application.h>
#include <vector>
#include <RenderEntity.h>
#include <glm/glm.hpp>

class RenderApp final:
    public App::Managers::Application
{
public:
    RenderApp(void) : App::Managers::Application("AlGoMa Sample"), m_vMousePos(glm::vec4(.0f)) { }
    RenderApp(std::string in_strAppName,
                  bool in_bFullScreen = false,
                  long in_lPosX = 0,
                  long in_lPosY = 0,
                  long in_lWidth = 0,
                  long in_lHeight = 0) : App::Managers::Application(in_strAppName.c_str(), in_lPosX, in_lPosY, in_lWidth, in_lHeight, in_bFullScreen, true),
                                    m_vMousePos(glm::vec4(.1f)) { }
    ~RenderApp(void) { }

    enum class Events : uint64_t { Resize, Moving, MouseClick, MouseHover, KeyEscPressed };

    inline const t_IntS64& Width      (void) { return GetAppDimentions().m_lWidth; }
    inline const t_IntS64& Height     (void) { return GetAppDimentions().m_lHeight; }
    inline const Point     Dimentions (void) { return GetAppDimentions().m_stSize; }
    inline const Point     Position   (void) { return GetAppDimentions().m_stPosition; }

private:
    virtual void                                         vInitialization(void);
    virtual void                                         vDeInitialization(void);
    virtual void                                         Draw(void);
    virtual void                                         LoadContent(void);
    virtual void                                         ProcessInput(void);
    virtual const App::Managers::Application::AppStatus  Update(void);
 
private:
    std::unique_ptr<RenderEntity>                        m_pEntity;
    glm::vec4                                            m_vMousePos;
};

