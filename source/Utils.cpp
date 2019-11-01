#include "Utils.h"

bool utils::dirExists(const std::string & path) {
	struct stat info;

	if(stat( path.c_str(), &info ) != 0) {
		return false;
	}
	else if(info.st_mode & S_IFDIR) {
		return true;
	}
	else {
		return false;
	}
}

std::string utils::getFileNameWithoutExtension(const std::string & path) {
	std::string base_filename = path.substr(path.find_last_of("/\\") + 1);
	std::string::size_type const pos(base_filename.find_last_of('.'));
	base_filename = base_filename.substr(0, pos);
	return base_filename;
}


void utils::read_record(const std::string & filePath, std::vector<std::string> & row) {

	std::fstream fin;

    // Open an existing file
	fin.open(filePath, std::ios::in);

    // Read the Data from the file as string Vector
	std::string line, temp;

	while (fin >> temp) {

        // read an entire row and
        // store it in a string variable 'line'
		getline(fin, line);
		std::cout << line << std::endl;
		row.push_back(line);
        
	}

}

bool utils::comp(const std::vector<cv::Point> & contour_lhs, const std::vector<cv::Point> & contour_rhs) {
    return (cv::contourArea(contour_lhs) < cv::contourArea(contour_rhs));
}

void utils::split_string(const std::string & str, const std::string & delimiter, std::vector<std::string> & splited_string) {

    std::string::size_type pos = 0;
    std::string::size_type prev = 0;
    while ((pos = str.find(delimiter, prev)) != std::string::npos) {
        splited_string.push_back(str.substr(prev, pos - prev));
        prev = pos + 1;
    }
 
    // To get the last substring (or only, if delimiter is not found)
    splited_string.push_back(str.substr(prev));
 
    return;
}
