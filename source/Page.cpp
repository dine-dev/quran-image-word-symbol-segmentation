#include "Page.hpp"

Page::Page(int pageNumber, std::string imageFilePath) : mPageNumber(pageNumber), mImageFilePath(imageFilePath) {
    init();
};

Page::~Page() {};

void Page::showQuranPage(bool displayAllProcessedImage) {
    
    if (displayAllProcessedImage) {
        cv::imshow( "original image", mRawImage);
        std::vector<std::string> windowTitle = {"background whitened image", "cropped image", "additionnal cropped image"};
        for (size_t indS = 0; indS < mPreprocessedImages.size(); ++indS) {
            cv::imshow( windowTitle[indS], mPreprocessedImages[indS]);
        }
    }
	
    cv::imshow( "processed image", mProcessedImage);
    return;
}

void Page::showSymbols() {
    cv::Mat img_disp = mProcessedImage.clone();
    for (size_t indR = 0; indR < mMatchSymbols.size(); ++indR) {
        cv::rectangle(img_disp, mMatchSymbols[indR].rect, cv::Scalar(0,0,255), 3);
        cv::imshow( "Result window", img_disp);
        cv::waitKey(0);
    }
    cv::imshow( "Result window", img_disp);
    return;
}

void Page::init() {
	// init image processor and analyzer
    processImage();
    analyzeImage();
    buildRect();
    sortSymbolRects();
}

void Page::processImage() {
	// load rawImage
	mRawImage = cv::imread(mImageFilePath, cv::IMREAD_COLOR);

	// whiten background
	cv::Scalar minHSVthr(24, 31, 250), maxHSVthr(35, 50, 255);
    ImageProcessor::whitenBackground(mRawImage, minHSVthr, maxHSVthr, mPreprocessedImages);

    // crop frame
    ImageProcessor::cropToTextFrame(mPreprocessedImages[0], mPreprocessedImages);

    // additionnal crop if required
    if (std::find(pageIndexRequireAdditionnalCropLeft.cbegin(), pageIndexRequireAdditionnalCropLeft.cend(), mPageNumber) != pageIndexRequireAdditionnalCropLeft.cend()) {
    	ImageProcessor::additionnalCrop(mPreprocessedImages[1], "left", "resources/template_match/left-border.jpg", mPreprocessedImages);
    }
    if (std::find(pageIndexRequireAdditionnalCropRight.cbegin(), pageIndexRequireAdditionnalCropRight.cend(), mPageNumber) != pageIndexRequireAdditionnalCropRight.cend()) {
    	ImageProcessor::additionnalCrop(mPreprocessedImages[1], "right", "resources/template_match/right-border.jpg", mPreprocessedImages);
    }

    // image to be displayed (only copy reference)
    mProcessedImage = mPreprocessedImages.back();

}

void Page::analyzeImage() {
    ImageAnalyzer::findSymbolInImage(mProcessedImage, ImageAnalyzer::tplsMatchSymbol, mMatchSymbolResult, true);
}

void Page::buildRect() {
    std::vector<std::string> splited_by_line, splited_by_space;
    utils::split_string(mMatchSymbolResult, "\n", splited_by_line);

    for (size_t indS = 0; indS < splited_by_line.size(); ++indS) {

        splited_by_space.clear();
        utils::split_string(splited_by_line[indS], " ", splited_by_space);
        if(splited_by_space.size() == 6) {
            cv::Rect rect(cv::Point( std::stoi(splited_by_space[1]), std::stoi(splited_by_space[2]) ), 
                          cv::Point( std::stoi(splited_by_space[3]), std::stoi(splited_by_space[4]) ));

            mMatchSymbols.push_back({rect,splited_by_space[5]});
        }
    }
}

void Page::sortSymbolRects() {
    //std::cout << (mMatchSymbols[0].rect.tl().y < mMatchSymbols[1].rect.tl().y) << std::endl;
    std::sort(mMatchSymbols.begin(), mMatchSymbols.end());
}






