//frameWork.cpp : Defines the entry point for the console application.


#include "stdafx.h"
#include "getData.h"
#include "model.h"
#include "svmModel.h"
#include "randomForest.h"


struct INPUTDATA
{
	double	d1;
	double	d2;
	double	d3;
	double	d4;
	double	d5;
};
// 
struct OUTPUTDATA
{
	double t1;
	double t2;
	double t3;
};

static vector<Ptr<ml::SVM>> svmVec;
static vector<Ptr<ml::RTrees>> randomForestVec;

void train_process(Model* model, string dirPath) {
	model->train(dirPath);
	delete model;
}

void predict_process(INPUTDATA& data, int dataLong, char* initFile) {
	ifstream init(initFile);
	vector<float>nVec(2 * dataLong, 0.0);
	for (int i = 0; i < dataLong * 2; ++i)
		init >> nVec[i];
	init.close();
	data.d1 = (data.d1 - nVec[0]) / nVec[0 + dataLong];
	data.d2 = (data.d2 - nVec[1]) / nVec[1 + dataLong];
	data.d3 = (data.d3 - nVec[2]) / nVec[2 + dataLong];
	data.d4 = (data.d4 - nVec[3]) / nVec[3 + dataLong];
	data.d5 = (data.d5 - nVec[4]) / nVec[4 + dataLong];

}

int train(char* dirPath, char* logPath, int* dataRange, int ndR, int* labelRange, int nlR, int* parameters, int nP) {
	getData data(dirPath, logPath, dataRange, ndR, labelRange, nlR, parameters, nP);
	if (data.dataPath.size() == 0)
		return 1;
	data.mainProcess(true, true);
	if (parameters[0] == 0) {
		Model* model = new RandomForestModel(data.floatData, data.dataIndex.size(), data.labelIndex.size(), data.getLogPath());
		train_process(model, dirPath);
	}
	else if (parameters[0] == 1) {
		Model* model = new SvmModel(data.floatData, data.dataIndex.size(), data.labelIndex.size(), data.getLogPath());
		train_process(model, dirPath);
	}
	return 0;
}

void loadModel(char* modelPath, int whichModel) {
	vector<cv::String>temp;
	glob(modelPath, temp, false);
	for (int i = 0; i < temp.size(); ++i) {
		if (temp[i].find_last_of(".xml") == (temp[i].size() - 1)) {
			if (whichModel == 1 && temp[i].find("SVM") != -1) {
				Ptr<ml::SVM> svmTemp = ml::SVM::create();
				svmTemp = ml::SVM::load(temp[i]);
				svmVec.push_back(svmTemp);
			}
			else if (whichModel == 0 && temp[i].find("RandomForest") != -1) {
				Ptr<ml::RTrees> randomForestTemp = ml::RTrees::create();
				randomForestTemp = ml::RTrees::load(temp[i]);
				randomForestVec.push_back(randomForestTemp);
			}
		}
	}
}

void predict(INPUTDATA data, int dataLong, char* initPath, OUTPUTDATA& output) {
	//debugʱʹ��file�����������ֵ��
	//ofstream file;
	//file << data.d1;
	predict_process(data, dataLong, initPath);

	//file.open("E:\\solve\\frameWork\\frameWork\\data\\12.txt", ios::app);
	Mat sample = Mat::zeros(1, dataLong, CV_32FC1);

	sample.at<float>(0, 0) = data.d1;
	sample.at<float>(0, 1) = data.d2;
	sample.at<float>(0, 2) = data.d3;
	sample.at<float>(0, 3) = data.d4;
	sample.at<float>(0, 4) = data.d5;

	if (svmVec.size() != 0) {
		output.t1 = svmVec[0]->predict(sample);
		output.t2 = svmVec[1]->predict(sample);
		output.t3 = svmVec[2]->predict(sample);
	}
	else {
		output.t1 = randomForestVec[0]->predict(sample);
		output.t2 = randomForestVec[1]->predict(sample);
		output.t3 = randomForestVec[2]->predict(sample);
	}

	//for (int i = 0; i < svmVec.size(); ++i) {
	//	file << svmVec[i]->predict(sample) << '\t';
	//}
	//file<<sample<<endl;
	//file.close();

}
/*
���㷨�����ظ��������⣬�ú����ѷ���
float predict(char* initPath,float* data, int dataLong, int whichModel,char* modelPath) {
predict_process(data, dataLong, initPath);//��Ԥ�����ݽ���Ԥ����


if (whichModel == 1) {
try {
Ptr<ml::SVM> svm = ml::SVM::create();
svm	= ml::SVM::load(modelPath);
Mat sample = Mat::zeros(1, dataLong, CV_32FC1);
for (int i = 0; i<dataLong; ++i)
sample.at<float>(0,i) = data[i];
return svm->predict(sample);
}
catch (cv::Exception& e) {
return 1.0;
}
}
else if (whichModel == 0) {
try {
Ptr<ml::RTrees> randomForest = ml::RTrees::load(modelPath);
Mat sample = Mat::zeros(dataLong, 1, CV_32FC1);
for (int i = 0; i<dataLong; ++i)
sample.at<float>(i, 0) = data[i];
return randomForest->predict(sample);
}
catch (cv::Exception& e) {
return 1.0;
}
}
return 2;

}
*/


