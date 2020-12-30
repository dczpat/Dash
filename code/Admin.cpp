#include "Admin.h"
Admin* Admin::a = NULL;

Admin::Admin() :activeUser(NULL)
{
	ifstream inf;
	//ȡid
	inf.open("docs/ids.txt", ios::in);
	string id;
	while (inf >> id)
	{
		this->users.insert(make_pair(id, User(id)));
	}
	inf.close();
	//ȡ�ı�
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

//����Ա������ADMIN_PWD
void Admin::mainMenu()
{
	system("cls");
	int input;

	cout << "��ѡ�����Ĳ�����ţ�" << endl << endl
		<< "1. ��ҵ�¼" << endl
		<< "2. ���ע��" << endl
		<< "3. ����Ա����" << endl
		<< "4. �˳�����" << endl << endl;
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
		cout << "���������Ա�����Խ���ϵͳ��" << endl << endl;
		cin >> s;
		if (s != ADMIN_PWD)
		{
			system("cls");
			//cout << "����Ա�������" << endl << endl;
			cout << "o(*�����*)oС����~" << endl << endl;
			Sleep(SLEEP_DURATION);
			this->mainMenu();
		}
		this->adminMenu();
		break;
	case 4:
		system("cls");
		//��������
		this->storeIDs();
		this->storeTxts();
		cout << "�ټ�~~~" << endl;
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
	cout << "��ѡ�����Ĺ���Ա������ţ�" << endl << endl
		<< "1. �鿴�����ı�" << endl
		<< "2. ������ϰ�ı�" << endl
		<< "3. ɾ����ϰ�ı�" << endl
		<< "4. Ϊ�û���������" << endl
		<< "5. �������˵�" << endl << endl;
	cin >> input;
	switch (input)
	{
	case 1:
		system("cls");
		cout << "�����ǵ�ǰ���õ��ı���" << endl;
		for (int i = 0; i < this->src.size(); ++i)
			cout << i << ". " << this->src[i] << endl;
		cout << endl;
		cout << "�ᰴ������Լ���..." << endl;
		while (1)
		{
			if (_kbhit())
				break;
		}
		break;
	case 2:
		system("cls");
		cout << "������ϣ��������ı����ƣ�" << endl << endl;
		getline(cin, s);
		getline(cin, s);
		this->addTxt(s);
		break;
	case 3:
		system("cls");
		cout << "������ϣ��ɾ�����ı����ƣ�" << endl << endl;
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
	cout << "�����������û��������룺" << endl << endl;
	cin >> i >> p;
	auto iter = this->users.find(i);
	//�˻�������
	if (iter == this->users.end())
	{
		system("cls");
		int input;
		cout << "��Ǹ����������û���������..." << endl
			<< "��ѡ������ѡ������Լ�����" << endl
			<< "1. ��������" << endl
			<< "2. ע��һ�����˻�" << endl
			<< "3. �������˵�" << endl << endl;
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
	//��������
	else if (iter->second.pwd != p)
	{
		int input;
		cout << "��Ǹ�����������������..." << endl
			<< "��ѡ������ѡ������Լ�����" << endl
			<< "1. ��������" << endl
			<< "2. ���������˻�����" << endl
			<< "3. �������˵�" << endl << endl;
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
	//������Ϸ
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
	cout << "��ѡ��������Ҳ�����" << endl
		<< "1. ��ʼ��Ϸ��" << endl
		<< "2. �鿴��ϰ��¼" << endl
		<< "3. �޸�����" << endl
		<< "4. �鿴���а�" << endl
		<< "5. �������˵�" << endl << endl;

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


//���ⲿ�ı�תΪ�ַ�����ֻ������ĸ
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
	cout << "�����������û�����  (��ʾ�����ܰ����ո񡢻س��ȿ��ַ���" << endl << endl;
	cin >> id;
	//id�ظ�
	if (this->users.find(id) != this->users.end())
	{
		cout << "���û����Ѵ��ڣ�" << endl;
		Sleep(SLEEP_DURATION);
		this->regist();
	}
	cout << "�������������룺" << endl << endl;
	cin >> pwd;
	cout << "�����������ǳƣ�" << endl << endl;
	cin >> nickname;

	this->users.insert(make_pair(id, User(id, nickname, pwd)));
	users[id].storeRecord();

	cout << "ע��ɹ���" << endl
		<< "���ڷ��ص�¼����..." << endl;
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

	cout << "��ѡ����ϣ��ѡ����ı���ţ�" << endl;
	for (int i = 0; i < this->src.size(); ++i)
		cout << i << ". " << this->src[i] << endl;
	cout << endl;
	cin >> input;
	if (input < 0 || input >= this->src.size())
	{
		cout << "���벻�Ϸ���" << endl;
		Sleep(SLEEP_DURATION);
		this->userMenu();
	}
	fn = this->src[input];
	str = txt2str("docs/" + fn + ".txt");

	system("cls");
	cout << "��ѡ����Ϸ����ʱ���Ӧ����ţ�" << endl
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
	cout << "��ѡ����Ϸ���Ѷ���ţ�" << endl
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
	//�ƺ��� �洢��¼
	activeUser->index++;
	record r(activeUser->index, g->diff, g->score, fn, g->duration, g->accuracy);
	activeUser->addRecord(r);
	activeUser->storeRecord();
	this->rankList.push_back(rank_pair(activeUser->id, g->score, fn));
	//����˴���Ϸ��Ϣ
	Sleep(2500);
	system("cls");
	cout << "������Ϸ�ſ���\n"
		<< "��ţ� " << r.index
		<< "\n�Ѷȣ� " << enum2str(r.diff)
		<< "\n�÷֣� " << r.score
		<< "\n����ʱ�䣺 " << setprecision(2) << r.duration << "min"
		<< "\n��ȷ�ʣ�" << fixed << setprecision(2) << r.accuracy << "%\n\n"
		<< "�ᰴ������Լ���..." << endl;
	while (1)
		if (_kbhit())
			break;
}

//ͬ����Ҫ����ADMIN_PWD
//Ĭ����������ΪDEFAULT_PWD
void Admin::resetPwd(string id)
{
	system("cls");

	string s;
	cout << "���������Ա�������������룺" << endl << endl;
	cin >> s;
	if (s != ADMIN_PWD)
	{
		cout << "o(*�����*)oС����~" << endl << endl;
		//cout << "������󣬲���ʧ�ܣ�" << endl << endl;
		Sleep(SLEEP_DURATION);
		return;
	}
	//����
	if (id == "")
	{
		cout << "�������Ӧ���û�����" << endl << endl;
		cin >> id;
	}
	if (this->users.find(id) == this->users.end())
	{
		cout << "���û������ڣ�" << endl;
		Sleep(SLEEP_DURATION);
		return;
	}
	this->users[id].pwd = DEFAULT_PWD;
	this->users[id].storeRecord();
	cout << "����������Ϊ��" << DEFAULT_PWD << endl;
	Sleep(SLEEP_DURATION);
}

bool Admin::changePwd(string id)
{
	system("cls");
	string input;
	cout << "����������ԭ���룺" << endl << endl;
	cin >> input;
	if (input != this->users[id].pwd)
	{
		cout << "ԭ�������" << endl;
		Sleep(SLEEP_DURATION);
		return false;
	}
	cout << "���������������룺" << endl << endl;
	cin >> input;
	this->users[id].pwd = input;
	cout << "�������óɹ���" << endl << endl;
	Sleep(SLEEP_DURATION);
	return true;
}

void Admin::addTxt(string filename)
{
	auto iter = find(this->src.begin(), this->src.end(), filename);
	if (iter != this->src.end())
	{
		cout << "���ı��Ѵ��ڣ�" << endl;
		Sleep(SLEEP_DURATION);
		return;
	}
	this->src.push_back(filename);
	cout << "�ı���ӳɹ���" << endl;
	Sleep(SLEEP_DURATION);
}

void Admin::rmTxt(string filename)
{
	auto iter = find(this->src.begin(), this->src.end(), filename);
	if (iter == this->src.end())
	{
		cout << "���ı������ڣ�" << endl;
		Sleep(SLEEP_DURATION);
		return;
	}
	this->src.erase(iter);
	cout << "�ı�ɾ���ɹ���" << endl;
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
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);//��ȡ����̨�ľ������������ı�������ɫ
	cout << setiosflags(ios::left) << setw(10);
	cout << "�û���";
	cout << setiosflags(ios::left) << setw(10);
	cout << "�÷�";
	cout << setiosflags(ios::left) << setw(10);
	cout << "��ϰ�ı�" << endl;
	for (int i = 0; i < 5 && i < this->rankList.size(); i++)
	{
		if (self.id == this->rankList[i].id)
		{
			SetConsoleTextAttribute(hOut,
				FOREGROUND_RED |
				FOREGROUND_INTENSITY);  //��ɫ
			cout << setiosflags(ios::left) << setw(10);
			cout << this->rankList[i].id;
			cout << setiosflags(ios::left) << setw(10);
			cout << this->rankList[i].score;
			cout << setiosflags(ios::left) << setw(10);
			cout << this->rankList[i].file << endl;
			SetConsoleTextAttribute(hOut,
				FOREGROUND_RED |
				FOREGROUND_GREEN |
				FOREGROUND_BLUE);  //��ɫ
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
	cout << endl << endl << "�ᰴ������Լ���..." << endl;
	while (1)
	{
		if (_kbhit())
			break;
	}
}

