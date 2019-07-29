#include  <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <string>
#include <features2d\features2d.hpp>
#include <vector> 
#include <fstream>
using namespace cv;
using namespace std;



void SplitString(const string& s, vector<string>& v, const string& c)
{
	string::size_type pos1, pos2;
	pos2 = s.find(c);
	pos1 = 0;
	while (string::npos != pos2)
	{
		v.push_back(s.substr(pos1, pos2 - pos1));

		pos1 = pos2 + c.size();
		pos2 = s.find(c, pos1);
	}
	if (pos1 != s.length())
		v.push_back(s.substr(pos1));
}

/***
rotation
*/
void  img_rotation(Mat &src, Mat &dst, float angle)
{
	float radian = (float)(angle / 180.0 * CV_PI);


	//首先算出以最长边为正方形的对角线长度。这样无论怎么旋转都不会超出去

	//然后计算旋转矩阵，这里是二维的

	//旋转之后，剪掉空出来的部分，因为当时以最长边的对角线为基准的

	//填充图像
	int maxBorder = (int)(max(src.cols, src.rows)*1.414);//即为sqrt(2)*max
	int dx = (maxBorder - src.cols) / 2;
	int dy = (maxBorder - src.rows) / 2;

	/*CV_EXPORTS_W void copyMakeBorder(InputArray src, OutputArray dst,
                                 int top, int bottom, int left, int right,
                                 int borderType, const Scalar& value = Scalar() );*/
	copyMakeBorder(src, dst, dy, dy, dx, dx, BORDER_CONSTANT);
	


	//旋转
	Point2f center((float)(dst.cols / 2), (float)(dst.rows / 2));

	//为啥这里是2维的矩阵，原因是，3x3 * 3x1 = 3x1  最后一维是1，所以是2x1  把每一行展开，乘起来就行
	Mat affine_matrix = getRotationMatrix2D(center, angle, 1.0);
	std::cout << affine_matrix << std::endl;
	warpAffine(dst, dst, affine_matrix, dst.size());

	//计算的思路还是蛮简单的
	//首先想象一下当矩形框旋转之后，包住它的矩形，然后用把两段加起来

	//计算图像旋转之后包含图像的最大矩形
	float sinVal = abs(sin(radian));
	float cosVal = abs(cos(radian));
	Size targetSize(
		(int)(src.cols * cosVal + src.rows 	* sinVal),
		(int)(src.cols * sinVal + src.rows * cosVal)
		);

	//剪掉多余边框
	int x = (dst.cols - targetSize.width) / 2;
	int y = (dst.rows - targetSize.height) / 2;
	Rect rect(x, y, targetSize.width, targetSize.height);
	dst = Mat(dst, rect);
	//imwrite("I:/sclead/LPTphotoes/img_prepoccess/img_rotation.jpg", dst);
}

/***
translation
*/
void img_translation(Mat &src, Mat &dst, int xoffset, int yoffset)
{

	//TODO: verify the xoffset and yoffset

	Mat t_mat = Mat::zeros(2, 3, CV_32FC1);
	Size dst_size = src.size();

	t_mat.at<float>(0, 0) = 1;
	t_mat.at<float>(0, 2) = xoffset;
	t_mat.at<float>(1, 1) = 1;
	t_mat.at<float>(1, 2) = yoffset;

	warpAffine(src, dst, t_mat, dst_size);
}

void img_translation(Mat &src, Mat &dst, int xoffset, int yoffset, string txt_name, string txt_path = "")
{
	img_translation(src, dst, 50, 50);

	ifstream txt_file;
	txt_file.open(txt_path);
	assert(txt_file.is_open());

	string row;

	vector<string> v;

	ofstream out_txt(txt_name);

	while (getline(txt_file, row))
	{
		SplitString(row, v, " ");
		/*for (vector<string>::size_type i = 0; i != v.size(); ++i)
			out_txt << v[i] << " ";
		out_txt << endl;*/
	}
	
	txt_file.close();
	out_txt.close();
}


