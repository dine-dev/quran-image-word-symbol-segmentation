#ifndef IMAGE_ANALYZER_H
#define IMAGE_ANALYZER_H

#include "Utils.h"

class ImageAnalyzer {

public:

    struct MatchTemplate {
        std::string filePath;
        double threshold;
        std::string type;
        bool canOnlyFoundOnce;
    };

    ImageAnalyzer(){};
    ~ImageAnalyzer(){};
    bool rectangleHasIntersection (const cv::Rect & lhs, const cv::Rect & rhs);
    void findSymbolInImage(const std::string & pathImageQuranPage, const std::list<MatchTemplate> & pathEndOfAyahSymbol, const std::string & pathMatchCoordinate = "", bool showIntermediate = false);
    void segmentWord(const std::string & pathImageQuranPage, const std::string & pathMatchCoordinate);

};

#endif
