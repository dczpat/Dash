#include "Game.h"
Game* Game::g = NULL;
mutex mtx;//为防止输出异常加的锁

Game::Game(string str, float du, difficulty di)
	:s(str), duration(du), diff(di) {}

Game::~Game()
{
	//delete Game::g;
	g = NULL;
}

Game* Game::getInstance(string str, float du, difficulty di)
{
	if (g == NULL)
		g = new Game(str, du, di);
	return g;
}

// 将光标移到控制台(x,y)处。
void Game::go_to(int x, int y)
{
	CONSOLE_SCREEN_BUFFER_INFO cs;
	HANDLE hConsoleOut = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(hConsoleOut, &cs);
	cs.dwCursorPosition.X = y;
	cs.dwCursorPosition.Y = x;
	SetConsoleCursorPosition(hConsoleOut,
		cs.dwCursorPosition);
}

void Game::checkInput()
{
	while (true)
	{
		if (_kbhit())
		{
			this->overallCnt++;
			this->input = _getch();
			//按下Esc键，强制结束游戏
			if (this->input == 27)
			{
				this->duration = (float)(time(NULL) - this->start) / 60;
				this->gameover = true;
				break;
			}
			if (this->str[this->front] == this->input)
			{
				this->score += points[this->diff];
				this->succCnt++;
				mtx.lock();
				go_to(0, 70);//游戏得分位置
				cout << this->score;
				mtx.unlock();
			}
			this->accuracy = (float)this->succCnt / this->overallCnt * 100;
			mtx.lock();
			go_to(2, 70);
			cout << fixed << setprecision(2) << this->accuracy << "%  ";
			mtx.unlock();

			//清除最下方的字符
			mtx.lock();
			go_to(this->x[this->front], this->y[this->front]);
			cout << ' ';
			mtx.unlock();
			this->x[this->front] = 0;
			this->str[this->front] = '\0';
			//相等时，当前屏幕已无字符出现，指针无需移动
			if (this->front != this->rear)
				this->front = (this->front + 1) % HEIGHT;
		}
		//超出时间 退出游戏
		if (time(NULL) - this->start > 60 * this->duration)
		{
			this->gameover = true;
			break;
		}
	}
}

bool Game::contin(int i)
{
	if (this->front > this->rear && this->front <= i)
		return i <= this->rear + HEIGHT;
	else
		return i <= this->rear;
}

void Game::mainGame()
{
	this->len = this->s.length();
	this->x = new int[HEIGHT];
	this->y = new int[HEIGHT];
	this->str = new char[HEIGHT];
	this->str[this->front] = this->s[this->pos];
	for (int i = 0; i < HEIGHT; ++i)
	{
		this->x[i] = 0;
		this->y[i] = rand() % 50;
	}

	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);//获取控制台的句柄，用于改变字体颜色

	system("cls");//清除屏幕当前所有信息
	go_to(0, 55);//游戏得分位置
	cout << "当前得分：";
	go_to(0, 70);
	cout << this->score;
	go_to(2, 55);//正确率位置
	cout << "当前正确率：";
	go_to(2, 70);
	cout << fixed << setprecision(2) << 100.0 << '%';
	go_to(4, 55);
	cout << "提示：按下Esc可提前结束游戏！" << endl;

	SetConsoleTextAttribute(hOut,
		FOREGROUND_RED |
		FOREGROUND_INTENSITY);  //红色
	go_to(HEIGHT, 0);
	cout << "--------------------------------------------------------" << endl;
	cout << "--------------------------------------------------------" << endl;

	SetConsoleTextAttribute(hOut,
		FOREGROUND_RED |
		FOREGROUND_GREEN |
		FOREGROUND_INTENSITY);  //蓝色

	//隐藏光标
	CONSOLE_CURSOR_INFO CursorInfo;
	GetConsoleCursorInfo(hOut, &CursorInfo);//获取控制台光标信息
	CursorInfo.bVisible = false; //隐藏控制台光标
	SetConsoleCursorInfo(hOut, &CursorInfo);//设置控制台光标状态

	this->start = time(NULL);

	thread check(&Game::checkInput, this);
	check.detach();

	//打印出字符的实时降落状态
	while (!this->gameover)
	{
		for (int i = this->front; contin(i); i = (i + 1) % HEIGHT)
		{
			mtx.lock();
			go_to(this->x[i], this->y[i]);
			cout << this->str[i];
			mtx.unlock();

			if (i == this->rear)
				break;
		}

		Sleep(1000 - this->diff * 200); //按难度加快字符掉落速度

		for (int i = this->front; contin(i); i = (i + 1) % HEIGHT)
		{
			mtx.lock();
			go_to(this->x[i], this->y[i]);
			cout << ' ';//擦掉当前字符
			mtx.unlock();
			this->x[i]++;//将每个字符的位置往下挪一行
			if (i == this->rear)
				break;
		}

		if (this->x[this->front] == 20) //头部已到底
		{
			this->overallCnt++;
			this->accuracy = (float)this->succCnt / this->overallCnt * 100;
			mtx.lock();
			go_to(2, 70);
			cout << fixed << setprecision(2) << this->accuracy << "%  ";
			mtx.unlock();

			this->x[this->front] = 0;
			this->str[this->front] = '\0';
			this->front = (this->front + 1) % HEIGHT;
		}
		this->rear = (this->rear + 1) % HEIGHT;
		this->pos = (this->pos + 1) % this->len;
		this->str[this->rear] = this->s[this->pos];
	}

	SetConsoleTextAttribute(hOut,
		FOREGROUND_RED |
		FOREGROUND_GREEN |
		FOREGROUND_BLUE);  //白色

	CursorInfo.bVisible = true; //恢复控制台光标
	SetConsoleCursorInfo(hOut, &CursorInfo);//设置控制台光标状态
}
