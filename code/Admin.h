#pragma once
#include "User.h"
#include "Game.h"

string enum2str(difficulty di);
const string DEFAULT_PWD = "12345";//Ĭ���û�����
const string ADMIN_PWD = "2020cs";//����Ա����
const int SLEEP_DURATION = 1500;

struct rank_pair
{
	string id;
	int score;
	string file;
	rank_pair(const string i, int s, string f) :id(i), score(s), file(f) {}
	bool operator<(const rank_pair& r) const
	{
		return score > r.score;
	}
};

class Admin
{
private:
	static Admin* a;//��֤Admin�����һ��ʵ��
	map<string, User> users;//�û���ɵ��ֵ�
	vector<string> src;//�����ı�����
	User* activeUser;//��ǰ��Ծ�û�
	vector<rank_pair> rankList;//���а�

	Admin();
	string txt2str(string filename);
	void storeIDs();//��������id��"ids.txt"
	void login();//��¼
	void regist();//ע��
	void resetPwd(string id = "");//��������
	bool changePwd(string id);//��������
	void play();//������Ϸ
	void storeTxts();//���������ı�����"src.txt"
	void addTxt(string filename);//�����ı�
	void rmTxt(string filename);//�Ƴ��ı�
	void userMenu();//�û��˵�
	void adminMenu();//����Ա�˵�
	void rank(const User& self);//չʾ���а�
public:
	static Admin* getInstance();
	void mainMenu();//���˵�

	~Admin();
};
