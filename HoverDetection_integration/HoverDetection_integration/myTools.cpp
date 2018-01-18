#include "myHeaders.h"

extern ofstream out;
/*
 * 功能：生成日志文件，用于实时输出程序的运行状态。日志文件和程序在同一路径下，前缀和程序相同，后缀为.log。
 */
char* GetLogFileName(){
	TCHAR *chBuf = new TCHAR[128];
	GetModuleFileName(NULL, chBuf, 1024);
	int length = WideCharToMultiByte(CP_ACP, 0, chBuf, -1, NULL, 0, NULL, NULL);
	char *fullname = new char[length];
	WideCharToMultiByte(CP_ACP, 0, chBuf, -1, fullname, length, NULL, NULL);
	vector<char*> strArr = split(fullname, "\\");
	char * logFile = new char[256];
	logFile[0] = '\0';
	for (int i = 0; i < strArr.size() - 1; i++){
		strcat(logFile, strArr[i]);
		strcat(logFile, "\\");
	}
	char * exeName = strtok(strArr[strArr.size() - 1], ".");
	strcat(logFile, exeName);
	strcat(logFile, ".log");
	delete[] chBuf;
	delete[] fullname;

	return logFile;
}


/*
 * 功能：获取程序的exe文件所在路径，用于在程序路径下新建一个xml文件，并将数据库中的配置存入xml文件中
 */
char* GetCurrentDir(){
	TCHAR *chBuf = new TCHAR[128];
	GetModuleFileName(NULL, chBuf, 1024);
	int length = WideCharToMultiByte(CP_ACP, 0, chBuf, -1, NULL, 0, NULL, NULL);
	char *fullname = new char[length];
	WideCharToMultiByte(CP_ACP, 0, chBuf, -1, fullname, length, NULL, NULL);
	vector<char*> strArr = split(fullname, "\\");
	char * directory = new char[256];
	directory[0] = '\0';
	for (int i = 0; i < strArr.size() - 1; i++){
		strcat(directory, strArr[i]);
		strcat(directory, "\\");
	}
	delete[] chBuf;
	delete[] fullname;
	out << "Current directory : " << directory << endl;
	cout << "Current directory : " << directory << endl;
	return directory;
}


/*
 * 功能：替换字符串的中的子串，返回替换后的新字符串
 * 参数：str-源字符串  a-目标子串  b-用以替换a的子串
 */
char* replace(const char* str, const char* a, const char* b){
	char *result = new char[1024];
	int index = 0; //result的队列指针
	int length_a = strlen(a);
	int length_b = strlen(b);
	for (int i = 0; str[i] != '\0'; i++, index++){
		if (strncmp(str + i, a, length_a) == 0){
			for (int j = 0; j < length_b; j++)
				result[index + j] = b[j];
			result[index + length_b] = '\0';
			index += length_b - 1;
			i += length_a - 1;
		}
		else{
			result[index] = str[i];
			result[index + 1] = '\0';
		}
	}
	return result;
}


/*
 * 功能：分割字符串，以vector的形式返回
 * 参数：str-待分割的字符串  delimiter-分隔符
 */
vector<char*> split(char * str, char* delimiter){
	vector<char*> strArr;
	char * tmp = strtok(str, delimiter);
	while (tmp){
		strArr.push_back(tmp);
		tmp = strtok(NULL, delimiter);
	}
	return strArr;
}


/*
 * 功能：输入两个坐标点，返回两点间的距离	
 */
int CalcPointsDistance(Point start, Point end){
	return (int)sqrt(abs(start.x - end.x)*abs(start.x - end.x) + abs(start.y - end.y)*abs(start.y - end.y));
}
