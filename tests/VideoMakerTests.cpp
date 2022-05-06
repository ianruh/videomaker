#include "gtest/gtest.h"
#include "VideoMaker.h"
#include <matplot/matplot.h>
#include <filesystem>
#include <cmath>

TEST(VideoMakerTests, 3dRotTest) { 
    VideoMaker::VideoMaker videoMaker(30);

    auto fig = matplot::figure(true);
    fig->size(1920,1080);
    auto t = matplot::iota(0, matplot::pi / 500, 40 * matplot::pi);
    auto xt =
        matplot::transform(t, [](auto t) { return (3. + cos(sqrt(32.) * t)) * cos(t); });
    auto yt = matplot::transform(t, [](auto t) { return sin(sqrt(32.) * t); });
    auto zt =
        matplot::transform(t, [](auto t) { return (3. + cos(sqrt(32.) * t)) * sin(t); });
    matplot::plot3(xt, yt, zt);
    matplot::axis(matplot::equal);
    matplot::xlabel("x(t)");
    matplot::ylabel("y(t)");
    matplot::zlabel("z(t)");

    auto ax = matplot::gca();
    float da = ax->azimuth();
    for (size_t i = 0; i <= 180; i+=1) {
        ax->azimuth(da + i);
        videoMaker.addFrame(fig);
    }

    std::filesystem::path path("3drot.mp4");
    videoMaker.save(path);
}
