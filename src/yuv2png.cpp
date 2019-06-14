#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include "opencv2/imgproc.hpp"
#include "opencv2/calib3d.hpp"
#include <opencv2/highgui.hpp>
#include <string>
#include <vector>
#include <fstream>

using namespace std;
using namespace cv;

Mat	 distCoeffs=(Mat1d(1, 5) << -0.3426, 0.1539, 0.0, 0.0, -0.0410);
Mat  cameraMatrix= ( Mat_<double> ( 3,3 ) << 326.1175, 0, 316.1840, 0, 326.2172, 226.4782, 0, 0, 1 );
Size imageSize(640,360);

void load_all_images(const string &path,vector<Mat>&images);
void load_all_yuvs(const string & filepath,vector<Mat> &Images);

int  main(int argc,char* argv[])
{
    cout<<"hello world"<<endl;
    string path=argv[1];
    vector<Mat> images;
    load_all_yuvs(path,images);
    // load_all_images(path,images);
    waitKey();
    // imwrite("11.png",img);
    // Mat img;
    // string imgpath = argv[1];  
    // img = imread(imgpath, 0); 
    // cout<<img<<endl; 
    //imshow("img", img);  
    return 0;
}
/************************************
 * function  load .png .jpg pictures and undistorted
 * input:
 * return:
 * author:
 * *******************************/
void load_all_images(const string &path,vector<Mat>&images){
    
    Mat map1,map2;
    string search_path=path+"*.png";

    const char *mpath=search_path.c_str();
    
    vector<String> file_lists;
    
    glob(mpath,file_lists,false);

    long  count=file_lists.size();
    cout<<"img NUM: "<<count<<endl;
    Mat img,img2;
    string imagePath;
    long num=0;
    for(int i=0;i<count;i++)
    {
        cout<<"info "<<file_lists[i]<<endl;
        img=imread(file_lists[i]);
        imagePath=file_lists[i];
        const char * cImagepath=imagePath.c_str();
        cout<<"size : "<<imagePath.size()<<endl;
        num=imagePath.size()-path.size()-4;  //4 for .yuv
        string newName(imagePath.substr(path.size(),num));

        imshow("img ",img);
        // 去畸变并保留最大图
	    initUndistortRectifyMap(cameraMatrix, distCoeffs, Mat(),getOptimalNewCameraMatrix(cameraMatrix, distCoeffs, imageSize, 1, imageSize, 0),imageSize, CV_16SC2, map1, map2);
        
        remap(img, img2, map1, map2, INTER_LINEAR);
	    // 去畸变至全图
	    initUndistortRectifyMap(cameraMatrix, distCoeffs, Mat(), Mat(),imageSize, CV_16SC2, map1, map2);
        
        remap(img, img2, map1, map2, INTER_LINEAR);

        imwrite(string("undistorted")+"/"+newName+".png",img2);    

        images.push_back(img);


        waitKey(5);
    }

}
/***************************************
 * function: 1. load all .yuv files  2. convert to png 3.undistored 
 * input：
 * output:
 * return:
 * ***************************************/
void load_all_yuvs(const string & filepath,vector<Mat> &Images)
{
    
    string search_path=filepath+"*.yuv";
    const char *mpath=search_path.c_str();
    vector<String> file_lists;
    // if(!get_filelist_from_dir(search_path,file_lists))
        // cout<<"open files error"<< endl;
    glob(mpath,file_lists,false);
    string imagePath;
    long iYuvDataBufSize = imageSize.width*imageSize.height ;
    unsigned char *pYuvDataBuf = new unsigned char[iYuvDataBufSize];
    long num=0;
    for(int i=0;i<file_lists.size();i++)
    {   
        imagePath=file_lists[i];
        const char * cImagepath=imagePath.c_str();
        cout<<"size : "<<imagePath.size()<<endl;
        num=imagePath.size()-filepath.size()-4;  //4 for .yuv
        string newName(imagePath.substr(filepath.size(),num));

        cout<<"newName: "<<newName<<endl;
        FILE *fp = fopen(cImagepath, "rb");
        if(NULL==fp)
            cout<<"failed"<<endl;
        fread(pYuvDataBuf, iYuvDataBufSize, 1, fp);
        fclose(fp);
        Mat img=cv::Mat::zeros(360,640,CV_8UC1);
        for(int i=0;i<360;i++)
        {
            for(int j=0;j<640;j++){
                img.at<uchar>(i,j)=((unsigned char*)pYuvDataBuf)[i*640+j];
            }
        }
        
        Images.push_back(img);

        imwrite(filepath+"/"+newName+".png",img);
        imshow("img_yuv",img);
        
        waitKey(3);
    }
    delete pYuvDataBuf;
    
}
