#include "more_art.h"
#include <string>
#include <cmath>



std::vector<std::string> paths_to_paintings{
"/Users/NellyVardanyan/ACA/filter2/res/Mona-Lisa-oil-wood-panel-Leonardo-da.jpg.webp",
"/Users/NellyVardanyan/ACA/filter2/art/Screenshot 2023-04-13 at 15.48.50.png", 
"/Users/NellyVardanyan/ACA/filter2/art/Screenshot 2023-04-14 at 22.07.08.png",
"/Users/NellyVardanyan/ACA/filter2/art/Screenshot 2023-04-18 at 16.06.42.png",
"/Users/NellyVardanyan/ACA/filter2/art/Screenshot 2023-04-18 at 19.20.24.png",
"/Users/NellyVardanyan/ACA/filter2/art/glasses_blue.png"
};


int euclid(cv::Point p1, cv::Point p2){
    return sqrt((p1.x-p2.x)*(p1.x-p2.x)+(p1.y-p2.y)*(p1.y-p2.y));
}

Art::Art(cv::Point p1, cv::Point p2, cv::Mat tmp_image):
_image(tmp_image){
    center1 = {p1.x*_image.cols, p1.y*_image.rows};
    center2 = {p2.x*_image.cols, p2.y*_image.rows};
}

Art::Art(const std::pair<double,double>& p1, const std::pair<double,double>& p2, cv::Mat tmp_image):_image(tmp_image)
{
    int k = p1.first * _image.cols;
    int j = p1.second*_image.rows;
    center1 = {k,j};
    k = p2.first * _image.cols;
    j = p2.second*_image.rows;
    center2 = {k,j};
}


void Art::set_centers(const std::pair<double,double>& p1, const std::pair<double,double>&p2){
    int k = p1.first * _image.cols;
    int j = p1.second*_image.rows;
    center1 = {k,j};
    k = p2.first * _image.cols;
    j = p2.second*_image.rows;
    center2 = {k,j};
}

void Art::more_art(Paintings painting){
    attach(painting);
}

int eye_dist(cv::Point c1, cv::Point c2){
    return euclid(c1, c2);
}

cv::Mat Art::get_painting(Paintings p){
    std::string _path = paths_to_paintings[p];
    cv::Mat art = cv::imread(_path);
    return art;
}


bool is_green(const cv::Vec3b& pixel){
    if(pixel[1]>=230) return true;
    return false;
}
bool is_black(const cv::Vec3b& pixel){
    if(pixel==cv::Vec3b(0,0,0)) return true;
    return false;
}
bool is_white(const cv::Vec3b& pixel){
    if(pixel==cv::Vec3b(255,255,255)) return true;
    return false;
}

void Art::blur(){
    cv::blur(_image, _image, cv::Size(10,10));
}

void Art::attach(Paintings p){
    switch(p){
        case MonaLisa:{
            _MonaLisa();
            break;
        }
        case AdamHands:{
            _AdamHands();
            break;
        }
        case Pearl:{
            _Pearl();
            break;
        }
        case BlackGlasses:{
            _SunGlasses();
            break;
        }
        case BlueGlasses:{
            _SunGlasses();
            break;
        }
        case Snow:{
            _add_snow();
            break;
        }
        case Rain:{
            _add_rain();
            break;
        }
        case Sparkles:{
            _add_sparkles();
            break;
        }
        case Grain:{
            _grain();
            break;
        }
        case Date:{
            date_and_time();
            break;
        }
        case FiveColors:{
            five_filters();
            break;
        }
        case Panorama:{
            _panorama();
            break;
        }
    }

}


void Art::to_black(cv::Mat& _art){
    cv::cvtColor(_image, _image, CV_RGB2GRAY);
    cv::cvtColor(_art, _art, CV_RGB2GRAY);
}



cv::Rect Art::calculate_dest(int rows, int cols){
    cv::Rect dest;
    cv::Point first, second; 
    first.x = std::min(center1.x,center2.x);
    first.y = (first.x == center1.x)? center1.y: center2.y;
    second = first == center1? center2: center1;
     int eye_dist = euclid(center1, center2);
    int mid = first.x+eye_dist/2;
    dest.x = mid-(cols/2)-eye_dist/2;
    dest.y = first.y-rows/2;
    dest.height = rows;
    dest.width = cols;
    return dest;
}

void Art::sizing(Paintings p,cv::Mat& art, int eye_distance){
    switch (p)
    {
    case AdamHands:{ 
        cv::resize(art, art, cv::Size(_image.cols, _image.rows/3), cv::INTER_LINEAR);
        break;
    }
    default:{
        cv::resize(art, art, cv::Size(eye_distance*2, eye_distance/2));
        break;
       }
    }
}

