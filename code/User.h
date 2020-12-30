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

//һ����Ϸ�ļ�¼
struct record
{
	int index;//��¼���
	difficulty diff;//�Ѷȵȼ�
	int score;//�÷�
	string file;//�ı���Դ
	float duration;//����ʱ�� 0.5/1/2/5 min
	float accuracy;//��ȷ��
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
	string id;//Ψһ��ʶ��
	string nickname;//�ǳ�
	string pwd;//����
	int index;//��ǰ�ļ�¼����
	vector<record> records;//��ϰ��¼
public:
	//ÿ���û�����һ�������ļ��洢���ļ�����Ϊid������id��"ids.txt"���д洢
	User(string id);
	User() = default;
	User(string id, string nm, string pwd);
	~User() = default;
	void displayRecord();//չʾ��ϰ��¼
	void addRecord(record r);//������ϰ��¼
	void storeRecord();//����¼�浽�ļ���
	friend ostream& operator<<(ostream& os, record& r);//����record���
	friend class Admin;
};
