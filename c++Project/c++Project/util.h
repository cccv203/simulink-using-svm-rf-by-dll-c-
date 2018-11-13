#pragma once
#include <io.h>
#include <direct.h>
#include <ctime>
#include <string>
#include <fstream>
#include <iostream>
#pragma warning(disable : 4996)

using namespace std;

class util {
public:
	ofstream logFile;
	ofstream initFile;
	time_t now = time(0);
	tm *ltm = localtime(&now);


	string makeDateDir(const string filePath) {
		string dateTime = to_string(ltm->tm_mon + 1) + "M_" + to_string(ltm->tm_mday) + "D_" + to_string(ltm->tm_hour)+"_H";
		mk_dir(filePath+"\\" + dateTime + "\\");
		string hourTime = to_string(ltm->tm_hour)+"H_"+to_string(ltm->tm_min) +"M_" + to_string(ltm->tm_sec)+"S";
		return filePath + "\\" + dateTime + "\\" + hourTime;

	}

	~util() {
		logFile.close();
		initFile.close();
	}
private:
	tm* get_date() {
		return ltm;
	}

	void mk_dir(string path) {

		if (_access(path.c_str(), 0) == -1) {
			_mkdir(path.c_str());

		}

	}
};
