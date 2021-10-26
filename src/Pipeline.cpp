#include <Pipeline.h>
#include <VertexBuffer.h>
#include <IndexBuffer.h>
#include <VulkanManager.h>
#include <math.h>

void Pipeline::CreateShader(const std::string in_strPathToFile, const char* in_strEntryPoint, ShaderType in_eType)
{
    VkPipelineShaderStageCreateInfo stShaderStageInfo{};

    VkShaderModule shaderModule = CreateShaderModule(in_strPathToFile);
    
    switch (in_eType)
    {
    case ShaderType::fragment:
        stShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        break;
    case ShaderType::vertex:
        stShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
        break;
    case ShaderType::compute:
        stShaderStageInfo.stage = VK_SHADER_STAGE_COMPUTE_BIT;
        stShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        break;
    case ShaderType::geometry:
        stShaderStageInfo.stage = VK_SHADER_STAGE_GEOMETRY_BIT;
        break;
    case ShaderType::tessellation_control:
        stShaderStageInfo.stage = VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
        break;
    case ShaderType::tessellation_eval:
        stShaderStageInfo.stage = VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
        break;
    }

    stShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    stShaderStageInfo.module = shaderModule;
    stShaderStageInfo.pName = in_strEntryPoint;

    m_vShaderRes.push_back(stShaderStageInfo);
}

VkResult Pipeline::CreatePipelineLayout(void)
{
    VkResult eResult = VK_SUCCESS;

    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 1;
    pipelineLayoutInfo.pSetLayouts = &Instance->GetSC()->GetCB()->GetDescriptor();

    if ((eResult = vkCreatePipelineLayout(Instance->GetDevice()->GetDevice(), &pipelineLayoutInfo, nullptr, &m_mPipelines[PipelineType::graphics].m_ppLayout)) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create pipeline layout!");
    }

    return eResult;
}

VkResult Pipeline::CreateComputePipelineLayout(void)
{
    VkResult eResult = VK_SUCCESS;

    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 1;
    pipelineLayoutInfo.pSetLayouts = &Instance->GetSC()->GetSB()->GetDescriptor();

    if ((eResult = vkCreatePipelineLayout(Instance->GetDevice()->GetDevice(), &pipelineLayoutInfo, nullptr, &m_mPipelines[PipelineType::compute].m_ppLayout)) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create compute pipeline layout!");
    }

    return eResult;
}

VkResult Pipeline::CreateGraphicsPipeline(void) 
{
    VkResult eResult = VK_ERROR_INITIALIZATION_FAILED;

    if (0 < m_vShaderRes.size())
    {
        VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
        vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        auto bindingDescription = VertexBuffer::getBindingDescription();
        auto attributeDescriptions = VertexBuffer::getAttributeDescriptions();

        vertexInputInfo.vertexBindingDescriptionCount = 1;
        vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
        vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
        vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

        VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
        inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        inputAssembly.primitiveRestartEnable = VK_FALSE;

        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = (float)Instance->GetSC()->GetExtents().width;
        viewport.height = (float)Instance->GetSC()->GetExtents().height;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;

        VkRect2D scissor{};
        scissor.offset = { 0, 0 };
        scissor.extent = Instance->GetSC()->GetExtents();

        VkPipelineViewportStateCreateInfo viewportState{};
        viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewportState.viewportCount = 1;
        viewportState.pViewports = &viewport;
        viewportState.scissorCount = 1;
        viewportState.pScissors = &scissor;

        VkPipelineRasterizationStateCreateInfo rasterizer{};
        rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        rasterizer.depthClampEnable = VK_FALSE;
        rasterizer.rasterizerDiscardEnable = VK_FALSE;
        rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
        rasterizer.lineWidth = 1.0f;
        rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
        rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
        rasterizer.depthBiasEnable = VK_FALSE;

        VkPipelineMultisampleStateCreateInfo multisampling{};
        multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        multisampling.sampleShadingEnable = VK_FALSE;
        multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

        VkPipelineColorBlendAttachmentState colorBlendAttachment{};
        colorBlendAttachment.blendEnable = VK_FALSE;
        colorBlendAttachment.colorWriteMask = 0xf;
        colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
        colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE;
        colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
        colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
        colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
        colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

        VkPipelineColorBlendStateCreateInfo colorBlending{};
        colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        colorBlending.logicOpEnable = VK_FALSE;
        colorBlending.logicOp = VK_LOGIC_OP_COPY;
        colorBlending.attachmentCount = 1;
        colorBlending.pAttachments = &colorBlendAttachment;
        colorBlending.blendConstants[0] = 0.0f;
        colorBlending.blendConstants[1] = 0.0f;
        colorBlending.blendConstants[2] = 0.0f;
        colorBlending.blendConstants[3] = 0.0f;

        VkGraphicsPipelineCreateInfo pipelineInfo{};
        pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipelineInfo.stageCount = m_vShaderRes.size() - 1;
        pipelineInfo.pStages = m_vShaderRes.data();
        pipelineInfo.pVertexInputState = &vertexInputInfo;
        pipelineInfo.pInputAssemblyState = &inputAssembly;
        pipelineInfo.pViewportState = &viewportState;
        pipelineInfo.pRasterizationState = &rasterizer;
        pipelineInfo.pMultisampleState = &multisampling;
        pipelineInfo.pColorBlendState = &colorBlending;
        pipelineInfo.layout = m_mPipelines[PipelineType::graphics].m_ppLayout;
        pipelineInfo.renderPass = m_pRenderPass;
        pipelineInfo.subpass = 0;
        pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

        if ((eResult = vkCreateGraphicsPipelines(Instance->GetDevice()->GetDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_mPipelines[PipelineType::graphics].m_pp)) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create graphics pipeline!");
        }
    }

    return eResult;
}

