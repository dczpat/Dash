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
//�ο������ʦ�����

const int points[4] = { 5,10,15,20 }; //����һ����ĸ�ڲ�ͬģʽ�µĵ÷�
const int HEIGHT = 20;

class Game
{
private:
	static Game* g;//��֤Game�����һ��ʵ��

	string s;//�ܵ���ϰ��
	int front = 0, rear = 0;//��ǰ��Ļ�����������������ַ����±� [0, HEIGHT-1]
	int* x, * y;//ÿ���ַ�����Ļ����
	char* str;
	int pos = 0;
	int len = 0;//s�ĳ���
	//��Ϸ����ʱ�� ���û�ѡ����Ϊ������Ϸ������
	//��ͨ��Ecsǿ���˳�����������
	float duration;
	int start;//��Ϸ��ʼʱ��
	int overallCnt = 0, succCnt = 0;//��ȷ�ĸ���
	float accuracy;//��ȷ��
	int score = 0;//��Ϸ�÷� ������ȷ���������ӷ� ����©�䲻�÷�
	char cur, input;//��ǰ��ȷ���ַ�����ǰ��ʵ������
	difficulty diff;//�Ѷȵȼ�
	bool gameover = false;//�Ƿ������Ϸ

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
