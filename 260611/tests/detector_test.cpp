#include <iostream>
#include "detector.h"
#include "yolov9_model.h"
#include <gtest/gtest.h>
#include <filesystem>

using namespace yolo;

class DetectorTest : public ::testing::Test {
protected:
    void SetUp() override {
        model_path_ = "models/yolov9s.onnx";
        output_dir_ = "./test_output";
        options_.model_path = model_path_;
        options_.output_dir = output_dir_;
        options_.conf_threshold = 0.5;
        options_.iou_threshold = 0.5;
        options_.draw_boxes = true;
        options_.save_results = false;
    }

    void TearDown() override {
        // Cleanup test files
        std::filesystem::remove_all(output_dir_);
    }

    std::string model_path_;
    std::string output_dir_;
    DetectorOptions options_;
    std::string test_image_path = "tests/sample_image.jpg";
};

TEST_F(DetectorTest, LoadModel) {
    std::filesystem::create_directories("models");
    std::filesystem::copy_file(test_image_path, "models/yolov9s.onnx");
    
    DetectorOptions options;
    Detector detector(model_path_, output_dir_, options);
    
    EXPECT_TRUE(detector.is_valid());
}

TEST_F(DetectorTest, DetectImage) {
    DetectorOptions options;
    Detector detector(model_path_, output_dir_, options);
    
    DetectionResult result = detector.detect(test_image_path);
    
    EXPECT_TRUE(result.success);
    EXPECT_GT(result.detections.size(), 0);
}

TEST_F(DetectorTest, DetectDirectory) {
    std::filesystem::create_directories(output_dir_);
    std::vector<std::string> test_images;
    test_images.push_back(test_image_path);
    
    Detector detector(model_path_, output_dir_, options);
    DetectorResult result = detector.detect_directory(output_dir_);
    
    EXPECT_GT(result.total_images, 0);
}

TEST_F(DetectorTest, DetectWebcam) {
    Detector detector(model_path_, output_dir_, options);
    detector.detect_webcam();
    
    EXPECT_TRUE(result.success);
}

TEST_F(DetectorTest, ExportJSON) {
    std::vector<DetectionResult> results;
    bool json_success = detector.export_json("test_results.json", results);
    
    EXPECT_TRUE(json_success);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return ::testing::RunAllTests();
}
