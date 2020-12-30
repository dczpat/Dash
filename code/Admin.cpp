#include "Admin.h"
Admin* Admin::a = NULL;

Admin::Admin() :activeUser(NULL)
{
	ifstream inf;
	//取id
	inf.open("docs/ids.txt", ios::in);
	string id;
	while (inf >> id)
	{
		this->users.insert(make_pair(id, User(id)));
	}
	inf.close();
	//取文本
	inf.open("docs/src.txt", ios::in);
	string filename;
	while (!inf.fail())
	{
		filename = "";
		getline(inf, filename);
		if (!filename.empty())
			this->src.push_back(filename);
	}
	inf.close();
	for (auto& u : users)
	{
		for (auto& r : u.second.records)
		{
			this->rankList.push_back(rank_pair(u.first, r.score, r.file));
		}
	}
}

Admin::~Admin()
{
	//delete this->a;
	a = NULL;
}

//管理员密码是ADMIN_PWD
void Admin::mainMenu()
{
	system("cls");
	int input;

	cout << "请选择您的操作序号：" << endl << endl
		<< "1. 玩家登录" << endl
		<< "2. 玩家注册" << endl
		<< "3. 管理员操作" << endl
		<< "4. 退出程序" << endl << endl;
	cin >> input;
	string s;
	switch (input)
	{
	case 1:
		this->login();

		this->mainMenu();
		break;
	case 2:
		this->regist();

		this->mainMenu();
		break;
	case 3:
		system("cls");
		cout << "请输入管理员密码以进入系统：" << endl << endl;
		cin >> s;
		if (s != ADMIN_PWD)
		{
			system("cls");
			//cout << "管理员密码错误！" << endl << endl;
			cout << "o(*￣︶￣*)o小样儿~" << endl << endl;
			Sleep(SLEEP_DURATION);
			this->mainMenu();
		}
		this->adminMenu();
		break;
	case 4:
		system("cls");
		//保存数据
		this->storeIDs();
		this->storeTxts();
		cout << "再见~~~" << endl;
		Sleep(SLEEP_DURATION);
		exit(0);
		break;
	default:
		this->mainMenu();
		break;
	}
}

void Admin::adminMenu()
{
	system("cls");
	string s;
	int input;
	cout << "请选择您的管理员操作序号：" << endl << endl
		<< "1. 查看可用文本" << endl
		<< "2. 增加练习文本" << endl
		<< "3. 删除练习文本" << endl
		<< "4. 为用户重置密码" << endl
		<< "5. 返回主菜单" << endl << endl;
	cin >> input;
	switch (input)
	{
	case 1:
		system("cls");
		cout << "以下是当前可用的文本：" << endl;
		for (int i = 0; i < this->src.size(); ++i)
			cout << i << ". " << this->src[i] << endl;
		cout << endl;
		cout << "轻按任意键以继续..." << endl;
		while (1)
		{
			if (_kbhit())
				break;
		}
		break;
	case 2:
		system("cls");
		cout << "请输入希望增添的文本名称：" << endl << endl;
		getline(cin, s);
		getline(cin, s);
		this->addTxt(s);
		break;
	case 3:
		system("cls");
		cout << "请输入希望删除的文本名称：" << endl << endl;
		getline(cin, s);
		getline(cin, s);
		this->rmTxt(s);
		break;
	case 4:
		system("cls");
		this->resetPwd();
		break;
	case 5:
	default:
		this->mainMenu();
		break;
	}
	this->adminMenu();
}


Admin* Admin::getInstance()
{
	if (a == NULL)
		a = new Admin();
	return a;
}

void Admin::storeIDs()
{
	ofstream outf;
	outf.open("docs/ids.txt", ios::out | ios::trunc);
	for (auto& user : users)
	{
		outf << user.first << endl;
	}
	outf.close();
}

void Admin::login()
{
	system("cls");
	string i, p;
	cout << "请输入您的用户名和密码：" << endl << endl;
	cin >> i >> p;
	auto iter = this->users.find(i);
	//账户不存在
	if (iter == this->users.end())
	{
		system("cls");
		int input;
		cout << "抱歉，您输入的用户名不存在..." << endl
			<< "请选择以下选项序号以继续：" << endl
			<< "1. 重新输入" << endl
			<< "2. 注册一个新账户" << endl
			<< "3. 返回主菜单" << endl << endl;
		cin >> input;
		switch (input)
		{
		case 1:
			this->login();
			break;
		case 2:
			this->regist();
			break;
		case 3:
			this->mainMenu();
			break;
		default:
			this->login();
			break;
		}
	}
	//密码有误
	else if (iter->second.pwd != p)
	{
		int input;
		cout << "抱歉，您输入的密码有误..." << endl
			<< "请选择以下选项序号以继续：" << endl
			<< "1. 重新输入" << endl
			<< "2. 申请重置账户密码" << endl
			<< "3. 返回主菜单" << endl << endl;
		cin >> input;
		switch (input)
		{
		case 1:
			this->login();
			break;
		case 2:
			this->resetPwd(i);
			break;
		case 3:
			this->mainMenu();
			break;
		default:
			this->login();
			break;
		}
	}
	//登入游戏
	else
	{
		activeUser = &this->users[i];
		this->userMenu();
	}
	this->mainMenu();
}

