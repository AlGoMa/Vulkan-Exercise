#include "RenderApp.h"
#include <exception>
#include <vector>
#include <string>
#include <VertexBuffer.h>
#include <IndexBuffer.h>
#include <chrono>
#define BARYCOORD(x0,x1,x2,p) (p < .5f) ? ((x1 * p * 2.f) + x0 * (.5f - p) * 2.f) : (x2 * (p - .5f) * 2.f + x1 * (1.f - p) * 2.f)
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
                           const auto app = static_cast<RenderApp*>(glfwGetWindowUserPointer(window));
                           app->UpdateEvent(static_cast<uint64_t>(Events::KeyEscPressed), static_cast<EventState>(GLFW_KEY_ESCAPE == key));
                       });

    if (glfwRawMouseMotionSupported())
        glfwSetInputMode(GetWndInstance<GLFWwindow>(), GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

    if (VK_SUCCESS != VulkanInstance->vkCreateVulkanInstance(GetAppName(), m_pInstance))
    {
        throw std::runtime_error("failed to create Vulkan Environment!");
    }

    m_pEntity->AddBuffer(new VertexBuffer());
    m_pEntity->AddBuffer(new IndexBuffer());

    VulkanInstance->GetPipeline()->CreateShader("Shaders/frag.spv",    "main", VukanPipeline::ShaderType::fragment);
    VulkanInstance->GetPipeline()->CreateShader("Shaders/vert.spv",    "main", VukanPipeline::ShaderType::vertex);
    /* ToDo: Synchronization issues. */
    VulkanInstance->GetPipeline()->CreateShader("Shaders/compute.spv", "main", VukanPipeline::ShaderType::compute);

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
    m_pEntity = std::make_unique<RenderEntity>();

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

    SData.sky        = ColorRGBA(BARYCOORD(182.0f, 207.0f, 23.0f, m_vMousePos.y),
                                 BARYCOORD(218.0f, 122.0f, 11.0f, m_vMousePos.y),
                                 BARYCOORD(228.0f, 80.0f,  40.0f, m_vMousePos.y)).Normalized();
    SData.lightColor = ColorRGBA(BARYCOORD(179.0f, 238.0f, 46.0f, m_vMousePos.y),
                                 BARYCOORD(174.0f, 212.0f, 21.0f, m_vMousePos.y),
                                 BARYCOORD(234.0f, 171.0f, 56.0f, m_vMousePos.y)).Normalized();

    SData.astroOutColor = ColorRGBA(BARYCOORD(216.0f, 205.0f, 243.0f, m_vMousePos.y),
                                    BARYCOORD(186.0f, 100.0f, 198.0f, m_vMousePos.y),
                                    BARYCOORD(122.0f, 65.0f,  238.0f, m_vMousePos.y)).Normalized();
    SData.astroMidColor = ColorRGBA(BARYCOORD(250.0f, 243.0f, 255.0f, m_vMousePos.y),
                                    BARYCOORD(247.0f, 168.0f, 255.0f, m_vMousePos.y),
                                    BARYCOORD(180.0f, 112.0f, 255.0f, m_vMousePos.y)).Normalized();
    SData.waveColor     = ColorRGBA(BARYCOORD(146.0f, 216.0f, 59.0f,  m_vMousePos.y),
                                    BARYCOORD(216.0f, 161.0f, 88.0f,  m_vMousePos.y),
                                    BARYCOORD(203.0f, 117.0f, 113.0f, m_vMousePos.y)).Normalized();
    SData.MousePosition = Vector(m_vMousePos.x, m_vMousePos.y, 0.f);
    
    VulkanInstance->GetSC()->GetCB()->GetSceneData() = SData;

    glfwPollEvents(); 

    return (App::Managers::Application::AppStatus)glfwWindowShouldClose(GetWndInstance<GLFWwindow>());
}
