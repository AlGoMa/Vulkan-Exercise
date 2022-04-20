#include "RenderApp.h"
#include <exception>
#include <vector>
#include <string>
#include <VertexBuffer.h>
#include <IndexBuffer.h>
#include <chrono>
#define BARYCOORD(x0,x1,x2,p) (p < 0.5) ? ((x1 * p * 2.0) + x0 * (0.5 - p) * 2.0) : (x2 * (p - 0.5) * 2.0 + x1 * (1.0 - p) * 2.0)
#define NORMCOLOR(x) ((x) * 0.004)
#include "VulkanManager.h"

void  RenderApp::vInitialization(void)
{
    AttachEvent(static_cast<uint64_t>(Events::Moving));
    AttachEvent(static_cast<uint64_t>(Events::Resize));
    AttachEvent(static_cast<uint64_t>(Events::MouseClick));
    AttachEvent(static_cast<uint64_t>(Events::MouseHover));
    AttachEvent(static_cast<uint64_t>(Events::KeyEscPressed));

    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    glfwWindowHint(GLFW_RESIZABLE, ResizableState());

    glfwWindowHint(GLFW_MAXIMIZED, FullScreenState());

    App::Managers::Application::stAppDimentions sCachedSize = GetAppDimentions();

    m_pInstance = glfwCreateWindow(reinterpret_cast<t_IntS32&>(sCachedSize.m_lWidth), 
                                   reinterpret_cast<t_IntS32&>(sCachedSize.m_lHeight), 
                                   GetAppName(), 
                                   nullptr, 
                                   nullptr);

    if (!GetWndInstance<GLFWwindow>())
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSetWindowUserPointer(GetWndInstance<GLFWwindow>(), this);

    glfwSetFramebufferSizeCallback(GetWndInstance<GLFWwindow>(),
                                   [](GLFWwindow* window, int width, int height) {
                                       auto app = reinterpret_cast<RenderApp*>(glfwGetWindowUserPointer(window));
                                       app->UpdateEvent(static_cast<uint64_t>(Events::Resize), EventState::active);
                                   });

    glfwSetCursorEnterCallback(GetWndInstance<GLFWwindow>(),
                               [](GLFWwindow* window, int entered) {
                                   auto app = reinterpret_cast<RenderApp*>(glfwGetWindowUserPointer(window));
                                   app->UpdateEvent(static_cast<uint64_t>(Events::MouseHover), static_cast<EventState>(entered));
                               });

    glfwSetKeyCallback(GetWndInstance<GLFWwindow>(), 
                       [](GLFWwindow* window, int key, int scancode, int action, int mods) {
                           auto app = reinterpret_cast<RenderApp*>(glfwGetWindowUserPointer(window));
                           app->UpdateEvent(static_cast<uint64_t>(Events::KeyEscPressed), static_cast<EventState>(GLFW_KEY_ESCAPE == key));
                       });

    if (glfwRawMouseMotionSupported())
        glfwSetInputMode(GetWndInstance<GLFWwindow>(), GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

    /*
    * Vulkan instantiation. Where the magic begins.
    */
    if (VK_SUCCESS != VulkanInstance->vkCreateVulkanInstance(GetAppName(), m_pInstance))
    {
        throw std::runtime_error("failed to create Vulkan Environment!");
    }

    m_pEntity.get()->AddBuffer(new VertexBuffer());
    m_pEntity.get()->AddBuffer(new IndexBuffer());

    VulkanInstance->GetPipeline()->CreateShader("Shaders/frag.spv",    "main", Pipeline::ShaderType::fragment);
    VulkanInstance->GetPipeline()->CreateShader("Shaders/vert.spv",    "main", Pipeline::ShaderType::vertex);
    /* ToDo: Syncronization issues. */
    VulkanInstance->GetPipeline()->CreateShader("Shaders/compute.spv", "main", Pipeline::ShaderType::compute);

    VulkanInstance->GetPipeline()->CreateGraphicsPipeline();
    VulkanInstance->GetPipeline()->CreateComputePipeline();
}

void  RenderApp::vDeInitialization(void)
{
    VulkanInstance->StopDevice();

    m_pEntity.get()->Destroy();

    VulkanInstance->vDestroy();

    glfwDestroyWindow(GetWndInstance<GLFWwindow>());

    m_pEntity.release();

    glfwTerminate();
}

void  RenderApp::Draw(void)
{
    VulkanInstance->vkDraw(m_pInstance, m_pEntity.get(),
                     EventState::active == GetEventState(static_cast<t_IntU64>(Events::Resize)));
}

void  RenderApp::LoadContent(void)
{
    m_pEntity = std::make_unique<Entity>();

    m_pEntity.get()->BuildBasisMesh(GeometryMesh::MeshesType::Plane);
    //m_pEntity.get()->LoadObj("Meshes/Cats_obj.obj");
}

void  RenderApp::ProcessInput(void)
{
    GetAppDimentions().m_lWidth = VulkanInstance->GetSC()->GetExtents().width;
    GetAppDimentions().m_lHeight = VulkanInstance->GetSC()->GetExtents().height;

    if (GetEventState(static_cast<uint32_t>(Events::MouseHover), false) == EventState::active)
    {
        double xpos = 0.0, ypos = 0.0;
        
        glfwGetCursorPos(GetWndInstance<GLFWwindow>(), &xpos, &ypos);

        m_vMousePos.x = static_cast<float>(xpos) / GetAppDimentions().m_lWidth;
        m_vMousePos.y = static_cast<float>(ypos) / GetAppDimentions().m_lHeight;
    }

    if (GetEventState(static_cast<uint32_t>(Events::KeyEscPressed), false) == EventState::active)
    {
        glfwSetWindowShouldClose(GetWndInstance<GLFWwindow>(), GLFW_TRUE);
    }
}

const App::Managers::Application::AppStatus RenderApp::Update(void)
{
    static auto oStartTime = std::chrono::high_resolution_clock::now();

    auto oCurrentTime = std::chrono::high_resolution_clock::now();
    float fTime = std::chrono::duration<float, std::chrono::seconds::period>(oCurrentTime - oStartTime).count();

    UniformData CBdata;
    CBdata.m_Model = glm::identity<glm::mat4>();
    glm::vec3 target = glm::rotate(glm::vec3(0.0f, 0.0f, 0.0f), glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::vec3 position(0.0f, 0.0f, -0.1f);
    glm::vec3 up(0.0f, -1.0f, 0.0f);

    CBdata.m_View = glm::lookAt(position, target, up); 
    //App::Math::Matrix proj = App::Math::Matrix::PerspectiveMatrix(90.0f, 16.0f / 9.0f, 0.0f, 100.0f);
    CBdata.m_Projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
    CBdata.m_Time = fTime;

    VulkanInstance->GetSC()->GetCB()->GetUniformData() = CBdata;

    SceneData SData;

    SData.sky        = glm::vec4(NORMCOLOR(BARYCOORD(182.0, 207.0, 23.0, m_vMousePos.y)),
                                 NORMCOLOR(BARYCOORD(218.0, 122.0, 11.0, m_vMousePos.y)),
                                 NORMCOLOR(BARYCOORD(228.0, 80.0,  40.0,  m_vMousePos.y)), 1.0f);
    SData.lightColor = glm::vec4(NORMCOLOR(BARYCOORD(179.0, 238.0, 46.0, m_vMousePos.y)),
                                 NORMCOLOR(BARYCOORD(174.0, 212.0, 21.0, m_vMousePos.y)),
                                 NORMCOLOR(BARYCOORD(234.0, 171.0, 56.0, m_vMousePos.y)), 1.0f);

    SData.astroOutColor = glm::vec4(NORMCOLOR(BARYCOORD(216.0, 205.0, 243.0, m_vMousePos.y)),
                                    NORMCOLOR(BARYCOORD(186.0, 100.0, 198.0, m_vMousePos.y)),
                                    NORMCOLOR(BARYCOORD(122.0, 65.0,  238.0, m_vMousePos.y)), 1.0f);
    SData.astroMidColor = glm::vec4(NORMCOLOR(BARYCOORD(250.0, 243.0, 255.0, m_vMousePos.y)),
                                    NORMCOLOR(BARYCOORD(247.0, 168.0, 255.0, m_vMousePos.y)),
                                    NORMCOLOR(BARYCOORD(180.0, 112.0, 255.0, m_vMousePos.y)), 1.0f);
    SData.waveColor     = glm::vec4(NORMCOLOR(BARYCOORD(146.0, 216.0, 59.0,  m_vMousePos.y)),
                                    NORMCOLOR(BARYCOORD(216.0, 161.0, 88.0,  m_vMousePos.y)),
                                    NORMCOLOR(BARYCOORD(203.0, 117.0, 113.0, m_vMousePos.y)), 1.0f);
    SData.MousePosition = m_vMousePos;
    
    VulkanInstance->GetSC()->GetCB()->GetSceneData() = SData;

    glfwPollEvents(); 

    return (App::Managers::Application::AppStatus)glfwWindowShouldClose(GetWndInstance<GLFWwindow>());
}
