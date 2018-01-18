#include "myHeaders.h"
ofstream out;
char * alarmInfo = new char[1024];

//ESB函数
InitESB InitESBFunc;
SendMsg2ESB SendMsg2ESBFunc;
HINSTANCE dbHandle;
AlarmInfo2DB_2 AlarmInfo2DB_2Func;

/*
 * 
 */
//int main(){
//	//获取Log文件
//	cout << GetLogFileName() << endl;
//	out.open(GetLogFileName());
//	if (!out.is_open()){
//		cout << "Open log file failed." << endl;
//		getchar();
//		out.close();
//		return -1;
//	}
//	//从xml文件中读取与算法相关的参数
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
//	//调用dll，从数据库中读取相机相关的配置信息
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

	/*结束*/
	getchar();
	out.close();
	return 0;
}


