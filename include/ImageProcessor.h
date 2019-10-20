#ifndef IMAGE_PROCESSOR_H
#define IMAGE_PROCESSOR_H

#include "Utils.h"

class ImageProcessor {
public:
	ImageProcessor(){};
	~ImageProcessor(){};
	void whitenBackground(const std::string & pathImageQuranPage, const cv::Scalar & minHSV, const cv::Scalar & maxHSV, bool showIntermediate = false, std::string pathSavedCroppeddImage = "");
	void cropToTextFrame(const std::string & pathImageQuranPage, bool showIntermediate = false, std::string pathSavedCroppeddImage = "");
	void additionnalCrop(const std::string & pathImageQuranPage, const std::string & leftOrRightSide, const std::string & pathToFrameBorder, bool showIntermediate = false);
	static bool comp(const std::vector<cv::Point> & contour_lhs, const std::vector<cv::Point> & contour_rhs);
};

#endif
