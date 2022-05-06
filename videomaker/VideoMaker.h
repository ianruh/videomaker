#ifndef VIDEOMAKER_VIDEOMAKER_H_
#define VIDEOMAKER_VIDEOMAKER_H_
#include <matplot/matplot.h>
#include <string>
#include <filesystem>
#include <random>
#include <exception>
#include <sstream>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <iostream>

namespace VideoMaker {

class VideoMaker {
 private:
    size_t numFrames = 0;
    size_t frameRate;
    std::filesystem::path tempDirectoryPath;

    static std::filesystem::path create_temporary_directory(size_t max_tries = 1000) {
        std::filesystem::path tmp_dir = std::filesystem::temp_directory_path();
        size_t i = 0;
        std::random_device dev;
        std::mt19937 prng(dev());
        std::uniform_int_distribution<uint64_t> rand(0);
        std::filesystem::path path;
        while (true) {
            std::stringstream ss;
            ss << std::hex << rand(prng);
            path = tmp_dir / ss.str();
            // true if the directory was created.
            if (std::filesystem::create_directory(path)) {
                break;
            }
            if (i == max_tries) {
                throw std::runtime_error("could not find non-existing directory");
            }
            i++;
        }
        return path;
    }

 public:

    VideoMaker(size_t frameRate = 10) {
        this->tempDirectoryPath = VideoMaker::create_temporary_directory();
        this->frameRate = frameRate;
    }

    ~VideoMaker() {
        std::filesystem::remove_all(this->tempDirectoryPath);
    }

    void addFrame(matplot::figure_handle figure) {
        std::stringstream ss;
        ss << this->numFrames << ".png";
        std::filesystem::path figurePath = this->tempDirectoryPath / ss.str();
        figure->save(figurePath.string());
        this->numFrames += 1;
    }

    void save([[maybe_unused]]const std::filesystem::path& path) {
        if(this->numFrames == 0) {
            return;
        }

        pid_t pid = fork();
        pid_t ret;
        int status;
        if(pid == -1) {
            throw std::runtime_error("unable to fork");
        } else if(pid != 0) {
            while ((ret = waitpid(pid, &status, 0)) == -1) {
                if (errno != EINTR) {
                    throw std::runtime_error("errno != EINTR");
                }
            }
            if ((ret == 0) ||
                !(WIFEXITED(status) && !WEXITSTATUS(status))) {
                throw std::runtime_error("Unexpected child status");
            }
        } else {
            // forked process
            char c1[] = "ffmpeg";
            char y[] = "-y";
            char quiet1[] = "-hide_banner";
            char quiet2[] = "-loglevel";
            char quiet3[] = "error";
            char c2[] = "-framerate";
            std::stringstream frameRatess;
            frameRatess << this->frameRate;
            char c4[] = "-i";
            std::stringstream pathss;
            pathss << this->tempDirectoryPath.string() << "/%d.png";
            char c6[] = "-c:v";
            char c7[] = "libx264";
            char c8[] = "-s";
            char c9[] = "1920x1080";
            std::string fileName = path.string();
            std::string pathString = pathss.str();
            std::string frameRateString = frameRatess.str();
            char* command[] = {c1, y, quiet1, quiet2, quiet3, c2, (char*)frameRateString.c_str(), c4, (char*)pathString.c_str(), c6, c7, c8, c9, (char*)fileName.c_str(), NULL};
            if(execvp("ffmpeg", command) == -1) {
                _Exit(127);
            }
        }
    }
};

}

#endif // VIDEOMAKER_VIDEOMAKER_H_