VkResult Pipeline::CreateComputePipeline(void)
{
    VkResult eResult = VK_ERROR_INITIALIZATION_FAILED;

    VkComputePipelineCreateInfo stPipelineCreateInfo = {};
    stPipelineCreateInfo.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
    stPipelineCreateInfo.stage = m_vShaderRes.back();
    stPipelineCreateInfo.layout = m_mPipelines[PipelineType::compute].m_ppLayout;
    
    if ((eResult = vkCreateComputePipelines(Instance->GetDevice()->GetDevice(), VK_NULL_HANDLE, 1, &stPipelineCreateInfo, NULL, &m_mPipelines[PipelineType::compute].m_pp)) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create compute pipeline layout!");
    }

    return eResult;
}

VkResult Pipeline::CreateRenderPass(void)
{
    VkAttachmentDescription stColorAttachment{};
    stColorAttachment.format = Instance->GetSC()->GetFormats();
    stColorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    stColorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    stColorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    stColorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    stColorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    stColorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    stColorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference stColorAttachmentRef{};
    stColorAttachmentRef.attachment = 0;
    stColorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription stSubpass{};
    stSubpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    stSubpass.colorAttachmentCount = 1;
    stSubpass.pColorAttachments = &stColorAttachmentRef;

    VkSubpassDependency stDependency{};
    stDependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    stDependency.dstSubpass = 0;
    stDependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    stDependency.srcAccessMask = 0;
    stDependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    stDependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

    VkRenderPassCreateInfo stRenderPassInfo{};
    stRenderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    stRenderPassInfo.attachmentCount = 1;
    stRenderPassInfo.pAttachments = &stColorAttachment;
    stRenderPassInfo.subpassCount = 1;
    stRenderPassInfo.pSubpasses = &stSubpass;
    stRenderPassInfo.dependencyCount = 1;
    stRenderPassInfo.pDependencies = &stDependency;

    return vkCreateRenderPass(Instance->GetDevice()->GetDevice(), &stRenderPassInfo, nullptr, &m_pRenderPass);
}

