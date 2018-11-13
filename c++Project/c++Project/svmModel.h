#pragma once

#include "Model.h"
#include "opencv2\opencv.hpp"
#include "opencv2\ml\ml.hpp"

#include <string>
#include <ctime>

using namespace std;
using namespace cv;
using namespace cv::ml;

class SvmModel :public Model {
public:

	SvmModel(vector<vector<float>>&floatData,int dataSize,int labelSize,string filePath) {
		train_data = Mat::zeros(floatData.size(), dataSize,CV_32FC1);
		train_label = Mat::zeros(floatData.size(), labelSize, CV_32FC1);
		init();
		sparseDataAndLabel(floatData, dataSize, labelSize);
		setLogFilePath(filePath);
	}

	void train(string savePath) {
		extraRequirement.logFile << "开始训练,为每个目标建立一个SVM分类器" << endl;
		string dataTime =extraRequirement.makeDateDir(savePath+"\\model\\svm");
		for (int i = 0; i < train_label.cols; ++i) {
			Mat temp = Mat::zeros(train_label.rows,1, CV_32FC1);
			temp = train_label.col(i).clone();
			Ptr<TrainData> tData = TrainData::create(train_data, ROW_SAMPLE,temp);
			svm->trainAuto(tData, 10, SVM::getDefaultGrid(SVM::C), SVM::getDefaultGrid(SVM::P),
				SVM::getDefaultGrid(SVM::DEGREE), SVM::getDefaultGrid(SVM::GAMMA));
			extraRequirement.logFile<< "训练第" << to_string(i+1) << "个完成" << endl;
			extraRequirement.logFile << "具体训练参数可查看保存文件" << endl;
			extraRequirement.logFile << "保存在" << dataTime + "_"+to_string(i + 1) + ".xml"  << endl;
			save(dataTime + "label_" + to_string(i + 1) + ".xml");
		}
	}

	void save(string filename) {
		svm->save(filename);
	}

	void load(string filename) {
		extraRequirement.logFile << "从" << filename << "中加载模型";
		svm = ml::SVM::load(filename);
		extraRequirement.logFile << "加载完毕" << endl;
	}

	//考虑到simulink仿真问题，该函数已废弃
	Mat predict(Mat pred_data, string modelPath) {
		Mat pred = Mat::zeros(pred_data.rows, 1, CV_32FC1);
		try {
			load(modelPath);
		}
		catch (cv::Exception& e) {
			extraRequirement.logFile << e.what() << endl;;
			err = 1;
			return pred;
		}
		for (int i = 0; i < train_data.rows; ++i) {
			Mat sample = train_data.row(i);
			try {
				pred.at<float>(i, 0) = svm->predict(sample);
			}
			catch(cv::Exception& e) {
				extraRequirement.logFile << e.what() << endl;;
				err = 2;
				return pred;
			}
		}
		return pred;
	}

	Mat getTrainData() {
		return train_data;
	}

	Mat getTrainLabel() {
		return train_label;
	}

private:
	Ptr<SVM> svm;
	Mat train_data, train_label;
	void init() {
		extraRequirement.logFile << "初始化SVM模型" << endl;
		svm = SVM::create();
		svm->setType(SVM::EPS_SVR);
		svm->setKernel(SVM::RBF);
		svm->setC(1.0);
		svm->setP(0.001);
		svm->setGamma(0.01);
		svm->setCoef0(0.0);
		svm->setDegree(3);
		extraRequirement.logFile << "SVM初始参数为C:1.0\tP:0.001\tGamma:0.01\tCoef:0.0\tDegree:3" << endl;
		
	}

	void sparseDataAndLabel(const vector<vector<float>>&floatData, int dataSize, int labelSize) {
		extraRequirement.logFile << "分割并转换数据，将其转换为SVM可用格式" << endl;
		for (int i = 0; i < floatData.size(); ++i) {
			for (int j = 0, k = 0,l=0; j < floatData[i].size(); ++j) {
				if (j<dataSize)
					train_data.at<float>(i, k++) = floatData[i][j];
				else if (j<labelSize+dataSize)
					train_label.at<float>(i, l++) = floatData[i][j];
			}
		}
	}
	
};
