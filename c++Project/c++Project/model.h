#pragma once

#include "opencv2\opencv.hpp"
#include "util.h"

#include <fstream>
#include <string>
#include <ctime>
#include <io.h>
#include <direct.h>
#pragma warning(disable : 4996)
using namespace cv;
using namespace std;


class Model {
public:
	int err = 0;
	util extraRequirement;

	void setLogFilePath(string filePath) {
		extraRequirement.logFile.open(filePath, ios::app);
	}

	virtual void train(std::string filename) = 0;
	virtual void save(std::string filename) = 0;
	virtual void load(std::string filename) = 0;
	virtual Mat predict(cv::Mat pred_data, string modelPath) = 0;
	virtual Mat getTrainData() = 0;
	virtual Mat getTrainLabel() = 0;

	void saveCsv(Mat& data, string path) {
		ofstream file(path, ios::out | ios::trunc);
		for (int i = 0; i < data.rows; ++i)
			file<<to_string(data.at<float>(i,0)) << "\n";
		file.close();
		extraRequirement.logFile << "保存csv文件于" << path << endl;
	}

	float calculate_mse(Mat& train_label, Mat& pred,int index) {
		float res = 0;
		for (int i = 0; i < pred.rows; ++i) {
			float temp = abs((train_label.at<float>(i, index) - pred.at<float>(i, 0)) / train_label.at<float>(i, index));
			temp = pow(res, 2);
			res = res > temp ? res : temp;
		}
			
		//	res += pow(train_label.at<float>(i, index) - pred.at<float>(i, 0), 2);
		//res /= pred.rows;
		return res;
	}
	

};
