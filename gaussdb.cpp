#include<iostream>
#include<algorithm>
#include<queue>
#include<cmath>
#include<iomanip>
#include<stack>
#include<map>
#include<vector>
#include<set>
#include<string>
#include<cstring>
#include<utility>
#include<string.h>
using namespace std;
typedef long long LL;
char s[100005]; char name[2000]; char text[10000]; string attribute_name[2000], values[2000]; int column[2000];
int min(int a, int b)
{
	return a < b ? a : b;
}
int max(int a, int b)
{
	return a > b ? a : b;
}
//字符串对比，只要两串的前cmplen个字符都相同，就返回0
int strcmp1(char a[], char b[], int cmplen = 20000)
{
	int len = min(cmplen, strlen(b));
	len = min(len, strlen(a));
	for (int i = 0; i < len; i++)
	{
		if (a[i] != b[i])return 1;
	}
	return 0;
}
//严格进行比较，只有两串完全相同才返回0
int strcmp2(char a[], char b[])
{
	int len = strlen(b);
	if (strlen(a) != len)return 1;
	for (int i = 0; i < len; i++)
	{
		if (a[i] != b[i])return 1;
	}
	return 0;
}
//判断字符串内是否含有点号
bool str_has_dot(string &a)//判断字符串中是否含有点号
{
	int i = 0;
	while (a[i] != '\0')
	{
		if (a[i] == '.')return true;
		i++;
	}
	return false;
}
bool str_has_dot(char a[])
{
	int i = 0;
	while (a[i] != '\0')
	{
		if (a[i] == '.')return true;
		i++;
	}
	return false;
}
//两个数字串或者两个text串进行比较，相同则返回allow_equal
bool greater_than(string &a, string &b, bool allow_equal = false)
{
	if ((a[0] != '-'&&!(a[0] >= '0'&&a[0] <= '9')) || (b[0] != '-'&&!(b[0] >= '0'&&b[0] <= '9')))//text类型之间的比较
	{
		if (allow_equal)return a >= b;
		return a > b;
	}
	//int类型之间的比较
	if (a[0] != '-'&&b[0] == '-')return true;
	if (a[0] == '-'&&b[0] != '-')return false;
	if (a[0] != '-'&&b[0] != '-')
	{
		int lena = a.length();
		int lenb = b.length();
		if (lena > lenb)return true;
		if (lena < lenb)return false;
		for (int i = 0; i < lena; i++)
		{
			if (a[i] > b[i])return true;
			if (a[i] < b[i])return false;
		}
		return allow_equal;
	}
	else
	{
		int lena = a.length();
		int lenb = b.length();
		if (lena > lenb)return false;
		if (lena < lenb)return true;
		for (int i = 1; i < lena; i++)
		{
			if (a[i] > b[i])return false;
			if (a[i] < b[i])return true;
		}
		return allow_equal;
	}
}
//转换成小写字母
void lowercase(char s[])
{
	int i = 0;
	while (s[i] != '\0')
	{
		if (s[i] >= 'A'&&s[i] <= 'Z')
		{
			s[i] = s[i] - 'A' + 'a';
		}
		i++;
	}
}
void lowercase(char &ch)
{
	if (ch >= 'A'&&ch <= 'Z')ch = ch - 'A' + 'a';
}
bool ischaracter(char ch)
{
	return (ch >= '0'&&ch <= '9' || ch >= 'a'&&ch <= 'z' || ch >= 'A'&&ch <= 'Z' || ch == '_' || ch == '*' || ch == '.' || ch == '-');
}
char* read_word(char &ch)//从当前位置往后读一个由合法字符组成的单词，执行完后，ch就是读完后遇到的第一个不合法字符
{
	ch = cin.get();
	while (!(ischaracter(ch)))ch = cin.get();
	if (ch == '*')//"*"单独算一个单词，特判掉select *from等情况
	{
		name[0] = '*'; name[1] = '\0'; return name;
	}
	int i = 0;
	while (ischaracter(ch))
	{
		lowercase(ch);
		name[i++] = ch;
		ch = cin.get();
	}
	name[i] = '\0';
	return name;
}
char* read_values(char &ch)//从当前位置往后读一个value，执行完后，ch就是读完后遇到的第一个不合法字符
{
	while ((!(ischaracter(ch))) && (ch != '\''))ch = cin.get();
	int i = 0;
	if (ch == '\'')
	{
		ch = cin.get();
		while (ch != '\'')
		{
			text[i++] = ch;
			ch = cin.get();
		}
	}
	else
	{
		if (ch == '-')
		{
			text[i++] = ch;
			ch = cin.get();
			while (!(ch >= '0'&&ch <= '9'))ch = cin.get();
		}
		while (ischaracter(ch))
		{
			lowercase(ch);
			text[i++] = ch;
			ch = cin.get();
		}
	}
	text[i] = '\0';
	return text;
}

map<string, vector<string>*>mp_table_pointer;// 从表名映射到二维表的二级指针
map<pair<string, string>, int>mp_which_column;//从<表名，属性名>映射到该属性在表中是第几列
map<string, int>mp_attribute_number;//从表名映射到这个表的列数
map<string, pair<vector<string>*, int>  >mp_table_dot_column;//从串"table_name.col"映射到对应的表指针和列号

vector<string>* create_table(string table_name, int attribute_number, string *attribute_name)
{
	vector<string>*p = new vector<string>[attribute_number];
	mp_table_pointer[table_name] = p;
	mp_attribute_number[table_name] = attribute_number;
	string table_name_dot = table_name + ".";
	for (int i = 0; i < attribute_number; i++)
	{
		p[i].push_back(attribute_name[i]);//在第0行插入各个属性名，p[i][0]即为这个表第i列的属性名，从第1行开始为属性值
		mp_which_column[make_pair(table_name, attribute_name[i])] = i;
		mp_table_dot_column[table_name_dot + attribute_name[i]] = make_pair(p, i);
	}
	return p;
}
void insert_into_table(string table_name, int values_number, string *values)
{
	vector<string>*p = mp_table_pointer[table_name];
	for (int i = 0; i < values_number; i++)
	{
		p[i].push_back(values[i]);
	}
}

