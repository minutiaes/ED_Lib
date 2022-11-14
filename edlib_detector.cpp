// #include "EDLib.h"
#include "edlib_detector.h"
#include <iostream>
#include <chrono>


EDLibDetector::EDLibDetector()
{
    std::cout << "init EDLibDetectors\n";
}


DetectedShapes EDLibDetector::detect(cv::Mat &image)
{
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    std::cout << "start EDLibDetector::detect\n";
    ED testED = ED(
        image,
        settings.op,
        settings.gradient_threshold,
        settings.anchor_threshold,
        settings.scan_interval,
        settings.min_path_length,
        settings.sigma,
        settings.sum_flag
    );

    std::cout << "1 EDLibDetector::detect\n";

    EDLines testEDLines = EDLines(testED);
    EDCircles testEDCircles = EDCircles(testEDLines);

    std::cout << "2 EDLibDetector::detect\n";

    std::vector<mCircle> found_circles = testEDCircles.getCircles();
    std::vector<mEllipse> found_ellipses = testEDCircles.getEllipses();

    std::cout << "3 EDLibDetector::detect\n";

    DetectedShapes shape;
    shape.circles = found_circles;
    shape.ellipses = found_ellipses;
    std::cout << "end EDLibDetector::detect\n";

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "Time difference in EDLib = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "[us]" << std::endl;

    return shape;
}