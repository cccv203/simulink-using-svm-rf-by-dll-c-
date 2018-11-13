initPath = 'E:\\solve\\frameWork\\frameWork\\data\\init.txt';
modelPath = 'E:\\solve\\frameWork\\frameWork\\model\\SVM\\11M_12D_17_H\\17H_13M_1Slabel_1.xml';
data = [1.0 2.0 3.0 4.0 5.0];

fun_initPath =  libpointer('cstring',initPath);
fun_data = libpointer('singlePtr',data);
fun_modelPath = libpointer('cstring',modelPath);
y = 

loadlibrary('frameWork','interface.h');
calllib('frameWork','loadModel','E:\\solve\\frameWork\\frameWork\\model\\SVM\\11M_13D_11_H', 1);

data = [ 10000,0.0,222.0,3.1,333.2];
output = [0,0,0];
fun_data = libpointer('singlePtr',data);
fun_output = libpointer('singlePtr',output);
calllib('frameWork','predict',fun_data,5,'E:\\solve\\frameWork\\frameWork\\data\\init.txt',fun_output);
get(fun_output,'Value');
