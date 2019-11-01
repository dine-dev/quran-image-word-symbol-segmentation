#ifndef UTILS_H
#define UTILS_H

#include <sys/types.h>
#include <sys/stat.h>
#include <cstdlib>
#include <string>
#include <list>
#include <vector>
#include <tuple>
#include <algorithm>
#include <numeric>
#include <iostream>
#include <iomanip>
#include <fstream>

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

namespace utils {

	bool dirExists(const std::string & path);
	std::string getFileNameWithoutExtension(const std::string & path);
	void read_record(const std::string & filePath, std::vector<std::string> & row);
	bool comp(const std::vector<cv::Point> & contour_lhs, const std::vector<cv::Point> & contour_rhs);
	void split_string(const std::string & str, const std::string & delimiter, std::vector<std::string> & splited_string);

};

#endif
