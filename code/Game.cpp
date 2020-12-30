#include "Game.h"
Game* Game::g = NULL;
mutex mtx;//Ϊ��ֹ����쳣�ӵ���

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

// ������Ƶ�����̨(x,y)����
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
			//����Esc����ǿ�ƽ�����Ϸ
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
				go_to(0, 70);//��Ϸ�÷�λ��
				cout << this->score;
				mtx.unlock();
			}
			this->accuracy = (float)this->succCnt / this->overallCnt * 100;
			mtx.lock();
			go_to(2, 70);
			cout << fixed << setprecision(2) << this->accuracy << "%  ";
			mtx.unlock();

			//������·����ַ�
			mtx.lock();
			go_to(this->x[this->front], this->y[this->front]);
			cout << ' ';
			mtx.unlock();
			this->x[this->front] = 0;
			this->str[this->front] = '\0';
			//���ʱ����ǰ��Ļ�����ַ����֣�ָ�������ƶ�
			if (this->front != this->rear)
				this->front = (this->front + 1) % HEIGHT;
		}
		//����ʱ�� �˳���Ϸ
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

	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);//��ȡ����̨�ľ�������ڸı�������ɫ

	system("cls");//�����Ļ��ǰ������Ϣ
	go_to(0, 55);//��Ϸ�÷�λ��
	cout << "��ǰ�÷֣�";
	go_to(0, 70);
	cout << this->score;
	go_to(2, 55);//��ȷ��λ��
	cout << "��ǰ��ȷ�ʣ�";
	go_to(2, 70);
	cout << fixed << setprecision(2) << 100.0 << '%';
	go_to(4, 55);
	cout << "��ʾ������Esc����ǰ������Ϸ��" << endl;

	SetConsoleTextAttribute(hOut,
		FOREGROUND_RED |
		FOREGROUND_INTENSITY);  //��ɫ
	go_to(HEIGHT, 0);
	cout << "--------------------------------------------------------" << endl;
	cout << "--------------------------------------------------------" << endl;

	SetConsoleTextAttribute(hOut,
		FOREGROUND_RED |
		FOREGROUND_GREEN |
		FOREGROUND_INTENSITY);  //��ɫ

	//���ع��
	CONSOLE_CURSOR_INFO CursorInfo;
	GetConsoleCursorInfo(hOut, &CursorInfo);//��ȡ����̨�����Ϣ
	CursorInfo.bVisible = false; //���ؿ���̨���
	SetConsoleCursorInfo(hOut, &CursorInfo);//���ÿ���̨���״̬

	this->start = time(NULL);

	thread check(&Game::checkInput, this);
	check.detach();

	//��ӡ���ַ���ʵʱ����״̬
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

		Sleep(1000 - this->diff * 200); //���Ѷȼӿ��ַ������ٶ�

		for (int i = this->front; contin(i); i = (i + 1) % HEIGHT)
		{
			mtx.lock();
			go_to(this->x[i], this->y[i]);
			cout << ' ';//������ǰ�ַ�
			mtx.unlock();
			this->x[i]++;//��ÿ���ַ���λ������Ųһ��
			if (i == this->rear)
				break;
		}

		if (this->x[this->front] == 20) //ͷ���ѵ���
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
		FOREGROUND_BLUE);  //��ɫ

	CursorInfo.bVisible = true; //�ָ�����̨���
	SetConsoleCursorInfo(hOut, &CursorInfo);//���ÿ���̨���״̬
}
