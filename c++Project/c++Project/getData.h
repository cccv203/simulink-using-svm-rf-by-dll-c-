#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

#include "preProcess.h"
#include "opencv2\opencv.hpp"

#include "util.h"

using namespace std;
using namespace cv;

class getData {
public:
	vector<vector<float>>floatData;  //保存为float数据
	vector<cv::String> dataPath;  //保存数据路径
	vector<int> dataIndex;  //全部数据列数，为dataRange与labelRange的和
	vector<int> labelIndex;  
	vector<int> index;
	string logFile;
	string initPath;
	util extraRequirement;
	
	int HEAD = 5;

	getData(char* dirPath, char* logPath, double* dataRange, int ndR, double* labelRange, int nlR, double* parameters, int nP) {
		getDataPath(dirPath);
		getIndex(dataRange, ndR, dataIndex);
		getIndex(labelRange, nlR, labelIndex);
		getParameters(parameters, nP);
		logFile = extraRequirement.makeDateDir(logPath) + ".txt";
		initPath = dirPath + string("\\data\\init.txt");
		extraRequirement.logFile.open(logFile,ios::app);
		
	}
	string getLogPath() {
		return logFile;
	}
	string getInitPath() {
		return initPath;
	}
	void mainProcess(bool process,bool debugBool){
		dir_debug();  //开始读取数据，将文件信息输入到日志文件中
		setIndex();   //得到全部数据列数
		getFloatData();//读取、清洗、过滤、转换数据格式
		getFloatDataHead();//输出数据头到日志文件中
		preProcess pre(floatData, getInitPath());//对数据进行预处理
		pre.process(floatData, dataIndex.size(), debugBool);
		extraRequirement.logFile <<"经过数据预处理后,";
		getFloatDataHead();//输出预处理后的数据到头文件中
	}


private:


	void setIndex() {
		//得到全部列数，为之后数据清洗做准备
		for (int i = 0; i < dataIndex.size(); ++i)
			index.push_back(dataIndex[i]);
		for (int i = 0; i < labelIndex.size(); ++i)
			index.push_back(labelIndex[i]);
		sort(index.begin(), index.end());
	}

	void getDataPath(char* dirPath) {
		//得到全部数据路径
		vector<cv::String>temp;
		glob(dirPath+string("\\data"), temp, false);
		for (int i = 0; i < temp.size(); ++i) {
			if (temp[i].find_last_of(".csv") == (temp[i].size()-1))
				dataPath.push_back(temp[i]);
		}
	}

	void getIndex(double* range, int nR, vector<int>& index) {
		for (int i = 0; i < nR; ++i) 
			index.push_back(int(range[i]));
	}

	void getParameters(double* parameters, int nP) {
		return;
	}

	template<typename T>
	void printInf(const vector<T> data) {
		//模板方法进行输出日志
		for (int i = 0; i < data.size(); ++i)
			extraRequirement.logFile<< data[i] << '\t';
		extraRequirement.logFile << endl;
	}

	void getRawData(vector<vector<string>>&data) {
		//从dirPath中得到全部csv文件并读取到data中
		for (int i = 0; i < dataPath.size(); ++i) {
			string lineStr;
			ifstream file(dataPath[i], ios::in);
			bool firstRow = true;
			while (getline(file, lineStr)) {
				if (firstRow) {
					firstRow = false;
					continue;
				}
				stringstream ss(lineStr);
				string str;
				vector<string> lineArray;
				while (getline(ss, str, ','))
					lineArray.push_back(str);
				data.push_back(lineArray);
			}
			file.close();
		}
		extraRequirement.logFile<< "读取文本数据完毕，其中包含数据" << data.size() << "行 " << data[0].size() << "列" << endl;
	}

	bool check_line_str(const vector<string>& lineStr, const vector<int>& index) {
		//数据清洗，此处为存在缺失样本则丢弃
		for (int i = 0; i < index.size(); ++i) {
			if (lineStr.size() < *max_element(index.begin(),index.end()) + 1 || lineStr[index[i]] == "")
				return false;
		}
		return true;
	}

	void getFloatData() {
		//数据过滤、清洗、转换格式
		vector<vector<string>>strData;
		getRawData(strData);
		extraRequirement.logFile<< "开始过滤、清洗数据" << endl;
		for (int i = 0; i < strData.size(); ++i) {
			if (check_line_str(strData[i], index)) {
				vector<float> floatTemp;
				for (int j = 0; j < dataIndex.size(); ++j)
					floatTemp.push_back(stof(strData[i][dataIndex[j]]));
				for (int j = 0; j < labelIndex.size(); ++j)
					floatTemp.push_back(stof(strData[i][labelIndex[j]]));
				floatData.push_back(floatTemp);
			}
		}
		extraRequirement.logFile << "转换完毕,其中包含可用数据" << floatData.size() << "行 " << floatData[0].size() << "列" << endl;
	}

	void dir_debug() {
		extraRequirement.logFile << "开始读取数据,数据文件如下:\n";
		printInf(dataPath);
		extraRequirement.logFile << "trainIndex如下:\n";
		printInf(dataIndex);
		extraRequirement.logFile<< "labelIndex如下:\n";
		printInf(labelIndex);
	}

	void getFloatDataHead() {
		extraRequirement.logFile << "数据头如下：" << endl;
		for (int i = 0; i < HEAD; ++i) {
			for (int j = 0; j < floatData[0].size(); ++j)
				extraRequirement.logFile << floatData[i][j] << '\t';
			extraRequirement.logFile << endl;
		}
	}
};