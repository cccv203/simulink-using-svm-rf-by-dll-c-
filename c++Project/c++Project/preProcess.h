#pragma once

#include <vector>

#include "util.h"
using namespace std;

class preProcess {
public:
	util extraRequirement;
	int samples;
	vector<float> sumData;
	vector<float> meanData;
	vector<float> stdData;
	void setLogFilePath(string filePath) {
		extraRequirement.logFile.open(filePath, ios::app);
	}
	void setInitFilePath(string filePath) {
		extraRequirement.initFile.open(filePath, ios::app|ios::in);
	}

	preProcess(vector<vector<float>>& dVec,string filePath) {
		samples = dVec.size();
		int n = dVec[0].size();
		sumData = vector<float>(n, 0);
		meanData = vector<float>(n, 0);
		stdData = vector<float>(n, 0);
		setInitFilePath(filePath);
	}
	void process(vector<vector<float>>& dVec, const int dataIndex,bool debugBool) {
		getSum(dVec);  
		getMean(dVec);
		getStd(dVec);
		debug(dataIndex);  //输出debug信息到日志文件中
		for (int i = 0; i < dVec.size(); ++i) {
			for (int j = 0; j < dataIndex; ++j) {
				//if (find(dataIndex.begin(), dataIndex.end(), j) != dataIndex.end())
					dVec[i][j] = (dVec[i][j] - meanData[j]) / stdData[j];
			}
		}
	}

	
private:

	template<typename T>
	void printInf(const vector<T> data,const int dataIndex) {
		for (int i = 0; i < dataIndex; ++i)
			extraRequirement.initFile << data[i] << ' ';
		extraRequirement.initFile << endl;
	}

	void debug(const int dataIndex) {
		//extraRequirement.logFile << "样本每个特征总和为:";
		//printInf(sumData);
		//extraRequirement.logFile << "样本每个特征平均值为:";
		printInf(meanData, dataIndex);
		//extraRequirement.logFile << "样本每个特征标准差为:";
		printInf(stdData, dataIndex);
	}

	void getSum(const vector<vector<float>>& dVec) {
		for (int i = 0; i < dVec.size(); ++i) {
			for (int j = 0; j < dVec[0].size(); ++j) {
				sumData[j] += dVec[i][j];
			}
		}
	}

	void getMean(const vector<vector<float>>& dVec) {
		for (int i = 0; i < meanData.size(); ++i) {
			meanData[i] = sumData[i] / samples;
		}
	}

	void getStd(const vector<vector<float>>& dVec) {
		for (int i = 0; i < dVec.size(); ++i) {
			for (int j = 0; j < dVec[0].size(); ++j) {
				stdData[j] += pow(dVec[i][j] - meanData[j], 2);
			}
		}
		for (int i = 0; i < stdData.size(); ++i) {
			stdData[i] = sqrt(stdData[i] / (samples - 1));
		}
	}


};

