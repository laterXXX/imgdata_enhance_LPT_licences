//#include <vector>
//#include <string>
//#include <iostream>
//using namespace std;
//
//void SplitString(const string& s, vector<string>& v, const string& c)
//{
//	string::size_type pos1, pos2;
//	pos2 = s.find(c);
//	pos1 = 0;
//	while (string::npos != pos2)
//	{
//		v.push_back(s.substr(pos1, pos2 - pos1));
//
//		pos1 = pos2 + c.size();
//		pos2 = s.find(c, pos1);
//	}
//	if (pos1 != s.length())
//		v.push_back(s.substr(pos1));
//}
//
//int main234235() {
//	string s = "a,b,c,d,e,f";
//	vector<string> v;
//	SplitString(s, v, ","); //可按多个字符来分隔;
//	for (vector<string>::size_type i = 0; i != v.size(); ++i)
//		cout << v[i] << " ";
//	cout << endl;
//	//输出: a b c d e f
//
//	system("pause");
//}