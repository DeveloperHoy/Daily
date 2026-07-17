#pragma once
#include <memory>
#include <vector>
#include <atomic>
#include <mutex>
#include "Camera/Input.h"
#include "Detection/Detector.h"
#include "Tracking/Tracker.h"
#include "Rendering/Renderer.h"
#include "Events/EventManager.h"
#include "Zones/ZoneManager.h"
#include "Recording/VideoRecorder.h"
#include "gui/GuiManager.h"
#include "database/DatabaseManager.h"

namespace ai::core {

// Pipeline stages: Each class implements the next stage's interface
class PipelineStage {
public:
    virtual ~PipelineStage() = default;
    virtual void process() = 0;
    virtual void onFrame(std::shared_ptr<utils::FrameData>) = 0;
    virtual void onError(const std::string& error) = 0;
};

class CameraStage : public PipelineStage {
public:
    CameraStage();
    void process();
    void onFrame(std::shared_ptr<utils::FrameData>) override;
    void onError(const std::string& error) override;
};

class DetectionStage : public PipelineStage {
public:
    DetectionStage();
    void process();
    void onFrame(std::shared_ptr<utils::FrameData>) override;
    void onError(const std::string& error) override;
};

class TrackingStage : public PipelineStage {
public:
    TrackingStage();
    void process();
    void onDetection(std::shared_ptr<utils::DetectedObject>) override;
    void onError(const std::string& error) override;
};

class RenderingStage : public PipelineStage {
public:
    RenderingStage();
    void process();
    void onRender(std::shared_ptr<utils::RenderFrame>) override;
    void onError(const std::string& error) override;
};

class EventStage : public PipelineStage {
public:
    EventStage();
    void process();
    void onEvent(std::shared_ptr<utils::Event>) override;
    void onError(const std::string& error) override;
};

class RecordingStage : public PipelineStage {
public:
    RecordingStage();
    void process();
    void onFrame(std::shared_ptr<utils::RenderFrame>) override;
    void onError(const std::string& error) override;
};

class DatabaseStage : public PipelineStage {
public:
    DatabaseStage();
    void process();
    void onEvent(std::shared_ptr<utils::Event>) override;
    void onError(const std::string& error) override;
};

class GUIModule : public PipelineStage {
public:
    GUIModule();
    void process();
    void onFrame(std::shared_ptr<utils::RenderFrame>) override;
    void onError(const std::string& error) override;
};

class NetworkStage : public PipelineStage {
public:
    NetworkStage();
    void process();
    void onEvent(std::shared_ptr<utils::Event>) override;
    void onError(const std::string& error) override;
};

// Pipeline orchestrator
class Pipeline : public PipelineStage {
public:
    enum class Stage {
        CAMERA,
        DETECTION,
        TRACKING,
        RENDERING,
        EVENTS,
        RECORDING,
        DATABASE,
        GUI,
        NETWORK
    };

    Pipeline(std::string& name);
    ~Pipeline();
    
    void addStage(Stage stage, PipelineStage::Ptr stage);
    void initialize();
    void run();
    void stop();
    
    [[nodiscard]] size_t getStageCount() const;
    [[nodiscard]] const std::vector<PipelineStage::Ptr>& getStages() const;
    [[nodiscard]] Stage getCurrentStage() const;
    [[nodiscard]] void switchStage(Stage stage);

    void onError(const std::string& error) override;

    // Multi-threaded execution
    void executeAsync(std::function<void()> callback);
    void waitForCompletion();

private:
    void executeStageSequentially();
    void executeStageParallel();
    void cleanup();

    std::string m_name;
    std::vector<PipelineStage::Ptr> m_stages;
    std::atomic<size_t> m_stageIndex{0};
    std::atomic<bool> m_running{false};
    std::atomic<bool> m_error{false};

    std::mutex m_mutex;
    std::condition_variable m_cv;

    utils::Timer m_timer;
    std::atomic<size_t> m_frameCount{0};
    std::atomic<size_t> m_errorCount{0};
    std::atomic<double> m_fps{0.0};
};

} // namespace ai::core
