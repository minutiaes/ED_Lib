# About

## ED_Lib

[Go to ED_Lib section](#sec:ED_Lib)

## This Fork

It aims to benefit from great detection capability of ED_Lib in python environments.
Instead of generating bindings with wrappers, it utilizes `named_pipes` for IPC. A
server, which has detector, answers the detection requests sent from python side.

# How to Build

| Prerequisite            | Version           |
|-------------------------|-------------------|
| Microsoft Visual Studio | v16 2019 or above |
| opencv                  | 3.4               |
| opencv_contrib          | 3.4               |


    $ git clone --depth=1 --branch=3.4 https://github.com/opencv/opencv.git
    $ git clone --depth=1 --branch=3.4 https://github.com/opencv/opencv_contrib.git
    $ md build-opencv
    $ cd build-opencv
    $ cmake -DWITH_JASPER=OFF -DWITH_WEBP=OFF -DBUILD_IPP_IW=OFF -DBUILD_ITT=OFF -DWITH_OPENEXR=OFF -DWITH_OPENJPEG=OFF -DBUILD_PROTOBUF=OFF -DWITH_TIFF=OFF -DBUILD_PERF_TESTS=OFF -DBUILD_TESTS=OFF -DBUILD_opencv_gapi=OFF -DBUILD_opencv_ml=OFF -DBUILD_opencv_objdetect=OFF -DBUILD_opencv_photo=OFF -DBUILD_opencv_stitching=OFF -DBUILD_opencv_aruco=OFF -DBUILD_opencv_bioinspired=OFF -DBUILD_opencv_fuzzy=OFF -DBUILD_opencv_hfs=OFF -DBUILD_opencv_img_hash=OFF -DBUILD_opencv_surface_matching=OFF -DBUILD_opencv_bgsegm=OFF -DBUILD_opencv_intensity_transform=OFF -DBUILD_opencv_optflow=OFF -DBUILD_opencv_phase_unwrapping=OFF -DBUILD_opencv_plot=OFF -DBUILD_opencv_rapid=OFF -DBUILD_opencv_reg=OFF -DBUILD_opencv_rgbd=OFF -DBUILD_opencv_saliency=OFF -DBUILD_opencv_shape=OFF -DBUILD_opencv_stereo=OFF -DBUILD_opencv_structured_light=OFF -DBUILD_opencv_superres=OFF -DBUILD_opencv_tracking=OFF -DBUILD_opencv_videostab=OFF -DBUILD_opencv_xfeatures2d=OFF -DOPENCV_EXTRA_MODULES_PATH=../opencv_contrib/modules ../opencv -G "Visual Studio 17 2022"
    $ cmake --build . --config Release -j 4
    $ set OpenCV_DIR=C:\projects\ed-lib\build-opencv
    $ cd ..
    $ md build
    $ cd build
    $ cmake .. -G "Visual Studio 17 2022"
    $ cmake --build . --config Release -j 4

# How to Use

1. Copy necessary OpenCV DLLs next to `pipe_server.exe`
2. Run `pipe_server.exe`
3. Use `EDLibDetector` class from `./python_scripts/edlib_detector.py`

## Simple Example

```python
PATH_PIPE_SERVER = r"\\path\\to\\server\\pipe_server.exe"
PATH_IMAGE = r"\\billiard.jpg"
pipe_server = Popen([PATH_PIPE_SERVER])
img = cv2.imread(PATH_IMAGE)

detector = EDLibDetector((60, 90))
result = detector.detect(img)
print(result)
```


<a name="sec:ED_Lib"></a>
# ED_Lib

EDGE DRAWING LIBRARY FOR GEOMETRIC FEATURE EXTRACTION AND VALIDATION

Keywords: edge detection, edge segment detection, color edge detection, line detection, line segment detection, circle detection, ellipse detection.

Edge Drawing (ED) algorithm is an proactive approach on edge detection problem.
In contrast to many other existing edge detection algorithms which follow a subtractive approach (i.e. after applying gradient filters onto an image eliminating pixels w.r.t. several rules, e.g. non-maximal suppression and hysteresis in Canny), ED algorithm works via an additive strategy, i.e. it picks edge pixels one by one, hence the name Edge Drawing.
Then we process those random shaped edge segments to extract higher level edge features, i.e. lines, circles, ellipses, etc.
The popular method of extraction edge pixels from the thresholded gradient magnitudes is non-maximal supressiun that tests every pixel whether it has the maximum gradient response along its gradient direction and eliminates if it does not.
However, this method does not check status of the neighboring pixels, and therefore might result low quality (in terms of edge continuity, smoothness, thinness, localization) edge segments.
Instead of non-maximal supression, ED points a set of edge pixels and join them by maximizing the total gradient response of edge segments.
Therefore it can extract high quality edge segments without need for an additional hysteresis step. 

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

The list of algorithms in this library:

ED: Edge Drawing detection algorithm. Detects edge segments in an input image and provides the result in segment form (a vector of edge segment pixels).

Paper: C. Topal, C. Akinlar, Edge Drawing: A Combined Real-Time Edge and Segment Detector, Journal of Visual Communication and Image Representation, vol.23, no.6, pp.862-872, August 2012.

[https://www.sciencedirect.com/science/article/pii/S1047320312000831]

Demo video: https://www.youtube.com/watch?v=-Bpb_OLfOts

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

EDPF: Edge Drawing - Parameter Free: Detects edge segments without need for parameter tuning. It runs naive ED with all parameters at their extreme to detect all possible edge segments, then invalidates false detected segments due to Helmholtz Principle.

Paper: C. Akinlar, C. Topal, EDPF: A Real-time Parameter-free Edge Segment Detector with a False Detection Control, Int’l Journal of Pattern Recognition and Artificial Intelligence, vol.26, no.1, 2012.

[https://doi.org/10.1142/S0218001412550026]

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

EDLines: Detects line segments in the image and provides the result as a vector list consisting of starting and ending points.
EDLines is alos a parameter-free algorithm which validates all detected lines via Helmholtz Principle.

Paper: C. Akinlar, C. Topal, EDLines: A real-time line segment detector with a false detection control, Pattern Recognition Letters, vol.32, iss.13, pg. 1633-1642, October 2011. 

[https://www.sciencedirect.com/science/article/pii/S0167865511001772]

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

EDCircles: Detects circles and ellipses (up to a level of eccentricity) in an input image and returns the result in a list of circle and ellipse parameters. Just like EDPF and EDLines, EDCircles is alos a parameter-free algorithm that applies validation via Helmholtz Principle.

Paper: C. Akinlar, C. Topal, EDCircles: A Real-time Circle Detector with a False Detection Control, Pattern Recognition, 46(3), 725-740, March 2013.

[https://www.sciencedirect.com/science/article/pii/S0031320312004268]

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

EDColor: Edge Drawing - Color: Detects edges in segment form on color image using a color gradient computation, hence provides better results than converting the image to grayscale.

Paper: C. Akinlar, C. Topal, ColorED: Color edge and segment detection by Edge Drawing (ED), Journal of Visual Communication and Image Representation, vol.44, pp.82-94, April 2017.

[https://www.sciencedirect.com/science/article/pii/S1047320317300305]

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Instructions:
You can detect edge segments with ED, EDPF and EDColor; and lines and circles with EDLines and EDCircles, easily.
It is also possible to feed ED instances to EDLines or EDCircles to extract line segments, circles and ellipses without need to run edge segment detection again.
In this way, redundant computation can be avoided.
Results might slightly differ between using directly EDLines and ED + EDX (EDLines, EDCircles, etc.) because of the differences in default gradient operators and parameters in the algorithms.
There are several usage scenarios available in test.cpp. 

Dependencies: OpenCV 3.4

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Disclaimer for Edge Drawing Library

This software library is provided "as is" and "with all faults." Authors of this library make no warranties of any kind concerning the safety, suitability, lack of viruses, inaccuracies, typographical errors, or other harmful components of this software product. 
There are inherent dangers in the use of any software, and you are solely responsible for determining whether this software product is compatible with your equipment and other software installed on your equipment. 
You are also solely responsible for the protection of your equipment and backup of your data, and the authors of this software product will not be liable for any damages you may suffer in connection with using, modifying, or distributing this software product. 
By using this library in any scientific work, you are implicitly presumed to have accepted all of the above statements, and accept to cite the aforementioned papers.
