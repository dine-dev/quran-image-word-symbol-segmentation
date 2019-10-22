#include "ImageAnalyzer.h"

bool ImageAnalyzer::rectangleHasIntersection (const cv::Rect & rect_lhs, const cv::Rect & rect_rhs) {
    return ((rect_lhs & rect_rhs).area() > 0);
}


void ImageAnalyzer::findSymbolInImage(const std::string & pathImageQuranPage, const std::list<ImageAnalyzer::MatchTemplate> & pathTplEndOfAyahSymbol, const std::string & pathMatchCoordinate, bool showIntermediate) {

    cv::Mat3b img = cv::imread(pathImageQuranPage, cv::IMREAD_COLOR), img_disp = img.clone();
    std::vector<cv::Rect> matchSymbolRectVector;
    std::vector<std::string> matchSymbolTypeVector;

    for (std::list<ImageAnalyzer::MatchTemplate>::const_iterator itTpl = pathTplEndOfAyahSymbol.cbegin(); itTpl != pathTplEndOfAyahSymbol.cend(); ++itTpl){

        std::string pathEndOfAyahSymbol = itTpl->filePath;

        cv::Mat3b templ = cv::imread(pathEndOfAyahSymbol, cv::IMREAD_COLOR);
        if( img.size().height < templ.size().height || img.size().width < templ.size().width) {
            continue;
        }

        cv::Mat1f result;
        cv::matchTemplate(img, templ, result, cv::TM_CCOEFF_NORMED);

        cv::threshold(result, result, itTpl->threshold, 1., cv::THRESH_BINARY);

        cv::Mat1b resb;
        result.convertTo(resb, CV_8U, 255);

        std::vector<std::vector<cv::Point>> contours;
        cv::findContours(resb, contours, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE);

        for (int indc = 0; indc < contours.size(); ++indc) {
            cv::Mat1b mask(result.rows, result.cols, uchar(0));
            cv::drawContours(mask, contours, indc, cv::Scalar(255), cv::FILLED);

            cv::Point max_point;
            double max_val;
            cv::minMaxLoc(result, NULL, &max_val, NULL, &max_point, mask);

            cv::Rect cRect(max_point.x, max_point.y, templ.cols, templ.rows);

            std::vector<cv::Rect>::iterator it = std::find_if(matchSymbolRectVector.begin(), matchSymbolRectVector.end(),
                [=] (const cv::Rect & rect) { return rectangleHasIntersection(cRect,rect); } );

            // if retangle has no intersection with existing rectangles in vector then add to vector
            if (it == matchSymbolRectVector.end() || itTpl->canOnlyFoundOnce) {
                matchSymbolRectVector.push_back(cRect);
                matchSymbolTypeVector.push_back(itTpl->type);
            }
            else if (!itTpl->canOnlyFoundOnce) {
                (*it) = (*it) & cRect;
            }

        }

    }

    if(!pathMatchCoordinate.empty()) {

        std::ofstream outfile;
        outfile.open(pathMatchCoordinate, std::ios_base::app);

        for (int indr = 0; indr < matchSymbolRectVector.size(); ++indr) {
            //std::cout
            outfile << utils::getFileNameWithoutExtension(pathImageQuranPage) << " "
            << indr << " "
            << matchSymbolRectVector[indr].tl().x << " "
            << matchSymbolRectVector[indr].tl().y << " "
            << matchSymbolRectVector[indr].br().x << " "
            << matchSymbolRectVector[indr].br().y << " "
            << matchSymbolTypeVector[indr] << std::endl;

        }

    }

    if (showIntermediate) {
        for (int indr = 0; indr < matchSymbolRectVector.size(); ++indr) {
            cv::rectangle(img_disp, matchSymbolRectVector[indr], cv::Scalar(0,0,255), 3);

        }
        cv::namedWindow("Result window", cv::WINDOW_AUTOSIZE);
        cv::imshow( "Result window", img_disp);
        cv::waitKey(0);
    }

}


void ImageAnalyzer::segmentWord(const std::string & pathImageQuranPage, const std::string & pathMatchCoordinate) {

    cv::Mat3b img = cv::imread(pathImageQuranPage, cv::IMREAD_COLOR), img_disp = img.clone();
    //cv::rectangle(img_disp, cv::Point(481,83), cv::Point(523,137), cv::Scalar( 255, 0, 0 ), 1);
    //cv::rectangle(img_disp, cv::Point(18,23), cv::Point(623,80), cv::Scalar( 255, 0, 0 ), 1);

    cv::Mat mask = cv::Mat::zeros(img.rows, img.cols, CV_8UC1);
    cv::Point pts[6] = {
        cv::Point(18, 23),
        cv::Point(623, 23),
        cv::Point(623, 137),
        cv::Point(523, 137),
        cv::Point(523, 80),
        cv::Point(18, 80)
    };

    cv::fillConvexPoly( mask, pts, 6, cv::Scalar(255) );
    
    cv::Mat imageDest = cv::Mat::zeros(img.rows, img.cols, CV_8UC3);
    imageDest.setTo(cv::Scalar(255,255,255));
    img.copyTo(imageDest, mask); 
    cv::imshow("mask",mask);
    cv::imshow("imageDest",imageDest);
    cv::waitKey(0);
}





