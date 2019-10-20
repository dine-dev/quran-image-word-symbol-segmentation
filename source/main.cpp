#include "Utils.h"
#include "Mushaf.h"
#include "ImageProcessor.h"
#include "ImageAnalyzer.h"


int main(int argc, char ** argv) {
	// call a function in another file
	Mushaf m("resources/hafs", Mushaf::Riwaya::hafs);

    if (!m.checkResources()) {
        return -1;
    }

    ImageProcessor processor;
    bool showIntermediate = true;

    std::list<int> pageList(604);
    std::iota(pageList.begin(), pageList.end(), 1);

    if (0){
        std::list<int> pageIndexRequireAdditionnalCropLeft({26, 64, 92, 268, 344});
        std::list<int> pageIndexRequireAdditionnalCropRight({77, 251});
    // Make iterate point to begining and incerement it one by one till it reaches the end of list.
        for (std::list<int>::iterator it = pageList.begin(); it != pageList.end(); ++it){

        //std::cout << (*it) << std::endl;

            cv::Scalar minHSVthr(24, 31, 250), maxHSVthr(35, 50, 255);
            processor.whitenBackground("resources/hafs/"+ std::to_string(*it) +".jpg", minHSVthr, maxHSVthr, showIntermediate, "resources/hafs_whitened/"+ std::to_string(*it) +".jpg");

            processor.cropToTextFrame("resources/hafs_whitened/"+ std::to_string(*it) +".jpg", showIntermediate, "resources/hafs_whitened_croped/"+ std::to_string(*it) +".jpg");

            if (std::find(pageIndexRequireAdditionnalCropLeft.cbegin(), pageIndexRequireAdditionnalCropLeft.cend(), (*it)) != pageIndexRequireAdditionnalCropLeft.cend()) {
                processor.additionnalCrop("resources/hafs_whitened_croped/"+ std::to_string(*it) +".jpg", "left", "resources/template_match/left-border.jpg", showIntermediate);
            }
            else if (std::find(pageIndexRequireAdditionnalCropRight.cbegin(), pageIndexRequireAdditionnalCropRight.cend(), (*it)) != pageIndexRequireAdditionnalCropRight.cend()) {
                processor.additionnalCrop("resources/hafs_whitened_croped/"+ std::to_string(*it) +".jpg", "right", "resources/template_match/right-border.jpg", showIntermediate);
            }
        }
    }

    ImageAnalyzer analyzer;
    std::list<ImageAnalyzer::MatchTemplate> tplsMatchSymbol({
            {"resources/template_match/end-of-ayah-symbol-first-page.jpg", 0.43, "ayah", false},
            {"resources/template_match/end-of-ayah-symbol-second-page.jpg", 0.43, "ayah", false},
            {"resources/template_match/end-of-ayah-symbol-1.jpg", 0.43, "ayah", false},
            {"resources/template_match/end-of-ayah-symbol-11.jpg", 0.43, "ayah", false},
            {"resources/template_match/end-of-ayah-symbol-88.jpg", 0.43, "ayah", false},
            {"resources/template_match/end-of-ayah-symbol-111.jpg", 0.43, "ayah", false},
            {"resources/template_match/footer-first-page.jpg", 0.7, "footer", true},
            {"resources/template_match/footer-second-page.jpg", 0.7, "footer", true},
            {"resources/template_match/footer.jpg", 0.7, "footer", true},
            {"resources/template_match/header-first-page.jpg", 0.7, "header", true},
            {"resources/template_match/header.jpg", 0.7, "header", true},
            {"resources/template_match/basmalah.jpg", 0.7, "basmalah", true},
            {"resources/template_match/basmalah-second-page.jpg", 0.7, "basmalah", true},
            {"resources/template_match/roubou3.jpg", 0.7, "roubou3", true},
            {"resources/template_match/soujoud.jpg", 0.7, "soujoud", true},
    });


    for (std::list<int>::iterator it = pageList.begin(); it != pageList.end(); ++it){
        analyzer.findSymbolInImage("resources/hafs_whitened_croped/"+ std::to_string(*it) +".jpg", tplsMatchSymbol, "", showIntermediate);
    }

    return 0;
}
