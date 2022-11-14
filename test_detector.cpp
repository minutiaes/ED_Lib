#include "edlib_detector.h"
#include <iostream>


int main(int argc, char** argv)
{
    char* filename;
    if (argc == 2)
        filename = argv[1];
    else if (argc < 2)
    {
        std::cout << "Image file is required!\n";
        return 0;
    }
    else
    {
        std::cout << "More than one image is not supported!\n";
        return 0;
    }

    // cv::Mat testImg = cv::imread(filename, 0);

    // EDLibDetector ed_det = EDLibDetector();
    // DetectorSettings new_settings;
    // // ed_det.change_parameters(new_settings);
    // DetectedShapes shapes = ed_det.detect(testImg);

    // std::cout << "Circles\n";
    // for (int i=0; i<shapes.circles.size(); i++)
    // {
    //     std::cout << (int)shapes.circles[i].center.x
    //         << " , " << (int)shapes.circles[i].center.y
    //         << " , r: " << (int)shapes.circles[i].r << std::endl;
    // }

    // std::cout << "Ellipses\n";
    // for (int i = 0; i < shapes.ellipses.size(); i++)
    // {
    //     std::cout << (int)shapes.ellipses[i].center.x
    //         << " , " << (int)shapes.ellipses[i].center.y
    //         << " | " << (int)shapes.ellipses[i].axes.width
    //         << " , " << (int)shapes.ellipses[i].axes.height
    //         << " theta: " << (double) shapes.ellipses[i].theta * 180 / CV_PI<< std::endl;

    // }

    return 0;
}
