#include "VulkanWindow.h"
#include <iostream>
#include <cstdlib>

int main() {
    VulkanWindow app("Vulkan - AlGoMa", 0, 0, 800, 600);

    try {
        app.Run();
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}