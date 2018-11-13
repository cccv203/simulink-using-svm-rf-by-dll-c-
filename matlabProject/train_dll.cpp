#include <stdio.h>
#include "mex.h"
#include "interface.h"
#include <windows.h>
#include "simstruc.h"

typedef void (* MYFUN1)(char dirPath[] ,char logPath[] ,double dataRange[],int ndR
        ,double labelRange[],int nlR, double parameters[],int nP);

void mexFunction(int nlhs,mxArray* plhs[],int nrhs,mxArray* prhs[])
{
    char* dirPath= mxArrayToString(prhs[0]);
    char* logPath= mxArrayToString(prhs[1]);
    double* dataRange = (double*)mxGetPr(prhs[2]);
    int ndR = mxGetScalar(prhs[3]);
    double* labelRange = (double*)mxGetPr(prhs[4]);
    int nlR = mxGetScalar(prhs[5]);
    double* parameters =(double*)mxGetPr(prhs[6]);
    int nP = mxGetScalar(prhs[7]);     
    HINSTANCE hDLL = NULL;
    MYFUN1 dlltrain = NULL;
    
    hDLL = LoadLibrary("frameWork.dll");
    dlltrain = (MYFUN1)GetProcAddress(hDLL,"train");
    
    dlltrain(dirPath,logPath, dataRange, ndR, labelRange, nlR, parameters, nP);
        
    return;
}










