#pragma once

#include "Model.h"
#include "opencv2\opencv.hpp"
#include "opencv2\ml\ml.hpp"

#include <string>
#include <ctime>

using namespace std;
using namespace cv;
using namespace cv::ml;

class RandomForestModel :public Model {
public:

	RandomForestModel(vector<vector<float>>&floatData, int dataSize, int labelSize, string filePath) {
		train_data = Mat::zeros(floatData.size(), dataSize, CV_32FC1);
		train_label = Mat::zeros(floatData.size(), labelSize, CV_32FC1);
		init();
		sparseDataAndLabel(floatData, dataSize, labelSize);
		setLogFilePath(filePath);
	}

	void train(string savePath) {
		extraRequirement.logFile << "��ʼѵ��,Ϊÿ��Ŀ�꽨��һ�����ɭ�ֻع�ģ��" << endl;
		string dataTime = extraRequirement.makeDateDir(savePath + "\\model\\RandomForest");
		for (int i = 0; i < train_label.cols; ++i) {
			Mat temp = Mat::zeros(train_label.rows, 1, CV_32FC1);
			temp = train_label.col(i).clone();
			Ptr<TrainData> tData = TrainData::create(train_data, ROW_SAMPLE, temp);
			rtrees->train(tData);
			extraRequirement.logFile << "ѵ����" << to_string(i + 1) << "�����" << endl;
			extraRequirement.logFile << "����ѵ�������ɲ鿴�����ļ�" << endl;
			extraRequirement.logFile << "������" << dataTime + "label_" + to_string(i + 1) + ".xml" << endl;
			save(dataTime + "label_" + to_string(i + 1) + ".xml");
		}
	}

	void save(string filename) {
		rtrees->save(filename);
	}

	void load(string filename) {
		extraRequirement.logFile << "��" << filename << "�м���ģ��";
		rtrees = ml::RTrees::load(filename);
		extraRequirement.logFile << "�������" << endl;
	}
	//���ǵ�simulink�������⣬�ú����ѷ���
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
				pred.at<float>(i, 0) = rtrees->predict(sample);
			}
			catch (cv::Exception& e) {
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
	Ptr<RTrees> rtrees;
	Mat train_data, train_label;
	void init() {
		extraRequirement.logFile << "��ʼ��RandomForestģ��" << endl;
		rtrees = RTrees::create();
		//�������������
		rtrees->setMaxDepth(15);
		//�ڵ���С��������
		rtrees->setMinSampleCount(10);
		//�ع�������ֹ��׼
		rtrees->setRegressionAccuracy(0);
		//�Ƿ���������ѵ�
		rtrees->setUseSurrogates(false);
		//���������
		rtrees->setMaxCategories(15);
		//�������������
		rtrees->setPriors(Mat());
		//����ı�����Ҫ��
		rtrees->setCalculateVarImportance(true);
		//���ڵ����ѡ��������Ӽ��Ĵ�С
		rtrees->setActiveVarCount(4);
		//��ֹ��׼
		rtrees->setTermCriteria(TermCriteria(TermCriteria::MAX_ITER + (0.01f > 0 ? TermCriteria::EPS : 0), 100, 0.01f));

	}

	void sparseDataAndLabel(const vector<vector<float>>&floatData, int dataSize, int labelSize) {
		extraRequirement.logFile << "�ָת�����ݣ�����ת��ΪRandomForest���ø�ʽ" << endl;
		for (int i = 0; i < floatData.size(); ++i) {
			for (int j = 0, k = 0, l = 0; j < floatData[i].size(); ++j) {
				if (j<dataSize)
					train_data.at<float>(i, k++) = floatData[i][j];
				else if (j<labelSize + dataSize)
					train_label.at<float>(i, l++) = floatData[i][j];
			}
		}
	}

};
