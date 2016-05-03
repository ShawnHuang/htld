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
 * MainX.cpp
 *
 *  Created on: Nov 17, 2011
 *      Author: Georg Nebehay
 */

#include "Main.h"

//#include "ImAcq.h"
#include "Gui.h"
#include "TLDUtil.h"
//#include "Trajectory.h"

using namespace tld;
using namespace cv;


Main::Main()
{
    showOutput = 1;
    threshold = 0.5;

	trajectoryLength = 0;

    reinit = 0;

    seed = 0;

    gui = NULL;
    modelPath = NULL;
    //imAcq = NULL;
}

Main::~Main()
{
    //delete tld;
    //imAcqFree(imAcq);
}

void Main::doWork()
{
    tld::TLD *tld = new tld::TLD();
    tld::TLD *tld2 = new tld::TLD();
    tld::TLD *tld3 = new tld::TLD();
    //CvCapture* cap = cvCaptureFromCAM(0);
    CvCapture* cap; 
    //cap = cvCreateCameraCapture(0);
    cap = cvCaptureFromAVI("../Project/resources/three.mp4");

    IplImage *img;
    while(cap)
    {
        double t = (double)getTickCount();

        img = cvQueryFrame(cap);
        //Mat grey(img->height, img->width, CV_8UC1);
        Mat output,grey;
        output = cvarrToMat(img);
        resize(output, output, Size(320,160));
        cvtColor(output, grey, CV_BGR2GRAY);

        tld->process(grey);
        tld2->process(grey);
        tld3->process(grey);


        if(showOutput)
        {
            cv::Scalar blue = cv::Scalar(0, 0, 255);

            if(tld->currBB != NULL)
            {
                cv::rectangle(output, tld->currBB->tl(), tld->currBB->br(), blue, 8, 8, 0);
            }
            if(tld2->currBB != NULL)
            {
                cv::rectangle(output, tld2->currBB->tl(), tld2->currBB->br(), blue, 8, 8, 0);
            }
            if(tld3->currBB != NULL)
            {
                cv::rectangle(output, tld3->currBB->tl(), tld3->currBB->br(), blue, 8, 8, 0);
            }
            if(showOutput)
            {
                gui->showImage(output);
                char key = gui->getKey();
                //imshow("tld", output);
                //char key = cv::waitKey(10);

                if(key == 'q') break;

                if(key == 'c')
                {
                    //clear everything
                    tld->release();
                    tld2->release();
                    tld3->release();
                }
                if(key == 'r')
                {
                    CvRect box;

                    if(getBBFromUser(output, box, gui) == PROGRAM_EXIT)
                    {
                        break;
                    }

                    Rect r = Rect(box);

                    if(tld->currBB == NULL)
                    tld->selectObject(grey, &r);
                    else if(tld2->currBB == NULL)
                    tld2->selectObject(grey, &r);
                    else
                    tld3->selectObject(grey, &r);
                }
            }

        }
        t=((double)getTickCount()-t)/((double)cvGetTickFrequency()*1000.);  
        std::cout<<t<<"  ms     "<<1000/t<<"  fps"<<std::endl;
    }//End of while-Loop...
    cvReleaseCapture(&cap);

  delete tld;
  delete tld2;
  delete tld3;
	//resetOutputStream();
#ifdef USE_HTLD
	//Destroy H-TLD...
	destroyHETLDMasterModule(tld->hTLDMaster);
#endif
}
