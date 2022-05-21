#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>
#include <iostream>

using namespace cv;
using namespace std;

/////////////// Plate Detector //////////////////////

void main() {

	Mat img;//матрица с исходным ихоображением
	VideoCapture cap(0);//получение видео с камеры 1 (если не работает поменять на 0)

	CascadeClassifier plateCascade;//создание каскада для расспознования русских номеров автомобиля
	plateCascade.load("Resources/haarcascade_russian_plate_number.xml");//загружаем файл xml

	if (plateCascade.empty()) { cout << "XML file not loaded" << endl; }

	vector<Rect> plates;//вектор с номерами автомобилей

	while (true) {//вечный цикл

		cap.read(img);//получаем кадр с камеры
		plateCascade.detectMultiScale(img, plates, 1.1, 10);//ищем номера и записываем в вектор

		for (int i = 0; i < plates.size(); i++)//пробегаемся по всему вектору
		{
			Mat imgCrop = img(plates[i]);//создаём матрицу с изображением номера
			//imshow(to_string(i), imgCrop);
			imwrite("Resources/Plates/" + to_string(i) + ".png", imgCrop);//создаём изображение номера в папке Plates
			rectangle(img, plates[i].tl(), plates[i].br(), Scalar(255, 0, 255), 3);//отрисовывем прямоугольник вокруг номера
		}

		imshow("Image", img);//выводим изображение с камеры
		waitKey(1);
	}
}