#include <stdio.h>
#include <opencv2/opencv.hpp>
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
using namespace cv;
using namespace std;
int main(int argc, char** argv )
{
    if ( argc != 2 )
    {
        printf("usage: DisplayImage.out <Image_Path>\n");
        return -1;
    }

    Mat image,imageori;
    image = imread( argv[1], 1 );
    image.copyTo(imageori);
    if ( !image.data )
    {
        printf("No image data \n");
        return -1;
    }

Mat dst;
int morph_elem = 0;
int morph_size = 17;
int morph_operator = 1;
int const max_operator = 4;
int const max_elem = 2;
int const max_kernel_size = 21;
  int operation = morph_operator + 2;
  //![operation]

  Mat element = getStructuringElement( morph_elem, Size( 2*morph_size + 1, 2*morph_size+1 ), Point( morph_size, morph_size ) );

 morphologyEx( image, dst, operation, element );
  imshow( "output Image", dst );

int thresh = 100;Mat edgeimagge; vector<vector<Point> > contours;
  vector<Vec4i> hierarchy;

Canny( dst, edgeimagge, thresh, thresh*2, 3 );
  findContours( edgeimagge, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
RNG rng(12345);
Mat drawing = Mat::zeros( edgeimagge.size(), CV_8UC3 );
  for( int i = 0; i< contours.size(); i++ )
     {
       Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
       drawContours( image, contours, i, color, 2, 8, hierarchy, 0, Point() );
     }

vector<RotatedRect> minRect( contours.size() );
 for( int i = 0; i < contours.size(); i++ )
     { minRect[i] = minAreaRect( Mat(contours[i]) );
      
     }

for( int i = 0; i< contours.size(); i++ )
     {
       Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
       Point2f rect_points[4]; minRect[i].points( rect_points );
       for( int j = 0; j < 4; j++ )
          line( image, rect_points[j], rect_points[(j+1)%4], color, 2, 8 );
     }


Mat M, rotated, cropped;
        // get angle and size from the bounding box
        float angle = minRect[0].angle;
        Size rect_size = minRect[0].size;
        if (minRect[0].angle < -45.) {
            angle += 90.0;
            swap(rect_size.width, rect_size.height);
        }
        // get the rotation matrix
        M = getRotationMatrix2D(minRect[0].center, angle, 1.0);
        // perform the affine transformation
        warpAffine(imageori, rotated, M, imageori.size(), INTER_CUBIC);
        // crop the resulting image
        getRectSubPix(rotated, rect_size, minRect[0].center, cropped);
cv::Size box_size = minRect[0].size;
  if (minRect[0].angle < -45.)
    std::swap(box_size.width, box_size.height);

  cv::getRectSubPix(rotated, box_size, minRect[0].center, cropped);

cv::imshow("Cropped", cropped);


cvtColor(cropped,cropped,CV_BGR2GRAY);
std::cout<<"number of response "<< countNonZero(cropped)<<std::endl;
std::cout<<"total_pixel "<< cropped.total()<<std::endl;
std::cout<<"Wornout_rate "<< (cropped.total()-countNonZero(cropped))*100 /  cropped.total()<<"%"<<std::endl;
imshow( "Contours", image );




   // namedWindow("Display Image", WINDOW_AUTOSIZE );
  //  imshow("Display Image", image);

    waitKey(0);

    return 0;
}
