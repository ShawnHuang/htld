#include "Main.h"

#include "ImAcq.h"
#include "Gui.h"
#include "TLDUtil.h"
#include "Trajectory.h"

using namespace tld;
using namespace cv;


Main::Main()
{
    //tld constructor
    threshold = 0.5;
    tld = new tld::TLD();
    cap = new cv::VideoCapture;
    cap->open("../Project/resources/three.mp4");

    showOutput = 1;

    initialBB = NULL;

}

Main::~Main()
{
    // tld deconstructor
    delete tld;
    delete cap;
}

void Main::doWork()
{
    cv::Mat frame,grey;

    bool reuseFrameOnce = true;
    bool skipProcessingOnce = true;

    while(cap->isOpened())
    {
        // processimage 
        if(reuseFrameOnce)
        {
            //cvReleaseImage(&img);
            //img = imAcqGetImg(imAcq);
            *cap>>frame;

            cvtColor(frame, grey, CV_BGR2GRAY);

            tld->detectorCascade->imgWidth     = grey.cols;
            tld->detectorCascade->imgHeight    = grey.rows;
            tld->detectorCascade->imgWidthStep = grey.step;

            tld->imBlurred                     = new cv::Mat(grey.rows, grey.cols, CV_8UC1);//To Allocate it Once...
            tld->ppHolder                      = new cv::Mat(grey.rows, grey.cols, CV_8UC1);//To Allocate it Once...
        #ifdef USE_HTLD
            //Initialize H-TLD...
            tld->hTLDMaster                = createHETLDMasterModule(grey.cols, grey.rows, 2.0, true, true);
            tld->detectorCascade->fastDet  = tld->hTLDMaster->getFastDet();
            tld->medianFlowTracker->fastTr = tld->hTLDMaster->getFastTr();
            tld->memMgr                    = tld->hTLDMaster->getMemModule();
        #endif

            initialBB = new int[4];
            initialBB[0] = 100;
            initialBB[1] = 0;
            initialBB[2] = 200;
            initialBB[3] = 200;

            Rect bb = tldArrayToRect(initialBB);
            tld->selectObject(grey, &bb);

            reuseFrameOnce = false;
        }
        else{
            *cap>>frame;

            cvtColor(frame, grey, CV_BGR2GRAY);

            double tic = cvGetTickCount();
            tld->processImage(frame);
            double toc = (cvGetTickCount() - tic) / cvGetTickFrequency();
            toc = toc / 1000000;

            float fps = 1 / toc;
        }

        
        if(showOutput)
        {
            CvScalar yellow = CV_RGB(255, 255, 0);
            CvScalar blue = CV_RGB(0, 0, 255);
            CvScalar black = CV_RGB(0, 0, 0);
            CvScalar white = CV_RGB(255, 255, 255);

            if(tld->currBB != NULL)
            {
                cv::Scalar rectangleColor = tld->isConf() ? blue : yellow;
                rectangle(frame,*tld->currBB, rectangleColor);
            }

            if(showOutput)
            {
                imshow("test", frame);
                char key = waitKey(10);

                if(key == 'q') break;

                if(key == 'c')
                {
                    //clear everything
                    tld->release();
                }

                //if(key == 'l')
                //{
                //    tld->learningEnabled = !tld->learningEnabled;
                //    printf("LearningEnabled: %d\n", tld->learningEnabled);
                //}

                //if(key == 'a')
                //{
                //    tld->alternating = !tld->alternating;
                //    printf("alternating: %d\n", tld->alternating);
                //}
            }

        }

    }//End of while-Loop...

    //cvReleaseImage(&img);
    //img = NULL;


	//resetOutputStream();
    #ifdef USE_HTLD
      //Destroy H-TLD...
      destroyHETLDMasterModule(tld->hTLDMaster);
    #endif
}
