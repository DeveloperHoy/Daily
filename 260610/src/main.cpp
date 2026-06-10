#include <iostream>
#include <chrono>
#include <opencv2/opencv.hpp>
#include "utils/ImageUtils.h"
#include "inference/YOLOv9Inference.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <image_path>" << std::endl;
        return 1;
    }

    try {
        // Load model
        std::string modelPath = "models/yolov9s.onnx";
        if (access(modelPath.c_str(), F_OK) != 0) {
            std::cerr << "Model not found: " << modelPath << std::endl;
            return 1;
        }

        std::cout << "Loading YOLOv9 model..." << std::endl;
        YOLOv9Inference inference(modelPath);

        // Load and preprocess image
        std::string imagePath = argv[1];
        std::cout << "Loading image: " << imagePath << std::endl;
        
        auto start = std::chrono::high_resolution_clock::now();
        cv::Mat image = ImageUtils::loadImage(imagePath);
        cv::Mat processed = ImageUtils::preprocessImage(image, 640, 640);
        auto end = std::chrono::high_resolution_clock::now();
        std::cout << "Preprocessing time: " 
                  << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms" << std::endl;

        // Run inference
        std::cout << "Running inference..." << std::endl;
        start = std::chrono::high_resolution_clock::now();
        auto results = inference.detectObjects(processed);
        end = std::chrono::high_resolution_clock::now();
        std::cout << "Inference time: " 
                  << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms" << std::endl;

        // Draw results
        std::cout << "Detecting objects..." << std::endl;
        int objectCount = 0;
        for (const auto& detection : results) {
            if (detection.confidence >= 0.45f) {
                objectCount++;
                
                // Get bounding box coordinates
                float centerX = detection.x + detection.width / 2.0f;
                float centerY = detection.y + detection.height / 2.0f;
                
                int x = static_cast<int>(centerX);
                int y = static_cast<int>(centerY);
                int w = static_cast<int>(detection.width * inference.modelInputWidth);
                int h = static_cast<int>(detection.height * inference.modelInputHeight);

                // Calculate box corners
                int x1 = std::max(0, x - w / 2);
                int y1 = std::max(0, y - h / 2);
                int x2 = std::min(704, x + w / 2);
                int y2 = std::min(704, y + h / 2);

                // Draw rectangle
                cv::Rect box(x1, y1, w, h);
                cv::rectangle(image, box, cv::Scalar(0, 255, 0), 2);

                // Draw label and confidence
                std::string label = inference.classNames[detection.classId];
                std::string confStr = std::to_string(detection.confidence);
                std::string labelConf = label + " " + confStr;

                cv::Size labelSize = cv::getTextSize(labelConf, cv::FONT_HERSHEY_SIMPLEX, 0.5, 1);
                int labelX = x1 + 5;
                int labelY = y1 + 15;

                // Fill background for label
                cv::rectangle(image, cv::Rect(labelX, labelY - labelSize.height, labelSize.width, labelSize.height),
                             cv::Scalar(0, 255, 0), -1);

                // Write text
                cv::putText(image, labelConf, cv::Point(labelX, labelY), cv::FONT_HERSHEY_SIMPLEX, 0.5, (0, 0, 0), 1);
            }
        }

        std::cout << "Detected " << objectCount << " objects" << std::endl;
        
        // Save result
        ImageUtils::saveImage(image, "output/detection_result.jpg");
        std::cout << "Result saved to detection_result.jpg" << std::endl;

        // Display result
        cv::imshow("Object Detection", image);
        cv::waitKey(0);

        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}
