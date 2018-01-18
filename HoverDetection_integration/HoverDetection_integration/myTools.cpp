#include "myHeaders.h"

extern ofstream out;
/*
 * ���ܣ�������־�ļ�������ʵʱ������������״̬����־�ļ��ͳ�����ͬһ·���£�ǰ׺�ͳ�����ͬ����׺Ϊ.log��
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
 * ���ܣ���ȡ�����exe�ļ�����·���������ڳ���·�����½�һ��xml�ļ����������ݿ��е����ô���xml�ļ���
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
 * ���ܣ��滻�ַ������е��Ӵ��������滻������ַ���
 * ������str-Դ�ַ���  a-Ŀ���Ӵ�  b-�����滻a���Ӵ�
 */
char* replace(const char* str, const char* a, const char* b){
	char *result = new char[1024];
	int index = 0; //result�Ķ���ָ��
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
 * ���ܣ��ָ��ַ�������vector����ʽ����
 * ������str-���ָ���ַ���  delimiter-�ָ���
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
 * ���ܣ�������������㣬���������ľ���	
 */
int CalcPointsDistance(Point start, Point end){
	return (int)sqrt(abs(start.x - end.x)*abs(start.x - end.x) + abs(start.y - end.y)*abs(start.y - end.y));
}
