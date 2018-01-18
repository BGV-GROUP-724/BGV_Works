#include "myHeaders.h"
// 算法参数
int areaThresMin = 500, areaThresMax = 10000; //目标区域面积的阈值
int vmin = 10, vmax = 256, smin = 30;
char* mode = "video"; //视频源的类型，包括video、IPC两种模式
char* video_dir = new char[128]; //视频文件名，在video模式中用到
int timeThres; //目标出现在视频的时间的阈值
vector<myPoint> ps;//不规则区域内，坐标点的集合
int width = 300, height = 300; //将视频帧resize后的宽、高

//摄像机相关信息
string ip;
int port;
string userName;
string password;
int channel;
int streamType;
char* cameraId;

/*
* 功能：从xml文件中获取与算法有关的参数
*/
bool GetParams(){
	out << "************** Parse Params *********************" << endl;
	cout << "************** Parse Params *********************" << endl;
	char* filepath = new char[256];
	filepath[0] = '\0';
	strcat(filepath, GetCurrentDir());
	strcat(filepath, "AlgorithmParams.xml");
	cout << "Path of xml file : " << filepath << endl;
	TiXmlDocument doc(filepath);
	bool loadOkay = doc.LoadFile();
	if (!loadOkay) {
		char * tmp = new char[128];
		sprintf(tmp, "Could not load test file %s. Error='%s'. Exiting.\n", filepath, doc.ErrorDesc());
		out << tmp;
		cout << tmp;
		delete[] tmp;
		return false;
	}
	TiXmlElement* root = doc.RootElement();
	TiXmlNode* node = root->FirstChild();
	const char* text = node->ToElement()->GetText();
	if (text) {
		out << "Min Threshold of area of target : " << text << endl;
		cout << "Min Threshold of area of target : " << text << endl;
		areaThresMin = atoi(text);
	}
	else{
		cout << "Params file is not completed." << endl;
		out << "Params file is not completed." << endl;
		return false;
	}
	node = root->IterateChildren(node);
	text = node->ToElement()->GetText();
	if (text) {
		out << "Max Threshold of area of target : " << text << endl;
		cout << "Max Threshold of area of target : " << text << endl;
		areaThresMax = atoi(text);
	}
	else{
		cout << "Params file is not completed." << endl;
		out << "Params file is not completed." << endl;
		return false;
	}
	node = root->IterateChildren(node);
	text = node->ToElement()->GetText();
	if (text) {
		out << "vmin : " << text << endl;
		cout << "vmin : " << text << endl;
		vmin = atoi(text);
	}
	else{
		cout << "Params file is not completed." << endl;
		out << "Params file is not completed." << endl;
		return false;
	}
	node = root->IterateChildren(node);
	text = node->ToElement()->GetText();
	if (text) {
		out << "vmax : " << text << endl;
		cout << "vmax : " << text << endl;
		vmax = atoi(text);
	}
	else{
		cout << "Params file is not completed." << endl;
		out << "Params file is not completed." << endl;
		return false;
	}
	node = root->IterateChildren(node);
	text = node->ToElement()->GetText();
	if (text) {
		out << "smin : " << text << endl;
		cout << "smin : " << text << endl;
		vmax = atoi(text);
	}
	else{
		cout << "Params file is not completed." << endl;
		out << "Params file is not completed." << endl;
		return false;
	}
	node = root->IterateChildren(node);
	text = node->ToElement()->GetText();
	if (text) {
		out << "mode  : " << text << endl;
		cout << "mode : " << text << endl;
		mode = new char[8];
		strcpy(mode, text);
	}
	else{
		cout << "Params file is not completed." << endl;
		out << "Params file is not completed." << endl;
		return false;
	}
	node = root->IterateChildren(node);
	text = node->ToElement()->GetText();
	if (text) {
		out << "path of video  : " << text << endl;
		cout << "path of video : " << text << endl;
		video_dir = new char[128];
		strcpy(video_dir, text);
	}
	else{
		cout << "Params file is not completed." << endl;
		out << "Params file is not completed." << endl;
		return false;
	}
	node = root->IterateChildren(node);
	text = node->ToElement()->GetText();
	if (text) {
		out << "time thres : " << text << endl;
		cout << "time thres : " << text << endl;
		timeThres = atoi(text);
	}
	else{
		cout << "Params file is not completed." << endl;
		out << "Params file is not completed." << endl;
		return false;
	}

	return true;
}


