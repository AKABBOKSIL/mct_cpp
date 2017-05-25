#include <iostream>
#include <opencv\cv.h>
#include <opencv\highgui.h>

using namespace std;
using namespace cv;

//const int  BLOCKS2 = 20, BLOCKS = 20;

class imageValidation{
	static const int BLOCKS2 = 20, BLOCKS = 20;
	int TH1 = 50;
	double TH2 = 1.00;
	bool retThreshold();
	void blur();
public:
	imageValidation(){
		//blur();
	retThreshold();
	}
};
void imageValidation:: blur(){
	IplImage* img = cvLoadImage("aaa (3).jpg", CV_LOAD_IMAGE_COLOR);;
	IplImage* dst = cvCreateImage(cvGetSize(img), 8, 3);

	cvSmooth(img, dst, CV_GAUSSIAN, 49);
	cvSaveImage("aaa (3).jpg", dst);

	cvReleaseImage(&img);

	cvReleaseImage(&dst);

	
}
bool imageValidation::retThreshold()
{
	int edgeblock[BLOCKS2][BLOCKS] = { 0 };
	int edgedata[BLOCKS2][BLOCKS] = { 0 };

	IplImage * jpeg = cvLoadImage("blur.jpg", CV_LOAD_IMAGE_COLOR);
	Mat src(jpeg, true);

	cvtColor(src, src, CV_BGR2GRAY);
	int width = src.size().width;
	int height = src.size().height;

	int w_bound = width / BLOCKS + 1;
	int h_bound = height / BLOCKS2 + 1;

	Mat sobel;
	Mat sobelX;
	Mat sobelY;

	Sobel(src, sobelX, CV_8U, 1, 0);
	Sobel(src, sobelY, CV_8U, 0, 1);
	sobel = abs(sobelX) + abs(sobelY);

	for (int j = 0; j<height; j++)
	{
		for (int k = 0; k<width; k++)
		{
			//sobel filter�� 127(�߰���)�� �������� thresholding�Ͽ� edge ������ ��
			edgedata[j / h_bound][k / w_bound] += sobel.data[j*width + k] > 127 ? 1 : 0;
		}
	}
	

	int flag = 0;//����� ���� �ȼ� ��
	//2��и�(?)
	for (int j = BLOCKS2 / 2 - 1; j >= 0; j--)
	{
		for (int k = BLOCKS / 2 - 1; k >= 0; k--)
		{
			//edgeblock(������� �ĺ�����)�� �ش��ϴ� edgedata(edge ����)���� edge ������ Ư�� ����(50)�� �Ѿ �� ���������� �����ϴ� block�� true�� �ٲ� 
			if (!edgeblock[j + 1][k] && !edgeblock[j][k + 1])//�ֺ��� tree ����� ���� ��(Th�� ����)
			{
				if (edgedata[j][k] > TH1)
				{
					edgeblock[j][k] = 1;
					flag++;
				}
			}
			else//�ֺ��� tree ����� ���� ��(Th�� ����)
			{
				if (edgedata[j][k] > TH1)
				{
					edgeblock[j][k] = 1;
					flag++;
				}
				if (edgedata[j][k]>TH2)
				{
					edgeblock[j][k] = 1;
					flag++;
				}
			}
		}
	}
	//3��и�(?)
	for (int j = BLOCKS2 / 2; j<BLOCKS2; j++)
	{
		for (int k = BLOCKS / 2 - 1; k >= 0; k--)
		{
			//edgeblock(������� �ĺ�����)�� �ش��ϴ� edgedata(edge ����)���� edge ������ Ư�� ����(50)�� �Ѿ �� ���������� �����ϴ� block�� true�� �ٲ� 
			if (!edgeblock[j - 1][k] && !edgeblock[j][k + 1])
			{
				if (edgedata[j][k] > TH1)
				{
					edgeblock[j][k] = 1;
					flag++;
				}
			}
			else
			{
				if (edgedata[j][k]>TH2)
				{
					edgeblock[j][k] = 1;
					flag++;
				}
			}
		}
	}
	//1��и�(?)
	for (int j = BLOCKS2 / 2; j<BLOCKS2; j++)
	{
		for (int k = BLOCKS / 2; k<BLOCKS; k++)
		{
			//edgeblock(������� �ĺ�����)�� �ش��ϴ� edgedata(edge ����)���� edge ������ Ư�� ����(50)�� �Ѿ �� ���������� �����ϴ� block�� true�� �ٲ� 
			if (!edgeblock[j - 1][k] && !edgeblock[j][k - 1])
			{
				if (edgedata[j][k] > TH1)
				{
					edgeblock[j][k] = 1;
					flag++;
				}
			}
			else
			{
				if (edgedata[j][k]>TH2)
				{
					edgeblock[j][k] = 1;
					flag++;
				}
			}
		}
	}
	//4��и�(?)
	for (int j = BLOCKS2 / 2 - 1; j >= 0; j--)
	{
		for (int k = BLOCKS / 2; k<BLOCKS; k++)
		{
			//edgeblock(������� �ĺ�����)�� �ش��ϴ� edgedata(edge ����)���� edge ������ Ư�� ����(50)�� �Ѿ �� ���������� �����ϴ� block�� true�� �ٲ� 
			if (!edgeblock[j + 1][k] && !edgeblock[j][k - 1])
			{
				if (edgedata[j][k] > TH1)
				{
					edgeblock[j][k] = 1;
					flag++;
				}
			}
			else
			{
				if (edgedata[j][k]>TH2)
				{
					edgeblock[j][k] = 1;
					flag++;
				}
			}
		}
	}

	
	//hole filling
	//���� ���� �ϰ� ���� ���߿� �մϴ�
	for (int j = 0; j<BLOCKS2; j++)
	{
		for (int k = 0; k<BLOCKS - 3; k++)
		{
			if (edgeblock[j][k] && edgeblock[j][k + 3])
			{
				edgeblock[j][k + 1] = 1;
				edgeblock[j][k + 2] = 1;
			}
			else if (edgeblock[j][k] && edgeblock[j][k + 2])
			{
				edgeblock[j][k + 1] = 1;
			}
		}
	}
	for (int j = 0; j<BLOCKS2 - 3; j++)
	{
		for (int k = 0; k<BLOCKS; k++)
		{
			if (edgeblock[j][k] && edgeblock[j + 3][k])
			{
				edgeblock[j + 1][k] = 1;
				edgeblock[j + 2][k] = 1;
			}
			else if (edgeblock[j][k] && edgeblock[j + 2][k])
			{
				edgeblock[j + 1][k] = 1;
			}
		}
	}

	for (int j = 0; j<height; j++)
	{
		for (int k = 0; k<jpeg->widthStep; k += 3)
		{
			//���������� �������� �� block ������ ������� �ĺ� ������ ����
			jpeg->imageData[j*jpeg->widthStep + k] *= edgeblock[j / h_bound][k / w_bound / 3];
			jpeg->imageData[j*jpeg->widthStep + k + 1] *= edgeblock[j / h_bound][k / w_bound / 3];
			jpeg->imageData[j*jpeg->widthStep + k + 2] *= edgeblock[j / h_bound][k / w_bound / 3];
		}
	}

	int flagX=0, flagY=0;
	int flag4 = 0;//������ ���� ī��Ʈ

	for (int j = 0; j<height; j++)
	{
		for (int k = 0; k<width; k++)
		{
			//sobel filter�� 127(�߰���)�� �������� thresholding�Ͽ� edge ������ ��
			flagX += sobelX.data[j*width + k] > 127 ? 1 : 0;
			flagY += sobelY.data[j*width + k] > 127 ? 1 : 0;
			if (sobelX.data[j*width + k] > 127 || sobelY.data[j*width + k] > 127) flag4++;
		}
	}

	//������� ���� flag�� Ư�� ��(90)�� �Ѿ �� true return
	cout << "Flag1 " <<flag<< endl;
	cout << "edgeY/edgeX  " <<(double)flagY/flagX<< endl;
	cout << "edgeX/edgeY  " << (double)flagX / flagY << endl;
	cout << "TH4 " << (double)flag4/flag/400 <<endl;

	if (flag > 100) return true;
	else return false;
}
void brightness(){
	Mat src = imread("bbb (11).jpg");

	double alpha = 1.8; int beta = 10;

	resize(src, src, Size(450, 300), 0, 0, CV_INTER_NN);
	Mat bright = Mat::zeros(src.size(), src.type());

	for (int y = 0; y < src.rows; y++)
	{
		for (int x = 0; x < src.cols; x++)
		{
			for (int c = 0; c < 3; c++)
			{
				bright.at<Vec3b>(y, x)[c] = saturate_cast<uchar>(alpha*(src.at<Vec3b>(y, x)[c]) + beta);
			}
		}
	}

	imshow("bright", bright);
	imwrite("bright.jpg", bright);
	waitKey(0);
}
void mct()
{
	Mat src = imread("bbb (15).jpg", 3);
	double alpha = 0.3; int beta = 50;
	//double alpha = 2.0; int beta = 10;

	resize(src, src, Size(450, 300), 0, 0, CV_INTER_NN);
	Mat bright = Mat::zeros(src.size(), src.type());

	for (int y = 0; y < src.rows; y++)
	{
		for (int x = 0; x < src.cols; x++)
		{
			for (int c = 0; c < 3; c++)
			{
				bright.at<Vec3b>(y, x)[c] = saturate_cast<uchar>(alpha*(src.at<Vec3b>(y, x)[c]) + beta);
			}
		}
	}

	//imwrite("bright.jpg", bright);
	imwrite("darkness.jpg", bright);

	Mat gray;
	cvtColor(bright, gray, CV_RGB2GRAY);
	imwrite("gray.jpg", gray);

	int height = src.rows; int width = src.cols;
	Mat tmpImg = Mat::zeros(src.rows+2, src.cols+2, CV_8U);


	for (int y = 1; y < height+1; y++)
	{
		for (int x = 1; x < width+1; x++)
		{
			tmpImg.at<uchar>(y,x) = gray.at<uchar>(y-1, x-1);
		}
	}//����

	unsigned char result[9] = { 0, };
	
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			for (int j = 0; j < 3; j++)
			{
				for (int i = 0; i < 3; i++)
				{
					if (tmpImg.at<uchar>(y + i, x + j)<tmpImg.at<uchar>(y + 1, x + 1))
						result[j*3 + i] = 1;
				}
			}
			int sum = 0;
			for (int k = 0; k < 9; k++)
			{
				 sum+= result[k] * pow(2, 8 - k);
				 result[k] = 0;//�ʱ�ȭ
			}
			tmpImg.at<uchar>(y, x) = (unsigned char)sum; 
		}
	}
	
	//imshow("mct", tmpImg);
	imwrite("mct.jpg", tmpImg);

	waitKey();
}
int main()
{
	//imageValidation IV = imageValidation();
	mct();
	
}