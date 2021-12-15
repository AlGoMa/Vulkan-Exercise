#include "VulkanWindow.h"
#include <exception>
#include <vector>
#include <string>
#include <VertexBuffer.h>
#include "IndexBuffer.h"
#include <chrono>
#define BARYCOORD(x0,x1,x2,p) (p < 0.5f) ? ((x1 * p * 2.0f) + x0 * (0.5f - p) * 2.0f) : (x2 * (p - 0.5f) * 2.0 + x1 * (1.0f - p) * 2.0f)
#define NORMCOLOR(x) ((x) * 0.004f)

void  VulkanWindow::vInitialization(void)
{
    AttachEvent(static_cast<uint64_t>(Events::Moving));
    AttachEvent(static_cast<uint64_t>(Events::Resize));
    AttachEvent(static_cast<uint64_t>(Events::MouseClick));
    AttachEvent(static_cast<uint64_t>(Events::MouseHover));

    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    glfwWindowHint(GLFW_RESIZABLE, ResizableState());

    glfwWindowHint(GLFW_MAXIMIZED, FullScreenState());

    App::Managers::Application::stAppDimentions sCachedSize = GetAppDimentions();

    m_pInstance = glfwCreateWindow(sCachedSize.m_lWidth, sCachedSize.m_lHeight, GetAppName(), nullptr, nullptr);

    glfwSetWindowUserPointer((GLFWwindow*)m_pInstance, this);

    glfwSetFramebufferSizeCallback((GLFWwindow*)m_pInstance, VulkanWindow::FramebufferResizeCallback);

    glfwSetCursorEnterCallback((GLFWwindow*)m_pInstance, Cursor_enter_callback);

    if (glfwRawMouseMotionSupported())
        glfwSetInputMode((GLFWwindow*)m_pInstance, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

    /*
    * Vulkan instantiation. Where the magic begins.
    */
    if (VK_SUCCESS != Instance->vkCreateVulkanInstance(GetAppName(), m_pInstance))
    {
        throw std::runtime_error("failed to create Vulkan Environment!");
    }

    m_pEntity.get()->AddBuffer(new VertexBuffer());
    m_pEntity.get()->AddBuffer(new IndexBuffer());

    Instance->GetPipeline()->CreateShader("Shaders/frag.spv",    "main",    Pipeline::ShaderType::fragment);
    Instance->GetPipeline()->CreateShader("Shaders/vert.spv",    "main",    Pipeline::ShaderType::vertex);
    /* ToDo: Syncronization issues. */
    Instance->GetPipeline()->CreateShader("Shaders/compute.spv", "main", Pipeline::ShaderType::compute);

    Instance->GetPipeline()->CreateGraphicsPipeline();
    Instance->GetPipeline()->CreateComputePipeline();
}

void  VulkanWindow::vDeInitialization(void)
{
    Instance->StopDevice();

    m_pEntity.get()->Destroy();

    Instance->vDestroy();

    glfwDestroyWindow((GLFWwindow*)m_pInstance) ;

    m_pEntity.release();

    glfwTerminate();
}

void  VulkanWindow::Draw(void)
{
    Instance->vkDraw(m_pInstance, m_pEntity.get(),
                     EventState::active == GetEventStateAndReset(static_cast<uint64_t>(Events::Resize)));
}

void  VulkanWindow::LoadContent(void)
{
    m_pEntity = std::make_unique<Entity>();

    m_pEntity.get()->BuildBasisMesh(GeometryMesh::MeshesType::Plane);
}

void  VulkanWindow::ProcessInput(void)
{
    uint32_t w = Instance->GetSC()->GetExtents().width;
    uint32_t h = Instance->GetSC()->GetExtents().height;

    double xpos = 0.0, ypos = 0.0;

    if (GetEventState(static_cast<uint32_t>(Events::MouseHover)) == EventState::active)
    {
        glfwGetCursorPos((GLFWwindow*)m_pInstance, &xpos, &ypos);

        m_vMousePos.x = static_cast<float>(xpos) / w;
        m_vMousePos.y = static_cast<float>(ypos) / h;
    }
}

const App::Managers::Application::AppStatus VulkanWindow::Update(void)
{
    static auto oStartTime = std::chrono::high_resolution_clock::now();

    auto oCurrentTime = std::chrono::high_resolution_clock::now();
    float fTime = std::chrono::duration<float, std::chrono::seconds::period>(oCurrentTime - oStartTime).count();

    UniformData CBdata;
    CBdata.m_Model = glm::identity<glm::mat4>();
    glm::vec3 target = glm::rotate(glm::vec3(0.0f, 0.0f, 0.0f), glm::radians(360.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::vec3 position(0.0f, 0.0f, -0.1f);
    glm::vec3 up(0.0f, -1.0f, 0.0f);

    CBdata.m_View = glm::lookAt(position, target, up); 
    CBdata.m_Projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
    CBdata.m_Time = fTime;

    Instance->GetSC()->GetCB()->GetUniformData() = CBdata;

    SceneData SData;

    SData.sky =       glm::vec4(NORMCOLOR(BARYCOORD(182.0f, 207.0f, 23.0f,  m_vMousePos.y)),
                                NORMCOLOR(BARYCOORD(218.0f, 122.0f, 11.0f,  m_vMousePos.y)),
                                NORMCOLOR(BARYCOORD(228.0f, 80.0f, 40.0f,  m_vMousePos.y)), 1.0f);
    SData.lightColor = glm::vec4(NORMCOLOR(BARYCOORD(179.0f, 238.0f, 46.0f, m_vMousePos.y)),
                                 NORMCOLOR(BARYCOORD(174.0f, 212.0f, 21.0f, m_vMousePos.y)),
                                 NORMCOLOR(BARYCOORD(234.0f, 171.0f, 56.0f, m_vMousePos.y)), 1.0f);

    SData.astroOutColor = glm::vec4(NORMCOLOR(BARYCOORD(216.0f, 205.0f, 243.0f, m_vMousePos.y)),
                                    NORMCOLOR(BARYCOORD(186.0f, 100.0f, 198.0f, m_vMousePos.y)),
                                    NORMCOLOR(BARYCOORD(122.0f, 65.0f,  238.0f, m_vMousePos.y)), 1.0f);
    SData.astroMidColor = glm::vec4(NORMCOLOR(BARYCOORD(250.0f, 243.0f, 255.0f, m_vMousePos.y)),
                                    NORMCOLOR(BARYCOORD(247.0f, 168.0f, 255.0f, m_vMousePos.y)),
                                    NORMCOLOR(BARYCOORD(180.0f, 112.0f, 255.0f, m_vMousePos.y)), 1.0f);
    SData.waveColor =     glm::vec4(NORMCOLOR(BARYCOORD(146.0f, 216.0f, 59.0f, m_vMousePos.y)),
                                    NORMCOLOR(BARYCOORD(216.0f, 161.0f, 88.0f, m_vMousePos.y)),
                                    NORMCOLOR(BARYCOORD(203.0f, 117.0f, 113.0f, m_vMousePos.y)), 1.0f);
    SData.MousePosition = m_vMousePos;
    
    Instance->GetSC()->GetCB()->GetSceneData() = SData;

    glfwPollEvents(); 
    
    return (App::Managers::Application::AppStatus)glfwWindowShouldClose((GLFWwindow*)m_pInstance);
}

void VulkanWindow::FramebufferResizeCallback(GLFWwindow* window,
                                             int width,
                                             int height)
{
    auto app = reinterpret_cast<VulkanWindow*>(glfwGetWindowUserPointer(window));
    app->UpdateEvent(static_cast<uint64_t>(Events::Resize), EventState::active);
}

void VulkanWindow::Cursor_position_callback(GLFWwindow* window,
                                            double xpos, 
                                            double ypos)
{
    auto app = reinterpret_cast<VulkanWindow*>(glfwGetWindowUserPointer(window));

    app->UpdateEvent(static_cast<uint64_t>(Events::Resize), EventState::active);

    std::cout << xpos << " " << ypos << std::endl;
}

void VulkanWindow::Cursor_enter_callback(GLFWwindow* window, int entered)
{
    auto app = reinterpret_cast<VulkanWindow*>(glfwGetWindowUserPointer(window));

    app->UpdateEvent(static_cast<uint64_t>(Events::MouseHover), static_cast<EventState>(entered));
}