VkShaderModule Pipeline::CreateShaderModule(const std::string& in_strFilename, VkStructureType in_unType)
{
    std::vector<char> vCode = App::Utilities::Gfx::readFile(in_strFilename);
    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = in_unType;
    createInfo.codeSize = vCode.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(vCode.data());

    VkShaderModule shaderModule;

    if (vkCreateShaderModule(Instance->GetDevice()->GetDevice(), &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
        throw std::runtime_error("failed to create shader module!");
    }

    return shaderModule;
}

VkResult Pipeline::CreateCommandBuffers(void)
{
    VkResult eResult = VK_SUCCESS;

    m_vCmdBuffers.resize(Instance->GetSC()->GetFramebuffer().size());

    VkCommandBufferAllocateInfo stAllocInfo{};
    stAllocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    stAllocInfo.commandPool = Instance->GetDevice()->GetCommandPool();
    stAllocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    stAllocInfo.commandBufferCount = (uint32_t)m_vCmdBuffers.size();

    if ((eResult = vkAllocateCommandBuffers(Instance->GetDevice()->GetDevice(), &stAllocInfo, m_vCmdBuffers.data())) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to allocate command buffers!");
    }

    return eResult;
}

VkResult Pipeline::UpdateCommandBuffers(Entity* in_pEntity, const uint32_t in_unFrame)
{
    VkResult eResult = VK_SUCCESS;

    vkResetCommandBuffer(m_vCmdBuffers[in_unFrame], 0);

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    if ((eResult = vkBeginCommandBuffer(m_vCmdBuffers[in_unFrame], &beginInfo)) != VK_SUCCESS) {
        throw std::runtime_error("failed to begin recording command buffer!");
    }
    
    //UpdateCSCommandBuffers(in_unFrame);

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = m_pRenderPass;
    renderPassInfo.framebuffer = Instance->GetSC()->GetFramebuffer()[in_unFrame];
    renderPassInfo.renderArea.offset = { 0, 0 };
    renderPassInfo.renderArea.extent = Instance->GetSC()->GetExtents();

    VkClearValue clearColor = { {{0.0f, 0.0f, 0.0f, 1.0f}} };
    renderPassInfo.clearValueCount = 1;
    renderPassInfo.pClearValues = &clearColor;

    vkCmdBeginRenderPass(m_vCmdBuffers[in_unFrame], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    vkCmdBindPipeline(m_vCmdBuffers[in_unFrame], VK_PIPELINE_BIND_POINT_GRAPHICS, m_mPipelines[PipelineType::graphics].m_pp);

    VkBuffer vertexBuffers[] = { in_pEntity->GetVertexBuffer(VertexBuffer::BufferType::vertex)->GetBuffer("Vertex").m_Buffer};
    VkDeviceSize offsets[] = { 0 };

    vkCmdBindVertexBuffers(m_vCmdBuffers[in_unFrame], 0, 1, vertexBuffers, offsets);

    vkCmdBindIndexBuffer(m_vCmdBuffers[in_unFrame], in_pEntity->GetVertexBuffer(VertexBuffer::BufferType::index)->GetBuffer("Index").m_Buffer, 0, VK_INDEX_TYPE_UINT32);

    VkDeviceSize limits = Instance->GetDevice()->GetGPUProperties().limits.minUniformBufferOffsetAlignment;

    uint32_t unOffset = 0;
    vkCmdBindDescriptorSets(m_vCmdBuffers[in_unFrame], VK_PIPELINE_BIND_POINT_GRAPHICS, m_mPipelines[PipelineType::graphics].m_ppLayout, 0, 1, &Instance->GetSC()->GetDescriptors()[in_unFrame], 1, &unOffset);

    vkCmdDrawIndexed(m_vCmdBuffers[in_unFrame], static_cast<uint32_t>(in_pEntity->GetIndex().size()), 1, 0, 0, 0);

    vkCmdEndRenderPass(m_vCmdBuffers[in_unFrame]);

    if ((eResult = vkEndCommandBuffer(m_vCmdBuffers[in_unFrame])) != VK_SUCCESS) {
        throw std::runtime_error("failed to record command buffer!");
    }

    return eResult;
}

VkResult Pipeline::UpdateCSCommandBuffers(const uint32_t in_unFrame)
{
    vkCmdBindPipeline(m_vCmdBuffers[in_unFrame], VK_PIPELINE_BIND_POINT_COMPUTE, m_mPipelines[PipelineType::compute].m_pp);
    
    //vkCmdPushConstants(m_vCmdBuffers[in_unFrame], m_mPipelines[PipelineType::compute].m_ppLayout, 
    //                   VK_SHADER_STAGE_COMPUTE_BIT, 0, sizeof(glm::vec2), 
    //                   &Instance->GetSC()->GetCB()->GetSceneData().MousePosition);

    vkCmdBindDescriptorSets(m_vCmdBuffers[in_unFrame], VK_PIPELINE_BIND_POINT_COMPUTE, m_mPipelines[PipelineType::compute].m_ppLayout, 0, 1, &Instance->GetSC()->GetDescriptors()[in_unFrame], 0, NULL);

    vkCmdDispatch(m_vCmdBuffers[in_unFrame], 1, 1, 1);

    return VK_SUCCESS;
}

void Pipeline::DestroyPipeLine(void)
{
    vkDestroyPipeline(Instance->GetDevice()->GetDevice(), m_mPipelines[PipelineType::graphics].m_pp, nullptr);
    vkDestroyPipelineLayout(Instance->GetDevice()->GetDevice(), m_mPipelines[PipelineType::graphics].m_ppLayout, nullptr);

    vkDestroyRenderPass(Instance->GetDevice()->GetDevice(), m_pRenderPass, nullptr);
}

void Pipeline::DestroyComputePipeLine(void)
{
    vkDestroyPipeline(Instance->GetDevice()->GetDevice(), m_mPipelines[PipelineType::compute].m_pp, nullptr);
    vkDestroyPipelineLayout(Instance->GetDevice()->GetDevice(), m_mPipelines[PipelineType::compute].m_ppLayout, nullptr);
}

void Pipeline::DestroyBuffers(void)
{
    vkFreeCommandBuffers(Instance->GetDevice()->GetDevice(), Instance->GetDevice()->GetCommandPool(), static_cast<uint32_t>(m_vCmdBuffers.size()), m_vCmdBuffers.data());
}

void Pipeline::Destroy(void)
{
    for (auto& desc : m_vShaderRes)
        vkDestroyShaderModule(Instance->GetDevice()->GetDevice(), desc.module, nullptr);
}