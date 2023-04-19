#pragma once
#include <iostream>
#include "opencv2/imgproc/imgproc_c.h"
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/objdetect.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <vector>


enum Paintings{
    MonaLisa = 1,
    AdamHands,
    Pearl,
    BlackGlasses,
    BlueGlasses,
    Snow,
    Sparkles,
    Rain,
    Grain,
    Date,
    FiveColors,
    Panorama
};


class Art{
public:
    Art(){}
    Art(cv::Mat);
    Art(cv::Point, cv::Point, cv::Mat);
    Art(const std::pair<double,double>&, const std::pair<double,double>&, cv::Mat);
    void set_centers(const std::pair<double,double>&, const std::pair<double,double>&);
    void set_image(cv::Mat&);
    void more_art(Paintings);
    void a_filter(const std::pair<double,double>&, const std::pair<double,double>&, cv::Mat&, Paintings);
    void b_filter(cv::Mat&, Paintings);
    void show(){
        cv::imshow("new", _image);
        cv::waitKey(0);
    }
private:
    void _MonaLisa();
    void _AdamHands();
    void _Pearl();
    void _SunGlasses();
    void _add_rain();
    void _add_snow();
    void _add_sparkles();
    void _grain();
    void five_filters();
    cv::Mat get_painting(Paintings);
    void blur();
    void attach(Paintings);
    void to_black(cv::Mat&);
    cv::Rect calculate_dest(int, int);
    void sizing(Paintings,cv::Mat& art, int eye_distance);
    void print(const cv::Mat&, cv::Rect, Paintings);
    void applyFilter(cv::Mat,cv::Scalar);
    void date_and_time();
    void _panorama();

private:
    cv::Point center1, center2;
    cv::Mat _image;
};




class BaseFilter{
public:

};