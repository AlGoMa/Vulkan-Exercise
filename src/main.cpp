#include "RenderApp.h"
#include <iostream>
#include <cstdlib>
#include <memory>
#include <Utilities.h>
#include <queue>
#include <coroutine>

int main() {
    std::unique_ptr<RenderApp> app = std::make_unique<RenderApp>("Vulkan - AlGoMa", true, 0, 0, 800, 600);

    App::Math::Vector sample;

    App::Math::Vector sumres = sample += App::Math::Vector(Vector(1.3f, 0.3f, 3.0f), 1.0f);

    App::Math::Vector mulres = sumres *= sample;

    App::Math::Vector mulscalar = mulres *= 3.4f;

    App::Math::Vector norm = mulscalar.Normalize();

    App::Math::Vector::Normalize(mulres);

    App::Math::Vector crossvec = mulscalar.Cross(sample);

#pragma region proof_of_concept
    STATIC_ASSERT(sizeof(double) == 8);
    double f = 82.3;

    t_IntU64 n = Reinterprete_Cast<t_IntU64>(f);
    f = Reinterprete_Cast<double>(n);

    double fRes = safediv(4.0, 3.0, 1.0);
    fRes = safediv(4.0, 0.0, 1.0);

    f = Reinterprete_Cast<double>(EndianessSwitcher(Reinterprete_Cast<t_IntU64>(f)));
    f = Reinterprete_Cast<double>(EndianessSwitcher(Reinterprete_Cast<t_IntU64>(f)));
#pragma endregion

    try {
        app->Run();
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    app.release();

    return EXIT_SUCCESS;
}