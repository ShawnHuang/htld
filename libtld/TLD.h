/*  Copyright 2011 AIT Austrian Institute of Technology
*
*   This file is part of OpenTLD.
*
*   OpenTLD is free software: you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*    the Free Software Foundation, either version 3 of the License, or
*   (at your option) any later version.
*
*   OpenTLD is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with OpenTLD.  If not, see <http://www.gnu.org/licenses/>.
*
*/

/*
 * TLD.h
 *
 *  Created on: Nov 17, 2011
 *      Author: Georg Nebehay
 */

#ifndef TLD_H_
#define TLD_H_

#include <opencv/cv.h>

#include "TLDGlobals.h"//Declare First for Compiler Options...
//#ifdef USE_HTLD
	#include "hetld_api.hpp"
//#endif
#include "MedianFlowTracker.h"
#include "DetectorCascade.h"

namespace tld
{

class TLD
{
    void storeCurrentData();
    void fuseHypotheses();
    void learn();
    void initialLearning();
public:
    bool trackerEnabled;
    bool detectorEnabled;
    bool learningEnabled;
    bool alternating;

#ifdef USE_HTLD
	Master *hTLDMaster;
	MemoryManagement *memMgr;
	int tldObjId;
#endif
    MedianFlowTracker *medianFlowTracker;
    DetectorCascade *detectorCascade;
    NNClassifier *nnClassifier;
    bool valid;
    bool wasValid;
    cv::Mat prevImg;
    cv::Mat currImg;
    cv::Rect *prevBB;
    cv::Rect *currBB;
    float currConf;
    float threshold;
    bool learning;

	//*************for Image Warping...************
	cv::RNG rng;
	cv::Mat *imBlurred;
	cv::Mat *ppHolder;
#ifndef USE_HTLD
	cv::Ptr<cv::FilterEngine> gaussianFilter;
#endif
	int *hull;//minX, minY, maxX, maxY...

    TLD();
    virtual ~TLD();
    void release();
    bool isConf();
    void selectObject(const cv::Mat &img, cv::Rect *bb);
    void processImage(const cv::Mat &img);
    void writeToFile(const char *path);
    void readFromFile(const char *path);
};

} /* namespace tld */
#endif /* TLD_H_ */
