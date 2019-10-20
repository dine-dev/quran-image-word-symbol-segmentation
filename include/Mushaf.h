#ifndef MUSHAF_H
#define MUSHAF_H

#include "Utils.h"

class Mushaf {
public:
    enum Riwaya {unknown = 0, hafs, warsh};
    Mushaf(std::string imageResourceDirectory, Riwaya riwaya);
    ~Mushaf();
    bool checkResources();
private:
    std::string mImageResourceDirectory;
    Riwaya mRiwaya;

};

#endif