struct Requirement//用来存放读入的查询条件
{
	vector<string>*p1;
	vector<string>*p2;
	int column_index;
	int column_index2;
	int comparison_operator = 0;//为0表示 col = constant ，为1表示 col != constant ，为2表示 col > constant，为3表示 col < constant，为4表示col >= constant，为5表示col <= constant
	string constant;
	int constant_type = 0;//为0表示常量为数字，为1表示常量为text，为2表示“常量”为另一列的列名
}require[42][42];//行与行之间为“or”关系，同一行内为“and”关系
int require_row_number = 0; int require_column_number[42]; //行的数量和每一行的条件数量

int which_column_in_table(string &constant, string &table_name)//返回列号，其中constant可以为column_name或者table_name.column_name的形式
{
	if (str_has_dot(constant))
	{
		return mp_table_dot_column[constant].second;
	}
	else return mp_which_column[make_pair(table_name, constant)];
}

bool should_be_selected(vector<string>*p, int row_index, string &table_name)//单表查询时，判断一行是否满足查询条件
{
	if (require_row_number == 0)return true;
	for (int i = 0; i < require_row_number; i++)
	{
		bool ret = true;
		for (int j = 0; j < require_column_number[i]; j++)
		{
			string &values = p[require[i][j].column_index][row_index];
			int &opt = require[i][j].comparison_operator;
			if (opt == 0)
			{
				if (require[i][j].constant_type != 2)//"col=int或'text'"的形式
				{
					ret = (values == require[i][j].constant);
					if (!ret)break;
				}
				else if (require[i][j].constant_type == 2)//"col1=col2" 的形式
				{
					int another_column_index = which_column_in_table(require[i][j].constant, table_name);
					ret = (values == p[another_column_index][row_index]);
					if (!ret)break;
				}
			}
			else if (opt == 1)
			{
				if (require[i][j].constant_type != 2)//"col!=int或'text'"的形式
				{
					ret = !(values == require[i][j].constant);
					if (!ret)break;
				}
				else if (require[i][j].constant_type == 2)//"col1!=col2" 的形式
				{
					int another_column_index = which_column_in_table(require[i][j].constant, table_name);
					ret = !(values == p[another_column_index][row_index]);
					if (!ret)break;
				}
			}
			else if (opt == 2)
			{
				if (require[i][j].constant_type != 2)//"col>int或'text'"的形式
				{
					ret = greater_than(values, require[i][j].constant);//可以处理对text比大小的情况
					if (!ret)break;
				}
				else if (require[i][j].constant_type == 2)//"col1>col2" 的形式
				{
					int another_column_index = which_column_in_table(require[i][j].constant, table_name);
					ret = greater_than(values, p[another_column_index][row_index]);
					if (!ret)break;
				}
			}
			else if (opt == 3)
			{
				if (require[i][j].constant_type != 2)//"col<int或'text'"的形式
				{
					ret = !(greater_than(values, require[i][j].constant, true));
					if (!ret)break;
				}
				else if (require[i][j].constant_type == 2)//"col1<col2" 的形式
				{
					int another_column_index = which_column_in_table(require[i][j].constant, table_name);
					ret = !greater_than(values, p[another_column_index][row_index], true);
					if (!ret)break;
				}
			}
			else if (opt == 4)
			{
				if (require[i][j].constant_type != 2)//"col>=int或'text'"的形式
				{
					ret = greater_than(values, require[i][j].constant, true);
					if (!ret)break;
				}
				else if (require[i][j].constant_type == 2)//"col1>=col2" 的形式
				{
					int another_column_index = which_column_in_table(require[i][j].constant, table_name);
					ret = greater_than(values, p[another_column_index][row_index], true);
					if (!ret)break;
				}
			}
			else if (opt == 5)
			{
				if (require[i][j].constant_type != 2)//"col<=int或'text'"的形式
				{
					ret = !(greater_than(values, require[i][j].constant));
					if (!ret)break;
				}
				else if (require[i][j].constant_type == 2)//"col1<=col2" 的形式
				{
					int another_column_index = which_column_in_table(require[i][j].constant, table_name);
					ret = !(greater_than(values, p[another_column_index][row_index]));
					if (!ret)break;
				}
			}
		}
		if (ret)return true;
	}
	return false;
}

