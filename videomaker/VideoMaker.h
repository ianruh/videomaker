#ifndef VIDEOMAKER_VIDEOMAKER_H_
#define VIDEOMAKER_VIDEOMAKER_H_
#include <matplot/matplot.h>
#include <filesystem>

namespace VideoMaker {

/**
 * @brief Utility class to help make video from matplotplusplus figures.
 */
class VideoMaker {
 private:
    size_t numFrames = 0;
    size_t frameRate;
    std::filesystem::path tempDirectoryPath;

    static std::filesystem::path create_temporary_directory(size_t max_tries = 1000);

 public:

    /**
     * @brief VideoMaker constructor.
     *
     * @param frameRate The framerate to generate the video at.
     */
    VideoMaker(size_t frameRate = 10);

    ~VideoMaker();

    /**
     * @brief Add a frame to the video.
     *
     * @param figure Figure that will be rendered as the frame.
     */
    void addFrame(matplot::figure_handle figure);

    /**
     * @brief Save the video.
     *
     * @param path The file path to save to.
     */
    void save(const std::filesystem::path& path);
};

}

#endif // VIDEOMAKER_VIDEOMAKER_H_