/***
shear
*/
void img_shear(Mat &src, Mat &dst,int offset_factor){
	//TODO : verify the xoffset and yoffset

	//剪切变换很简单，就是x方向上或y方向上位置整体移动就行了

	//3点的放射变换
	Point2f src_point[3];
	Point2f dst_point[3];

	src_point[0] = cv::Point2f(0, 0);
	src_point[1] = cv::Point2f(src.cols - 1, 0);
	src_point[2] = cv::Point2f(0, src.rows - 1);

	dst_point[0] = cv::Point2f(src.cols*0.1, src.rows*0.13);
	dst_point[1] = cv::Point2f(src.cols*0.8, src.rows*0.32);
	dst_point[2] = cv::Point2f(src.cols*0.16, src.rows*0.7);

	//dst_point[0] = cv::Point2f(src.cols*offset_factor, 0);
	//dst_point[1] = cv::Point2f(src.cols - 1 , 0);
	//dst_point[2] = cv::Point2f(0, src.rows - 1);

	cv::Mat warp_mat(cv::Size(2, 3), CV_32F);
	warp_mat = cv::getAffineTransform(src_point, dst_point);

	warpAffine(src, dst, warp_mat, src.size());
	//imwrite("I:/sclead/LPTphotoes/img_prepoccess/img_shear.jpg", dst);
}

/***
scale
*/
void img_scale(Mat &src, Mat &dst, float scale){

	Point2f src_point[3];
	Point2f dst_point[3];

	src_point[0] = cv::Point2f(0, 0);
	src_point[1] = cv::Point2f(src.cols - 1, 0);
	src_point[2] = cv::Point2f(0, src.rows - 1);

	dst_point[0] = cv::Point2f(0, 0);
	dst_point[1] = cv::Point2f(src.cols*scale, 0);
	dst_point[2] = cv::Point2f(0, src.rows*scale);

	cv::Mat warp_mat(cv::Size(2, 3), CV_32F);
	warp_mat = cv::getAffineTransform(src_point, dst_point);

	warpAffine(src, dst, warp_mat, src.size());
	//imwrite("I:/sclead/LPTphotoes/img_prepoccess/img_scale.jpg", dst);
}


/***
perspective
*/
void img_perspective(Mat &src, Mat &dst){
	//直接设置矩阵  难度太大
	//Mat perspective_mat = Mat::zeros(3,3,CV_32FC1);
	//perspective_mat.at<float>(0, 0) = 1;
	//perspective_mat.at<float>(1, 1) = 1;
	//perspective_mat.at<float>(2, 0) = 0.1;
	//perspective_mat.at<float>(2, 1) = 0.1;
	//perspective_mat.at<float>(2, 2) = 1;

	//通过点去计算投影矩阵
	Point2f Points0[4];
	Point2f Points1[4];

	Points0[0] = cv::Point2f(0, 0);
	Points0[1] = cv::Point2f(src.cols - 1, 0);
	Points0[2] = cv::Point2f(0, src.rows - 1);
	Points0[3] = cv::Point2f(src.cols - 1, src.rows - 1);


	Points1[0] = cv::Point2f(src.cols *0.2, 0);
	Points1[1] = cv::Point2f(src.cols*0.8, 0);
	Points1[2] = cv::Point2f(0, src.rows*0.8);
	Points1[3] = cv::Point2f(src.rows*0.8, src.rows*0.8);
	Mat  perspective_mat  = getPerspectiveTransform(Points0, Points1);

	warpPerspective(src, dst, perspective_mat, src.size());
	//imwrite("H:/sclead/LPTphotoes/img_prepoccess/img_perspective.jpg", dst);
}

/***
gaussian
*/
void img_gaussian_blur(Mat &src, Mat &dst){
	GaussianBlur(src, dst, Size(15, 15), 0, 0);
	//imwrite("I:/sclead/LPTphotoes/img_prepoccess/img_gaussian_blur.jpg", dst);
}

/***
avarge_value blue
*/
void img_avarge_value_blur(Mat &src, Mat &dst){
	blur(src, dst, Size(3,3));
	//imwrite("I:/sclead/LPTphotoes/img_prepoccess/img_avarge_value_blur.jpg", dst);
}

void read_img(string path,Mat &img)
{
	//TODO: check the path

	img = cv::imread(path);
	resize(img, img, Size(416, 416));
	
}

/***
save img
*/
void save_img(string path, Mat &img)
{
	//"H:/sclead/LPTphotoes/img_prepoccess/img_avarge_value_blur.jpg"
	imwrite(path, img);
}

/***

*/
void get_filenames(string path,string &file_name)
{

}

//int main()
//{
//	string filePath = "I:/sclead/LPTphotoes/Camera";
//	vector<string> files;
//	string format = "";				 //查找文件的格式
//	getAllFiles(filePath, files, format);
//	int size = files.size();
//	for (int i = 0; i<size; i++)
//	{
//		cout << files[i] << endl;
//	}
//	system("pause");
//	return 0;
//}


void save_txt()
{

}

