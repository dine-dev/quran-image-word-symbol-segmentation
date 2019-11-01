#ifndef IMAGE_PROCESSOR_H
#define IMAGE_PROCESSOR_H

#include "Utils.h"

namespace ImageProcessor {

	void whitenBackground(const std::string & pathImageQuranPage, const cv::Scalar & minHSV, const cv::Scalar & maxHSV, bool showIntermediate = false, std::string pathSavedCroppeddImage = "");
	void whitenBackground(const cv::Mat & imageQuranPage, const cv::Scalar & minHSV, const cv::Scalar & maxHSV, std::vector<cv::Mat> & preprocessedImage);
	void cropToTextFrame(const std::string & pathImageQuranPage, bool showIntermediate = false, std::string pathSavedCroppeddImage = "");
	void cropToTextFrame(const cv::Mat & imageQuranPage, std::vector<cv::Mat> & preprocessedImage);
	void additionnalCrop(const std::string & pathImageQuranPage, const std::string & leftOrRightSide, const std::string & pathToFrameBorder, bool showIntermediate = false);
	void additionnalCrop(const cv::Mat & imageQuranPage, const std::string & leftOrRightSide, const std::string & pathToFrameBorder, std::vector<cv::Mat> & preprocessedImage);

};

#endif
