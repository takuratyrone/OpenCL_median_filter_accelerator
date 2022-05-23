#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;
using namespace std::chrono;

Mat img, gray, blurred, sobelx, sobely, sobelxy;

int main()
{
    // Reading in the image. If the image is not found it displays the message Could not open of find the image!
    img = imread("/Users/faith/Documents/cvtest/CTImage.png");
    if (img.empty())
    {
        cout << "Could not open or find the image!" << endl;
        return -1;
    }
    // This part displays the original image
    cv::imshow("Original Image", img);
    cv::imwrite("/Users/faith/Documents/cvtest/OriginalImage.jpeg", img);
    
    //This part converts the image into gray scale
    cvtColor(img, gray, COLOR_BGR2GRAY);
    
    //This part blurs the image using the median filter and the image is then displayed
    auto startMedian = high_resolution_clock::now(); //start the time for median filter
    medianBlur(gray, blurred, 21);
    auto stopMedian = high_resolution_clock::now(); //stop the time for median filter
    auto medianDuration = duration_cast<microseconds>(stopMedian - startMedian); //for use in calculating median filter speedup
    cv::imshow("Median filtered", blurred);
    cv::imwrite("/Users/faith/Documents/cvtest/MedianImage.jpeg", blurred);

    // This part does the image processing using the Canny edge detection algorithm and the canny edge detected image is then displayed
    auto startSobel = high_resolution_clock::now(); //start the time for Canny
    Sobel(blurred, sobelx, CV_64F, 1, 0, 5);
    Sobel(blurred, sobely, CV_64F, 0, 1, 5);
    Sobel(blurred, sobelxy, CV_64F, 1, 1, 5);
    auto stopSobel = high_resolution_clock::now(); //stop the time for Canny
    auto sobelDuration = duration_cast<microseconds>(stopSobel - startSobel); //for use in calculating edge detection speedup
    cv::imshow("Sobel X edge detection", sobelx);
    cv::imshow("Sobel Y edge detection", sobely);
    cv::imshow("Sobel XY edge detection", sobelxy);
    cv::imwrite("/Users/faith/Documents/cvtest/SobelXImage.jpeg", sobelx);
    cv::imwrite("/Users/faith/Documents/cvtest/SobelYImage.jpeg", sobely);
    cv::imwrite("/Users/faith/Documents/cvtest/SobelXYImage.jpeg", sobelxy);
    cv::waitKey(0);

    //display time taken by median filter
    cout << "Time taken by the Median Filter: " << medianDuration.count() << " microseconds" << endl;
    
    //display time taken by canny edge detection
    cout << "Time taken by the Sobel Edge Detection: " << sobelDuration.count() << " microseconds" << endl;
}