struct operations
{
	const string img_translation = "img_translation";
	const string img_scale = "img_scale";
	const string img_gaussian_blur = "img_gaussian_blur";
	const string img_avarge_value_blur = "img_avarge_value_blur";
};


int main(){
	//std::cout << "hello world" << std::endl;

	cv::Mat img = cv::imread("H:/sclead/LPTphotoes/Camera/IMG_20190601_103406.jpg");

	ifstream file;
	file.open("H:/sclead/LPTphotoes/img_names.txt");
	assert(file.is_open());

	const string img_path = "H:/sclead/LPTphotoes/Camera/";
	const string op_img_path = "H:/sclead/LPTphotoes/hanled_data/";
	const string label_path = "H:/sclead/LPTphotoes/train_data/";
	const string labels_path = "H:/sclead/LPTphotoes/labels/";

	string op_img;

	string img_name;

	operations ops;
	

	Mat dst;

	while (getline(file, img_name))
	{
		//resize(img, img, Size(416, 416));
		
		//cv::imshow("src", img);
		//imwrite("H:/sclead/LPTphotoes/img_prepoccess/src.jpg", img);
		/*img_rotation(img, dst, 30);
		save_img("I:/sclead/LPTphotoes/hanled_data", s, "img_rotation", dst);*/

		img = imread(img_path + img_name + ".jpg");
		
		assert(img.empty());
		resize(img, img, Size(416, 416));

		/**translation**/
		//op_img = op_img_path + img_name + ops.img_translation + "150150"+".jpg";
		//img_translation(img, dst, 150, 150);
		//save_img(op_img, dst);


		///**scale**/
		//op_img = op_img_path + img_name + ops.img_scale + "09" + ".jpg";
		//img_scale(img, dst, 0.9);
		//save_img(op_img,dst);

		img_shear(img, dst, 0.9);
		//save_img("I:/sclead/LPTphotoes/hanled_data", img_name, "img_shear",dst);
		imshow("img_shear", dst);
		waitKey(0);

		img_perspective(img, dst);
		//save_img("I:/sclead/LPTphotoes/hanled_data", img_name, "img_perspective",dst);
		imshow("img_perspective", dst);
		waitKey(0);

		///**gaussian_blur**/
		//op_img = op_img_path + img_name + ops.img_gaussian_blur + ".jpg";
		//img_gaussian_blur(img, dst);
		//save_img(op_img, dst);


		///**avarge_value_blur**/
		/*op_img = op_img_path + img_name + ops.img_avarge_value_blur + ".jpg";
		img_avarge_value_blur(img, dst);
		save_img(op_img, dst);*/
	}


	file.close();
	

	//imshow("dst", dst);

	//imwrite("I:/sclead/LPTphotoes/img_prepoccess/1.jpg", dst);

	cv::waitKey(0);
	system("pause");
	return 0;
}

/***
保持了全图，但是依然有偏移，貌似没有太大的必要
*/
Mat imgTranslate(Mat &matSrc, int xOffset, int yOffset, bool bScale)
{
	// 判断是否改变图像大小,并设定被复制ROI
	int nRows = matSrc.rows;
	int nCols = matSrc.cols;
	int nRowsRet = 0;
	int nColsRet = 0;
	Rect rectSrc;
	Rect rectRet;
	if (bScale)
	{
		nRowsRet = nRows + abs(yOffset);
		nColsRet = nCols + abs(xOffset);
		rectSrc.x = 0;
		rectSrc.y = 0;
		rectSrc.width = nCols;
		rectSrc.height = nRows;
	}
	else
	{
		nRowsRet = matSrc.rows;
		nColsRet = matSrc.cols;
		if (xOffset >= 0)
		{
			rectSrc.x = 0;
		}
		else
		{
			rectSrc.x = abs(xOffset);
		}
		if (yOffset >= 0)
		{
			rectSrc.y = 0;
		}
		else
		{
			rectSrc.y = abs(yOffset);
		}
		rectSrc.width = nCols - abs(xOffset);
		rectSrc.height = nRows - abs(yOffset);
	}
	// 修正输出的ROI
	if (xOffset >= 0)
	{
		rectRet.x = xOffset;
	}
	else
	{
		rectRet.x = 0;
	}
	if (yOffset >= 0)
	{
		rectRet.y = yOffset;
	}
	else
	{
		rectRet.y = 0;
	}
	rectRet.width = rectSrc.width;
	rectRet.height = rectSrc.height;
	// 复制图像
	Mat matRet(nRowsRet, nColsRet, matSrc.type(), Scalar(0));
	matSrc(rectSrc).copyTo(matRet(rectRet));
	return matRet;
}