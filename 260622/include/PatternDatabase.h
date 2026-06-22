#pragma once
#include "IFeatureExtractor.h"
#include <opencv2/core/typesafe_cv.hpp>
#include <map>
#include <memory>

// Structure to hold all necessary data for one unique pattern template
struct PatternTemplate {
/**
 * @brief Manages the collection, comparison, and storage of unique visual patterns.
 */
class PatternDatabase {
public:
    using TemplateMap = std::map<int, PatternTemplate>;

    PatternDatabase(const std::string& pattern_dir, 
                     std::shared_ptr<IFeatureExtractor> extractor);
    ~PatternDatabase() = default;

    /**
     * @brief Processes a detected object bounding box to determine its status.
     * If it's new or matches an existing template, the template ID is updated/returned.
     * 
     * @param frame The source image for cropping and extraction.
     * @param bbox The bounding box of the detection.
     * @param cropper_image Reference to the cropped patch (pre-allocated).
     * @return The assigned Pattern ID.
     */
    int processDetection(const cv::Mat& frame, 
                         const Detection& detection, 
                         cv::Mat& cropper_image);

    /**
     * @brief Retrieves a reference to the stored template for a given pattern ID.
     */
    std::optional<PatternTemplate> getTemplate(int pattern_id) const;

    /**
     * @brief Prints the current statistics of patterns discovered.
     */
    void printStats() const;

private:
    const std::string base_dir_;
    TemplateMap templates_;
    std::shared_ptr<IFeatureExtractor> extractor_;
    int next_pattern_id_ = 1;

    // Internal helper functions
    cv::Mat cropAndExtract(const cv::Mat& frame, const cv::Rect& bbox) const;
    int processNewPattern(const std::string& class_name, const cv::Mat& template_img);

    void saveTemplate(const cv::Mat& image, int pattern_id, const std::string& filename);
};
