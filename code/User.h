#pragma once
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <iomanip>
#include <fstream>
#include <string>
#include <Windows.h>
#include <conio.h>
#include <queue>
#include <algorithm>
using namespace std;

enum difficulty
{
	EASY, MEDIUM, HARD, HELL
};

//一次游戏的记录
struct record
{
	int index;//记录编号
	difficulty diff;//难度等级
	int score;//得分
	string file;//文本来源
	float duration;//持续时间 0.5/1/2/5 min
	float accuracy;//正确率
	record(int i = -1, difficulty di = EASY, int sc = 0, string s = "", float du = 0.0, float c = 0.0) :
		index(i), diff(di), score(sc), file(s), duration(du), accuracy(c) {}
};

ostream& operator<<(ostream& os, const record& r);
istream& operator>>(istream& is, record& r);
string enum2str(difficulty di);
class Admin;

class User
{
private:
	string id;//唯一标识符
	string nickname;//昵称
	string pwd;//密码
	int index;//当前的记录个数
	vector<record> records;//练习记录
public:
	//每个用户都用一个单独文件存储，文件名即为id，所有id用"ids.txt"另行存储
	User(string id);
	User() = default;
	User(string id, string nm, string pwd);
	~User() = default;
	void displayRecord();//展示练习记录
	void addRecord(record r);//增加练习记录
	void storeRecord();//将记录存到文件中
	friend ostream& operator<<(ostream& os, record& r);//重载record输出
	friend class Admin;
};
