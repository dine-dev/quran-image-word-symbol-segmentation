#ifndef PAGE_H
#define PAGE_H

#include "Utils.h"
#include "ImageProcessor.h"
#include "ImageAnalyzer.h"


class Page {
public:

    struct MatchSymbol {
        cv::Rect rect;
        std::string type;

        bool operator < (const MatchSymbol & ms) const {
            int offset = 10;
            // if one symbole is significantly higher than another then the higher is the first
            if( std::abs(rect.tl().y - ms.rect.tl().y) > offset) {
                return (rect.tl().y < ms.rect.tl().y);
            }
            else {
                return (rect.tl().x > ms.rect.tl().x);
            }
            
        }
    };

    Page(int pageNumber, std::string imageFilePath);
    ~Page();

    void showQuranPage(bool displayAllProcessedImage = true);
    void showSymbols();

    const std::list<int> pageIndexRequireAdditionnalCropLeft  = {26, 64, 92, 268, 344};
    const std::list<int> pageIndexRequireAdditionnalCropRight = {77, 251};
private:

	int mPageNumber;
    std::string mImageFilePath;
    std::vector<cv::Mat> mPreprocessedImages;
    std::vector<MatchSymbol> mMatchSymbols;
    cv::Mat mRawImage;
    cv::Mat mProcessedImage;
    std::string mMatchSymbolResult;

	void init();
	void processImage();
    void analyzeImage();
    void buildRect();
    void sortSymbolRects();

};

#endif // end PAGE_H