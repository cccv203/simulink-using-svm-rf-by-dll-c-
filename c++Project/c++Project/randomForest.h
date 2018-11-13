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
		extraRequirement.logFile << "开始训练,为每个目标建立一个随机森林回归模型" << endl;
		string dataTime = extraRequirement.makeDateDir(savePath + "\\model\\RandomForest");
		for (int i = 0; i < train_label.cols; ++i) {
			Mat temp = Mat::zeros(train_label.rows, 1, CV_32FC1);
			temp = train_label.col(i).clone();
			Ptr<TrainData> tData = TrainData::create(train_data, ROW_SAMPLE, temp);
			rtrees->train(tData);
			extraRequirement.logFile << "训练第" << to_string(i + 1) << "个完成" << endl;
			extraRequirement.logFile << "具体训练参数可查看保存文件" << endl;
			extraRequirement.logFile << "保存在" << dataTime + "label_" + to_string(i + 1) + ".xml" << endl;
			save(dataTime + "label_" + to_string(i + 1) + ".xml");
		}
	}

	void save(string filename) {
		rtrees->save(filename);
	}

	void load(string filename) {
		extraRequirement.logFile << "从" << filename << "中加载模型";
		rtrees = ml::RTrees::load(filename);
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
		extraRequirement.logFile << "初始化RandomForest模型" << endl;
		rtrees = RTrees::create();
		//树的最大可能深度
		rtrees->setMaxDepth(15);
		//节点最小样本数量
		rtrees->setMinSampleCount(10);
		//回归树的终止标准
		rtrees->setRegressionAccuracy(0);
		//是否建立替代分裂点
		rtrees->setUseSurrogates(false);
		//最大聚类簇数
		rtrees->setMaxCategories(15);
		//先验类概率数组
		rtrees->setPriors(Mat());
		//计算的变量重要性
		rtrees->setCalculateVarImportance(true);
		//树节点随机选择的特征子集的大小
		rtrees->setActiveVarCount(4);
		//终止标准
		rtrees->setTermCriteria(TermCriteria(TermCriteria::MAX_ITER + (0.01f > 0 ? TermCriteria::EPS : 0), 100, 0.01f));

	}

	void sparseDataAndLabel(const vector<vector<float>>&floatData, int dataSize, int labelSize) {
		extraRequirement.logFile << "分割并转换数据，将其转换为RandomForest可用格式" << endl;
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