void Admin::userMenu()
{
	system("cls");

	int input;
	cout << "请选择以下玩家操作：" << endl
		<< "1. 开始游戏！" << endl
		<< "2. 查看练习记录" << endl
		<< "3. 修改密码" << endl
		<< "4. 查看排行榜" << endl
		<< "5. 返回主菜单" << endl << endl;

	cin >> input;
	switch (input)
	{
	case 1:
		this->play();
		this->userMenu();
		break;
	case 2:
		activeUser->displayRecord();
		this->userMenu();
		break;
	case 3:
		if (this->changePwd(activeUser->id))
			activeUser->storeRecord();
		this->userMenu();
		break;
	case 4:
		this->rank(*activeUser);
		this->userMenu();
		break;
	case 5:
	default:
		activeUser = NULL;
		this->mainMenu();
		break;
	}
}


//将外部文本转为字符串，只保留字母
string Admin::txt2str(string filename)
{
	ifstream inf;
	inf.open(filename, ios::in);
	char c;
	string s;
	while (inf >> c)
	{
		if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
			s += c;
	}
	inf.close();
	return s;
}

void Admin::regist()
{
	system("cls");

	string id;
	string pwd;
	string nickname;
	cout << "请输入您的用户名：  (提示：不能包含空格、回车等空字符）" << endl << endl;
	cin >> id;
	//id重复
	if (this->users.find(id) != this->users.end())
	{
		cout << "此用户名已存在！" << endl;
		Sleep(SLEEP_DURATION);
		this->regist();
	}
	cout << "请输入您的密码：" << endl << endl;
	cin >> pwd;
	cout << "请输入您的昵称：" << endl << endl;
	cin >> nickname;

	this->users.insert(make_pair(id, User(id, nickname, pwd)));
	users[id].storeRecord();

	cout << "注册成功！" << endl
		<< "正在返回登录界面..." << endl;
	Sleep(SLEEP_DURATION);
	this->login();
}

void Admin::play()
{
	system("cls");
	int input;
	string str;
	float du;
	difficulty di;
	string fn;

	cout << "请选择您希望选择的文本序号：" << endl;
	for (int i = 0; i < this->src.size(); ++i)
		cout << i << ". " << this->src[i] << endl;
	cout << endl;
	cin >> input;
	if (input < 0 || input >= this->src.size())
	{
		cout << "输入不合法！" << endl;
		Sleep(SLEEP_DURATION);
		this->userMenu();
	}
	fn = this->src[input];
	str = txt2str("docs/" + fn + ".txt");

	system("cls");
	cout << "请选择游戏持续时间对应的序号：" << endl
		<< "1. 30s" << endl
		<< "2. 1min" << endl
		<< "3. 2min" << endl
		<< "4. 5min" << endl << endl;
	cin >> input;
	switch (input)
	{
	case 1:
		du = 0.5;
		break;
	case 2:
		du = 1;
		break;
	case 3:
		du = 2;
		break;
	case 4:
		du = 5;
		break;;
	default:
		du = 1;
		break;
	}

	system("cls");
	cout << "请选择游戏的难度序号：" << endl
		<< "1. EASY" << endl
		<< "2. MEDIUM" << endl
		<< "3. HARD" << endl
		<< "4. HELL" << endl << endl;
	cin >> input;
	switch (input)
	{
	case 1:
		di = EASY;
		break;
	case 2:
		di = MEDIUM;
		break;
	case 3:
		di = HARD;
		break;
	case 4:
		di = HELL;
		break;
	default:
		di = EASY;
		break;
	}

	Game* g = Game::getInstance(str, du, di);
	g->mainGame();
	//善后工作 存储记录
	activeUser->index++;
	record r(activeUser->index, g->diff, g->score, fn, g->duration, g->accuracy);
	activeUser->addRecord(r);
	activeUser->storeRecord();
	this->rankList.push_back(rank_pair(activeUser->id, g->score, fn));
	//输出此次游戏信息
	Sleep(2500);
	system("cls");
	cout << "本次游戏概况：\n"
		<< "编号： " << r.index
		<< "\n难度： " << enum2str(r.diff)
		<< "\n得分： " << r.score
		<< "\n持续时间： " << setprecision(2) << r.duration << "min"
		<< "\n正确率：" << fixed << setprecision(2) << r.accuracy << "%\n\n"
		<< "轻按任意键以继续..." << endl;
	while (1)
		if (_kbhit())
			break;
}

