# Video Maker

A small utility library to make videos from [Matplot++](https://github.com/alandefreitas/matplotplusplus)
figures. Depends on having `ffmpeg` installed. Feel free to use, but I doubt
it's very portable. I just wanted it for a couple projects.

## Using CMAKE FetchContent

```
FetchContent_Declare(
    videomaker
    GIT_REPOSITORY git@github.com:ianruh/videomaker.git
)
FetchContent_MakeAvailable(videomaker)
```

## Example

```cpp
VideoMaker::VideoMaker videoMaker;

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
for (size_t i = 0; i <= 360; i+=5) {
    ax->azimuth(da + i);
    videoMaker.addFrame(fig);
}

std::filesystem::path path("3drot.mp4");
videoMaker.save(path);
```

Output: ![](./example/3drot.mp4)
