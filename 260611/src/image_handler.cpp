#include "image_handler.h"
#include <iostream>
#include <opencv2/videoio.hpp>
#include <sstream>

namespace yolo {

ImageHandler::ImageHandler() : camera_id_(0), loop_(true) {}

bool ImageHandler::open_image(const std::string& filepath) {
    img_ = cv::imread(filepath);
    return img_.empty() == false;
}

bool ImageHandler::open_webcam(int camera_id) {
    camera_id_ = camera_id;
    cap_.open(camera_id);
    return cap_.isOpened();
}

bool ImageHandler::process_webcam(int max_frames) {
    if (cap_.isOpened()) {
        frame_count_ = 0;
        while (cap_.read() && (max_frames == -1 || frame_count_ < max_frames)) {
            frame_count_++;
            std::string output_path = "./webcam_" + std::to_string(frame_count_) + ".jpg";
            cv::imwrite(output_path, img_);
        }
    }
    return false;
}

cv::Mat ImageHandler::get_next_image() {
    if (img_.empty()) {
        if (cap_.isOpened()) {
            cap_.retrieve(img_, 1);
        }
    }
    return img_;
}

bool ImageHandler::close() {
    if (cap_.isOpened()) {
        cap_.release();
        return true;
    }
    return false;
}

ImageHandler::ImageHandler(int camera_id, bool loop, int max_frames)
    : camera_id_(camera_id), loop_(loop), max_frames_(max_frames) {}

} // namespace yolo
