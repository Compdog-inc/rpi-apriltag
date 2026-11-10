#include <iostream>
#include <iomanip>

#include <opencv2/opencv.hpp>

extern "C"
{
#include "apriltag.h"
#include "tagCircle21h7.h"
}

using namespace std;
using namespace cv;

int main(int argc, char *argv[])
{
    cout << "Enabling video capture" << endl;

    TickMeter meter;
    meter.start();

    // Initialize camera

    std::string pipeline =
        "v4l2src device=/dev/video0 "
        "extra-controls=s,auto_exposure=1,exposure_time_absolute=10,backlight_compensation=2,brightness=0,gain=30 ! "
        "image/jpeg,width=640,height=480,framerate=120/1 ! "
        "jpegdec ! "
        "videoconvert ! "
        "video/x-raw,format=GRAY8 ! "
        "appsink max-buffers=1 drop=true sync=false";

    VideoCapture cap(pipeline, cv::CAP_GSTREAMER);
    if (!cap.isOpened())
    {
        cerr << "Couldn't open video capture device" << endl;
        return -1;
    }

    // Initialize tag detector
    apriltag_family_t *tf = tagCircle21h7_create();
    apriltag_detector_t *td = apriltag_detector_create();
    apriltag_detector_add_family_bits(td, tf, 0);

    if (errno == ENOMEM)
    {
        printf("Unable to add family to detector due to insufficient memory to allocate the tag-family decoder with a maximum hamming value of 0.\n");
        exit(-1);
    }

    td->quad_decimate = 1.0;
    td->quad_sigma = 0.0;
    td->nthreads = 4;
    td->debug = false;
    td->refine_edges = true;

    meter.stop();
    cout << "Detector initialized in "
         << std::fixed << std::setprecision(3) << meter.getTimeSec() << " seconds" << endl;
#if CV_MAJOR_VERSION > 3
    cout << "  " << cap.get(CAP_PROP_FRAME_WIDTH) << "x" << cap.get(CAP_PROP_FRAME_HEIGHT) << " @" << cap.get(CAP_PROP_FPS) << "FPS" << endl;
#else
    cout << "  " << cap.get(CV_CAP_PROP_FRAME_WIDTH) << "x" << cap.get(CV_CAP_PROP_FRAME_HEIGHT) << " @" << cap.get(CV_CAP_PROP_FPS) << "FPS" << endl;
#endif
    meter.reset();

    Mat gray;
    while (true)
    {
        errno = 0;
        cap >> gray;

        // Make an image_u8_t header for the Mat data
        image_u8_t im = {gray.cols, gray.rows, gray.cols, gray.data};

        zarray_t *detections = apriltag_detector_detect(td, &im);

        if (errno == EAGAIN)
        {
            printf("Unable to create the %d threads requested.\n", td->nthreads);
            exit(-1);
        }

        // Draw detection outlines
        for (int i = 0; i < zarray_size(detections); i++)
        {
            apriltag_detection_t *det;
            zarray_get(detections, i, &det);

            if (det->decision_margin < 30)
                continue;

            line(gray, Point(det->p[0][0], det->p[0][1]),
                 Point(det->p[1][0], det->p[1][1]),
                 Scalar(0, 0xff, 0), 2);
            line(gray, Point(det->p[0][0], det->p[0][1]),
                 Point(det->p[3][0], det->p[3][1]),
                 Scalar(0, 0, 0xff), 2);
            line(gray, Point(det->p[1][0], det->p[1][1]),
                 Point(det->p[2][0], det->p[2][1]),
                 Scalar(0xff, 0, 0), 2);
            line(gray, Point(det->p[2][0], det->p[2][1]),
                 Point(det->p[3][0], det->p[3][1]),
                 Scalar(0xff, 0, 0), 2);

            stringstream ss;
            ss << det->id;
            String text = ss.str();
            int fontface = FONT_HERSHEY_SCRIPT_SIMPLEX;
            double fontscale = 1.0;
            int baseline;
            Size textsize = getTextSize(text, fontface, fontscale, 2,
                                        &baseline);
            putText(gray, text, Point(det->c[0] - textsize.width / 2, det->c[1] + textsize.height / 2),
                    fontface, fontscale, Scalar(0xff, 0x99, 0), 2);
        }
        apriltag_detections_destroy(detections);

        imshow("Tag Detections", gray);
        if (waitKey(1) >= 0)
            break;
    }

    apriltag_detector_destroy(td);
    tagCircle21h7_destroy(tf);

    return 0;
}