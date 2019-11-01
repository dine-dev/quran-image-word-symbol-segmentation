#include "ImageAnalyzer.h"

const std::list<ImageAnalyzer::MatchTemplate>  ImageAnalyzer::tplsMatchSymbol = {
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
            outfile << /*utils::getFileNameWithoutExtension(*/pathImageQuranPage/*)*/ << " "
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
    }

}

void ImageAnalyzer::findSymbolInImage(const cv::Mat & imageQuranPage, const std::list<ImageAnalyzer::MatchTemplate> & pathTplMatchSymbols, std::string & matchCoordinateResult, bool showIntermediate) {

    cv::Mat3b img_disp = imageQuranPage.clone();
    std::vector<cv::Rect> matchSymbolRectVector;
    std::vector<std::string> matchSymbolTypeVector;

    for (std::list<ImageAnalyzer::MatchTemplate>::const_iterator itTpl = pathTplMatchSymbols.cbegin(); itTpl != pathTplMatchSymbols.cend(); ++itTpl){

        std::string pathEndOfAyahSymbol = itTpl->filePath;

        cv::Mat3b templ = cv::imread(pathEndOfAyahSymbol, cv::IMREAD_COLOR);
        if( img_disp.size().height < templ.size().height || img_disp.size().width < templ.size().width) {
            continue;
        }

        cv::Mat1f result;
        cv::matchTemplate(img_disp, templ, result, cv::TM_CCOEFF_NORMED);

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

    // output symbole rectangle
    std::stringstream buffer;
    for (int indr = 0; indr < matchSymbolRectVector.size(); ++indr) {
        buffer << indr << " "
        << matchSymbolRectVector[indr].tl().x << " "
        << matchSymbolRectVector[indr].tl().y << " "
        << matchSymbolRectVector[indr].br().x << " "
        << matchSymbolRectVector[indr].br().y << " "
        << matchSymbolTypeVector[indr] << "\n";
    }

    matchCoordinateResult = buffer.str();

}


void ImageAnalyzer::segmentWordInPage(const std::string & pathImageQuranPage, const std::string & pathMatchCoordinate) {

    
}


void ImageAnalyzer::getPolygonFromAyahSymbolPos(const std::string & pathImageQuranPage, const std::vector<cv::Point> polygon) {

}

void ImageAnalyzer::segmentWordInPolygon(const std::string & pathImageQuranPage, const std::vector<cv::Point> & polygon) {
    cv::Mat3b img = cv::imread(pathImageQuranPage, cv::IMREAD_COLOR), img_disp = img.clone();
    //cv::rectangle(img_disp, cv::Point(481,83), cv::Point(523,137), cv::Scalar( 255, 0, 0 ), 1);
    //cv::rectangle(img_disp, cv::Point(18,23), cv::Point(623,80), cv::Scalar( 255, 0, 0 ), 1);

    cv::Mat mask = cv::Mat::zeros(img.rows, img.cols, CV_8UC1);
    std::vector<cv::Point> pts = {
        cv::Point(18, 23),
        cv::Point(623, 23),
        cv::Point(623, 137),
        cv::Point(523, 137),
        cv::Point(523, 80),
        cv::Point(18, 80)
    };

    cv::fillConvexPoly( mask, pts.data(), pts.size(), cv::Scalar(255) );
    
    cv::Mat imageDest = cv::Mat::zeros(img.rows, img.cols, CV_8UC3);
    imageDest.setTo(cv::Scalar(255,255,255));
    img.copyTo(imageDest, mask); 
    
    //cv::imshow("mask",mask);
    //cv::imshow("imageDest",imageDest);

    cv::Mat src_gray_thr;
    cv::cvtColor(imageDest, src_gray_thr, cv::COLOR_BGR2GRAY);

    cv::threshold( src_gray_thr, src_gray_thr, 240, 255, cv::THRESH_BINARY);
    
    // invert black and white
    cv::bitwise_not ( src_gray_thr, src_gray_thr );
    

    cv::Mat kernel = (cv::Mat_<uchar>(9,9) << 0,0,0,0,1,0,0,0,0,
                                              0,0,1,1,1,0,0,0,0,
                                              0,0,0,1,1,0,0,0,0,
                                              0,0,0,0,1,0,0,0,0,
                                              0,0,1,1,1,1,1,0,0,
                                              0,0,0,0,1,0,0,0,0,
                                              0,0,0,0,1,0,0,0,0,
                                              0,0,0,0,1,0,0,0,0,
                                              0,0,0,0,1,0,0,0,0);

    cv::Mat dilation_dst;
    cv::dilate( src_gray_thr, dilation_dst, kernel);

    std::vector<std::vector<cv::Point> > contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(dilation_dst, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE, cv::Point(0, 0));
    std::vector<cv::Rect> contour_rect;

    // get bounding box of contours
    for (size_t idx = 0; idx < contours.size(); idx++) {
        cv::Rect rect = cv::boundingRect(contours[idx]);

        // check intersection of curret rectangle with other present rectangle
        std::vector<cv::Rect>::iterator it = std::find_if(contour_rect.begin(), contour_rect.end(),
                [=] (const cv::Rect & cRect) { return rectangleHasIntersection(cRect,rect); } );
        
        // if curretn rectangle does not have intersection with other rectangles just add it
        if(it == contour_rect.end()){
            contour_rect.push_back(rect);
        }
        // if current rectangle has an intersection with other rectangles 
        else {
            cv::Rect iRect = (*it);

            // if the intersection is exactly rectangle in vector that mean that rect contains the rectangle in vector
            if ( iRect == (iRect & rect) ) {
                (*it) = rect;
            }
            // if the intersection is exactly current rectangle that means that that the rectangle in vector contains rect contain
            else if ( rect == (iRect & rect) ){
                continue;
            }
            else {
                contour_rect.push_back(rect);
            }

        }
        
        // Draw the contours rectangle
        cv::drawContours( img, contours, idx, cv::Scalar(0, 0, 255), 1, 8, hierarchy, 0);
        //cv::rectangle(img, rect.tl(), rect.br(), cv::Scalar(255, 0, 0));
    }

    for (size_t idx = 0; idx < contour_rect.size(); idx++) {
        
        // Draw the contours rectangle
        cv::rectangle(img, contour_rect[idx].tl(), contour_rect[idx].br(), cv::Scalar(255, 0, 0));
    }    
    
    cv::imshow("imageDest",src_gray_thr);
    cv::imshow("dilation_dst",dilation_dst);
    cv::imshow("img",img);
}






