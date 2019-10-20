#include "ImageProcessor.h"

void ImageProcessor::whitenBackground(const std::string & pathImageQuranPage, const cv::Scalar & minHSV, const cv::Scalar & maxHSV, bool showIntermediate, std::string pathSavedCroppeddImage) {
    cv::Mat image = cv::imread(pathImageQuranPage, cv::IMREAD_COLOR);
    cv::Mat src, src_HSV, src_threshold;

    image.copyTo(src);

    // convert from BGR to HSV colorspace
    cv::cvtColor(image, src_HSV, cv::COLOR_BGR2HSV);

    // threshold the HSV image to get only yellow color
    cv::inRange(src_HSV, minHSV, maxHSV, src_threshold);
    // src_threshold.rowRange(height, src.rows) = 0;

    src.setTo(255, src_threshold);

    if (!pathSavedCroppeddImage.empty()) {
        cv::imwrite(pathSavedCroppeddImage, src);
    }

    if (showIntermediate) {
        cv::imshow( "original image", image);
        cv::imshow( "background whitened image", src);
        cv::waitKey(0);
    }

}

void ImageProcessor::cropToTextFrame(const std::string & pathImageQuranPage, bool showIntermediate, std::string pathSavedCroppeddImage) {
    cv::Mat src = cv::imread(pathImageQuranPage, cv::IMREAD_COLOR);
    cv::Mat cropped, src_gray, src_HSV, dst;

    // threshold in grayscale
    // Convert the image to Gray
    cv::cvtColor(src, src_gray, cv::COLOR_BGR2GRAY);

    // 0: Binary 1: Binary Inverted 2: Threshold Truncated 3: Threshold to Zero 4: Threshold to Zero Inverted
    int threshold_type = 4, threshold_value = 185; // max value = 255;
    int const max_BINARY_value = 255;

    cv::threshold(src_gray, dst, threshold_value, max_BINARY_value, threshold_type);

    // determine contours
    std::vector<std::vector<cv::Point> > contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(dst, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE, cv::Point(0, 0));

    // find max area contours
    std::vector<std::vector<cv::Point> >::iterator result;
    result = std::max_element(contours.begin(), contours.end(), comp);

    cv::Rect bounding_rect = cv::boundingRect((*result));
    cropped = src(bounding_rect);

    if (!pathSavedCroppeddImage.empty()) {
        cv::imwrite(pathSavedCroppeddImage, cropped);
    }

    // display intermediate
    if(showIntermediate) {
        cv::Scalar color = cv::Scalar(0, 0, 0);
        cv::rectangle(src, bounding_rect.tl(), bounding_rect.br(), color, 2, 8, 0);
        cv::imshow("thresholded", dst);
        cv::imshow("src with bounding rectangle", src);
        cv::imshow("cropped", cropped);
        cv::waitKey(0);
    }

    return;
}

bool ImageProcessor::comp(const std::vector<cv::Point> & contour_lhs, const std::vector<cv::Point> & contour_rhs) {
    return (cv::contourArea(contour_lhs) < cv::contourArea(contour_rhs));
}

void ImageProcessor::additionnalCrop(const std::string & pathImageQuranPage, const std::string & leftOrRightSide, const std::string & pathToFrameBorder, bool showIntermediate) {

    // find border frame in image
    cv::Mat tpl_gray = cv::imread(pathToFrameBorder, cv::IMREAD_GRAYSCALE);
    cv::Mat tpl = cv::imread(pathToFrameBorder, cv::IMREAD_COLOR);

    cv::Mat src_gray = cv::imread(pathImageQuranPage, cv::IMREAD_GRAYSCALE);
    cv::Mat src = cv::imread(pathImageQuranPage, cv::IMREAD_COLOR);

    // method: CV_TM_SQDIFF, CV_TM_SQDIFF_NORMED, CV_TM _CCORR, CV_TM_CCORR_NORMED, CV_TM_CCOEFF, CV_TM_CCOEFF_NORMED
    cv::Mat result_mat;
    int match_method = cv::TM_CCORR_NORMED;
    cv::matchTemplate(src_gray, tpl_gray, result_mat, match_method);
    cv::normalize(result_mat, result_mat, 0, 1, cv::NORM_MINMAX, -1, cv::Mat());

    double minVal; double maxVal;
    cv::Point minLoc, maxLoc, matchLoc;
    cv::minMaxLoc(result_mat, &minVal, &maxVal, &minLoc, &maxLoc, cv::Mat() );
    matchLoc = match_method  == cv::TM_SQDIFF || match_method == cv::TM_SQDIFF_NORMED ? minLoc:maxLoc;

    // replace the border
    tpl.copyTo(src(cv::Rect(matchLoc.x, matchLoc.y, tpl.cols, tpl.rows)));
    cv::imshow("border replacement", src);

    // crop from new border
    if (leftOrRightSide.compare("left") == 0) {
        src = src(cv::Rect(matchLoc.x, 0, src.cols - matchLoc.x, src.rows));
    }
    else if (leftOrRightSide.compare("right") == 0) {
        src = src(cv::Rect(0, 0, matchLoc.x + tpl.cols, src.rows));
    }


    if (showIntermediate) {
        cv::Mat img = cv::imread(pathImageQuranPage, cv::IMREAD_GRAYSCALE);
        cv::rectangle(img, matchLoc, cv::Point(matchLoc.x + tpl.cols , matchLoc.y + tpl.rows), CV_RGB(255,0,0), 3);
        cv::imshow("border match", img);
        cv::imshow("border match and crop", src);
        cv::waitKey(0);
    }

    cv::imwrite(pathImageQuranPage, src);

}

