#include <opencv2/opencv.hpp>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <algorithm>
#include <fstream>
#include <string>
#include <stack>
#include <sstream>
using namespace std;
using namespace cv;

int row[] = { -1,0,1,0 };
int col[] = { 0,1,0,-1 };

class road {
private:
	stack<int> r;
	stack<int> c;
	stack<int> dir;
public:
	road()
	{
	}
	bool empty() {
		return (r.empty() && c.empty() && dir.empty());
	}
	void push(int r, int c, int d) {
		road::r.push(r);
		road::c.push(c);
		road::dir.push(d);
	}
	void pop() {
		road::r.pop();
		road::c.pop();
		road::dir.pop();
	}
	int rtop() {
		return r.top();
	}
	int ctop() {
		return c.top();
	}
	int dtop() {
		return dir.top();
	}
	int size() {
		return r.size();
	}
};

//bool isExit(int x, int y) {
//	if (x == exitx && y == exity)
//		return true;
//	else
//		return false;
//}


road solution(vector<vector<int>> mazing,int ex,int ey) {
	int visited[100][100]{0};
	road road;
	road.push(0, 1, 2);
	visited[0][1] = 1;
	
	while (road.rtop() != ex || road.ctop() != ey) {
		int r = road.rtop(), c = road.ctop(), d = road.dtop();
		road.pop();//unstack
		while (d < 4) {//move next
			int g = r + row[d];int h = c + col[d];
			if (g == ex && h == ey) {
				road.push(r, c, d);
				road.push(g, h, d);
				return road;
			}
			if (mazing[g][h] == 0 && visited[g][h] == 0) {
				visited[g][h] = 1;
				road.push(r, c, d + 1);
				r = g; c = h; d = 0;
			}
			else
				d++;
		}
	}
}

int main(int argc, char* argv[])
{
	String input = argv[1];
	Mat img = imread(input, CV_8UC1);
	//imshow("Display Window", img);
	//waitKey(0);


	vector<vector<int>> puzzle;
	vector<int> temp;

	for (int i = 0; i < img.rows ; i+=10) {
		for (int j = 0; j < img.cols; j+=10) {
			int x = img.at<uchar>(i, j);
			if (x == 255)
				x = 0;
			else
				x = 1;
			temp.push_back(x);
		}
		puzzle.push_back(temp);
		temp.clear();
	}

	int exitx = 0, exity = 0;

	for (int i = 0; i < puzzle.size(); i++) {
		for (int j = 0; j < puzzle[i].size(); j++) {
			if (i == puzzle.size() - 1 || j == puzzle[i].size() - 1)
				if (puzzle[i][j] == 0) {
					exitx = i;
					exity = j;
				}
		}
	}

	road answer = solution(puzzle,exitx,exity);
	vector<vector<int>> copypuzzle = puzzle;
	
	while(!answer.empty()) {
		copypuzzle[answer.rtop()][answer.ctop()] = 2;
		answer.pop();
	}

	Mat output1;
	output1.create(img.rows, img.cols, CV_8UC1);
	for (int i = 0; i < copypuzzle.size(); i++) {
		for (int j = 0; j < copypuzzle[i].size(); j++) {
			int color = 0;
			if (copypuzzle[i][j] == 0)
				color = 255;
			else if (copypuzzle[i][j] == 1)
				color = 0;
			else if (copypuzzle[i][j] == 2)
				color = 0;

			for (int x = 0; x < 10; x++) {
				for (int y = 0; y < 10; y++) {
					output1.at<uchar>(x + i * 10, y + j * 10) = (uchar)color;
				}
			}
		}
	}
	Mat output2;
	output2.create(img.rows, img.cols, CV_8UC1);
	for (int i = 0; i < puzzle.size(); i++) {
		for (int j = 0; j < puzzle[i].size(); j++) {
			int color = 0;
			if (puzzle[i][j] == 0)
				color = 255;
			else if (puzzle[i][j] == 1)
				color = 0;
			else if (puzzle[i][j] == 2)
				color = 255;

			for (int x = 0; x < 10; x++) {
				for (int y = 0; y < 10; y++) {
					output2.at<uchar>(x + i * 10, y + j * 10) = (uchar)color;
				}
			}
		}
	}
	//imshow("Display Window", output);
	//waitKey(0);
	//w 255 255 255 b 0 0 0 g 0 255 0

	Mat newChannel[3] = { output1 , output2 , output1 };

	Mat BGRimage;
	merge(newChannel, 3, BGRimage);
	imshow("Display Window", BGRimage);
	waitKey(0);

	imwrite(argv[2], BGRimage);

	//string path = "output1.png";
	//Mat check = imread(path, IMREAD_COLOR);
	//imshow("Display Window", check);
	//waitKey(0);

	//int error = 0;
	//for (int i = 0; i < check.rows; i++) {
	//	for (int j = 0; j < check.cols; j++) {
	//		if (check.at<uchar>(i, j) == BGRimage.at<uchar>(i, j)) {
	//			continue;
	//		}
	//		else {
	//			error++;
	//		}
	//	}
	//}
	//cout << error;

	//cout << BGRimage << endl;

	//cout << check << endl;
}