/*
���ǵ�simulinkʵʱ��Ҫ�󣬸ú����ѷ���

int predict_process(Model* model, string modelPath,int nlR) {
auto pred = model->predict(model->getTrainData(), modelPath);
if (model->err)
return model->err + 1;
string temp = string(modelPath);
model->saveCsv(pred, temp.replace(temp.find(".xml"), string::size_type(4), "_Pred" + string(".csv")));

if (nlR != 0)
model->saveCsv(model->getTrainLabel(), temp.replace(temp.find("Pred"), string::size_type(4), "Target"));
delete model;
return 0;
}

int predict(char* dirPath, char* logPath ,char* modelPath, int* dataRange, int ndR, int* labelRange, int nlR, int* parameters, int nP) {
getData data(dirPath,logPath, dataRange, ndR, labelRange, nlR, parameters, nP);
if (data.dataPath.size() == 0)
return 1;
data.mainProcess(true, true);
if (parameters[0] == 0) {
Model* model = new RandomForestModel(data.floatData, data.dataIndex.size(), data.labelIndex.size(), data.getLogPath());
return predict_process(model, modelPath,nlR);
}
else if (parameters[0] == 1) {
Model* model = new SvmModel(data.floatData, data.dataIndex.size(), data.labelIndex.size(), data.getLogPath());
return predict_process(model, modelPath, nlR);
}
}

*/



int main()
{
	/*
	*		����Ϊ���������������вο�
	*
	*		train�ӿڽ���8������
	*			dirPath����Ŀ·��
	*			logPath����־�ļ�·��
	*			dataRange��ѵ�������еڼ���Ϊ����
	*			ndR��dataRange����
	*			labelRange��ѵ�������еڼ���Ϊ���
	*			nlR��labelRange����								����ͼ����2��5��6��9��10����5��Ϊ���룬��3��4��7��������Ϊ�����
	*															��Ϊ�������������˹����������㷨��Ҫ�ֱ𱣴�ģ��ϵ����
	*
	*			parameters�������֣��˴�����Ϊparameters[0] ==1 ��ʹ��SVM�㷨��parameters[0] ==0 ��ʹ��RandomForest�㷨
	*			nP��parameters����
	*
	*			ע��
	*					train�ӿڽ���Ϊ����ѵ��ʹ�ã����㷨ѵ��Ϊ���Բ��ɷֹ��̣���matlab/simulink�Ĳ������л��Ʋ����ݣ��ʽ��ṩ�ӿڣ���matlab�е���train_dll�ű�����
	*
	*
	*
	*		loadModel�ӿڽ�����������
	*			modelPath���㷨ϵ����ַ����Ԥ��H����ϵ�����ã�ע��������ģ��Ӧ��parametersѡ��ģ��һ�¡�
	*			whichModel��ֵΪ1ʱѡ��SVM��Ϊ0ʱѡ��RandomForest
	*
	*		predict�ӿڽ����ĸ�����
	*			INPUTDATA:�����������ͣ����Զ���
	*			datalong���������ݳ��ȣ�ѵ�������������ȡ�INPUTDATA���ݳ��ȡ���ֵ�뱣��һ��
	*			initPath: ��Ԥ�����ݽ���Ԥ����ʹ�ã��˴�����Ϊ��ֵ�ͱ�׼��ֵ
	*			OUTPUTDATA��������ݣ����Զ��壬����ģ����������һ��
	*
	*
	*
	*/


	char* dirPath = "E:\\solve\\frameWork\\frameWork";
	char* modelPath = "E:\\solve\\frameWork\\frameWork\\model\\SVM\\11M_13D_11_H";
	char* logPath = "E:\\solve\\frameWork\\frameWork\\log";
	char* initPath = "E:\\solve\\frameWork\\frameWork\\data\\init.txt";
	int dataRange[] = { 2,5,6,9,10 };
	int ndR = 5;
	int labelRange[] = { 3,4,7 };
	int nlR = 3;
	int parameters[] = { 1 };
	int nP = 1;
	INPUTDATA data{ 0,21.2,21.5,40.,0 };
	OUTPUTDATA output{ 0.0,0,0 };
	train(dirPath, logPath, dataRange, ndR, labelRange, nlR, parameters, nP);
	loadModel(modelPath, 1);
	predict(data, 5, initPath, output);
	return 0;
}

