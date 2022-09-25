#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <algorithm>
#include <map>

using namespace std;

int numoff = 0;
int numofcurfile;
map<int, string> rec;

class Tree 
{
private:
	string name;
	int typekey;//0-dir,1-file,2-hidden
	int whencreated;
	vector<Tree> subtrees;
	void putObj(string);
	bool vecCheck(string);
	void PrintDir(string,int);// for -a
	void PrintDir(); // for -R
public:
	friend vector<string> Split(string, char);
	Tree(string);
	void printTrees(int);
	void StrMng(vector<string>);
	void FindFiles(vector<string>, int);
	
};

vector <string> Split(string s, char delim)
{
	vector <string> substr;
	istringstream iss(s);
	string item;
	while (getline(iss, item, delim))//delim '/'
	{
		substr.push_back(item);
	}

	substr.erase(substr.begin());
	return substr;
}

Tree::Tree(string nam)
{
	name = nam;
}

void Tree::printTrees(int lvl) //tree output
{
	if (subtrees.size() == 0)
	{
		return;
	}
	for (vector<Tree>::iterator i = subtrees.begin(); i != subtrees.end(); i++) //for (Tree& i : subtrees)
	{
		for (int j = 0; j < lvl; ++j)
		{
			cout << " ";
		}
		cout << i->name << "-" << i->typekey << endl;
		i->printTrees(lvl + 2);
	}
	
}

void Tree::putObj(string child)
{
	Tree obj(child);
	if (child[0] == '.')
	{
		obj.typekey = 2;
		obj.whencreated = numofcurfile;
		numofcurfile++;
	}
	else if (Split(child,'.').size() == 1)
	{
		obj.typekey = 1;
		obj.whencreated = numofcurfile;
		numofcurfile++;
	}
	else
	{
		obj.typekey = 0;
	}
	this->subtrees.push_back(obj);
}

void Tree::StrMng(vector<string> inp)
{
	
	if (vecCheck(inp[0]))
	{
		this->putObj(inp[0]);
	}
	
	for (vector<Tree>::iterator i = subtrees.begin(); i != subtrees.end(); i++)//for (Tree& i : subtrees)
	{
		if (inp[0] == i->name)
		{
			if (inp.size() == 1)
				return;
			inp.erase(inp.begin());
			i->StrMng(inp);
		}
	}

}

bool Tree::vecCheck(string newname)
{
	if (subtrees.size() == 0)
	{
		return true;
	}
	for (vector<Tree>::iterator i = subtrees.begin(); i != subtrees.end(); i++)
	{
		if (newname == i->name)
		{
			return false;//exists
		}
	}
	return true;//doesnt exists
}
void Tree::PrintDir(string dir,int key)
{
	if (subtrees.size() == 0)
	{
		return;
	}
	if (key == 0)
	{
		for (vector<Tree>::iterator i = subtrees.begin(); i != subtrees.end(); i++)//for (Tree& i : subtrees)
		{
			if (i->typekey == 1)
			{
				rec.insert(make_pair(i->whencreated, i->name));
				//cout << i->name << endl;
				numoff++;
			}
		}
	}
	else if (key == 2)
	{
		for (vector<Tree>::iterator i = subtrees.begin(); i != subtrees.end(); i++)//for (Tree& i : subtrees)
		{
			if (i->typekey == 1 || i->typekey == 2)
			{
				rec.insert(make_pair(i->whencreated, i->name));
				//cout << i->name << endl;
				numoff++;
			}
		}
	}
	else if (key == 1)
	{
		for (vector<Tree>::iterator i = subtrees.begin(); i != subtrees.end(); i++)//for (Tree& i : subtrees)
		{
			if (i->typekey == 1)
			{
				rec.insert(make_pair(i->whencreated, i->name));
			}
			i->PrintDir();
		}
	}
}

void Tree::PrintDir()
{
	if (subtrees.size() == 0)
	{
		return;
	}

	for (vector<Tree>::iterator i = subtrees.begin(); i != subtrees.end(); i++)//for (Tree& i : subtrees)
	{
		if (i->typekey == 1)
		{
			rec.insert(make_pair(i->whencreated, i->name));
			//cout << i->name<<"-"<<i->whencreated << endl;
			numoff++;
		}
		
		i->PrintDir();
	}
}

void Tree::FindFiles(vector<string> path, int key)
{
	for (vector<Tree>::iterator i=subtrees.begin(); i!=subtrees.end();i++)//for (Tree& i : subtrees)
	{
		if (i->name == path[0])
		{
			if (path.size() == 1)
			{
				i->PrintDir(path[0], key);
				return;
			}
			path.erase(path.begin());
			i->FindFiles(path, key);
		}
	}
}

int main()
{
	Tree root("rt");

	string inp,dir,lsc,key;
	vector<string> minp;
	vector<string> keys;
	while (true)
	{
		cin >> inp;
		if (inp=="cd")
		{
			break;
		}
		minp = Split(inp,'/');
		root.StrMng(minp);
		//root.printTrees(0); // tree output
		minp.clear();
	}
	cin >> dir;
	cin.ignore();
	getline(cin, key);

	minp.clear();
	minp = Split(dir,'/');
	
	keys = Split(key, ' ');

	if (keys.size() == 0) //0,1-R,2-a
	{
		root.FindFiles(minp, 0);
	}
	else if (keys[0] == "-R")
	{
		root.FindFiles(minp, 1);
	}
	else if (keys[0] == "-a")
	{
		root.FindFiles(minp, 2);
	}
	// output here
	if (numoff == 0)
	{
		cout << "NO FILES.";
	}
	else
	{
		map<int, string>::iterator it = rec.begin();
		for (int i = 0; it != rec.end(); ++i, ++it)
		{
			cout << it->second << endl;
		}
	}
}