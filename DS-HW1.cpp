#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <math.h>
#include <iomanip>
#include <algorithm>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <cctype>
#include <cmath>
using namespace std;

void print(double a)
{
	a = round(a * 100) / 100;
	if (a == 0)
		a = 0;
	string s = to_string(a);
	s.erase(s.find('.') + 3, s.length() - s.find('.') - 3);

	int n = s.length();

	if (s[n - 1] == '0' and s[n - 2] == '0')
		cout << s.substr(0, s.find('.'));
	else if (s[n - 1] == '0')
		cout << s.substr(0, s.find('.') + 2);
	else
		cout << s;
}

int getPrecedence(char op) {
	if (op == '^')
		return 3;
	else if (op == '*' || op == '/')
		return 2;
	else if (op == '+' || op == '-')
		return 1;
	return 0;
}

bool isOperator(char c) {
	return (c == '+' || c == '-' || c == '*' || c == '/' || c == '^');
}

// 函数用于在表达式开头添加零
string preprocessExpression(const char* infix) {
	string processedExpr = infix;
	if (processedExpr[0] == '-') {
		processedExpr = "0" + processedExpr;
	}
	return processedExpr;
}

void intopost(const char* infix, char* postfix) {
	// 先对中缀表达式进行预处理
	string processedInfix = preprocessExpression(infix);

	stack<char> operators;
	int postfixIndex = 0;
	bool lastWasOperator = true; // Check if the last token was an operator

	for (int i = 0; i < processedInfix.length(); i++) {
		char current = processedInfix[i];

		if (isspace(current)) {
			continue;
		}

		if ((isdigit(current) && (i == 0 || processedInfix[i - 1] == '(' || lastWasOperator)) ||
			(current == '-' && (i == 0 || processedInfix[i - 1] == '(' || lastWasOperator))) {
			// Handle numbers, including negative numbers
			postfix[postfixIndex++] = current;
			while (i + 1 < processedInfix.length() && (isdigit(processedInfix[i + 1]) || processedInfix[i + 1] == '.')) {
				postfix[postfixIndex++] = processedInfix[i + 1];
				i++;
			}
			postfix[postfixIndex++] = ' ';
			lastWasOperator = false;
		}
		else if (current == '(') {
			operators.push(current);
			lastWasOperator = true;
		}
		else if (current == ')') {
			while (!operators.empty() && operators.top() != '(') {
				postfix[postfixIndex++] = operators.top();
				postfix[postfixIndex++] = ' ';
				operators.pop();
			}
			if (!operators.empty() && operators.top() == '(') {
				operators.pop(); // Pop the '('
			}
			lastWasOperator = false;
		}
		else if (isOperator(current)) {
			while (!operators.empty() && getPrecedence(operators.top()) >= getPrecedence(current) && operators.top() != '(') {
				postfix[postfixIndex++] = operators.top();
				postfix[postfixIndex++] = ' ';
				operators.pop();
			}
			operators.push(current);
			lastWasOperator = true;
		}
	}

	while (!operators.empty()) {
		postfix[postfixIndex++] = operators.top();
		postfix[postfixIndex++] = ' ';
		operators.pop();
	}

	postfix[postfixIndex] = '\0'; // Null-terminate the postfix string
}

double getvalue(const char* postfix) {
	stack<double> operands;

	for (int i = 0; i < strlen(postfix); i++) {
		char current = postfix[i];

		if (isspace(current)) {
			continue;
		}

		if (isdigit(current) || (current == '-' && isdigit(postfix[i + 1]))) {
			// Handle numbers, including negative numbers
			char* end;
			double num = strtod(&postfix[i], &end);
			operands.push(num);
			i = end - postfix;
		}
		else if (current == '+' || current == '-' || current == '*' || current == '/' || current == '^') {
			if (operands.size() < 2) {
				cerr << "錯誤：運算子不足" << endl;
				exit(1);
			}

			double operand2 = operands.top();
			operands.pop();
			double operand1 = operands.top();
			operands.pop();

			double result;
			switch (current) {
			case '+':
				result = operand1 + operand2;
				break;
			case '-':
				result = operand1 - operand2;
				break;
			case '*':
				result = operand1 * operand2;
				break;
			case '/':
				if (operand2 == 0) {
					cerr << "錯誤：除以零" << endl;
					exit(1);
				}
				result = operand1 / operand2;
				break;
			case '^':
				result = pow(operand1, operand2);
				break;
			default:
				cerr << "錯誤：無效的運算符" << endl;
				exit(1);
			}

			operands.push(result);
		}
	}

	if (operands.size() != 1) {
		cerr << "錯誤：表達式不正確" << endl;
		exit(1);
	}

	return operands.top();
}

