#include "User.h"

string enum2str(difficulty di)
{
	string ret;
	switch (di)
	{
	case EASY:
		ret = "EASY";
		break;
	case MEDIUM:
		ret = "MEDIUM";
		break;;
	case HARD:
		ret = "HARD";
		break;
	case HELL:
		ret = "HELL";
		break;
	default:
		break;
	}
	return ret;
}

difficulty str2enum(string di)
{
	difficulty ret;
	if (di == "EASY")
		ret = EASY;
	else if (di == "MEDIUM")
		ret = MEDIUM;
	else if (di == "HARD")
		ret = HARD;
	else if (di == "HELL")
		ret = HELL;
	return ret;
}

ostream& operator<<(ostream& os, const record& r)
{
	os << r.index << endl
		<< enum2str(r.diff) << endl
		<< r.score << endl
		<< r.file << endl
		<< r.duration << endl
		<< r.accuracy << endl;
	return os;
}

istream& operator>>(istream& is, record& r)
{
	string di;
	is >> r.index >> di >> r.score;
	is.ignore();
	getline(is, r.file);
	is >> r.duration >> r.accuracy;
	r.diff = str2enum(di);
	return is;
}


void User::displayRecord()
{
	system("cls");

	string di;
	cout << "Hi��" << this->nickname << "��\n�����������ϰ��¼��" << endl << endl;
	for (auto r : this->records)
		cout << "��ţ� " << r.index
		<< "\n�Ѷȣ� " << enum2str(r.diff)
		<< "\n�÷֣� " << r.score
		<< "\n��ϰ�ı��� " << r.file
		<< "\n����ʱ�䣺 " << setprecision(2) << r.duration << "min"
		<< "\n��ȷ�ʣ�" << fixed << setprecision(2) << r.accuracy << "%\n\n";
	cout << "�ᰴ������Լ���..." << endl;
	while (1)
	{
		if (_kbhit())
			return;
	}
}

void User::addRecord(record r)
{
	this->records.push_back(r);
}

void User::storeRecord()
{
	ofstream outf;
	outf.open("docs/" + this->id + ".txt", ios::out | ios::trunc);
	outf << this->nickname << endl
		<< this->pwd << endl
		<< this->index << endl;
	for (auto r : this->records)
		outf << r;
	outf.close();
}

User::User(string id) :id(id)
{
	ifstream inf;
	inf.open("docs/" + id + ".txt", ios::in);
	inf >> this->nickname
		>> this->pwd
		>> this->index;
	record r;
	for (int i = 0; i < this->index; ++i)
	{
		inf >> r;
		this->records.push_back(r);
	}
	inf.close();
}

User::User(string id, string nm, string pwd) :
	id(id), nickname(nm), pwd(pwd), index(0) {}