void regulate_size(cv::Mat& _image1,cv::Mat& _image2){
    if(_image1.rows>_image2.rows){
        cv::resize(_image1, _image1, cv::Size(_image1.cols, _image2.rows), cv::INTER_LINEAR);
        }else if(_image1.rows<_image2.rows){
        cv::resize(_image2, _image2, cv::Size(_image2.cols, _image1.rows), cv::INTER_LINEAR);
        }
    if(_image1.cols>_image2.cols){
        cv::resize(_image1, _image1, cv::Size(_image2.cols, _image1.rows), cv::INTER_LINEAR);
    }else if(_image1.cols<_image2.cols){
        cv::resize(_image2, _image2, cv::Size(_image1.cols, _image2.rows), cv::INTER_LINEAR);
    }
}


bool check(cv::Vec3b pixel, Paintings p)
{
   if(pixel == cv::Vec3b(255, 255, 255) || pixel == cv::Vec3b(0, 255, 0)) return true;
   return false;
}

void Art::print(const cv::Mat& art, cv::Rect r, Paintings p){
    for(int i=0;i<r.width;i++){
        if(i+r.x > _image.cols ) continue;
        for(int j=0;j<r.height;j++){
            if(j+r.y>_image.rows)continue;
            if(check(art.at<cv::Vec3b>(j, i), p))
            {
                continue;
            }
            _image.at<cv::Vec3b>(j+r.y,i+r.x) = art.at<cv::Vec3b>(j, i);
        }
    }
}



void Art::_add_rain(){
    std::string rain_path = "/Users/NellyVardanyan/ACA/filter2/art/rain-texture-on-black-background-vector-31712235.jpg";
    cv::Mat rain = cv::imread(rain_path);
    regulate_size(rain, _image);
    cv:: addWeighted(_image, 0.9, rain, 0.7, 0.0, _image);
}
void Art::_add_snow(){
    std::string snow_path = "/Users/NellyVardanyan/ACA/filter2/art/Image Preview rain.jpg";
    cv::Mat snow = cv::imread(snow_path);
    regulate_size(snow,_image);
    cv:: addWeighted(_image, 0.9, snow, 0.7, 0.0, _image);
}
void Art::_add_sparkles(){
    std::string sparkles_path = "/Users/NellyVardanyan/ACA/filter2/art/skynews-star-sky-night-somerset_4641946.jpg";
    cv::Mat sparkles = cv::imread(sparkles_path);
    regulate_size(sparkles,_image);
    cv:: addWeighted(_image, 0.9, sparkles, 0.7, 0.0, _image);
}


void Art::_MonaLisa(){
    cv::Mat _art = get_painting(MonaLisa);
    int eye_distance = eye_dist(center1, center2);
    sizing(MonaLisa, _art, eye_distance);
    cv::Rect r;
    r = calculate_dest(_art.rows, _art.cols);
    cv::Mat destRoi;
    try {
        destRoi = _image(r);
    }  catch (...) {
        std::cerr << "Trying to create roi out of image boundaries" << std::endl;
    }
    print(_art, r, MonaLisa);
    cv::cvtColor(_image, _image, CV_BGR2GRAY);
}

 void Art::_AdamHands(){
    cv::Mat _art = get_painting(AdamHands);
    to_black(_art);
    regulate_size(_art, _image);
    cv::addWeighted(_image, 0.7, _art, 0.5, 0.0, _image);
    return;
 }
 
 void Art::_Pearl(){
    cv::Mat _art = get_painting(Pearl);
    int eye_distance = eye_dist(center1, center2);
    sizing(Pearl, _art, eye_distance);
    cv::Rect r;
    r = calculate_dest(_art.rows, _art.cols);
    cv::Mat destRoi;
    try {
        destRoi = _image(r);
    }  catch (...) {
        std::cerr << "Trying to create roi out of image boundaries" << std::endl;
    }
    print(_art, r, Pearl);
    cv::cvtColor(_image, _image, CV_BGR2GRAY);
    this->blur();
 }
 
void Art::_SunGlasses(){
    cv::Mat _art = get_painting(BlackGlasses);
    int eye_distance = eye_dist(center1, center2);
    sizing(BlackGlasses, _art, eye_distance);
    cv::Rect r;
    r = calculate_dest(_art.rows, _art.cols);
    cv::Mat destRoi;
    try {
        destRoi = _image(r);
    }  catch (...) {
        std::cerr << "Trying to create roi out of image boundaries" << std::endl;
    }
    print(_art, r, BlackGlasses);
}



