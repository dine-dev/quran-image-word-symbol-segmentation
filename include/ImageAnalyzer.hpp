#ifndef IMAGE_ANALYZER_H
#define IMAGE_ANALYZER_H

#include "Utils.hpp"

namespace ImageAnalyzer {


    struct MatchTemplate {
        std::string filePath;
        double threshold;
        std::string type;
        bool canOnlyFoundOnce;
    };

    const std::list<MatchTemplate> tplsMatchSymbol = {
            {"resources/template_match/end-of-ayah-symbol-first-page.jpg", 0.43, "ayah", false},
            {"resources/template_match/end-of-ayah-symbol-second-page.jpg", 0.43, "ayah", false},
            {"resources/template_match/end-of-ayah-symbol-1.jpg", 0.43, "ayah", false},
            {"resources/template_match/end-of-ayah-symbol-2.jpg", 0.9, "ayah", false},
            {"resources/template_match/end-of-ayah-symbol-11.jpg", 0.43, "ayah", false},
            {"resources/template_match/end-of-ayah-symbol-88.jpg", 0.43, "ayah", false},
            {"resources/template_match/end-of-ayah-symbol-111.jpg", 0.43, "ayah", false},
            {"resources/template_match/footer-first-page.jpg", 0.7, "footer", true},
            {"resources/template_match/footer-second-page.jpg", 0.7, "footer", true},
            {"resources/template_match/footer.jpg", 0.7, "footer", true},
            {"resources/template_match/header-first-page.jpg", 0.7, "header", true},
            {"resources/template_match/header.jpg", 0.7, "header", true},
            {"resources/template_match/basmalah.jpg", 0.6, "basmalah", true},
            {"resources/template_match/basmalah-first-page.jpg", 0.7, "basmalah", true},
            {"resources/template_match/basmalah-second-page.jpg", 0.9, "basmalah", true},
            {"resources/template_match/roubou3.jpg", 0.7, "roubou3", true},
            {"resources/template_match/roubou3-close.jpg", 0.9, "roubou3", true},
            {"resources/template_match/soujoud.jpg", 0.7, "soujoud", true},
    };

    const int MIN_AREA = 200;

    bool rectangleHasIntersection (const cv::Rect & lhs, const cv::Rect & rhs);
    void findSymbolInImage(const std::string & pathImageQuranPage, const std::list<MatchTemplate> & pathEndOfAyahSymbol, const std::string & pathMatchCoordinate = "", bool showIntermediate = false);
    void findSymbolInImage(const cv::Mat & imageQuranPage, const std::list<ImageAnalyzer::MatchTemplate> & pathTplMatchSymbols, std::string & matchCoordinateResult, bool showIntermediate = false);
    void segmentWordInPage(const std::string & pathImageQuranPage, const std::string & pathMatchCoordinate);
    void segmentWordInPolygon(const std::string & pathImageQuranPage, const std::vector<cv::Point> & polygon);
    void getPolygonFromAyahSymbolPos(const std::string & pathImageQuranPage, const std::vector<cv::Point> polygon);
    void getTextFrameRect(const cv::Mat & imageQuranPage, cv::Rect & textFrame);

}

#endif
