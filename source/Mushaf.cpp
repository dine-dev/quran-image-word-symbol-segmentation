#include "Mushaf.h"

Mushaf::Mushaf(std::string imageResourceDirectory, Mushaf::Riwaya riwaya)
: mImageResourceDirectory(imageResourceDirectory), mRiwaya(riwaya) {}

Mushaf::~Mushaf(){}

bool Mushaf::checkResources() {
    // TODO check good number of pages

    switch(mRiwaya) {
        case hafs:
     return utils::dirExists(mImageResourceDirectory);
     case warsh:
     return false;
     default:
     return false;
 }

}