#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include<algorithm>
using namespace std;

vector<int> stringtonumber(string line)
{
	int temp = 0;
	vector<int> data;
	for (int i = 0; i < line.size(); i++)
	{
		if (line[i] >= '0' && line[i] <= '9')
			temp = temp * 10 + line[i] - '0';
		else if (line[i] == ',')
		{
			data.push_back(temp);
			temp = 0;
		}
	}
	data.push_back(temp);
	return data;
}

bool compare(vector<int> a, vector<int> b)
{
	for (int i = 0; i < a.size(); i++)
	{
		if (a[i] >= b[i])
			continue;
		else
			return false;
	}
	return true;
}

bool isFinish(vector<bool> a)
{
	for (int i = 0; i < a.size(); i++)
		if (a[i] == false)
			return false;
	return true;
}

vector<int> Add(vector<int> a, vector<int> b)
{
	for (int i = 0; i < a.size(); i++)
		a[i] = a[i] + b[i];
	return a;
}

vector<int> Minus(vector<int> a, vector<int> b)
{
	for (int i = 0; i < a.size(); i++)
		a[i] = a[i] - b[i];
	return a;
}

int findid(vector<string> list, string id)
{
	for (int i = 0; i < list.size(); i++)
		if (list[i] == id)
			return i;
	return -1;
}

bool check(vector<int> a)
{
	for (int i = 0; i < a.size(); i++)
		if (a[i] < 0)
			return true;
	return false;
}

//isSafe(need,0 1 0,available)
bool isSafe(vector<vector<int>> need, vector<vector<int>> allocation
	, vector<int> work, vector<int> req, int id)
{
	need[id] = Minus(need[id], req);
	allocation[id] = Add(allocation[id], req);
	work = Minus(work, req);
	if (check(need[id]) || check(work))//如果need或是work有負數
		return false;

	vector<bool> finish;
	for (int i = 0; i < need.size(); i++)
		finish.push_back(false);//所以process都還沒做
	int judge = 0;
	while (!isFinish(finish))
	{
		for (int i = 0; i < need.size(); i++)//每個看是否可以
		{
			if (compare(work, need[i]) && finish[i] == false)
			{
				work = Add(work, allocation[i]);
				finish[i] = true;
			}
		}
		judge++;
		if (judge > need.size())
			return false;
	}
	return true;
}

int main(int argc, char* argv[])
{
	ifstream input(argv[1]);
	string line;
	input >> line;

	vector<int> available = stringtonumber(line), original = stringtonumber(line);

	string id;
	vector<string> idlist;
	vector<vector<int>> data, allocation, need;//max , alloc , need

	while (input >> id && !input.eof())
	{
		string action, resource;
		input >> action;
		if (action == "in")
		{
			if (findid(idlist, id) == -1)//不在idlist裡面
			{
				idlist.push_back(id);//紀錄id
				input >> resource;
				vector<int> source = stringtonumber(resource);
				if (compare(original, source))
				{//不可以超過最大資源量
					data.push_back(source);
					need.push_back(source);//進來的時候need跟Max一樣大
					vector<int> tempzero;//這個id的allocation
					for (int i = 0; i < source.size(); i++)
						tempzero.push_back(0);
					allocation.push_back(tempzero);
				}
			}
		}
		else if (action == "want")
		{
			input >> resource;//out
			if (findid(idlist, id) != -1)
				//有在名單上,否則reject
			{
				int idnumber = findid(idlist, id);
				if (isSafe(need, allocation, available, need[idnumber], idnumber))//看是不是安全的
				{
					available = Add(available, allocation[idnumber]);//釋放空間

					allocation.erase(allocation.begin() + idnumber);
					data.erase(data.begin() + idnumber);
					idlist.erase(idlist.begin() + idnumber);
					need.erase(need.begin() + idnumber);
				}
			}
		}
		else if (action == "request")
		{
			input >> resource;
			vector<int> asksource = stringtonumber(resource);
			if (findid(idlist, id) != -1 && compare(available, asksource))
			    //有在名單上,否則reject      不超過一開始的資源
			{
				int idnumber = findid(idlist, id);
				if (isSafe(need, allocation, available, asksource, idnumber))//看是不是安全的
				{
					need[idnumber] = Minus(need[idnumber], asksource);
					allocation[idnumber] = Add(allocation[idnumber], asksource);
					available = Minus(available, asksource);
				}
			}
		}
	}


	for (int x = 0;x < idlist.size(); x++)
	{
		int number1 = stoi(idlist[x].substr(1, idlist[x].size() - 1));
		for (int i = x + 1; i < idlist.size(); i++)
		{
			int number2 = stoi(idlist[i].substr(1, idlist[i].size() - 1));
			if (number1 > number2)
			{
				vector<int> atemp = allocation[x];
				allocation[x] = allocation[i];
				allocation[i] = atemp;
				string stemp = idlist[x];
				idlist[x] = idlist[i];
				idlist[i] = stemp;
				number1 = stoi(idlist[x].substr(1, idlist[x].size() - 1));
				number2 = stoi(idlist[i].substr(1, idlist[i].size() - 1));
			}
		}
	}

	for (int i = 0; i < idlist.size(); i++)
	{
		cout << idlist[i] << " ";
		for (int j = 0; j < allocation[i].size(); j++)
		{
			cout << allocation[i][j];
			if (j < allocation[i].size() - 1)
				cout << ',';
		}
		cout << endl;
	}

	return 0;
}