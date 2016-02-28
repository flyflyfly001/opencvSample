#include <opencv.hpp>

using namespace std;
using namespace cv;

//4对相对应的像点计算图像的单应 Homography
vector<Point2f> left_image;
vector<Point2f> right_image;

Mat image_logo;
Mat image_main;

void showFinal(Mat src1, Mat src2)
{
	Mat gray, gray_inv, src1final, src2final;
	cvtColor(src2, gray, CV_BGR2GRAY);
	threshold(gray, gray, 0, 255, CV_THRESH_BINARY);

	bitwise_not(gray, gray_inv);
	src1.copyTo(src1final, gray_inv);
	src2.copyTo(src2final, gray);

	Mat final_image = src1final + src2final;
	namedWindow("output", WINDOW_AUTOSIZE);
	imshow("output", final_image);

	waitKey();
}

void on_mouse(int e, int x, int y, int d, void *ptr)
{
	if (e == EVENT_LBUTTONDOWN){

		if (right_image.size() < 4){

			right_image.push_back(Point2f(float(x), float(y)));
			cout << x << " " << y << endl;
		}
		else {

			cout << "Calculating Homography" << endl;
			setMouseCallback("Display window", nullptr, nullptr);
			Mat H = findHomography(left_image, right_image, 0);
			Mat logoWarped;
			warpPerspective(image_logo, logoWarped,H, image_main.size());
			showFinal(image_main, logoWarped);
		}
	}
}
int main()
{
	image_main = imread("4.jpg", CV_LOAD_IMAGE_COLOR);
	image_logo = imread("lenna.jpg", CV_LOAD_IMAGE_COLOR);

	left_image.push_back(Point2f(0, 0));
	left_image.push_back(Point2f(0, image_logo.rows));
	left_image.push_back(Point2f(image_logo.cols, image_logo.rows));
	left_image.push_back(Point2f(image_logo.cols, 0));

	namedWindow("Display window", WINDOW_AUTOSIZE);
	imshow("Display window", image_main);
	setMouseCallback("Display window", on_mouse, nullptr);

	while (true){

		int key = waitKey(10);
		if (key == 27)
			break;
	}

	return 0;
}