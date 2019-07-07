#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;

int main()
{
    cv::VideoCapture cap(2);

    if (!cap.isOpened()) {
        std::cout<<"Can't open camera"<<std::endl;
        return -1;
    }

    cap.set(CV_CAP_PROP_FRAME_WIDTH, 1280);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT, 720);

    //获取视频帧的宽度
    double width = cap.get(CV_CAP_PROP_FRAME_WIDTH);
    //获取视频帧的高度
    double height = cap.get(CV_CAP_PROP_FRAME_HEIGHT);

    cout << "视频帧尺寸 : "<< width << " x "<< height << endl;

    cv::Mat frame;

    cv::Mat cameraMatrix;// = cv::Mat::zeros(3, 3, CV_64F);
    cv::Mat distCoeffs;// = cv::Mat::zeros(8, 1, CV_64F);

    std::vector<cv::Mat> rvecs, tvecs;
    std::vector<cv::Point2f> corners;
    std::vector< std::vector<cv::Point2f> > corners2;
    std::vector<cv::Point3f> worldPoints;
    std::vector< std::vector<cv::Point3f> > worldPoints2;
    bool finishCalibration = false;
     // Create Object Points
    for (int j = 0; j < 6; j++) {
        for (int k = 0; k < 9; k++) {
            worldPoints.push_back(cv::Point3f(j * 1.0, k * 1.0, 0.0f));
        }
    }
    while(1)
    {

        bool success = cap.read(frame);
        if(!success)
        {
            cout << "不能从视频流中读取帧！"<< endl;
            break;
        }

        cv::imshow("camera", frame);
        if (finishCalibration) {
               cv::Mat undistortImage
               cv::undistort(frame, undistortImage, cameraMatrix, distCoeffs);
               cv::imshow("undistort", undistortImage);
        } else {
            bool foundCoroners = cv::findChessboardCorners(frame, cv::Size(9, 6), corners,
                                                           CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_NORMALIZE_IMAGE);
            if(foundCoroners) {
//            cout << "找到角点" << endl;
//            corners.push_back(cornerFrame);
                cv::drawChessboardCorners(frame, cv::Size(9, 6), corners, foundCoroners);
                cv::imshow("corner_draw", frame);
                corners2.push_back(corners);
                worldPoints2.push_back(worldPoints);
                if(corners2.size() >= 20) {
                    // 标定完成
                    cv::calibrateCamera(worldPoints2, corners2, frame.size(), cameraMatrix, distCoeffs, rvecs, tvecs,
                                        CV_CALIB_FIX_PRINCIPAL_POINT);
                    //*************************************查看参数*****************************************
                    cout << " Camera intrinsic: " << cameraMatrix.rows << "x" << cameraMatrix.cols << endl;
                    cout << cameraMatrix.at<double>(0, 0) << " " << cameraMatrix.at<double>(0, 1) << " "
                         << cameraMatrix.at<double>(0, 2) << endl;
                    cout << cameraMatrix.at<double>(1, 0) << " " << cameraMatrix.at<double>(1, 1) << " "
                         << cameraMatrix.at<double>(1, 2) << endl;
                    cout << cameraMatrix.at<double>(2, 0) << " " << cameraMatrix.at<double>(2, 1) << " "
                         << cameraMatrix.at<double>(2, 2) << endl;
                    cout << distCoeffs.rows << "x" << distCoeffs.cols << endl;
                    cout << distCoeffs << endl;
                    for (int i = 0; i < distCoeffs.cols; i++) {
                        cout << distCoeffs.at<double>(0, i) << " ";
                    }
                    cout << endl;
                    finishCalibration = true;
                }
            }
        }
        if(cv::waitKey(30) == 27)
        {
            cout << "ESC键被按下"<< endl;
            break;
        }
    }
    return 0;
}
