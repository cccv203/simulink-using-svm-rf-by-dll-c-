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
	vector<vector<float>>floatData;  //����Ϊfloat����
	vector<cv::String> dataPath;  //��������·��
	vector<int> dataIndex;  //ȫ������������ΪdataRange��labelRange�ĺ�
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
		dir_debug();  //��ʼ��ȡ���ݣ����ļ���Ϣ���뵽��־�ļ���
		setIndex();   //�õ�ȫ����������
		getFloatData();//��ȡ����ϴ�����ˡ�ת�����ݸ�ʽ
		getFloatDataHead();//�������ͷ����־�ļ���
		preProcess pre(floatData, getInitPath());//�����ݽ���Ԥ����
		pre.process(floatData, dataIndex.size(), debugBool);
		extraRequirement.logFile <<"��������Ԥ�����,";
		getFloatDataHead();//���Ԥ���������ݵ�ͷ�ļ���
	}


private:


	void setIndex() {
		//�õ�ȫ��������Ϊ֮��������ϴ��׼��
		for (int i = 0; i < dataIndex.size(); ++i)
			index.push_back(dataIndex[i]);
		for (int i = 0; i < labelIndex.size(); ++i)
			index.push_back(labelIndex[i]);
		sort(index.begin(), index.end());
	}

	void getDataPath(char* dirPath) {
		//�õ�ȫ������·��
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
		//ģ�巽�����������־
		for (int i = 0; i < data.size(); ++i)
			extraRequirement.logFile<< data[i] << '\t';
		extraRequirement.logFile << endl;
	}

	void getRawData(vector<vector<string>>&data) {
		//��dirPath�еõ�ȫ��csv�ļ�����ȡ��data��
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
		extraRequirement.logFile<< "��ȡ�ı�������ϣ����а�������" << data.size() << "�� " << data[0].size() << "��" << endl;
	}

	bool check_line_str(const vector<string>& lineStr, const vector<int>& index) {
		//������ϴ���˴�Ϊ����ȱʧ��������
		for (int i = 0; i < index.size(); ++i) {
			if (lineStr.size() < *max_element(index.begin(),index.end()) + 1 || lineStr[index[i]] == "")
				return false;
		}
		return true;
	}

	void getFloatData() {
		//���ݹ��ˡ���ϴ��ת����ʽ
		vector<vector<string>>strData;
		getRawData(strData);
		extraRequirement.logFile<< "��ʼ���ˡ���ϴ����" << endl;
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
		extraRequirement.logFile << "ת�����,���а�����������" << floatData.size() << "�� " << floatData[0].size() << "��" << endl;
	}

	void dir_debug() {
		extraRequirement.logFile << "��ʼ��ȡ����,�����ļ�����:\n";
		printInf(dataPath);
		extraRequirement.logFile << "trainIndex����:\n";
		printInf(dataIndex);
		extraRequirement.logFile<< "labelIndex����:\n";
		printInf(labelIndex);
	}

	void getFloatDataHead() {
		extraRequirement.logFile << "����ͷ���£�" << endl;
		for (int i = 0; i < HEAD; ++i) {
			for (int j = 0; j < floatData[0].size(); ++j)
				extraRequirement.logFile << floatData[i][j] << '\t';
			extraRequirement.logFile << endl;
		}
	}
};