#include "stdafx.h"
////
//#include "interface.h"
//#include "getData.h"
//#include "model.h"
//#include "svmModel.h"
//#include "randomForest.h"
//#include <iostream>
////
//
//
//static vector<Ptr<ml::SVM>> svmVec;
//static vector<Ptr<ml::RTrees>> randomForestVec;
//
//void train_process(Model* model, string dirPath) {
//	model->train(dirPath);
//	delete model;
//}
//
//
//
//void predict_process(INPUTDATA& data, int dataLong, char* initFile) {
//	ifstream init(initFile);
//	vector<float>nVec(2 * dataLong, 0.0);
//	for (int i = 0; i < dataLong * 2; ++i)
//		init >> nVec[i];
//	init.close();
//	data.d1 = (data.d1 - nVec[0]) / nVec[0 + dataLong];
//	data.d2 = (data.d2 - nVec[1]) / nVec[1 + dataLong];
//	data.d3 = (data.d3 - nVec[2]) / nVec[2 + dataLong];
//	data.d4 = (data.d4 - nVec[3]) / nVec[3 + dataLong];
//	data.d5 = (data.d5 - nVec[4]) / nVec[4 + dataLong];
//
//}
//
//int train(char* dirPath, char* logPath, double* dataRange, int ndR, double* labelRange, int nlR, double* parameters, int nP) {
//	getData data(dirPath, logPath, dataRange, ndR, labelRange, nlR, parameters, nP);
//	if (data.dataPath.size() == 0)
//		return 1;
//	data.mainProcess(true, true);
//	if (parameters[0] == 0) {
//		Model* model = new RandomForestModel(data.floatData, data.dataIndex.size(), data.labelIndex.size(), data.getLogPath());
//		train_process(model, dirPath);
//	}
//	else if (parameters[0] == 1) {
//		Model* model = new SvmModel(data.floatData, data.dataIndex.size(), data.labelIndex.size(), data.getLogPath());
//		train_process(model, dirPath);
//	}
//	return 0;
//}
//
//void loadModel(char* modelPath, int whichModel) {
//	vector<cv::String>temp;
//	glob(modelPath, temp, false);
//	for (int i = 0; i < temp.size(); ++i) {
//		if (temp[i].find_last_of(".xml") == (temp[i].size() - 1)) {
//			if (whichModel == 1 && temp[i].find("SVM") != -1) {
//				Ptr<ml::SVM> svmTemp = ml::SVM::create();
//				svmTemp = ml::SVM::load(temp[i]);
//				svmVec.push_back(svmTemp);
//			}
//			else if (whichModel == 0 && temp[i].find("RandomForest") != -1) {
//				Ptr<ml::RTrees> randomForestTemp = ml::RTrees::create();
//				randomForestTemp = ml::RTrees::load(temp[i]);
//				randomForestVec.push_back(randomForestTemp);
//			}
//		}
//	}
//}
//
//void predict(INPUTDATA data, int dataLong, char* initPath, OUTPUTDATA& output) {
//	//ofstream file;
//	//file << data.d1;
//	predict_process(data, dataLong, initPath);
//	
//	//file.open("E:\\solve\\frameWork\\frameWork\\data\\12.txt", ios::app);
//	Mat sample = Mat::zeros(1, dataLong, CV_32FC1);
//
//	sample.at<float>(0, 0) = data.d1;
//	sample.at<float>(0, 1) = data.d2;
//	sample.at<float>(0, 2) = data.d3;
//	sample.at<float>(0, 3) = data.d4;
//	sample.at<float>(0, 4) = data.d5;
//
//	if (svmVec.size() != 0) {
//		output.t1 = svmVec[0]->predict(sample);
//		output.t2 = svmVec[1]->predict(sample);
//		output.t3 = svmVec[2]->predict(sample);
//	}
//	else {
//		output.t1 = randomForestVec[0]->predict(sample);
//		output.t2 = randomForestVec[1]->predict(sample);
//		output.t3 = randomForestVec[2]->predict(sample);
//	}
//
//	//for (int i = 0; i < svmVec.size(); ++i) {
//	//	file << svmVec[i]->predict(sample) << '\t';
//	//}
//	//file<<sample<<endl;
//	//file.close();
//
//}