vector<double> getanswer(vector<vector<double>> number, char order[]) {
	stack<int> s1;
	int i = 0;
	double result = 0;
	int x1 = 0, x2 = 0;
	while (order[i] != '\0') {
		if (order[i] >= '0' && order[i] <= '9') {
			int x = 0;
			while (order[i] >= '0' && order[i] <= '9') {
				x = x * 10 + (order[i] - '0');
				i++;
			}
			//cout << x << endl;
			s1.push(x);
		}
		else if (order[i] == ' ')
			i++;
		else if (order[i] == '*') {
			x2 = s1.top();
			s1.pop();
			x1 = s1.top();
			s1.pop();
			//找最長的
			if (number[x1].size() > number[x2].size()) {
				int temp = x1;
				x1 = x2;
				x2 = temp;
			}
			//cout << number[x1].size() << " " << number[x2].size() << endl;
			//計算
			for (int j = 0; j < number[x2].size(); j++) {
				number[x2][j] *= (float)number[x1][j % number[x1].size()];
				//cout << number[x2][j] << " ";
			}
			//cout << endl;
			s1.push(x2);
			i++;
		}
		else if (order[i] == '/') {
			x2 = s1.top();
			s1.pop();
			x1 = s1.top();
			s1.pop();
			//找最長的
			int maxx = x2;
			int max = number[x2].size();
			if (number[x1].size() > number[x2].size()) {
				maxx = x1;
				max = number[x1].size();
			}
			//cout << number[x1].size() << " " << number[x2].size() << endl;
			//計算
			for (int j = 0; j < max; j++) {
				number[maxx][j] = (float)number[x1][j % number[x1].size()] / (float)number[x2][j % number[x2].size()];
				//cout << number[maxx][j] << " ";
			}
			//cout << endl;
			s1.push(maxx);
			i++;
		}
		else if (order[i] == '+') {
			x2 = s1.top();
			s1.pop();
			x1 = s1.top();
			s1.pop();
			//找最長的
			if (number[x1].size() > number[x2].size()) {
				int temp = x1;
				x1 = x2;
				x2 = temp;
			}
			//cout << number[x1].size() << " " << number[x2].size() << endl;
			//計算
			for (int j = 0; j < number[x2].size(); j++) {
				number[x2][j] += (float)number[x1][j % number[x1].size()];
				//cout << number[x2][j] << " ";
			}
			//cout << endl;
			s1.push(x2);
			i++;
		}
		else if (order[i] == '-') {
			x2 = s1.top();
			s1.pop();
			x1 = s1.top();
			s1.pop();
			//找最長的
			int maxx = x2;
			int max = number[x2].size();
			if (number[x1].size() > number[x2].size()) {
				maxx = x1;
				max = number[x1].size();
			}
			//cout << number[x1].size() << " " << number[x2].size() << endl;
			//計算
			for (int j = 0; j < max; j++) {
				number[maxx][j] = (float)number[x1][j % number[x1].size()] - (float)number[x2][j % number[x2].size()];
				//cout << number[maxx][j] << " ";
			}
			//cout << endl;
			s1.push(maxx);
			i++;
		}
	}
	result = s1.top();
	return number[result];
}

int main(int argc, char* argv[])
{
	ifstream fin;
	fin.open(argv[1], ios::in);
	vector<string> datalist;
	string line;
	//讀入
	while (getline(fin, line)) {
		datalist.push_back(line);
	}
	//去除空格
	for (int i = 0; i < datalist.size(); i++) {
		for (int j = 0; j < datalist[i].size(); j++) {
			datalist[i].erase(remove(datalist[i].begin(), datalist[i].end(), ' '), datalist[i].end());
		}
		//cout << datalist[i] << endl;
	}

	//處理內部的算式
	//[2+3,5-3]+[9*8,(3+1)^3,9/2]
	int matrixnumber = -1;
	int i = 0, j = 0;
	for (i; i < datalist.size(); i++) {
		vector<vector<double>> numbers;
		vector<double> tempnumbers;
		vector<char> tempoperators;
		for (j = 0; j < datalist[i].size(); j++) {
			if (datalist[i][j] == '[')
				matrixnumber++;
			else if (datalist[i][j] != ',' || datalist[i][j] != ']') {
				int x = 0;
				if (datalist[i][j] >= '0' && datalist[i][j] <= '9' ||
					datalist[i][j] == '-' && datalist[i][j - 1] >= '0' && datalist[i][j - 1] <= '9' ||
					datalist[i][j - 1] == '(' ||
					datalist[i][j - 1] == ',' ||
					datalist[i][j - 1] == '[') {
					char temp[1000]{};
					while (j < datalist[i].size()) {
						if (datalist[i][j] == ',')
							break;
						if (datalist[i][j] == ']')
							break;
						temp[x] = datalist[i][j];
						//cout << temp[x];
						j++;
						x++;
					}
					//cout << endl;
					char postfix[1000]{};
					intopost(temp, postfix);
					float result = getvalue(postfix);
					//cout << result << " ";
					tempnumbers.push_back(result);
				}
				else if (datalist[i][j] == '+' || datalist[i][j] == '-'
					|| datalist[i][j] == '*' || datalist[i][j] == '/') {
					tempoperators.push_back(datalist[i][j]);
					//cout << datalist[i][j] << endl;
					j++;
				}
			}
			if (datalist[i][j] == ']') {
				numbers.push_back(tempnumbers);
				//cout << endl;
				tempnumbers.clear();
			}
		}

		//處理外部的向量
		char vectors[1000]{};
		char vectororder[1000]{};

		int p = 0, k = 0;
		for (k; k < tempoperators.size(); k++) {
			vectors[p++] = k + '0';
			vectors[p++] = tempoperators[k];
		}
		vectors[p++] = k + '0';

		intopost(vectors, vectororder);

		/*for (k = 0; k < 50; k++) {
			cout << vectororder[k];
		}
		cout << endl;*/

		vector<double> ans;
		ans = getanswer(numbers, vectororder);

		cout << '[';
		for (int k = 0; k < ans.size(); k++) {

			//cout << setprecision(15);
			// 四捨五入到小數點後2位

			print(ans[k]);

			/*int answ = round(ans[k] * 100);
			double answw = static_cast<double>(answ) / 100;
			cout << answw;*/

			/*int w = 2, answ = abs(ans[k]);
			while (answ >= 1) {
				answ /= 10;
				w++;
			}
			cout << setprecision(w) << ans[k];*/

			if (k < ans.size() - 1)
				cout << ',';
		}
		cout << ']' << endl;
	}


	return 0;
}