bool should_be_selected3(int row1_index, int row2_index, vector<string>*t1, vector<string>*t2)//两表笛卡尔积后，判断新表中的一行是否满足查询条件
{
	if (require_row_number == 0)return true;
	for (int i = 0; i < require_row_number; i++)
	{
		bool ret = true;
		for (int j = 0; j < require_column_number[i]; j++)
		{
			string values;
			if (require[i][j].p1 == t1)
			{
				values = (require[i][j].p1)[require[i][j].column_index][row1_index];
			}
			else values = (require[i][j].p1)[require[i][j].column_index][row2_index];
			int &opt = require[i][j].comparison_operator;
			if (opt == 0)
			{
				if (require[i][j].constant_type != 2)//"col=int或'text'"的形式
				{
					ret = (values == require[i][j].constant);
					if (!ret)break;
				}
				else if (require[i][j].constant_type == 2)//"col1=col2" 的形式
				{
					if (require[i][j].p2 == t1)
					{
						ret = (values == (require[i][j].p2)[require[i][j].column_index2][row1_index]);
					}
					else ret = (values == (require[i][j].p2)[require[i][j].column_index2][row2_index]);
					if (!ret)break;
				}
			}
			else if (opt == 1)
			{
				if (require[i][j].constant_type != 2)//"col!=int或'text'"的形式
				{
					ret = !(values == require[i][j].constant);
					if (!ret)break;
				}
				else if (require[i][j].constant_type == 2)//"col1!=col2" 的形式
				{
					if (require[i][j].p2 == t1)
					{
						ret = !(values == (require[i][j].p2)[require[i][j].column_index2][row1_index]);
					}
					else ret = !(values == (require[i][j].p2)[require[i][j].column_index2][row2_index]);
					if (!ret)break;
				}
			}
			else if (opt == 2)
			{
				if (require[i][j].constant_type != 2)//"col>int或'text'"的形式
				{
					ret = greater_than(values, require[i][j].constant);
					if (!ret)break;
				}
				else if (require[i][j].constant_type == 2)//"col1>col2" 的形式
				{
					if (require[i][j].p2 == t1)
					{
						ret = greater_than(values, (require[i][j].p2)[require[i][j].column_index2][row1_index]);
					}
					else ret = greater_than(values, (require[i][j].p2)[require[i][j].column_index2][row2_index]);
					if (!ret)break;
				}
			}
			else if (opt == 3)
			{
				if (require[i][j].constant_type != 2)//"col<int或'text'"的形式
				{
					ret = !(greater_than(values, require[i][j].constant, true));
					if (!ret)break;
				}
				else if (require[i][j].constant_type == 2)//"col1<col2" 的形式
				{
					if (require[i][j].p2 == t1)
					{
						ret = !greater_than(values, (require[i][j].p2)[require[i][j].column_index2][row1_index], true);
					}
					else ret = !greater_than(values, (require[i][j].p2)[require[i][j].column_index2][row2_index], true);
					if (!ret)break;
				}
			}
			else if (opt == 4)
			{
				if (require[i][j].constant_type != 2)//"col>=int或'text'"的形式
				{
					ret = greater_than(values, require[i][j].constant, true);
					if (!ret)break;
				}
				else if (require[i][j].constant_type == 2)//"col1>=col2" 的形式
				{
					if (require[i][j].p2 == t1)
					{
						ret = greater_than(values, (require[i][j].p2)[require[i][j].column_index2][row1_index], true);
					}
					else ret = greater_than(values, (require[i][j].p2)[require[i][j].column_index2][row2_index], true);
					if (!ret)break;
				}
			}
			else if (opt == 5)
			{
				if (require[i][j].constant_type != 2)//"col<=int或'text'"的形式
				{
					ret = !(greater_than(values, require[i][j].constant));
					if (!ret)break;
				}
				else if (require[i][j].constant_type == 2)//"col1<=col2" 的形式
				{
					if (require[i][j].p2 == t1)
					{
						ret = !(greater_than(values, (require[i][j].p2)[require[i][j].column_index2][row1_index]));
					}
					else ret = !(greater_than(values, (require[i][j].p2)[require[i][j].column_index2][row2_index]));
					if (!ret)break;
				}
			}
		}
		if (ret)return true;
	}
	return false;
}
bool should_be_selected_for_one_table(vector<string>*p, int row_index, string&table_name,int require_row_number=1)//在所有条件中，只使用针对此表的条件进行选择
{
	if (require_row_number == 0)return true;
	for (int i = require_row_number-1; i < require_row_number; i++)
	{
		bool ret = true;
		for (int j = 0; j < require_column_number[i]; j++)
		{
			if (!(require[i][j].p1 == p&&require[i][j].p2 == p))continue;//忽视连接条件和针对另一个表的条件
			string values = p[require[i][j].column_index][row_index];
			int &opt = require[i][j].comparison_operator;
			if (opt == 0)
			{
				if (require[i][j].constant_type != 2)//"col=int或'text'"的形式
				{
					ret = (values == require[i][j].constant);
					if (!ret)break;
				}
				else if (require[i][j].constant_type == 2)//"col1=col2" 的形式
				{
					ret = (values == p[require[i][j].column_index2][row_index]);
					if (!ret)break;
				}
			}
			else if (opt == 1)
			{
				if (require[i][j].constant_type != 2)//"col!=int或'text'"的形式
				{
					ret = !(values == require[i][j].constant);
					if (!ret)break;
				}
				else if (require[i][j].constant_type == 2)//"col1!=col2" 的形式
				{
					ret = !(values == p[require[i][j].column_index2][row_index]);
					if (!ret)break;
				}
			}
			else if (opt == 2)
			{
				if (require[i][j].constant_type != 2)//"col>int或'text'"的形式
				{
					ret = greater_than(values, require[i][j].constant);
					if (!ret)break;
				}
				else if (require[i][j].constant_type == 2)//"col1>col2" 的形式
				{
					ret = greater_than(values, p[require[i][j].column_index2][row_index]);
					if (!ret)break;
				}
			}
			else if (opt == 3)
			{
				if (require[i][j].constant_type != 2)//"col<int或'text'"的形式
				{
					ret = !(greater_than(values, require[i][j].constant, true));
					if (!ret)break;
				}
				else if (require[i][j].constant_type == 2)//"col1<col2" 的形式
				{
					ret = !greater_than(values, p[require[i][j].column_index2][row_index], true);
					if (!ret)break;
				}
			}
			else if (opt == 4)
			{
				if (require[i][j].constant_type != 2)//"col>=int或'text'"的形式
				{
					ret = greater_than(values, require[i][j].constant, true);
					if (!ret)break;
				}
				else if (require[i][j].constant_type == 2)//"col1>=col2" 的形式
				{
					ret = greater_than(values, p[require[i][j].column_index2][row_index], true);
					if (!ret)break;
				}
			}
			else if (opt == 5)
			{
				if (require[i][j].constant_type != 2)//"col<=int或'text'"的形式
				{
					ret = !(greater_than(values, require[i][j].constant));
					if (!ret)break;
				}
				else if (require[i][j].constant_type == 2)//"col1<=col2" 的形式
				{
					ret = !(greater_than(values, p[require[i][j].column_index2][row_index]));
					if (!ret)break;
				}
			}
		}
		if (ret)return true;
	}
	return false;
}
void select_from_a_table(string table_name, int column_number, string* attribute_name, bool without_requirement = true)
{//单表查询，全表扫描法
	int real_column_number = 0; int width_of_table = mp_attribute_number[table_name];
	//处理读入的属性名
	for (int i = 0; i < column_number; i++)
	{
		if (attribute_name[i] == "*")
		{
			for (int j = 0; j < width_of_table; j++)
			{
				column[real_column_number++] = j;
			}
		}
		else
		{
			if (str_has_dot(attribute_name[i]))
			{
				column[real_column_number++] = mp_table_dot_column[attribute_name[i]].second;
			}
			else
			{
				column[real_column_number++] = mp_which_column[make_pair(table_name, attribute_name[i])];
			}
		}
	}
	vector<string>*p = mp_table_pointer[table_name];
	int table_len = p[0].size();
	for (int i = 1; i < table_len; i++)//遍历行
	{
		if (without_requirement || should_be_selected(p, i, table_name))
		{
			cout << p[column[0]][i];
			for (int j = 1; j < real_column_number; j++)//输出每一列
			{
				cout << "|" << p[column[j]][i];
			}
			cout << "\n";
		}
	}
}

