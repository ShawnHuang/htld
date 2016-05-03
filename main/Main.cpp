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

#include <opencv2/core/core.hpp>
#include "Main.h"

#include "ImAcq.h"
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
    imAcq = NULL;
}

Main::~Main()
{
    //delete tld;
    imAcqFree(imAcq);
}

void Main::doWork()
{

    tld::TLD *tld = new tld::TLD();
    tld::TLD *tld2 = new tld::TLD();
    tld::TLD *tld3 = new tld::TLD();
    //IplImage *img;
    cv::Mat img,grey;
    cv::VideoCapture cap("../Project/resources/three.mp4");
    //while(imAcqHasMoreFrames(imAcq))
    int frame =0;
    while(cap.isOpened())
    {
        double t = (double)getTickCount();  
        //img = imAcqGetImg(imAcq);
        printf("frame : %d\n", frame);
        if (frame==22)
        {
           CvRect box;
           box.x = 192;
           box.y = 51;
           box.width = 78;
           box.height = 76;
           Rect r = Rect(box);
           printf("Starting at %d %d %d %d\n", r.x, r.y, r.width, r.height);
           if(tld->currBB == NULL)
             tld->selectObject(grey, &r);
           else if(tld2->currBB == NULL)
             tld2->selectObject(grey, &r);
           else
             tld3->selectObject(grey, &r);
        }
        frame++;
        cap>>img;
        resize(img,img, cv::Size(320, 180));
        //Mat grey(img->height, img->width, CV_8UC1);
        cvtColor(img, grey, CV_BGR2GRAY);


        tld->process(grey);
        tld2->process(grey);
        tld3->process(grey);

		int confident = (tld->currConf >= threshold) ? 1 : 0;

        if(showOutput)
        {

            CvScalar yellow = CV_RGB(255, 255, 0);
            CvScalar blue = CV_RGB(0, 0, 255);
            CvScalar black = CV_RGB(0, 0, 0);
            CvScalar white = CV_RGB(255, 255, 255);

            if(tld->currBB != NULL)
            {
                CvScalar rectangleColor = (confident) ? blue : yellow;
                //cvRectangle(img, tld->currBB->tl(), tld->currBB->br(), rectangleColor, 8, 8, 0);
                cv::rectangle(img, tld->currBB->tl(), tld->currBB->br(), rectangleColor, 8, 8, 0);
            }
            if(tld2->currBB != NULL)
            {
                CvScalar rectangleColor = (confident) ? blue : yellow;
                //cvRectangle(img, tld->currBB->tl(), tld->currBB->br(), rectangleColor, 8, 8, 0);
                cv::rectangle(img, tld2->currBB->tl(), tld2->currBB->br(), rectangleColor, 8, 8, 0);
            }
            if(tld3->currBB != NULL)
            {
                CvScalar rectangleColor = (confident) ? blue : yellow;
                //cvRectangle(img, tld->currBB->tl(), tld->currBB->br(), rectangleColor, 8, 8, 0);
                cv::rectangle(img, tld3->currBB->tl(), tld3->currBB->br(), rectangleColor, 8, 8, 0);
            }
            if(showOutput)
            {
                gui->showImage(&img);
                char key = gui->getKey();

                if(key == 'q') break;

                if(key == 'c')
                {
                    //clear everything
                    tld->release();
                }
                if(key == 'r')
                {

                    //if(getBBFromUser(&img, box, gui) == PROGRAM_EXIT)
                    //{
                    //    break;
                    //}

                    CvRect box;
                    box.x = 192;
                    box.y = 51;
                    box.width = 78;
                    box.height = 76;
                    Rect r = Rect(box);
                    printf("Starting at %d %d %d %d\n", r.x, r.y, r.width, r.height);
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
      //cvReleaseImage(&img);
      //img = NULL;
    }//End of while-Loop...

}
