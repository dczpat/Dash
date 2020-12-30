#pragma once
#include "User.h"
#include "Game.h"

string enum2str(difficulty di);
const string DEFAULT_PWD = "12345";//默认用户密码
const string ADMIN_PWD = "2020cs";//管理员密码
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
	static Admin* a;//保证Admin类仅有一个实例
	map<string, User> users;//用户组成的字典
	vector<string> src;//打字文本集合
	User* activeUser;//当前活跃用户
	vector<rank_pair> rankList;//排行榜

	Admin();
	string txt2str(string filename);
	void storeIDs();//保存所有id至"ids.txt"
	void login();//登录
	void regist();//注册
	void resetPwd(string id = "");//重置密码
	bool changePwd(string id);//更改密码
	void play();//加载游戏
	void storeTxts();//保存所有文本名至"src.txt"
	void addTxt(string filename);//增加文本
	void rmTxt(string filename);//移除文本
	void userMenu();//用户菜单
	void adminMenu();//管理员菜单
	void rank(const User& self);//展示排行榜
public:
	static Admin* getInstance();
	void mainMenu();//主菜单

	~Admin();
};