vector<string>* table1; vector<string>* table2;//参与表连接的两个表的指针
bool has_join_requirement = false;
bool equal_join = true;//连接条件为等值/非等值连接
bool there_is_no_OR = true;//连接条件是否存在“OR”
int t1_column_index; int t2_column_index;//（如果有连接条件）用于存储连接条件中的两个列号
vector<int> t1_selected_row;
vector<int> t2_selected_row;//存储选择运算之后，两个表各自被选出的那些行的行号

bool cmp_t1_row(int a, int b)
{
	return table1[t1_column_index][a] < table1[t1_column_index][b];
}
bool cmp_t2_row(int a, int b)
{
	return table2[t2_column_index][a] < table2[t2_column_index][b];
}

struct TandC
{
	vector<string>*p;
	int column_index;
}tandc[2000];

void convert_to_tandc(int &column_number, string* attribute_name, vector<string>*p1, vector<string>*p2, string&table1_name, string&table2_name)
{//将读入的col，t1.col等转换成对应的表指针和列号
	int real_column_number = 0;
	int width_of_table1 = mp_attribute_number[table1_name], width_of_table2 = mp_attribute_number[table2_name];
	for (int i = 0; i < column_number; i++)
	{
		if (attribute_name[i] == "*")
		{
			for (int j = 0; j < width_of_table1; j++)
			{
				tandc[real_column_number].p = p1;
				tandc[real_column_number++].column_index = j;
			}
			for (int j = 0; j < width_of_table2; j++)
			{
				tandc[real_column_number].p = p2;
				tandc[real_column_number++].column_index = j;
			}
		}
		else
		{
			if (str_has_dot(attribute_name[i]))
			{
				tandc[real_column_number].p = mp_table_dot_column[attribute_name[i]].first;
				tandc[real_column_number++].column_index = mp_table_dot_column[attribute_name[i]].second;
			}
			else
			{
				if (mp_which_column.count(make_pair(table1_name, attribute_name[i])))
				{
					tandc[real_column_number].p = p1;
					tandc[real_column_number++].column_index = mp_which_column[make_pair(table1_name, attribute_name[i])];
				}
				else
				{
					tandc[real_column_number].p = p2;
					tandc[real_column_number++].column_index = mp_which_column[make_pair(table2_name, attribute_name[i])];
				}
			}
		}
	}
	column_number = real_column_number;//用实际要求输出的列数目替代原来读到的列数目
}

void select_join_table_by_cartesian_product_without_OR(string table1_name, string table2_name, int column_number, string* attribute_name)
{//适用于两表查询，无连接条件，可有普通条件，但不存在“OR”(也就是只涉及一次表连接)
	vector<string>*p1 = mp_table_pointer[table1_name];
	vector<string>*p2 = mp_table_pointer[table2_name];
	int table1_len = p1[0].size();
	int table2_len = p2[0].size();
	convert_to_tandc(column_number, attribute_name, p1, p2, table1_name, table2_name);
	t1_selected_row.clear(); t2_selected_row.clear();
	for (int i = 1; i < table1_len; i++)//选择运算先做
	{
		if (should_be_selected_for_one_table(p1, i, table1_name))
		{
			t1_selected_row.push_back(i);
		}
	}
	for (int i = 1; i < table2_len; i++)
	{
		if (should_be_selected_for_one_table(p2, i, table2_name))
		{
			t2_selected_row.push_back(i);
		}
	}
	int size1 = t1_selected_row.size(); int size2 = t2_selected_row.size();
	for (int i1 = 0; i1 < size1; i1++)//选完再笛卡尔积
	{
		for (int i2 = 0; i2 < size2; i2++)
		{
			if (tandc[0].p == p1)
			{
				cout << (tandc[0].p)[tandc[0].column_index][t1_selected_row[i1]];
			}
			else cout << (tandc[0].p)[tandc[0].column_index][t2_selected_row[i2]];
			for (int j = 1; j < column_number; j++)
			{
				if (tandc[j].p == p1)
				{
					cout << "|" << (tandc[j].p)[tandc[j].column_index][t1_selected_row[i1]];
				}
				else cout << "|" << (tandc[j].p)[tandc[j].column_index][t2_selected_row[i2]];
			}
			cout << "\n";
		}
	}
}
void select_join_table_by_cartesian_product(string table1_name, string table2_name, int column_number, string* attribute_name)
{//适用于任意形式的两表连接查询，但要求两表规模较小
	vector<string>*p1 = mp_table_pointer[table1_name];
	vector<string>*p2 = mp_table_pointer[table2_name];
	int table1_len = p1[0].size();
	int table2_len = p2[0].size();
	convert_to_tandc(column_number, attribute_name, p1, p2, table1_name, table2_name);
	for (int i1 = 1; i1 < table1_len; i1++)//先笛卡尔积
	{
		for (int i2 = 1; i2 < table2_len; i2++)
		{
			if (should_be_selected3(i1, i2, p1, p2))//如果新表中的这一行满足条件，就输出
			{
				if (tandc[0].p == p1)
				{
					cout << (tandc[0].p)[tandc[0].column_index][i1];
				}
				else cout << (tandc[0].p)[tandc[0].column_index][i2];
				for (int j = 1; j < column_number; j++)
				{
					if (tandc[j].p == p1)
					{
						cout << "|" << (tandc[j].p)[tandc[j].column_index][i1];
					}
					else cout << "|" << (tandc[j].p)[tandc[j].column_index][i2];
				}
				cout << "\n";
			}
		}
	}
}

