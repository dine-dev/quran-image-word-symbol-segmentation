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

        for (int indr = 0; indr < matchSymbolRectVector.size(); ++indr) {
            std::cout << pathImageQuranPage << " "
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
        cv::namedWindow("Result window", cv::WINDOW_NORMAL);
        cv::resizeWindow("Result window", 600,1000);
        cv::imshow( "Result window", img_disp);
        cv::waitKey(0);
    }

}