/*
* 功能：调用dll（dll中的函数会从配置文件中读取数据库的账户信息），获取相机的配置信息
*/
bool GetConfig(){
	out << "************** Parse Database Config *********************" << endl;
	cout << "************** Parse Database Config *********************" << endl;
	CoInitialize(NULL);
	HINSTANCE hdll = LoadLibrary(_T("dllLoadCameraInfo.dll"));
	if (hdll == NULL){
		FreeLibrary(hdll);
		out << "Load AlarmData2DB DLL failed.\n";
		cout << "Load AlarmData2DB DLL failed.\n";
		getchar();
		return false;
	}
	GetCameraInfo GetCameraInfoFunc = (GetCameraInfo)GetProcAddress(hdll, "GetCameraInfoWithProName");
	if (GetCameraInfoFunc == NULL){
		FreeLibrary(hdll);
		out << "Load AlarmInfo2DB function failed.\n";
		cout << "Load AlarmInfo2DB function failed.\n";
		getchar();
		return false;
	}
	string exeName = "HoverDetection_integration.exe";
	string cameraInfo;
	string errorInfo;
	bool ret = GetCameraInfoFunc(exeName, cameraInfo, errorInfo);
	cout << cameraInfo.c_str() << endl;
	out << cameraInfo.c_str() << endl;
	ofstream camInfo("camera.xml");
	char * xmlStrAfterReplace = new char[1024];
	strcpy(xmlStrAfterReplace, cameraInfo.c_str());
	xmlStrAfterReplace = replace(xmlStrAfterReplace, "&lt;", "<");
	xmlStrAfterReplace = replace(xmlStrAfterReplace, "&gt;", ">");
	camInfo << xmlStrAfterReplace << endl;
	camInfo.close();
	cout << "********************* Camera infomation *******************" << endl;
	out << "********************* Camera infomation *******************" << endl;
	char *filepath = "camera.xml";
	TiXmlDocument doc(filepath);
	bool loadOkay = doc.LoadFile();
	if (!loadOkay) {
		char * tmp = new char[128];
		sprintf(tmp, "Could not load test file %s. Error='%s'. Exiting.\n", filepath, doc.ErrorDesc());
		cout << tmp;
		cout << tmp;
		delete[] tmp;
		return false;
	}
	TiXmlElement* root = doc.RootElement();
	TiXmlNode* node = root->FirstChild();
	TiXmlElement* element;
	const char* text = node->ToElement()->GetText();
	if (text) {
		out << "cameraID : " << text << endl;
		cout << "cameraID : " << text << endl;
		cameraId = new char[strlen(text) + 1];
		strcpy(cameraId, text);
	}
	else{
		cout << "Params file is not completed." << endl;
		out << "Params file is not completed." << endl;
		return false;
	}
	node = root->IterateChildren(node);
	text = node->ToElement()->GetText();
	if (text) {
		out << "height : " << text << endl;
		cout << "height : " << text << endl;
		height = atoi(text);
	}
	else{
		cout << "Params file is not completed." << endl;
		out << "Params file is not completed." << endl;
		return false;
	}
	node = root->IterateChildren(node);
	text = node->ToElement()->GetText();
	if (text) {
		out << "width : " << text << endl;
		cout << "width : " << text << endl;
		width = atoi(text);
	}
	else{
		cout << "Params file is not completed." << endl;
		out << "Params file is not completed." << endl;
		return false;
	}
	node = root->IterateChildren(node);
	TiXmlNode *node2 = node->FirstChild();
	text = node2->ToElement()->Attribute("id");
	if (text) {
		out << "area id : " << text << endl;
		cout << "area id : " << text << endl;
	}
	else{
		cout << "Params file is not completed." << endl;
		out << "Params file is not completed." << endl;
		return false;
	}
	TiXmlNode *node3 = node2->FirstChild();
	text = node3->ToElement()->GetText();
	if (text) {
		out << "area type : " << text << endl;
		cout << "area type : " << text << endl;
	}
	else{
		cout << "Params file is not completed." << endl;
		out << "Params file is not completed." << endl;
		return false;
	}
	node3 = node2->IterateChildren(node3);
	TiXmlNode *node4 = node3->FirstChild();
	TiXmlNode *node5 = node4->FirstChild();
	while (node5 != NULL){
		text = node5->ToElement()->GetText();
		out << text << endl;
		cout << text << endl;
		char *tmp1 = new char[strlen(text + 1)];
		strcpy(tmp1, text);
		vector<char*> tmp2 = split(tmp1, " ");
		myPoint p;
		p.x = atof(tmp2.at(0)) * width;
		p.y = atof(tmp2.at(1)) * height;
		ps.push_back(p);
		node5 = node4->IterateChildren(node5);
	}
	for (int i = 0; i < ps.size(); i++){
		out << ps.at(i).x << " " << ps.at(i).y << endl;
		cout << ps.at(i).x << " " << ps.at(i).y << endl;
	}
	node3 = node2->IterateChildren(node3);
	text = node3->ToElement()->GetText();
	if (text){
		out << "threshold : " << text << endl;
		cout << "threshold : " << text << endl;
	}
	else{
		cout << "Params file is not completed." << endl;
		out << "Params file is not completed." << endl;
		return false;
	}
	node3 = node2->IterateChildren(node3);
	text = node3->ToElement()->GetText();
	if (text){
		out << "valid : " << text << endl;
		cout << "valid : " << text << endl;
	}
	else{
		cout << "Params file is not completed." << endl;
		out << "Params file is not completed." << endl;
		return false;
	}
	node = root->IterateChildren(node);
	text = node->ToElement()->GetText();
	if (text){
		out << "cameraIp : " << text << endl;
		cout << "cameraIp : " << text << endl;
		ip = text;
	}
	else{
		cout << "Params file is not completed." << endl;
		out << "Params file is not completed." << endl;
		return false;
	}
	node = root->IterateChildren(node);
	text = node->ToElement()->GetText();
	if (text){
		out << "cameraPort : " << text << endl;
		cout << "cameraPort : " << text << endl;
		port = atoi(text);
	}
	else{
		cout << "Params file is not completed." << endl;
		out << "Params file is not completed." << endl;
		return false;
	}
	node = root->IterateChildren(node);
	text = node->ToElement()->GetText();
	if (text){
		out << "cameraLoginName : " << text << endl;
		cout << "cameraLoginName : " << text << endl;
		userName = text;
	}
	else{
		cout << "Params file is not completed." << endl;
		out << "Params file is not completed." << endl;
		return false;
	}
	node = root->IterateChildren(node);
	text = node->ToElement()->GetText();
	if (text){
		out << "cameraLoginPassword : " << text << endl;
		cout << "cameraLoginPassword : " << text << endl;
		password = text;
	}
	else{
		cout << "Params file is not completed." << endl;
		out << "Params file is not completed." << endl;
		return false;
	}
	node = root->IterateChildren(node);
	text = node->ToElement()->GetText();
	if (text){
		out << "cameraChannel : " << text << endl;
		cout << "cameraChannel : " << text << endl;
		channel = atoi(text);
	}
	else{
		cout << "Params file is not completed." << endl;
		out << "Params file is not completed." << endl;
		return false;
	}
	node = root->IterateChildren(node);
	text = node->ToElement()->GetText();
	if (text){
		out << "streamType : " << text << endl;
		cout << "streamType : " << text << endl;
		streamType = atoi(text);
	}
	else{
		cout << "Params file is not completed." << endl;
		out << "Params file is not completed." << endl;
		return false;
	}
	return true;
}