void select_join_table_with_join_requirement_without_OR(string table1_name, string table2_name, int column_number, string* attribute_name)
{//适用于两表等值连接查询，且不存在“OR”
	vector<string>*p1 = mp_table_pointer[table1_name];
	vector<string>*p2 = mp_table_pointer[table2_name];
	int table1_len = p1[0].size();
	int table2_len = p2[0].size();
	convert_to_tandc(column_number, attribute_name, p1, p2, table1_name, table2_name);
	t1_selected_row.clear(); t2_selected_row.clear();
	for (int i = 1; i < table1_len; i++)//选择运算先做
	{
		if (should_be_selected_for_one_table(p1, i, table1_name))
		{
			t1_selected_row.push_back(i);
		}
	}
	for (int i = 1; i < table2_len; i++)
	{
		if (should_be_selected_for_one_table(p2, i, table2_name))
		{
			t2_selected_row.push_back(i);
		}
	}
	sort(t1_selected_row.begin(), t1_selected_row.end(), cmp_t1_row);//选完后，两表各自连接条件排序
	sort(t2_selected_row.begin(), t2_selected_row.end(), cmp_t2_row);

	int row1 = 0, row2 = 0,row2_last=0, len1 = t1_selected_row.size(), len2 = t2_selected_row.size();
	while (row1 < len1&&row2 < len2)//排序合并算法
	{
		if (p1[t1_column_index][t1_selected_row[row1]] == p2[t2_column_index][t2_selected_row[row2]])
		{
			if (tandc[0].p == p1)
			{
				cout << (tandc[0].p)[tandc[0].column_index][t1_selected_row[row1]];
			}
			else cout << (tandc[0].p)[tandc[0].column_index][t2_selected_row[row2]];
			for (int j = 1; j < column_number; j++)
			{
				if (tandc[j].p == p1)
				{
					cout << "|" << (tandc[j].p)[tandc[j].column_index][t1_selected_row[row1]];
				}
				else cout << "|" << (tandc[j].p)[tandc[j].column_index][t2_selected_row[row2]];
			}
			cout << "\n";
			if (row2 + 1 < len2&&p2[t2_column_index][t2_selected_row[row2]] == p2[t2_column_index][t2_selected_row[row2 + 1]])row2++;
			else if (row1 + 1 < len1&&p1[t1_column_index][t1_selected_row[row1]] == p1[t1_column_index][t1_selected_row[row1 + 1]])
			{
				row1++;
				row2 = row2_last;
			}
			else
			{
				row1++; row2++;
				row2_last = row2;
			}
		}
		while (row1 < len1&&row2 < len2&&p1[t1_column_index][t1_selected_row[row1]] > p2[t2_column_index][t2_selected_row[row2]])
		{
			row2++; row2_last = row2;
		}
		while (row1 < len1&&row2 < len2&&p1[t1_column_index][t1_selected_row[row1]] < p2[t2_column_index][t2_selected_row[row2]])row1++;
	}
}

map<pair<int, int>, int>mp_row_pair;//用于存储被输出过的<row1_index,row2_index>组合
void select_join_table_with_OR(string table1_name, string table2_name, int column_number, string* attribute_name)
{//适用于存在OR的两表连接，可以支持大表连接
	vector<string>*p1 = mp_table_pointer[table1_name];
	vector<string>*p2 = mp_table_pointer[table2_name];
	int table1_len = p1[0].size();
	int table2_len = p2[0].size();
	mp_row_pair.clear();//清空map
	convert_to_tandc(column_number, attribute_name, p1, p2, table1_name, table2_name);
	for (int k = 1; k <= require_row_number; k++)//条件矩阵中每有一行，都要做一次连接运算，行内只有AND，行间为OR关系
	{
		t1_selected_row.clear(); t2_selected_row.clear();
		for (int i = 1; i < table1_len; i++)//选择运算先做
		{
			if (should_be_selected_for_one_table(p1, i, table1_name,k))
			{
				t1_selected_row.push_back(i);
			}
		}
		for (int i = 1; i < table2_len; i++)
		{
			if (should_be_selected_for_one_table(p2, i, table2_name,k))
			{
				t2_selected_row.push_back(i);
			}
		}
		//判断此次连接应该用什么算法
		bool  has_join_requirement_flag = false;
		for (int i = 0; i < require_column_number[k - 1]; i++)
		{
			if (require[k - 1][i].p1 != require[k - 1][i].p2)
			{
				has_join_requirement_flag = true;
				if (p1 == require[k - 1][i].p1)
				{
					t1_column_index = require[k - 1][i].column_index;
					t2_column_index = require[k - 1][i].column_index2;
				}
				else
				{
					t1_column_index = require[k - 1][i].column_index2;
					t2_column_index = require[k - 1][i].column_index;
				}
				break;
			}
		}
		if (has_join_requirement_flag)//有连接条件，使用排序合并连接
		{
			sort(t1_selected_row.begin(), t1_selected_row.end(), cmp_t1_row);//选完后，两表各自连接条件排序
			sort(t2_selected_row.begin(), t2_selected_row.end(), cmp_t2_row);

			int row1 = 0, row2 = 0, row2_last = 0, len1 = t1_selected_row.size(), len2 = t2_selected_row.size();
			while (row1 < len1&&row2 < len2)
			{
				if (p1[t1_column_index][t1_selected_row[row1]] == p2[t2_column_index][t2_selected_row[row2]])
				{
					pair<int, int>combination(t1_selected_row[row1], t2_selected_row[row2]);
					if (!mp_row_pair.count(combination))//只有当这种组合不曾被输出过，才进行输出
					{
						mp_row_pair[combination] = 1;//记录下这种组合，并输出
						if (tandc[0].p == p1)
						{
							cout << (tandc[0].p)[tandc[0].column_index][t1_selected_row[row1]];
						}
						else cout << (tandc[0].p)[tandc[0].column_index][t2_selected_row[row2]];
						for (int j = 1; j < column_number; j++)
						{
							if (tandc[j].p == p1)
							{
								cout << "|" << (tandc[j].p)[tandc[j].column_index][t1_selected_row[row1]];
							}
							else cout << "|" << (tandc[j].p)[tandc[j].column_index][t2_selected_row[row2]];
						}
						cout << "\n";
					}
					
					if (row2 + 1 < len2&&p2[t2_column_index][t2_selected_row[row2]] == p2[t2_column_index][t2_selected_row[row2 + 1]])row2++;
					else if (row1 + 1 < len1&&p1[t1_column_index][t1_selected_row[row1]] == p1[t1_column_index][t1_selected_row[row1 + 1]])
					{
						row1++;
						row2 = row2_last;
					}
					else
					{
						row1++; row2++;
						row2_last = row2;
					}
				}
				while (row1 < len1&&row2 < len2&&p1[t1_column_index][t1_selected_row[row1]] > p2[t2_column_index][t2_selected_row[row2]])
				{
					row2++; row2_last = row2;
				}
				while (row1 < len1&&row2 < len2&&p1[t1_column_index][t1_selected_row[row1]] < p2[t2_column_index][t2_selected_row[row2]])row1++;
			}
		}
		else//没有连接条件，直接笛卡尔积
		{
			int size1 = t1_selected_row.size(); int size2 = t2_selected_row.size();
			for (int i1 = 0; i1 < size1; i1++)//选完直接笛卡尔积
			{
				for (int i2 = 0; i2 < size2; i2++)
				{
					pair<int, int>combination(t1_selected_row[i1], t2_selected_row[i2]);
					if (!mp_row_pair.count(combination))
					{
						mp_row_pair[combination] = 1;
						if (tandc[0].p == p1)
						{
							cout << (tandc[0].p)[tandc[0].column_index][t1_selected_row[i1]];
						}
						else cout << (tandc[0].p)[tandc[0].column_index][t2_selected_row[i2]];
						for (int j = 1; j < column_number; j++)
						{
							if (tandc[j].p == p1)
							{
								cout << "|" << (tandc[j].p)[tandc[j].column_index][t1_selected_row[i1]];
							}
							else cout << "|" << (tandc[j].p)[tandc[j].column_index][t2_selected_row[i2]];
						}
						cout << "\n";
					}
				}
			}
		}
	}
}

