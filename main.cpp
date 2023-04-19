#include <iostream>
#include "more_art.h"

std::pair<double, double> eye1{0.656313241 , 0.355795026};
std::pair<double,double> eye2{0.824149907 , 0.398724645};

std::pair<double, double> p_eye1{0.61055249, 0.32887283};
std::pair<double,double> p_eye2{0.70457077, 0.327895314};


int main(){
    std::string path = "/Users/NellyVardanyan/ACA/filter2/faces/picture copy.jpg";
    cv::Mat image = cv::imread(path);
    Art art;
    art.a_filter(eye1, eye2, image, MonaLisa);
    art.show();
    return 0;
}