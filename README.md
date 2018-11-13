# simulink-using-svm-rf-by-dll-c-

## **`这是一个使用matlab调用c++生成的dll库样例。`**<br>
**c++主要实现了svm和RandomForest两种算法（opencv库），通过dll封装，使用matlab调用的过程。<br>
主要服务于经常使用simulink的传统形公司。
其中训练为matlab脚本所写，进行线下训练，预测封装为simulink模块可进行联合仿真。<br>
主要使用过程参见 主要流程.docx。<br>**
<br>
<br>
<br>
<br>
### 记录一些坑：<br>
#### 一、需求不明，一开始以为训练与预测皆线下，导致预测部分重写多次。<br>
#### 二、训练脚本编写不易，Matlab自带的loadlibrary不支持结构体（或者我写错了），导致一开始的直接loadlibrary + calllib失败，无奈编写mex函数。<br>
#### 三、预测写为S-function，一开始准备参数使用指针传递，结果经过测试发现，指针的值传递失败，后改为结构体并使用引用传递。<br>
#### 四、c++部分习惯使用int*，结果matlab默认double*，随可强制类型转换，但丢失数据导致错误不可运行。<br>
#### 五、因c++->dll->simulink导致多数文件内容重复，仅后缀名不同，导致后续链接错误或易发生命名冲突等问题，注意修改。<br>