void read_all_requirements_one_table(string &table_name, char &ch)
{//适用于单表查询，在读掉where之后，调用此函数来读入所有查询条件
	require_row_number = 0; int j = 0;
	while (ch != ';') //句尾的分号
	{
		read_values(ch); int already_read_column = 0;
		if (ch == '\'') //如果是text，则有单引号开头
		{
			require[require_row_number][j].constant = text;
			require[require_row_number][j].constant_type = 1;
		}
		else if (text[0] >= '0'&&text[0] <= '9' || text[0] == '-') //int
		{
			require[require_row_number][j].constant = text;
			require[require_row_number][j].constant_type = 0;
		}
		else //column_name或table_name.column_name
		{
			if (str_has_dot(text))
			{
				require[require_row_number][j].column_index = mp_table_dot_column[text].second;
			}
			else require[require_row_number][j].column_index = mp_which_column[make_pair(table_name, text)];
			already_read_column = 1;
		}
		//读入比较运算符，并以正确形式记录下来
		while (ch != '='&&ch != '!'&&ch != '>'&&ch != '<')ch = cin.get();
		if (ch == '=')
		{
			require[require_row_number][j].comparison_operator = 0;
		}
		else if (ch == '!')
		{
			require[require_row_number][j].comparison_operator = 1;
			while (ch != '=')ch = cin.get();
		}
		else if (ch == '>')
		{
			if (cin.peek() != '=')
			{
				if (already_read_column == 1)
				{
					require[require_row_number][j].comparison_operator = 2;
				}
				else require[require_row_number][j].comparison_operator = 3;
			}
			else
			{
				ch = cin.get();
				if (already_read_column == 1)
				{
					require[require_row_number][j].comparison_operator = 4;
				}
				else require[require_row_number][j].comparison_operator = 5;
			}
		}
		else if (ch == '<')
		{
			if (cin.peek() != '=')
			{
				if (already_read_column == 1)
				{
					require[require_row_number][j].comparison_operator = 3;
				}
				else require[require_row_number][j].comparison_operator = 2;
			}
			else
			{
				ch = cin.get();
				if (already_read_column == 1)
				{
					require[require_row_number][j].comparison_operator = 5;
				}
				else require[require_row_number][j].comparison_operator = 4;
			}
		}
		//运算符右边的表达式
		read_values(ch);
		if (ch == '\'')
		{
			require[require_row_number][j].constant = text;
			require[require_row_number][j].constant_type = 1;
		}
		else if (text[0] >= '0'&&text[0] <= '9' || text[0] == '-')
		{
			require[require_row_number][j].constant = text;
			require[require_row_number][j].constant_type = 0;
		}
		else
		{
			if (already_read_column == 0)
			{
				if (str_has_dot(text))
				{
					require[require_row_number][j].column_index = mp_table_dot_column[text].second;
				}
				else require[require_row_number][j].column_index = mp_which_column[make_pair(table_name, text)];
			}
			else if (already_read_column == 1)
			{
				require[require_row_number][j].constant = text;
				require[require_row_number][j].constant_type = 2;
			}
		}
		while (ch == ' ' || ch == '\n' || ch == '\'')ch = cin.get();
		if (ch == ';')
		{
			require_column_number[require_row_number] = j + 1;
			require_row_number++;
			break;
		}
		if (ch == 'a' || ch == 'A')//遇到 and
		{
			read_word(ch);
			j++;
		}
		else if (ch == 'o' || ch == 'O')//遇到 or
		{
			read_word(ch);
			require_column_number[require_row_number] = j + 1;
			require_row_number++;
			j = 0;
		}
	}
}

