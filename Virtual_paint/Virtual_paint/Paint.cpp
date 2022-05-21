#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

Mat img;//������� � ���������, ����������� �� �����
VideoCapture cap(0);//��������� ������� �� ������ (���� �� �������� �������� 1 �� 0)
vector<vector<int>> newPoints;


//������ ����� ������ ����� ���� Color_picker.cpp
vector<vector<int>> myColors{ {73,75,94,104,255,255},//�����
	{0,161,94,12,255,255} };//orange
	//{0, 0, 0, 179, 255, 79} };//������ (������ �� ����� ����� ����)
vector<Scalar> myColorsValue{ {255, 0, 0 }, {69, 0, 200}};

Point getContours(Mat image) {//����� ������ �� img


	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;

	findContours(image, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	vector<vector<Point>> conPoly(contours.size());
	vector<Rect> boundRect(contours.size());

	Point myPoint(0, 0);

	for (int i = 0; i < contours.size(); i++)
	{
		int area = contourArea(contours[i]);//������� �������

		string objectType;

		if (area > 300)//���� ������� ������ 300 (������ �� ������������ �� ��������� ��������)
		{
			float peri = arcLength(contours[i], true);
			approxPolyDP(contours[i], conPoly[i], 0.02 * peri, true);

			boundRect[i] = boundingRect(conPoly[i]);
			myPoint.x = boundRect[i].x + boundRect[i].width / 2;
			myPoint.y = boundRect[i].y;
		}
	}
	return myPoint;
}


vector<vector<int>> findColor(Mat img)//����� ������� �����
{
	Mat imgHSV;
	cvtColor(img, imgHSV, COLOR_BGR2HSV);//������������ �������� � HSV

	for (int i = 0; i < myColors.size(); i++)
	{
		Scalar lower(myColors[i][0], myColors[i][1], myColors[i][2]);//������ ������� ������ �����
		Scalar upper(myColors[i][3], myColors[i][4], myColors[i][5]);//������� ������� ������ �����
		Mat mask;//�����
		inRange(imgHSV, lower, upper, mask);//�������� �����
		Point myPoint = getContours(mask);//����� ����� �� �������
		if (myPoint.x != 0) {
			newPoints.push_back({ myPoint.x,myPoint.y,i });
		}
	}
	return newPoints;
}

void drawOnCanvas(vector<vector<int>> newPoints, vector<Scalar> myColorValues)//�������� �� ������
{

	for (int i = 0; i < newPoints.size(); i++)//������ �� ���� ������ ����� � ���������� ������������ �����, �������� 10
	{
		circle(img, Point(newPoints[i][0], newPoints[i][1]), 20, myColorValues[newPoints[i][2]], FILLED);
	}
}


void main() {

	while (true) {
		cap.read(img);//�������� �������� � ������
		newPoints = findColor(img);//���� �����
		drawOnCanvas(newPoints, myColorsValue);//������ �� ������ ������

		imshow("Image", img);//������� �������� � ����
		waitKey(1);
	}
}