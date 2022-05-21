#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

Mat img;//матрица с картинкой, выводящаяся на экран
VideoCapture cap(0);//получение сигнала от камеры (если не работает поменять 1 на 0)
vector<vector<int>> newPoints;


//данные цвета ищутся через файл Color_picker.cpp
vector<vector<int>> myColors{ {73,75,94,104,255,255},//синий
	{0,161,94,12,255,255} };//orange
	//{0, 0, 0, 179, 255, 79} };//чёрный (только на очень ярком фоне)
vector<Scalar> myColorsValue{ {255, 0, 0 }, {69, 0, 200}};

Point getContours(Mat image) {//найти контур на img


	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;

	findContours(image, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	vector<vector<Point>> conPoly(contours.size());
	vector<Rect> boundRect(contours.size());

	Point myPoint(0, 0);

	for (int i = 0; i < contours.size(); i++)
	{
		int area = contourArea(contours[i]);//площадь контура

		string objectType;

		if (area > 300)//если площадь дольше 300 (защита от срабатывания на небольших пикселях)
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


vector<vector<int>> findColor(Mat img)//поиск нужного цвета
{
	Mat imgHSV;
	cvtColor(img, imgHSV, COLOR_BGR2HSV);//конвертируем картинку в HSV

	for (int i = 0; i < myColors.size(); i++)
	{
		Scalar lower(myColors[i][0], myColors[i][1], myColors[i][2]);//нижние границы поиска цвета
		Scalar upper(myColors[i][3], myColors[i][4], myColors[i][5]);//верхние границы поиска цвета
		Mat mask;//маска
		inRange(imgHSV, lower, upper, mask);//создание маски
		Point myPoint = getContours(mask);//задаём точки по контуру
		if (myPoint.x != 0) {
			newPoints.push_back({ myPoint.x,myPoint.y,i });
		}
	}
	return newPoints;
}

void drawOnCanvas(vector<vector<int>> newPoints, vector<Scalar> myColorValues)//рисовать на экране
{

	for (int i = 0; i < newPoints.size(); i++)//рисуем по всем точкам круги с начальными координатами точки, радиусом 10
	{
		circle(img, Point(newPoints[i][0], newPoints[i][1]), 20, myColorValues[newPoints[i][2]], FILLED);
	}
}


void main() {

	while (true) {
		cap.read(img);//получаем картинку с камеры
		newPoints = findColor(img);//ищем точки
		drawOnCanvas(newPoints, myColorsValue);//рисуем по нужным точкам

		imshow("Image", img);//выводим картинку в окне
		waitKey(1);
	}
}