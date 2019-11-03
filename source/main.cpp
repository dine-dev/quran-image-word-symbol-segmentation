#include "Utils.hpp"
#include "Mushaf.hpp"
#include "Page.hpp"
#include "ImageProcessor.hpp"
#include "ImageAnalyzer.hpp"


int main(int argc, char ** argv) {
	// call a function in another file
	Mushaf m("resources/hafs", Mushaf::Riwaya::hafs);

    if (!m.checkResources()) {
        return -1;
    }

    bool showIntermediate = true;

    std::list<int> pageList(604);
    std::iota(pageList.begin(), pageList.end(), 1);

    if (0){
        std::list<int> pageIndexRequireAdditionnalCropLeft({26, 64, 92, 268, 344});
        std::list<int> pageIndexRequireAdditionnalCropRight({77, 251});
    // Make iterate point to begining and increment it one by one till it reaches the end of list.
        for (std::list<int>::iterator it = pageList.begin(); it != pageList.end(); ++it){

        //std::cout << (*it) << std::endl;

            cv::Scalar minHSVthr(24, 31, 250), maxHSVthr(35, 50, 255);
            ImageProcessor::whitenBackground("resources/hafs/"+ std::to_string(*it) +".jpg", minHSVthr, maxHSVthr, showIntermediate, "resources/hafs_whitened/"+ std::to_string(*it) +".jpg");

            ImageProcessor::cropToTextFrame("resources/hafs_whitened/"+ std::to_string(*it) +".jpg", showIntermediate, "resources/hafs_whitened_croped/"+ std::to_string(*it) +".jpg");

            if (std::find(pageIndexRequireAdditionnalCropLeft.cbegin(), pageIndexRequireAdditionnalCropLeft.cend(), (*it)) != pageIndexRequireAdditionnalCropLeft.cend()) {
                ImageProcessor::additionnalCrop("resources/hafs_whitened_croped/"+ std::to_string(*it) +".jpg", "left", "resources/template_match/left-border.jpg", showIntermediate);
            }
            else if (std::find(pageIndexRequireAdditionnalCropRight.cbegin(), pageIndexRequireAdditionnalCropRight.cend(), (*it)) != pageIndexRequireAdditionnalCropRight.cend()) {
                ImageProcessor::additionnalCrop("resources/hafs_whitened_croped/"+ std::to_string(*it) +".jpg", "right", "resources/template_match/right-border.jpg", showIntermediate);
            }
        }
    }

    if(0) {

        for (std::list<int>::iterator it = pageList.begin(); it != pageList.end(); ++it){
            ImageAnalyzer::findSymbolInImage("resources/hafs_whitened_croped/"+ std::to_string(*it) +".jpg", ImageAnalyzer::tplsMatchSymbol, "", showIntermediate);
        }
    }

    if(1) {
        //pageList = {370, 445, 498, 521, 564, 582, 585, 592, 603};
        for (std::list<int>::iterator it = pageList.begin(); it != pageList.end(); ++it){
            std::cout << "page " << (*it) << std::endl;
            Page page((*it), "resources/hafs/"+ std::to_string(*it) + ".jpg");
            //page.showQuranPage(false);
            page.showSymbols();

            cv::waitKey(0);
        }
        
    }
    
    return 0;
}
