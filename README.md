# angleMeasure
测量图像中一点到摄像头中心点的角度，程序主要是参照 https://blog.csdn.net/zmdsjtu/article/details/54729448 中的程序修改的，在使用中只要点击图像上的某点在终端上会输出当前点测量得到的角度。

摄像头的内参矩阵需要自行标定，程序会读取out_camera_data.xml文件（opencv自带标定程序标定后输出的内参文件），之后打开摄像头，我使用的摄像头是KS8A17，分辨率为1920*1080。

角度测试图片可以参考test.jpg

