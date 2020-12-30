#pragma once
#include <thread>
#include <mutex>
#include <ctime>
#include <conio.h>
#include <Windows.h>
#include <vector>
#include <iomanip>
#include <cassert>
#include <string>
#include <iostream>

using namespace std;
class Admin;
enum difficulty;
//参考节奏大师的设计

const int points[4] = { 5,10,15,20 }; //击中一个字母在不同模式下的得分
const int HEIGHT = 20;

class Game
{
private:
	static Game* g;//保证Game类仅有一个实例

	string s;//总的练习库
	int front = 0, rear = 0;//当前屏幕上最下面和最上面的字符的下标 [0, HEIGHT-1]
	int* x, * y;//每个字符的屏幕坐标
	char* str;
	int pos = 0;
	int len = 0;//s的长度
	//游戏持续时间 由用户选择，作为结束游戏的依据
	//若通过Ecs强制退出，则需重置
	float duration;
	int start;//游戏开始时间
	int overallCnt = 0, succCnt = 0;//正确的个数
	float accuracy;//正确率
	int score = 0;//游戏得分 仅对正确的输入作加分 输错或漏输不得分
	char cur, input;//当前正确的字符，当前的实际输入
	difficulty diff;//难度等级
	bool gameover = false;//是否结束游戏

	void go_to(int x, int y);
	void checkInput();
	bool contin(int i);
	Game(string str, float du, difficulty di);
public:
	static Game* getInstance(string str, float du, difficulty di);
	~Game();
	void mainGame();
	friend class Admin;
};
