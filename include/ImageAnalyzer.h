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

    static const std::list<MatchTemplate> tplsMatchSymbol;

    ImageAnalyzer(){};
    ~ImageAnalyzer(){};
    bool rectangleHasIntersection (const cv::Rect & lhs, const cv::Rect & rhs);
    void findSymbolInImage(const std::string & pathImageQuranPage, const std::list<MatchTemplate> & pathEndOfAyahSymbol, const std::string & pathMatchCoordinate = "", bool showIntermediate = false);
    void findSymbolInImage(const cv::Mat & imageQuranPage, const std::list<ImageAnalyzer::MatchTemplate> & pathTplMatchSymbols, std::string & matchCoordinateResult, bool showIntermediate = false);
    void segmentWordInPage(const std::string & pathImageQuranPage, const std::string & pathMatchCoordinate);
    void segmentWordInPolygon(const std::string & pathImageQuranPage, const std::vector<cv::Point> & polygon);
    void getPolygonFromAyahSymbolPos(const std::string & pathImageQuranPage, const std::vector<cv::Point> polygon);

};

#endif
