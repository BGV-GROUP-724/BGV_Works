#include "myHeaders.h"
ofstream out;
char * alarmInfo = new char[1024];

//ESB����
InitESB InitESBFunc;
SendMsg2ESB SendMsg2ESBFunc;
HINSTANCE dbHandle;
AlarmInfo2DB_2 AlarmInfo2DB_2Func;

/*
 * 
 */
//int main(){
//	//��ȡLog�ļ�
//	cout << GetLogFileName() << endl;
//	out.open(GetLogFileName());
//	if (!out.is_open()){
//		cout << "Open log file failed." << endl;
//		getchar();
//		out.close();
//		return -1;
//	}
//	//��xml�ļ��ж�ȡ���㷨��صĲ���
//	if (GetParams()){
//		cout << "Get params successful." << endl;
//		out << "Get params successful." << endl;
//		cout << "************************************************************" << endl;
//		out << "************************************************************" << endl;
//	}
//	else{
//		cout << "Get params failed." << endl;
//		out << "Get params failed." << endl;
//		cout << "************************************************************" << endl;
//		out << "************************************************************" << endl;
//		getchar();
//		out.close();
//		return -1;
//	}
//	//����dll�������ݿ��ж�ȡ�����ص�������Ϣ
//	if (GetConfig()){
//		cout << "Get config successful." << endl;
//		out << "Get config successful." << endl;
//		cout << "************************************************************" << endl;
//		out << "************************************************************" << endl;
//	}
//	else{
//		cout << "Get config failed." << endl;
//		out << "Get config failed." << endl;
//		cout << "************************************************************" << endl;
//		out << "************************************************************" << endl;
//		getchar();
//		out.close();
//		return -1;
//	}
//	
//	if (strcmp(mode, "video") == 0){
//		cout << "************** Get Frame from Video ****************";
//		out << "************** Get Frame from Video ****************";
//		if (!RunAlgorithm()){
//			cout << "Error eccur in video analyse" << endl;
//			out << "Error eccur in video analyse" << endl;
//			cout << "************************************************************";
//			out << "************************************************************";
//		}
//	}
//}

int main(){
	strcpy(video_dir, "hover1.avi");
	if (!Vibe_init())
		return false;
	while (Vibe_apply()){
		imshow("mask", mask);
		ExtractTargetFromMask();
		MarkTarget();
		imshow("mask_bbox", mask_bbox);
		waitKey(30);
	}

	/*����*/
	getchar();
	out.close();
	return 0;
}


