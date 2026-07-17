#pragma once
#include <memory>
#include <string>
#include <atomic>
#include "utils/Logger.h"

namespace ai::camera {

enum class InputType {
    VIDEO_FILE,
    WEBCAM,
    RTSP,
    RTMP,
    FILE,
    TCP,
    UDP,
    UNKNOWN
};

enum class VideoFormat {
    YUV420P,
    YUV420SP,
    YUV444P,
    RGB,
    BGR,
    RGBA,
    BGRA,
    AUTO
};

enum class ResolutionType {
    HIGH = 1080p,
    MEDIUM = 720p,
    STANDARD = 480p,
    AUTO,
    CUSTOM
};

struct VideoInputConfig {
    std::string path;
    InputType type = InputType::VIDEO_FILE;
    VideoFormat format = VideoFormat::BGR;
    ResolutionType resolution = ResolutionType::AUTO;
    int desiredWidth = 640;
    int desiredHeight = 480;
    int fps = 30;
    int bufferSize = 1024;  // Frames in buffer
    
    // RTSP specific
    std::string rtspUsername;
    std::string rtspPassword;
    std::string rtspTimeout;

    bool isLoop = false;
    bool enableAudio = false;
    bool dropFrames = false;
};

class InputSource {
public:
    InputSource(const VideoInputConfig& config, const utils::LoggerPtr& logger);
    ~InputSource();
    
    // Non-copyable
    InputSource(const InputSource&) = delete;
    InputSource& operator=(const InputSource&) = delete;

    [[nodiscard]] bool isOpen() const;
    [[nodiscard]] InputType getDataType() const;
    [[nodiscard]] VideoFormat getFormat() const;
    [[nodiscard]] bool isReady() const;
    [[nodiscard]] bool open();
    void close();
    
    [[nodiscard]] CV_8UC3 readFrame();
    [[nodiscard]] bool readFrameSync(std::vector<CV_8UC3>& frame);
    
    [[nodiscard]] int getFps() const;
    [[nodiscard]] void setFps(int fps);

    [[nodiscard]] std::string getLastError() const;

    void setLogger(utils::LoggerPtr logger);
    void logError(const std::string& message);

protected:
    // Virtual implementation for different input types
    [[nodiscard]] virtual bool openRaw() = 0;
    [[nodiscard]] virtual std::vector<CV_8UC3> readRaw() = 0;
    [[nodiscard]] virtual void closeRaw() = 0;

    // Frame caching for streaming
    std::vector<CV_8UC3> m_cachedFrames;
    size_t m_cachedFrameIndex = 0;

private:
    VideoInputConfig m_config;
    std::shared_ptr<utils::Logger> m_logger;
    std::atomic<bool> m_isClosed{false};
    std::atomic<bool> m_error{false};
    
    std::mutex m_mutex;
    std::condition_variable m_cv;

    utils::Timer m_frameTimer;
    std::atomic<size_t> m_totalFramesRead{0};
    std::atomic<size_t> m_totalFramesCached{0};
};

class WebcamInput : public InputSource {
public:
    WebcamInput(int cameraIndex = 0, const utils::LoggerPtr& logger = nullptr);
    std::vector<CV_8UC3> readRaw() override;
    void closeRaw() override;
    [[nodiscard]] bool openRaw() override;
};

class FileInput : public InputSource {
public:
    FileInput(const std::string& filepath, const utils::LoggerPtr& logger = nullptr);
    [[nodiscard]] bool openRaw() override;
    std::vector<CV_8UC3> readRaw() override;
    void closeRaw() override;
};

class RTSPInput : public InputSource {
public:
    RTSPInput(const std::string& url, 
              const std::string& username = "",
              const std::string& password = "",
              const utils::LoggerPtr& logger = nullptr);
    [[nodiscard]] bool openRaw() override;
    [[nodiscard]] void rtspTimeoutHandler() override;
    std::vector<CV_8UC3> readRaw() override;
    void closeRaw() override;

protected:
    [[nodiscard]] std::string getValidatedUrl() const;
    [[nodiscard]] std::string buildRTSPUrl() const;
};

} // namespace ai::camera