//同样需要输入ADMIN_PWD
//默认重置密码为DEFAULT_PWD
void Admin::resetPwd(string id)
{
	system("cls");

	string s;
	cout << "请输入管理员密码以重置密码：" << endl << endl;
	cin >> s;
	if (s != ADMIN_PWD)
	{
		cout << "o(*￣︶￣*)o小样儿~" << endl << endl;
		//cout << "密码错误，操作失败！" << endl << endl;
		Sleep(SLEEP_DURATION);
		return;
	}
	//重载
	if (id == "")
	{
		cout << "请输入对应的用户名：" << endl << endl;
		cin >> id;
	}
	if (this->users.find(id) == this->users.end())
	{
		cout << "该用户不存在！" << endl;
		Sleep(SLEEP_DURATION);
		return;
	}
	this->users[id].pwd = DEFAULT_PWD;
	this->users[id].storeRecord();
	cout << "密码已重置为：" << DEFAULT_PWD << endl;
	Sleep(SLEEP_DURATION);
}

bool Admin::changePwd(string id)
{
	system("cls");
	string input;
	cout << "请输入您的原密码：" << endl << endl;
	cin >> input;
	if (input != this->users[id].pwd)
	{
		cout << "原密码错误！" << endl;
		Sleep(SLEEP_DURATION);
		return false;
	}
	cout << "请输入您的新密码：" << endl << endl;
	cin >> input;
	this->users[id].pwd = input;
	cout << "密码重置成功！" << endl << endl;
	Sleep(SLEEP_DURATION);
	return true;
}

void Admin::addTxt(string filename)
{
	auto iter = find(this->src.begin(), this->src.end(), filename);
	if (iter != this->src.end())
	{
		cout << "该文本已存在！" << endl;
		Sleep(SLEEP_DURATION);
		return;
	}
	this->src.push_back(filename);
	cout << "文本添加成功！" << endl;
	Sleep(SLEEP_DURATION);
}

void Admin::rmTxt(string filename)
{
	auto iter = find(this->src.begin(), this->src.end(), filename);
	if (iter == this->src.end())
	{
		cout << "该文本不存在！" << endl;
		Sleep(SLEEP_DURATION);
		return;
	}
	this->src.erase(iter);
	cout << "文本删除成功！" << endl;
	Sleep(SLEEP_DURATION);
}

void Admin::storeTxts()
{
	ofstream outf;
	outf.open("docs/src.txt", ios::out | ios::trunc);
	for (auto fn : this->src)
		outf << fn << endl;
	outf.close();
}

void Admin::rank(const User& self)
{
	system("cls");
	sort(this->rankList.begin(), this->rankList.end());
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);//获取控制台的句柄，用于随意改变字体颜色
	cout << setiosflags(ios::left) << setw(10);
	cout << "用户名";
	cout << setiosflags(ios::left) << setw(10);
	cout << "得分";
	cout << setiosflags(ios::left) << setw(10);
	cout << "练习文本" << endl;
	for (int i = 0; i < 5 && i < this->rankList.size(); i++)
	{
		if (self.id == this->rankList[i].id)
		{
			SetConsoleTextAttribute(hOut,
				FOREGROUND_RED |
				FOREGROUND_INTENSITY);  //红色
			cout << setiosflags(ios::left) << setw(10);
			cout << this->rankList[i].id;
			cout << setiosflags(ios::left) << setw(10);
			cout << this->rankList[i].score;
			cout << setiosflags(ios::left) << setw(10);
			cout << this->rankList[i].file << endl;
			SetConsoleTextAttribute(hOut,
				FOREGROUND_RED |
				FOREGROUND_GREEN |
				FOREGROUND_BLUE);  //白色
		}
		else
		{
			cout << setiosflags(ios::left) << setw(10);
			cout << this->rankList[i].id;
			cout << setiosflags(ios::left) << setw(10);
			cout << this->rankList[i].score;
			cout << setiosflags(ios::left) << setw(10);
			cout << this->rankList[i].file << endl;
		}
	}
	cout << endl << endl << "轻按任意键以继续..." << endl;
	while (1)
	{
		if (_kbhit())
			break;
	}
}

