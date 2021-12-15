#include "VulkanWindow.h"
#include <iostream>
#include <cstdlib>

char triangle(const std::string& row)
{
    static char cResult = row.front();

    std::map<char, std::vector<int>> { {'G', { (int)('B' + 'R'), (int)('G' + 'G'), (int)('G') }},
                                      { 'R', {(int)('B' + 'G'), (int)('R' + 'R'), (int)('R') } },
                                      { 'B', {(int)('G' + 'R'), (int)('B' + 'B'), (int)('B') } }};

    for (int i = 0; i < static_cast<int>(row.size() / 2); i++)
    {

    }

    return cResult;
}


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