void read_all_requirements_two_tables2(string &table1_name, string &table2_name, char &ch)
{//适用于两表查询，读掉where之后，调用此函数读入所有查询条件
	require_row_number = 0; int j1 = 0;
	has_join_requirement = false;
	there_is_no_OR = true;
	string first_word, second_word; int first_type = 0, second_type = 0, opt = 0;//type=1表示读到的word为 'text'，0表示数字串，其他用2表示
	vector<string>*ta; vector<string>*tb;
	int cola, colb;
	vector<string>*t1 = mp_table_pointer[table1_name];
	vector<string>*t2 = mp_table_pointer[table2_name];
	while (ch != ';')
	{
		read_values(ch);
		int already_read_column = 0;
		if (ch == '\'')
		{
			first_type = 1;
		}
		else if (text[0] >= '0'&&text[0] <= '9' || text[0] == '-')
		{
			first_type = 0;
		}
		else
		{
			first_type = 2;
			already_read_column = 1;
		}
		first_word = text;
		while (ch != '='&&ch != '!'&&ch != '>'&&ch != '<')ch = cin.get();
		if (ch == '=')
		{
			opt = 0;
		}
		else if (ch == '!')
		{
			opt = 1;
			while (ch != '=')ch = cin.get();
		}
		else if (ch == '>')
		{
			if (cin.peek() != '=')
			{
				if (already_read_column == 1)
				{
					opt = 2;
				}
				else opt = 3;
			}
			else
			{
				ch = cin.get();
				if (already_read_column == 1)
				{
					opt = 4;
				}
				else opt = 5;
			}
		}
		else if (ch == '<')
		{
			if (cin.peek() != '=')
			{
				if (already_read_column == 1)
				{
					opt = 3;
				}
				else opt = 2;
			}
			else
			{
				ch = cin.get();
				if (already_read_column == 1)
				{
					opt = 5;
				}
				else opt = 4;
			}
		}
		read_values(ch);
		if (ch == '\'')
		{
			second_type = 1;
		}
		else if (text[0] >= '0'&&text[0] <= '9' || text[0] == '-')
		{
			second_type = 0;
		}
		else
		{
			second_type = 2;
		}
		second_word = text;
		//读完了一个条件单元，进行处理
		if (first_type == 2 && second_type == 2)//ta.cola=tb.colb的形式
		{
			if (str_has_dot(first_word))
			{
				ta = mp_table_dot_column[first_word].first;
				cola = mp_table_dot_column[first_word].second;
			}
			else
			{
				if (mp_which_column.count(make_pair(table1_name, first_word)))
				{
					ta = t1;
					cola = mp_which_column[make_pair(table1_name, first_word)];
				}
				else
				{
					ta = t2;
					cola = mp_which_column[make_pair(table2_name, first_word)];
				}
			}
			if (str_has_dot(second_word))
			{
				tb = mp_table_dot_column[second_word].first;
				colb = mp_table_dot_column[second_word].second;
			}
			else
			{
				if (mp_which_column.count(make_pair(table1_name, second_word)))
				{
					tb = t1;
					colb = mp_which_column[make_pair(table1_name, second_word)];
				}
				else
				{
					tb = t2;
					colb = mp_which_column[make_pair(table2_name, second_word)];
				}
			}
			if (ta == tb)//条件单元左右两边对应同一个表
			{
				require[require_row_number][j1].column_index = cola;
				require[require_row_number][j1].column_index2 = colb;
				require[require_row_number][j1].comparison_operator = opt;
				require[require_row_number][j1].constant = second_word;
				require[require_row_number][j1].constant_type = 2;
				if (ta == t1)
				{
					require[require_row_number][j1].p1 = t1;
					require[require_row_number][j1].p2 = t1;
				}
				else
				{
					require[require_row_number][j1].p1 = t2;
					require[require_row_number][j1].p2 = t2;
				}
			}
			else//否则，一个条件单元涉及两个不同表，为连接条件
			{
				table1 = t1;
				table2 = t2;
				has_join_requirement = true;
				if (opt == 1)equal_join = false;
				else equal_join = true;
				if (table1 == ta)
				{
					t1_column_index = cola;
					t2_column_index = colb;
					require[require_row_number][j1].column_index = cola;
					require[require_row_number][j1].column_index2 = colb;
					require[require_row_number][j1].comparison_operator = opt;
					require[require_row_number][j1].constant = second_word;
					require[require_row_number][j1].constant_type = 2;
					require[require_row_number][j1].p1 = ta;
					require[require_row_number][j1].p2 = tb;
				}
				else
				{
					t1_column_index = colb;
					t2_column_index = cola;
					require[require_row_number][j1].column_index = colb;
					require[require_row_number][j1].column_index2 = cola;
					if (opt == 2 || opt == 4)opt++;
					else if (opt == 3 || opt == 5)opt--;
					require[require_row_number][j1].comparison_operator = opt;
					require[require_row_number][j1].constant = first_word;
					require[require_row_number][j1].constant_type = 2;
					require[require_row_number][j1].p1 = tb;
					require[require_row_number][j1].p2 = ta;
				}
			}
		}
		else if (first_type == 2 && second_type != 2)//ta.cola=int或'text'的形式
		{
			if (str_has_dot(first_word))
			{
				ta = mp_table_dot_column[first_word].first;
				cola = mp_table_dot_column[first_word].second;
			}
			else
			{
				if (mp_which_column.count(make_pair(table1_name, first_word)))
				{
					ta = t1;
					cola = mp_which_column[make_pair(table1_name, first_word)];
				}
				else
				{
					ta = t2;
					cola = mp_which_column[make_pair(table2_name, first_word)];
				}
			}
			require[require_row_number][j1].column_index = cola;
			require[require_row_number][j1].comparison_operator = opt;
			require[require_row_number][j1].constant = second_word;
			require[require_row_number][j1].constant_type = second_type;
			if (ta == t1)
			{
				require[require_row_number][j1].p1 = t1;
				require[require_row_number][j1].p2 = t1;
			}
			else
			{
				require[require_row_number][j1].p1 = t2;
				require[require_row_number][j1].p2 = t2;
			}
		}
		else if (first_type != 2 && second_type == 2)//int或'text'=tb.colb的形式
		{
			if (str_has_dot(second_word))
			{
				tb = mp_table_dot_column[second_word].first;
				colb = mp_table_dot_column[second_word].second;
			}
			else
			{
				if (mp_which_column.count(make_pair(table1_name, second_word)))
				{
					tb = t1;
					colb = mp_which_column[make_pair(table1_name, second_word)];
				}
				else
				{
					tb = t2;
					colb = mp_which_column[make_pair(table2_name, second_word)];
				}
			}
			require[require_row_number][j1].column_index = colb;
			require[require_row_number][j1].comparison_operator = opt;
			require[require_row_number][j1].constant = first_word;
			require[require_row_number][j1].constant_type = first_type;
			if (tb == t1)
			{
				require[require_row_number][j1].p1 = t1;
				require[require_row_number][j1].p2 = t1;
			}
			else
			{
				require[require_row_number][j1].p1 = t2;
				require[require_row_number][j1].p2 = t2;
			}
		}
		while (ch == ' ' || ch == '\n' || ch == '\'')ch = cin.get();
		if (ch == ';')
		{
			require_column_number[require_row_number] = j1 + 1;
			require_row_number++;
			break;
		}
		if (ch == 'a' || ch == 'A')//遇到 and
		{
			read_word(ch);
			j1++;
		}
		else if (ch == 'o' || ch == 'O')//遇到 or
		{
			there_is_no_OR = false;
			read_word(ch);
			require_column_number[require_row_number] = j1 + 1;
			require_row_number++;
			j1 = 0;
		}
	}
}