void Art::date_and_time() {
    if (_image.empty())
    {
        std::cout << "Could not open or find the image" << std::endl;
    }
    time_t now = time(0);
    struct tm tstruct;
    char text[80];
    tstruct = *localtime(&now);
    strftime(text, sizeof(text), "%d.%m.%Y %X", &tstruct);
    std::string date_time = text;
    cv::Scalar brightness = mean(_image);
    int font = cv::FONT_HERSHEY_SIMPLEX; //tareri dzev
    double font_scale = 0.9; //tarachap
    cv::Scalar font_color;
    if (brightness[0] < 127) {
        font_color = cv::Scalar(255, 255, 255);
    } else {
        font_color = cv::Scalar(0, 0, 0);
    }
    int font_thickness = 2;
    cv::Size text_size = cv::getTextSize(date_time, font, font_scale, font_thickness, nullptr);
    cv::Point text_pos(_image.cols - text_size.width - 10, _image.rows - text_size.height - 10);
    putText(_image, date_time, text_pos, font, font_scale, font_color, font_thickness);
}

void Art::applyFilter(cv::Mat image, cv::Scalar color)
{
    cv::Mat filter(image.size(), image.type(), color);
    addWeighted(image, 0.5, filter, 0.5, 0, image);
}
void Art::five_filters(){
    if (_image.empty()) {
        std::cout << "Could not open or find the image" << std::endl;
        return;
    }
    int width = _image.cols / 5;
    std::vector<cv::Scalar> filter_colors = {
        cv::Scalar(255, 0, 0),   // blue filter
        cv::Scalar(0, 255, 0),   // green filter
        cv::Scalar(0, 0, 255),   // red filter
        cv::Scalar(0, 255, 255), // yellow filter
        cv::Scalar(255, 0, 255)  // purple filter
    };
    cv::Mat combined(_image.rows, _image.cols, _image.type(), cv::Scalar(0, 0, 0));
    for (int i = 0; i < 5; i++) {
        std::vector<cv::Point> pts(4);
        pts[0] = cv::Point(i * width, 0);
        pts[1] = cv::Point((i + 1) * width, 0);
        pts[2] = cv::Point((i + 1) * width, _image.rows);
        pts[3] = cv::Point(i * width, _image.rows);
        cv::Mat mask(_image.size(), CV_8UC1, cv::Scalar(0));
        fillConvexPoly(mask, pts, cv::Scalar(255));
        cv::Mat roi(_image.size(), _image.type(), filter_colors[i]);
        _image.copyTo(roi, mask);
        applyFilter(roi, filter_colors[i]);
        roi.copyTo(combined, mask);
    }
      _image=combined;
}
void Art::_grain(){
    if (_image.empty())
    {
        std::cout << "Could not open or find the image" << std::endl;
        return;
    }
    cv::Mat noise = cv::Mat::zeros(_image.size(), _image.type());
    randn(noise, cv::Scalar::all(0), cv::Scalar::all(50)); // mean=0, SD=50
    _image+=noise;
}

void Art::_panorama(){
    double stretch_factor = 2.5;
    double compression_factor = 0.7;
    cv::Point2f source[4] = {
        cv::Point2f(_image.cols/4, 0),
        cv::Point2f(_image.cols*3/4, 0),
        cv::Point2f(_image.cols/4, _image.rows),
        cv::Point2f(_image.cols*3/4, _image.rows)
    };
    cv::Point2f destination[4] = {
        cv::Point2f(_image.cols/4, 0),
        cv::Point2f(_image.cols*3/4 * stretch_factor, 0),
        cv::Point2f(_image.cols/4, _image.rows * compression_factor),
        cv::Point2f(_image.cols*3/4 * stretch_factor, _image.rows * compression_factor)
    };
    cv::Mat transform_matrix = getPerspectiveTransform(source, destination);  // sarqenq 3x3 matric warpi 3rd paranetri hamar
    cv::Mat modified_img;
    warpPerspective(_image, modified_img, transform_matrix, cv::Size(_image.cols, _image.rows*compression_factor));
    _image=modified_img;
}


void Art::a_filter(const std::pair<double,double>& eye1, const std::pair<double,double>& eye2, cv::Mat& image, Paintings p){
    set_image(image);
    set_centers(eye1, eye2);
   
    switch (p)
    {
        case MonaLisa:case AdamHands:{
            _MonaLisa();
            break;
        }      
        case Pearl:{
            _Pearl();
            break;
        }   
        case BlackGlasses:{
            _SunGlasses();
            break;
        }
        default:{
            break;
        }
    }
}
void Art::b_filter(cv::Mat& image, Paintings p){
    set_image(image);
    switch (p)
    {
        case AdamHands:{
            _AdamHands();
            break;
        }   
        case  Grain:{
            _grain();
            break;
        }
        case  Date:{
            date_and_time();
            break;
        }
        case  FiveColors:{
            five_filters();
            break;
        }
        case Snow:{
            _add_snow();
            break;
        }
        case Sparkles:{
            _add_sparkles();
            break;
        }
        case Rain:{
            _add_rain();
            break;
        }
        case Panorama:{
            _panorama();
            break;
        }
        default:
            break;
        }
}

void Art::set_image(cv::Mat& image){
    _image = image;
}