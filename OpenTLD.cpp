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

/**
  * @author Georg Nebehay
  */

//#include "Main.h"
#include "TLDUtil.h"
#include "TLD.h"
//#include "Config.h"
//#include "ImAcq.h"
//#include "Gui.h"

using namespace tld;
using namespace cv;

static CvPoint point;
static cv::Rect *bb;
static int drag = 0;

static void mouseHandler(int event, int x, int y, int flags, void *param)
{
    /* user press left button */
    if(event == CV_EVENT_LBUTTONDOWN && !drag)
    {
        point = cvPoint(x, y);
        drag = 1;
    }

    if(event == CV_EVENT_LBUTTONUP && drag)
    {
        *bb = cvRect(point.x, point.y, x - point.x, y - point.y);
        drag = 0;
    }
}

int main(int argc, char **argv)
{

    tld::TLD *tld = new tld::TLD();
    tld::TLD *tld2 = new tld::TLD();
    tld::TLD *tld3 = new tld::TLD();
    //CvCapture* cap = cvCaptureFromCAM(0);
    CvCapture* cap; 
    //cap = cvCreateCameraCapture(0);
    cap = cvCaptureFromAVI("../Project/resources/three.mp4");

    IplImage *img;
    int count = 0;
    while(cap)
    {
        count ++;
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


        if(true)
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
            if(true)
            {
                //gui->showImage(output);
                //char key = gui->getKey();
                imshow("tld", output);
                char key = cv::waitKey(10);

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
                    cv::Rect box;
                    box = cv::Rect(-1, -1, -1, -1);
                    bb = &box;
                    cvSetMouseCallback("tld", mouseHandler, NULL);
                    cv::waitKey(0);
                    
                    if (box.x != -1)
                    {
                      if(box.width < 0)
                      {
                          box.x += box.width;
                          box.width = abs(box.width);
                      }

                      if(box.height < 0)
                      {
                          box.y += box.height;
                          box.height = abs(box.height);
                      }


                      if(tld->currBB == NULL)
                      tld->selectObject(grey, &box);
                      else if(tld2->currBB == NULL)
                      tld2->selectObject(grey, &box);
                      else
                      tld3->selectObject(grey, &box);
                    }
                }
            }

        }
        t=((double)getTickCount()-t)/((double)cvGetTickFrequency()*1000.);  
        std::cout<<t<<"  ms     "<<1000/t<<"  fps"<<std::endl;
        std::cout<<"frame   "<<count<<std::endl;
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
    return EXIT_SUCCESS;
}
