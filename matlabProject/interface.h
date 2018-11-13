#pragma once

#ifdef __cplusplus
extern "C" {
#endif
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

	__declspec(dllexport) int train(char* dirPath, char* logPath, double* dataRange, int ndR, 
		double* labelRange, int nlR, double* parameters, int nP);
	__declspec(dllexport) void loadModel(char* modelPath, int whichModel);
	__declspec(dllexport) void predict(INPUTDATA data, int dataLong, char* initPath, OUTPUTDATA& output);
#ifdef __cplusplus
}
#endif

