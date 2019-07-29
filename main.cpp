#include <opencv2/opencv.hpp>
#include <iostream>
#include<string>
#include <Driver/RMVideoCapture.hpp>
using namespace cv;
using namespace std;
RNG g_rng(12345);//生成随机数，用于生成随机颜色
bool drawRect = false;
Point LeftPnt=Point(-1,-1);
Point mousePos= Point(-1, -1);
Mat cameraMatrix;
Mat distCoeffs;
//int flag=0;
void calAngle(Mat cam,Mat dis,int x,int y)
{

    double fx=cam.at<double>(0,0);
    double fy=cam.at<double>(1,1);
    double cx=cam.at<double>(0,2);
    double cy=cam.at<double>(1,2);
    double k1=dis.at<double>(0);
    double k2=dis.at<double>(1);
    double p1=dis.at<double>(2);
    double p2=dis.at<double>(3);
    Point2f pnt;
    vector<cv::Point2f>in;
    vector<cv::Point2f>out;
    in.push_back(Point2f(x,y));
    //对像素点去畸变
    undistortPoints(in,out,cam,dis,noArray(),cam);
    pnt=out.front();
    double rx=(pnt.x-cx)/fx;
    double ry=(pnt.y-cy)/fy;

    double tanx=(rx);
    double tany=(ry);
    cout <<out<<endl;

    cout<< "xscreen: "<<x<<" xNew:"<<pnt.x<<endl;
    cout<< "yscreen: "<<y<<" yNew:"<<pnt.y<<endl;
    cout<< "angx: "<<atan((x-cx)/fx)/CV_PI*180<<" angleNew:"<<atan(rx)/CV_PI*180<<endl;
    cout<< "angy: "<<atan((y-cy)/fy)/CV_PI*180<<" angleNew:"<<atan(ry)/CV_PI*180<<endl;
}

void on_mouse(int event, int x, int y, int flags, void *ustc)
//event鼠标事件代号，x,y鼠标坐标，flags拖拽和键盘操作的代号
{
//    flag=1;
    Mat& image = *(cv::Mat*) ustc;//这样就可以传递Mat信息了，很机智
    char temp[16];
    switch (event) {
    case CV_EVENT_LBUTTONDOWN://按下左键
    {
        sprintf(temp, "(%d,%d)", x, y);
        putText(image, temp, Point(x, y), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0, 255));
        drawRect = true;
        LeftPnt= Point(x, y);
        calAngle(cameraMatrix,distCoeffs,x,y);
    }	break;
    case CV_EVENT_MOUSEMOVE://移动鼠标
    {
        mousePos = Point(x, y);
        if (drawRect)
        { }
    }break;
    case EVENT_LBUTTONUP:
    {
        drawRect = false;
        sprintf(temp, "(%d,%d)", x, y);
        putText(image, temp, Point(x, y), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0, 255));
        //调用函数进行绘制
        cv::rectangle(image,LeftPnt, mousePos, cv::Scalar(g_rng.uniform(0, 255), g_rng.uniform(0, 255), g_rng.uniform(0, 255)));//随机颜色
    }break;
}
}

int main(int argc, char *argv[])
{
    Mat img;
    RMVideoCapture cap;
    cap.open(0);
    cap.setVideoFormat(1920,1080);

    FileStorage fs("./out_camera_data.xml", FileStorage::READ);
    fs["camera_matrix"] >> cameraMatrix;
    fs["distortion_coefficients"] >> distCoeffs;
    cap.startStream();
    Mat org ,temp1,temp2;
    while (waitKey(30) != 'q') {
        if(!cap.grab())continue;
        cap.retrieve( org);
    org.copyTo(temp1);//用来显示点的坐标以及临时的方框
    namedWindow("img");//定义一个img窗口
    setMouseCallback("img", on_mouse, (void*)&org);//调用回调函数
//    if(flag==1)
//    {
//        imwrite("test.jpg",org);
//        flag=0;
//    }
    if(drawRect)rectangle(temp1, LeftPnt, mousePos, cv::Scalar(g_rng.uniform(0, 255), g_rng.uniform(0, 255), g_rng.uniform(0, 255)));//随机颜色
    putText(temp1,"("+std::to_string(mousePos.x)+","+std::to_string(mousePos.y)+")" , mousePos, FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0, 255));
    imshow("img",temp1);
    }
}
