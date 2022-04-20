#include "RenderApp.h"
#include <iostream>
#include <cstdlib>
#include <memory>
#include <Utilities.h>
#include <queue>
#include <coroutine>
#include <BaseManager.h>
#include <fstream>
#include <Ray.h>

int main() {
    std::unique_ptr<RenderApp> app = std::make_unique<RenderApp>("Vulkan - AlGoMa", false, 0, 0, 800, 600);

    std::ofstream file;

    file.open("sample.ppm", std::ios_base::out);

    file << "P3\n" << app->Width() << " " << app->Height() << "\n255\n";

    Vector l(-2.0f, -1.0f, -1.0f, 0.0f);
    Vector h(4.0f, 0.0f, 0.0f, 0.0f);
    Vector v(0.0f, 2.0f, 0.0f, 0.0f);
    Vector o(0.0f);

    auto func = [](const App::Math::RayTraicing::Ray& r) {

        Vector dir = Vector::Normalize(r.Direction());
        float delta = 0.5f * (dir.y() + 1.0f);
        return (1.0f - delta) * Vector(1.0f) + delta * Vector(0.5f, 0.7f, 1.0f);
    };

    //for (t_IntS32 j = app->GetHeight() - 1; j >= 0; j--)
    //{
    //    for (t_IntS32 i = 0; i < app->GetWidth(); i++)
    //    {
    //        float fu = float(i) / float(app->GetWidth());
    //        float fv = float(j) / float(app->GetHeight());

    //        App::Math::RayTraicing::Ray r(o, l + fu * h + fv * v);

    //        Vector color = func(r);

    //        file << t_IntS32(255.99 * color.x()) << " " << t_IntS32(255.99 * color.y()) << " " << t_IntS32(255.99 * color.z()) << "\n"; 
    //    }
    //}


    file.close();

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