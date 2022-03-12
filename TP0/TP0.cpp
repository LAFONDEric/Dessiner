#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include <iostream>

using namespace cv;
using namespace std;
int moy = 0;
int dessine = 0;
int thresh = 20;
RNG rng(5);
void thresh_callback(int, void*);
Mat img;
Mat img2;
Mat img3;
Mat img4;


int rouge = 255; int vert = 255; int bleu = 255;
void main()
{
	VideoCapture cap(0);
	cap.read(img);
	img4 = Mat::zeros(img.size(), CV_8UC3);
	// Valleur pour callibré notre detection objet De base callibré pour du bleu
	int iLowH = 115;
	int iHighH = 125;

	int iLowS = 97;
	int iHighS = 230;

	int iLowV = 51;
	int iHighV = 174;
	namedWindow("Control", (640, 200)); //creaer une fenetre

	createTrackbar("LowH", "Control", &iLowH, 179); //permette de callibrer low h
	createTrackbar("HighH", "Control", &iHighH, 179);

	createTrackbar("LowS", "Control", &iLowS, 255); 
	createTrackbar("HighS", "Control", &iHighS, 255);

	createTrackbar("LowV", "Control", &iLowV, 255);
	createTrackbar("HighV", "Control", &iHighV, 255);

	// couleur
	
	createTrackbar("rouge", "Control", &rouge, 255);
	createTrackbar("vert", "Control", &vert, 255);
	createTrackbar("bleu", "Control", &bleu, 255);

	createTrackbar("Dessine", "Control", &dessine, 1);
	createTrackbar("Moyenne", "Control", &moy, 1);
	while (true)
	{
		cap.read(img);
		
		cvtColor(img, img2, COLOR_BGR2HSV);// permette de changer notre vition


		inRange(img2, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), img3);

		dilate(img3, img3, getStructuringElement(MORPH_RECT, Size(3, 3)));

		blur(img3, img3, Size(3, 3));



		imshow("Reglage", img3);

		imshow("source", img);

		const int max_thresh = 255;
		// on verifie si l'utillisateur veut dessiner
		if(dessine == 1){
			thresh_callback(0, 0);
		}

		
		waitKey(5);
	}
}

void thresh_callback(int, void*)
{
	Mat canny_output;
	Canny(img3, canny_output, thresh, thresh * 2);
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	findContours(canny_output, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);
	Mat drawing = Mat::zeros(canny_output.size(), CV_8UC3);
	Mat img6 = Mat::zeros(canny_output.size(), CV_8UC3);
	cv::Point_<int>  somme;
	for (vector<Point> i : contours)
	{
		cv::Moments M = moments(i);
		int  cx = M.m10 / M.m00; int  cy = M.m01 / M.m00;
		if (moy == 1)
		{
			cv::Point_<int> center = cv::Point_<int>(cx, cy);
			somme = somme + center;
		}
		else
		{
			cv::Point_<int> center = cv::Point_<int>(cx, cy);
			circle(drawing, center, 7, (255, 255, 255), -1);
		}
	}
	if (moy == 1 && contours.size() != 0)
	{
		cv::Point_<int>  motenne;
		motenne.x = somme.x / contours.size();
		motenne.y = somme.y / contours.size();

		circle(drawing, motenne, 7, (255, 255, 255), -1);
	}


		for (int i = 0; i < drawing.rows; i++)
		{
			for (int j = 0; j < drawing.cols; j++)
			{
				
					
				// crai un calc en fonction de se qui est creer
				if (img4.at<Vec3b>(i, j)[0] == 0 && img4.at<Vec3b>(i, j)[1] == 0 && img4.at<Vec3b>(i, j)[2] == 0) {

					img4.at<Vec3b>(i, j)[0] = drawing.at<Vec3b>(i, j)[0];
					img4.at<Vec3b>(i, j)[1] = drawing.at<Vec3b>(i, j)[1];
					img4.at<Vec3b>(i, j)[2] = drawing.at<Vec3b>(i, j)[2];
				}
				
				// inverse les couleur du calc
				if (img4.at<Vec3b>(i, j)[0] == 0 && img4.at<Vec3b>(i, j)[1] == 0 && img4.at<Vec3b>(i, j)[2] == 0) {

					img6.at<Vec3b>(i, j)[0] = 255; img6.at<Vec3b>(i, j)[1] = 255; img6.at<Vec3b>(i, j)[2] = 255;
				}
				else
				{
					img6.at<Vec3b>(i, j)[0] = 0; img6.at<Vec3b>(i, j)[1] = 0; img6.at<Vec3b>(i, j)[2] = 0;
				}
			}
		
	}
	Mat img5;

	//Crai calque
	img.copyTo(img5, img6);

	// change la couleur
	for (int i = 0; i < drawing.rows; i++)
	{
		for (int j = 0; j < drawing.cols; j++)
		{
			if (img5.at<Vec3b>(i, j)[0] == 0 && img5.at<Vec3b>(i, j)[1] == 0 && img5.at<Vec3b>(i, j)[2] == 0) { img5.at<Vec3b>(i, j)[0] = bleu; img5.at<Vec3b>(i, j)[1] = vert; img5.at<Vec3b>(i, j)[2] = rouge; }
		}
	}


	imshow("resultat", img5);

}















