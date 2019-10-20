#ifndef UTILS_H
#define UTILS_H

#include <sys/types.h>
#include <sys/stat.h>
#include <string>
#include <list>
#include <tuple>
#include <algorithm>
#include <numeric>
#include <iostream>
#include <iomanip>

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

namespace utils {

	bool dirExists(const std::string & path);

};

#endif