int main()
{
	std::ios_base::sync_with_stdio(false);
	cin.tie(NULL); cout.tie(NULL);
	//	freopen("training-1.txt", "r", stdin);
	//	freopen("out1.txt", "w", stdout);

	while (cin >> s)//读入第一个单词，然后分类讨论
	{
		lowercase(s);
		if (strcmp1(s, "create") == 0)
		{
			cin >> s; lowercase(s);
			if (strcmp1(s, "table") == 0) //SQL语句：create table table_name ( column1 data_type1, column2 data_type2, ... );
			{
				char ch;
				string table_name = read_word(ch);//读入表名
				int attribute_number = 0;
				while (ch != ')')//依次读入属性名
				{
					attribute_name[attribute_number++] = read_word(ch);
					read_word(ch);//读掉数值类型，因不区分int和text，所以读入了也不管它
					while (ch != ','&&ch != ')')ch = cin.get();
				}
				while (ch != ';')ch = cin.get();
				create_table(table_name, attribute_number, attribute_name);  //建表
			}
			else if (strcmp1(s, "index") == 0)
			{
				char ch = cin.get(); while (ch != ';')ch = cin.get();
			}
		}
		else if (strcmp1(s, "insert") == 0) //SQL语句：insert into table_name values ( val1, val2, ... , valn );
		{
			string table_name; char ch;
			cin >> s;//读掉单词 "INTO"
			table_name = read_word(ch);//读入表名
			read_word(ch);//读掉单词 "VALUES"
			while (ch != '(')ch = cin.get();
			int values_number = 0;
			while (ch != ')')//依次读入插入的属性值
			{
				values[values_number++] = read_values(ch);
				while (ch != ','&&ch != ')')ch = cin.get();
			}
			while (ch != ';')ch = cin.get();
			insert_into_table(table_name, values_number, values); //插入数据
		}
		else if (strcmp1(s, "select", 4) == 0)
		{
			char ch;
			int column_number = 0;
			while (strcmp2(read_word(ch), "from") != 0)//依次读入被查询的列名
			{
				attribute_name[column_number++] = name;
			}
			string table_name = read_word(ch);//读入表名
			while (ch != ';'&&ch != 'w'&&ch != 'W'&&ch != ',')ch = cin.get();//根据表名后面的字符，判断属于什么查询
			//单表查询格式：select col1, col2, ..., coln from table_name where col1=const and col2=col3 and const=col4;
			if (ch == ';')//不带 where 的单表查询
			{
				select_from_a_table(table_name, column_number, attribute_name);
			}
			else if (ch == 'w' || ch == 'W')//带where的单表查询
			{
				read_word(ch);//读掉where
				read_all_requirements_one_table(table_name, ch);//读取所有条件
				select_from_a_table(table_name, column_number, attribute_name, false);
			}
			//两表关联查询格式：select t1.col1, t2.col2  from t1, t2 where t1.col1=t2.col1 and t1.col3=const and const=t2.col4;
			else if (ch == ',')
			{
				string table2_name = read_word(ch);//读入第二个表名
				while (ch != ';'&&ch != 'w'&&ch != 'W')ch = cin.get();
				if (ch == ';')//不带where的两表查询
				{
					require_row_number = 0;
					select_join_table_by_cartesian_product(table_name, table2_name, column_number, attribute_name);
				}
				else if (ch == 'w' || ch == 'W')//带where的两表查询
				{
					read_word(ch);//读掉where
					read_all_requirements_two_tables2(table_name, table2_name, ch);//读入所有查询条件

					//先得到两个表的长度，辅助决定用什么查询策略，不同策略性能不同
					vector<string>*tp1_temp = mp_table_pointer[table_name];
					vector<string>*tp2_temp = mp_table_pointer[table2_name];
					int tp1_temp_len = tp1_temp[0].size();
					int tp2_temp_len = tp2_temp[0].size();

					if (has_join_requirement == false && there_is_no_OR)
					{//先进行选择运算，再笛卡尔积
						select_join_table_by_cartesian_product_without_OR(table_name, table2_name, column_number, attribute_name);
					}
					else if (has_join_requirement&&there_is_no_OR&&tp1_temp_len > 6000 && tp2_temp_len > 6000)
					{//先进行选择运算，再排序合并
						select_join_table_with_join_requirement_without_OR(table_name, table2_name, column_number, attribute_name);
					}
					else if (there_is_no_OR == false&&tp1_temp_len>4000&&tp2_temp_len>4000)
					{//存在OR，条件矩阵每有一行，就进行一次表连接
						select_join_table_with_OR(table_name, table2_name, column_number, attribute_name);
					}
					else
					{//循环嵌套法
						select_join_table_by_cartesian_product(table_name, table2_name, column_number, attribute_name);
					}
				}
			}
		}
	}
}