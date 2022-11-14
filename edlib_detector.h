#include "EDLib.h"
#include "opencv2/ximgproc.hpp"
#include "opencv2/imgcodecs.hpp"

struct DetectedShapes
{
    std::vector<mCircle> circles;
    std::vector<mEllipse> ellipses;
};


struct DetectorSettings
{
    GradientOperator op = SOBEL_OPERATOR;
    int gradient_threshold = 36;
    int anchor_threshold = 8;
    int scan_interval = 1;
    int min_path_length = 10;
    double sigma = 1.0;
    bool sum_flag = true;
};


class EDLibDetector
{
public:
    EDLibDetector();
    DetectedShapes detect(cv::Mat &image);

    DetectorSettings settings;

private:
    // cv::Mat image;
    
    ED* ed;
    EDLines* ed_lines;
    EDCircles* ed_circles;
};