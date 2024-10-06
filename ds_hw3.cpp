#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>
#include <algorithm>
using namespace std;

int judge(string line)
{
	string temp = line.substr(0, 2);
	if (temp == "In")
		return 0;
	else if (temp == "Pr")
		return 1;
	else if (temp == "Po")
		return 2;
	else
		return -1;
}

vector<string> doline(string line)
{
	vector<string> temp;
	int i = 7;
	int tempf = 0, templ = 0;
	for (i; i < line.size(); i++)
	{
		if (line[i] == '(')
			tempf = i + 1;
		if (line[i] == ')')
		{
			templ = i;
			string subline = line.substr(tempf, templ - tempf);
			temp.push_back(subline);
		}
	}
	return temp;
}

int find(vector<string> data, string val)
{
	for (int i = 0; i < data.size(); i++)
		if (data[i] == val)
			return i;
	return -1;
}

void recursion(vector<string>& valname, vector<int>& valtemp, 
	vector<string> data1, vector<string>& data2, int now, int val)
{
	if (find(data1, data2[now]) != -1)
	{
		vector<string> ltemp, rtemp;
		int i = 0;
		valtemp.push_back(val);
		valname.push_back(data2[now]);
		//cout << "now val = " << val << endl;
		//cout << "now find = " << data2[now] << endl;
		for (i; i < find(data1, data2[now]); i++)
			ltemp.push_back(data1[i]);//左邊的
		if (now < data2.size() && !ltemp.empty())//確定不會超過範圍
			recursion(valname, valtemp, ltemp, data2, now + 1, val * 2);
		for (++i; i < data1.size(); i++)
			rtemp.push_back(data1[i]);//右邊的
		if (now < data2.size() && !rtemp.empty())//確定不會超過範圍
			recursion(valname, valtemp, rtemp, data2, now + 1, val * 2 + 1);
	}
	else
		if (now < data2.size() && !data1.empty())//確定不會超過範圍和要找的不為空
			recursion(valname, valtemp, data1, data2, now + 1, val);
}

void swap(vector<string>& data1, vector<string>& data2, int type1, int type2)
{
	if (type1 == 1 || type1 == 2)//把第一個設成inorder
	{
		int temp = type1;
		type1 = type2;
		type2 = temp;
		vector<string> stemp = data1;
		data1 = data2;
		data2 = stemp;
	}
	if (type2 == 2)
		reverse(data2.begin(),data2.end());
}

int main(int argc, char* argv[])
{
	ifstream input(argv[1]);
	vector<string> table;
	string line; int number = 0;
	while (getline(input, line))
	{
		number++;
		table.push_back(line);
	}

	vector<int> type;//In Po Pr 判斷
	for (int i = 0; i < table.size(); i++)
		type.push_back(judge(table[i]));

	vector<vector<string>> data;//裡面的值按順序輸入data
	for (int i = 0; i < table.size(); i++)
	{
		//cout << table[i] << endl;
		vector<string> temp = doline(table[i]);
		data.push_back(temp);
	}

	//for (int i = 0; i < data.size(); i++)
	//{
	//	for (int j = 0; j < data[i].size(); j++)
	//		cout << data[i][j] << " ";
	//	cout << endl;
	//}

	//for (int i = 0; i < data.size(); i++)
	//{

	//}
	for (int x = 0; x < data.size(); x+=2)
	{
		vector<int> val;
		vector<string> valname;
		swap(data[x], data[x + 1], type[x], type[x + 1]);
		recursion(valname, val, data[x], data[x+1], 0, 1);
		for (int i = 0; i < val.size(); i++)
		{
			for (int j = i + 1; j < val.size(); j++)
			{
				if (val[i] > val[j])
				{
					int temp = val[i];
					val[i] = val[j];
					val[j] = temp;
					string nametemp = valname[i];
					valname[i] = valname[j];
					valname[j] = nametemp;
				}
			}
		}
		for (int i = 0; i < val.size(); i++)
		{
			cout << valname[i] << ":" << val[i] << endl;
		}
	}
}