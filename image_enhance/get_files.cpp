//#include <io.h>      
//#include <string>    
//#include <vector>    
//#include <iostream>    
//using namespace std;
////获取特定格式的文件名    
//void getAllFiles212(string path, vector<string>& files, string format)
//{
//	long  hFile = 0;//文件句柄  64位下long 改为 intptr_t
//	struct _finddata_t fileinfo;//文件信息 
//	string p;
//	if ((hFile = _findfirst(p.assign(path).append("\\*" + format).c_str(), &fileinfo)) != -1) //文件存在
//	{
//		do
//		{
//			if ((fileinfo.attrib & _A_SUBDIR))//判断是否为文件夹
//			{
//				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)//文件夹名中不含"."和".."
//				{
//					files.push_back(p.assign(path).append("\\").append(fileinfo.name)); //保存文件夹名
//					getAllFiles212(p.assign(path).append("\\").append(fileinfo.name), files, format); //递归遍历文件夹
//				}
//			}
//			else
//			{
//				files.push_back(p.assign(path).append("\\").append(fileinfo.name));//如果不是文件夹，储存文件名
//			}
//		} while (_findnext(hFile, &fileinfo) == 0);
//		_findclose(hFile);
//	}
//}
//
//int main32535()
//{
//	string filePath = "I:/sclead/LPTphotoes/Camera";
//	vector<string> files;
//	string format = "";				 //查找文件的格式
//	getAllFiles212(filePath, files, format);
//	int size = files.size();
//	for (int i = 0; i<size; i++)
//	{
//		cout << files[i] << endl;
//	}
//	system("pause");
//	return 0;
//}
