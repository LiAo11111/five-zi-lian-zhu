#include"cmd_console_tools.h"
#include<iostream>
#include <conio.h>
#include <fstream>
#include <Windows.h>
#include<vector>
#include <stdio.h>
#include <stdlib.h>
#pragma comment(lib, "winmm.lib")//用于音乐处理
using namespace std;

struct direction//记录去某个方向时，横纵坐标的变化值
{
	int a;//横坐标变化量
	int b;//纵坐标变化量
};
//游戏类
class five_game
{
private:
	int map[9][9];//最大是9*9的棋盘
	int record[3];//记录个人历史最好的十次成绩
	int hang;//实际行数，在7/8/9中选择
	int lie;//实际列数，在7/8/9中选择
	int Music_num;//本局游戏的音乐，暂时有0和1两种音乐
	int total_score;//依据游戏的成绩
public:
	five_game();//构造函数
	void show_music();//显示当前的背景音乐
	void best_record(int a);//记录本次游戏的成绩，更新排行榜
	bool restore();//用于开始下一次游戏时将某些数据恢复到初始值
	void Strat();//游戏开始函数
	int  fun();//游戏过程控制函数
	void Music();//音乐播放函数
	void show_score();//打印出成绩排行榜
	bool choose(int flag);//选择棋盘的规格以及是否退出游戏
	void Color(int next_3[3]);//给出下一轮即将产生的3个珠子的颜色
	void Score();//显示出得分栏
	void xinxi(int now[10], int die[10]);//打印出棋盘中各种颜色的珠子的信息，包括现有数量、占比，已经消去的数量
	void fenjie(float i, int& a1, int& a2, int& a3, int& a4);//将float类型的小数（珠子数量占比）分解成四位整数
	int mouse();//鼠标控制函数
	int judge(int remove[]);//判断四个方向能否删除，并返回得分，并且通过参数数组返回消去的珠子的具体颜色和具体数量
	void find_num(int now[7]);//统计6种珠子的数量
	int Isfull();//判断棋盘是不是满的，游戏的终止条件之一
	void writeArrayToFile(const std::string& filename);//将游戏得分写入文件
	void readArrayFromFile(const std::string& filename);//从文件中读取排行榜数据
	void shape2();//打印出五子连珠的棋盘
	void add( int next_3[3]);//在棋盘上添加3个珠子
	void donghua(int startx, int starty, int printff[]);//动态模拟珠子移动过程
	void move_qiu( int startx, int starty, int endx, int endy);//珠子移动的控制函数
	int movepath1( int mark[9][9], int printff[50], int num, int startx, int starty, int s, int t, int endx, int endy, direction move[]);//回溯法寻找路径
};
five_game::five_game()
{
	total_score = 0;
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
			map[i][j] = 0;
	}
	choose(0);//进入游戏开始界面
}
void five_game::show_music()
{
	cct_showstr(PLACE_1 + 20, 3, "背景音乐", COLOR_HCYAN, COLOR_BLACK, 1);
	if (Music_num == 0)
	{
		cct_showstr(PLACE_1 + 19, 4, "<<铃芽之旅>>", COLOR_HCYAN, COLOR_BLACK, 1);
	}
	else 
	{
		cct_showstr(PLACE_1 + 19, 4, "<<天空之城>>", COLOR_HCYAN, COLOR_BLACK, 1);
	}
	
}
void five_game::show_score()
{
	int  choose_lie = 40;
	cct_showstr(choose_lie, 5, "╔═══════╗", COLOR_HCYAN, COLOR_BLACK, 1);
	cct_showstr(choose_lie, 6, "║  成绩排行榜  ║", COLOR_HCYAN, COLOR_BLACK, 1);
	cct_showstr(choose_lie, 7, "║Best：        ║", COLOR_HCYAN, COLOR_BLACK, 1);
	cct_showstr(choose_lie, 8, "║Second：      ║", COLOR_HCYAN, COLOR_BLACK, 1);
	cct_showstr(choose_lie, 9, "║Third：       ║", COLOR_HCYAN, COLOR_BLACK, 1);
	cct_showstr(choose_lie, 10,"╚═══════╝", COLOR_HCYAN, COLOR_BLACK, 1);
	cct_showint(choose_lie + 8, 7, record[0], COLOR_HCYAN, COLOR_BLACK, 1);
	cct_showint(choose_lie + 10, 8, record[1], COLOR_HCYAN, COLOR_BLACK, 1);
	cct_showint(choose_lie + 9, 9, record[2], COLOR_HCYAN, COLOR_BLACK, 1);
}
void five_game::writeArrayToFile( const std::string& filename)
{
	//写文件到record数组
	std::ofstream file(filename);

	if (file.is_open()) 
	{
		for (int i = 0; i < 3; i++) 
		{
			file << record[i] << " ";
		}
		file.close();
	}
	else 
	{
		cout << "Unable to open the file." << endl;
	}
}
void five_game::readArrayFromFile( const std::string& filename)
{
	//读文件到record数组
	std::ifstream file(filename);

	if (file.is_open()) 
	{
		for (int i = 0; i < 3; i++) 
		{
			file >> record[i];
		}
		file.close();
	}
	else
	{
		cout << "Unable to open the file." << endl;
	}
}
void five_game::best_record(int a)
{
	std::string filename = "array_data.txt";

	// 将数组内容写入文件
	readArrayFromFile(filename);
	//加入新的成绩，只有比最好的三次成绩好才会加入排行榜
	int i;
	for (i = 0; i <3; i++)
	{
		if (record[i] < a)
		{
			for (int j = 2; j > i; j--)
			{
				record[j]= record[j-1];
			}
			break;
		}	 
	}
	record[i] = a;
	//将更新后的数组心写入文件
	writeArrayToFile( filename);

	// 读取文件中的数组元素
}
void five_game::Music()
{
	// 播放音乐,直到程序结束
	if (Music_num == 0)
	{
		PlaySound(TEXT("铃芽之旅.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
	}
	else
	{
		PlaySound(TEXT("天空之城.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
	}
}
bool five_game::restore()
{
	//得分置零，更新map棋盘信息
	total_score = 0;
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
			map[i][j] = 0;
	}
	//回到起始选择界面
	int temp=choose(1);
	if (temp == 0)
	{
		cct_showstr(0,0, "0", COLOR_BLACK, COLOR_WHITE, 1);//恢复为黑底白字
		cct_cls();
		return 0;
	}
	return 1;
}
void five_game::shape2()
{
	//打印出行*列的棋盘
	cct_cls();
	int cols, lines, buffer_cols, buffer_lines;
	cct_setconsoleborder(SIZE5, SIZE4, SIZE5, SIZE4);
	cct_getconsoleborder(cols, lines, buffer_cols, buffer_lines);
	cct_setfontsize("新宋体", ZITI, ZITI);
	cct_showstr(0,0, " ", COLOR_BLACK, COLOR_WHITE, 1);//恢复颜色为黑底白字
	cout << "     左键选择、右键退出";
	cout << endl;
	cct_showstr(0, 1, "╔", COLOR_HCYAN, COLOR_BLACK, 1);
	for (int i = 0; i < lie * 2 - 1; i++)
	{
		if (i % 2 == 0)
		{
			cct_showstr(i * 2 + 2, 1, "═", COLOR_HCYAN, COLOR_BLACK, 1);
		}
		else
		{
			cct_showstr(i * 2 + 2, 1, "╦", COLOR_HCYAN, COLOR_BLACK, 1);
		}
	}
	cct_showstr(lie * 2 * 2, 1, "╗", COLOR_HCYAN, COLOR_BLACK, 1);
	int ii = 0;
	for (int i = 0; i < hang * 2 - 1; i++)
	{
		if (i % 2 == 0)
		{
			int jj = 0;
			cct_showstr(0, i + 2, "║", COLOR_HCYAN, COLOR_BLACK, 1);
			for (int j = 0; j < lie * 2; j++)
			{
				if (j % 2 == 0)
				{
					if (map[ii][jj] == 0)
					{
						cct_showstr(j * 2 + 2, i + 2, "  ", COLOR_HCYAN, COLOR_HYELLOW, 1);
					}
					else
					{
						cct_showstr(j * 2 + 2, i + 2, "○", map[ii][jj], COLOR_WHITE, 1);
					}
					jj++;
				}
				else
				{
					cct_showstr(j * 2 + 2, i + 2, "║", COLOR_HCYAN, COLOR_BLACK, 1);
				}
			}
			ii++;
		}
		else
		{
			cct_showstr(0, i + 2, "╠", COLOR_HCYAN, COLOR_BLACK, 1);
			for (int j = 0; j < lie * 2 - 1; j++)
			{
				if (j % 2 == 0)
				{
					cct_showstr(j * 2 + 2, i + 2, "═", COLOR_HCYAN, COLOR_BLACK, 1);
				}
				else
				{
					cct_showstr(j * 2 + 2, i + 2, "╬", COLOR_HCYAN, COLOR_BLACK, 1);
				}

			}
			cct_showstr(4 * lie, i + 2, "╣", COLOR_HCYAN, COLOR_BLACK, 1);
		}
	}
	cct_showstr(0, 2 * hang + 1, "╚", COLOR_HCYAN, COLOR_BLACK, 1);
	for (int i = 0; i < lie * 2 - 1; i++)
	{
		if (i % 2 == 0)
		{
			cct_showstr(i * 2 + 2, 2 * hang + 1, "═", COLOR_HCYAN, COLOR_BLACK, 1);
		}
		else
		{
			cct_showstr(i * 2 + 2, 2 * hang + 1, "╩", COLOR_HCYAN, COLOR_BLACK, 1);
		}
	}
	cct_showstr(lie * 4, 2 * hang + 1, "╝", COLOR_HCYAN, COLOR_BLACK, 1);
	show_music();//显示当前音乐
	cct_showstr(lie * 4 + 2, 2 * hang + 1, "  ", COLOR_BLACK, COLOR_WHITE, 1);

}
void five_game::donghua(int startx, int starty, int printff[])
{
	//隐藏光标
	cct_setcursor(CURSOR_INVISIBLE);
	int i = 0;
	int x = 0, y = 0, x1 = 0, y1 = 0;
	//将相对位置转化成绝对位置
	x = starty * 4 + 2;
	y = startx * 2 + 2;
	x1 = printff[1] * 4 + 2;
	y1 = printff[0] * 2 + 2;
	cct_showstr(x, y, "○", map[startx][starty], COLOR_WHITE, 1);
	Sleep(Sleep_time);
	if ((x + x1) / 2 != x)//左右移动
	{
		cct_showstr(x, y, "  ", COLOR_HCYAN, COLOR_HCYAN, 1);
		cct_showstr((x + x1) / 2, y, "○", map[startx][starty], COLOR_WHITE, 1);
		Sleep(Sleep_time);
		cct_showstr((x + x1) / 2, y, "║", COLOR_HCYAN, COLOR_BLACK, 1);
		cct_showstr(x1, y, "○", map[startx][starty], COLOR_WHITE, 1);
		Sleep(Sleep_time);
	}
	else//上下移动
	{
		cct_showstr(x, y, "  ", COLOR_HCYAN, COLOR_HCYAN, 1);
		cct_showstr(x, (y + y1) / 2, "○", map[startx][starty], COLOR_WHITE, 1);
		Sleep(Sleep_time);
		cct_showstr(x, (y + y1) / 2, "═", COLOR_HCYAN, COLOR_BLACK, 1);
		cct_showstr(x, y1, "○", map[startx][starty], COLOR_WHITE, 1);
		Sleep(Sleep_time);
	}
	//通过循环，动态地显示出移动过程
	i = 2;
	while (printff[i] != 99)
	{
		x1 = printff[i + 1] * 4 + 2;
		y1 = printff[i] * 2 + 2;
		x = printff[i - 1] * 4 + 2;
		y = printff[i - 2] * 2 + 2;
		if ((x + x1) / 2 != x)//左右移动
		{
			cct_showstr(x, y, "  ", COLOR_HCYAN, COLOR_HCYAN, 1);
			cct_showstr((x + x1) / 2, y, "○", map[startx][starty], COLOR_WHITE, 1);
			Sleep(Sleep_time);
			cct_showstr((x + x1) / 2, y, "║", COLOR_HCYAN, COLOR_BLACK, 1);
			cct_showstr(x1, y, "○", map[startx][starty], COLOR_WHITE, 1);
			Sleep(Sleep_time);
		}
		else//上下移动
		{
			cct_showstr(x, y, "  ", COLOR_HCYAN, COLOR_HCYAN, 1);
			cct_showstr(x, (y + y1) / 2, "○", map[startx][starty], COLOR_WHITE, 1);
			Sleep(Sleep_time);
			cct_showstr(x, (y + y1) / 2, "═", COLOR_HCYAN, COLOR_BLACK, 1);
			cct_showstr(x, y1, "○", map[startx][starty], COLOR_WHITE, 1);
			Sleep(Sleep_time);
		}
		i = i + 2;
	}
	cct_setcursor(CURSOR_VISIBLE_NORMAL);
}
int  five_game::movepath1(int mark[9][9], int printff[MAX], int num, int startx, int starty, int s, int t, int endx, int endy, direction move[])
{
	int i, m, p;
	if (startx == endx && starty == endy)//已经到达终点
	{
		return 1;
	}
	else//未到达终点
	{
		for (i = 0; i < 4; i++)//对四个方向依次进行试探
		{
			m = startx + move[i].a;
			p = starty + move[i].b;//坐标变化
			if (map[m][p] == 0 && mark[m][p] == 0 && 0 <= m && m < hang && p < lie && 0 <= p)//表明可以走
			{
				mark[startx][starty] = 1;
				int temp = movepath1( mark, printff, num + 2, m, p, s, t, endx, endy, move);
				if (temp != 0)
				{
					printff[num++] = m;
					printff[num++] = p;//记录路径
					return 1;
				}
				else
				{
					continue;
				}
			}
		}
		if (i == 4)//四个方向都已经试探完成
		{
			return 0;
		}
	}
	return 0;
}
void five_game::move_qiu( int startx, int starty, int endx, int endy)
{

	int printff[MAX];//记录移动顺序，不包含起点
	for (int i = 0; i < MAX; i++)
	{
		printff[i] = 99;
	}
	int mark[9][9] = { 0 };//标记数组。值为0说明不可以走，值为1到9可以走
	int start = startx, start1 = starty, end = endx, end1 = endy;//出发点的坐标和终点的坐标
	direction move[4] = { {-1,0},{0,1},{0,-1},{1,0} };//四个方向的坐标变化值
	//printff[]中的是路径，寻找路径
	movepath1(mark,printff, 0, start, start1, start, start1, end, end1, move);

	if (printff[0] == 99)
	{
		cct_showstr(0, 2 * hang + 4, "无法找到移动路径", COLOR_HCYAN, COLOR_BLACK, 1);
		Sleep(Sleep_time*100);
	}
	else
	{
		//动态移动过程
		donghua(startx, starty, printff);
		//将起点珠子的位置改到终点，起点内容改成0
		map[endx][endy] = map[startx][starty];
		map[startx][starty] = 0;
	}
}
bool five_game::choose(int flag)
{
	//起始选择界面
	cct_showint(0, 0, ' ', COLOR_BLACK, COLOR_BLACK, 1);
	cct_cls();
	//读取并修排行榜文件
	best_record(0);
	//打印出最好的三次成绩
	show_score();
	int choose_lie = 20;
	cct_setcursor(CURSOR_INVISIBLE);//设置光标不可见
	cct_showstr(choose_lie, 5, "默认为：9行9列   ", COLOR_HCYAN, COLOR_BLACK);
	cct_showstr(choose_lie, 6, "请选择行数：7 8 9", COLOR_HCYAN, COLOR_BLACK);
	cct_showstr(choose_lie, 7, "请选择列数：7 8 9", COLOR_HCYAN, COLOR_BLACK);
	cct_showstr(choose_lie, 9, "      确认       ", COLOR_HCYAN, COLOR_BLACK);
	if (flag == 1)//第一次游戏完成之后才会出现“退出游戏”
	{
		cct_showstr(choose_lie, 11, "    退出游戏    ", COLOR_HCYAN, COLOR_BLACK);
	}
	cct_enable_mouse();//可以使用鼠标
	int MX, MY, MAction, keycode1, keycode2;//前三个记录鼠标的位置和按键值
	int x, y;
	int h = 9, l = 9;
	while (1)
	{
		cct_read_keyboard_and_mouse(MX, MY, MAction, keycode1, keycode2);
		x = MX;
		y = MY;
		if (MAction == MOUSE_LEFT_BUTTON_CLICK)
		{
			if (y >= 6 && y <= 7 && x >= 12+ choose_lie && x <= 16+ choose_lie)
			{
				if (y == 6)
				{
					//将上次选择的位置恢复成未选择状态
					if (h == 7)
					{
						cct_showstr(choose_lie+h * 2 - 2, y, "7", COLOR_HCYAN, COLOR_BLACK);//上次选中的7回到正常状态
					}
					else if (h == 8)
					{
						cct_showstr(choose_lie+h * 2 - 2, y, "8", COLOR_HCYAN, COLOR_BLACK);//上次选中的8回到正常状态
					}
					else if (h == 9)
					{
						cct_showstr(choose_lie+h * 2 - 2, y, "9", COLOR_HCYAN, COLOR_BLACK);//上次选中的9回到正常状态
					}
					//选择新位置
					if (x == 12+ choose_lie || x == 13+ choose_lie)
					{
						cct_showstr(choose_lie+12, y, "7", COLOR_RED, COLOR_BLACK);//新选中的突出
						h = 7;
					}
					else if (x == 14+ choose_lie || x == 15+ choose_lie)
					{
						cct_showstr(choose_lie+14, y, "8", COLOR_RED, COLOR_BLACK);
						h = 8;
					}
					else if (x == 16+ choose_lie || x == 17+ choose_lie)
					{
						cct_showstr(choose_lie+16, y, "9", COLOR_RED, COLOR_BLACK);
						h = 9;
					}
				}
				else if (y == 7)
				{
					//将上次选择的位置恢复成未选择状态
					if (l == 7)
					{
						cct_showstr(choose_lie+l * 2 - 2, y, "7", COLOR_HCYAN, COLOR_BLACK);//上次选中的7回到正常状态
					}
					else if (l == 8)
					{
						cct_showstr(choose_lie+l * 2 - 2, y, "8", COLOR_HCYAN, COLOR_BLACK);//上次选中的8回到正常状态
					}
					else if (l == 9)
					{
						cct_showstr(choose_lie+l * 2 - 2, y, "9", COLOR_HCYAN, COLOR_BLACK);//上次选中的9回到正常状态
					}
					//选择新位置
					if (x == 12+ choose_lie || x == 13 + choose_lie)
					{
						cct_showstr(choose_lie+12, y, "7", COLOR_RED, COLOR_BLACK);//新选中的突出
						l = 7;
					}
					else if (x == 14 + choose_lie || x == 15 + choose_lie)
					{
						cct_showstr(choose_lie+14, y, "8", COLOR_RED, COLOR_BLACK);
						l = 8;
					}
					else if (x == 16 + choose_lie || x == 17 + choose_lie)
					{
						cct_showstr(choose_lie+16, y, "9", COLOR_RED, COLOR_BLACK);
						l = 9;
					}
				}
				
			}
			else if (y == 9)//确认进入游戏
			{
				if (x >= choose_lie + 6 && x <= choose_lie + 9)
				{
					cct_showstr(0,0, " ", COLOR_BLACK, COLOR_BLACK);//恢复颜色
					hang = h;
					lie = l;
					cct_disable_mouse();//取消使用鼠标
					return 1;
				}
			}
			else if (y == 11 && flag == 1)//退出游戏
			{
				if (x >= choose_lie + 4 && x <= choose_lie + 11)
				{
					cct_showstr(0, 0, " ", COLOR_BLACK, COLOR_BLACK);//恢复颜色
					cct_disable_mouse();//取消使用鼠标
					return 0;
				}
			}
		}
	}
}
int five_game::mouse()
{
	cct_setcursor(CURSOR_INVISIBLE);//设置光标不可见
	int MX, MY, MAction, keycode1, keycode2;//前三个记录鼠标的位置和按键值
	int z1, z;
	cct_enable_mouse();//可以使用鼠标
	int x = 0, y = 0;
	cct_getxy(z1, z);
	while (1)
	{
		cct_read_keyboard_and_mouse(MX, MY, MAction, keycode1, keycode2);
		if (x != MX || y != MY)
		{
			cct_showstr(z1, z, "                          ", COLOR_BLACK, COLOR_BLACK);
			cct_showstr(z1, z, " ", COLOR_BLACK, COLOR_WHITE);
		}
		//给出当前鼠标所在的位置
		x = MX;
		y = MY;
		if (y % 2 == 0 && y / 2 <= hang)
		{
			if ((x + 2) % 4 == 0 && (x + 2) / 4 <= lie)
			{
				cct_gotoxy(0, 2 * hang + 2);
				cout << "      [当前光标]";
				cout << char(64 + MY / 2) << "行";
				cout << (x + 2) / 4 << "列";
			}
		}
		//点击右键时，退出
		if (MAction == MOUSE_RIGHT_BUTTON_CLICK)
		{
			cct_showstr(0, hang * 2 + 2, "        本局游戏结束               ", COLOR_BLACK, COLOR_WHITE);
			best_record(total_score);
			Sleep(Sleep_time*100);
			return 1;
			break;
		}
		//点击左键时选择珠子
		else if (MAction == MOUSE_LEFT_BUTTON_CLICK)
		{
			if (y % 2 == 0 && y / 2 <= hang && (x + 2) % 4 == 0 && (x + 2) / 4 <= lie)
			{
				if (map[y / 2 - 1][(x + 2) / 4 - 1] != 0)//选择珠子，珠子的样式发生变化
				{
					cct_showstr(x, y, "◎", map[y / 2 - 1][(x + 2) / 4 - 1], COLOR_WHITE, 1);
					cct_showstr(0, hang * 2 + 2, "         珠子已被选中             ", COLOR_BLACK, COLOR_WHITE);
					while (1)
					{
						int MX1, MY1, MAction1, keycode11, keycode21;
						cct_read_keyboard_and_mouse(MX1, MY1, MAction1, keycode11, keycode21);
						if (MAction1 == MOUSE_LEFT_BUTTON_CLICK)
						{
							if (MY1 % 2 == 0 && MY1 / 2 <= hang && (MX1 + 2) % 4 == 0 && (MX1 + 2) / 4 <= lie)
							{
								if (map[MY1 / 2 - 1][(MX1 + 2) / 4 - 1] != 0)//选择其他的珠子
								{
									cct_showstr(x, y, "○", map[y / 2 - 1][(x + 2) / 4 - 1], COLOR_WHITE, 1);
									cct_showstr(MX1, MY1, "◎", map[MY1 / 2 - 1][(MX1 + 2) / 4 - 1], COLOR_WHITE, 1);
									x = MX1;
									y = MY1;
								}
								else//选择到空的位置，准备进行移动
								{
									//在map中对应的起点和终点坐标
									int startx = y / 2 - 1;
									int starty = (x + 2) / 4 - 1;
									int endx = MY1 / 2 - 1;
									int endy = (MX1 + 2) / 4 - 1;
									cct_showstr(0, hang * 2 + 2, "           珠子正在移动               ", COLOR_BLACK, COLOR_WHITE);
									move_qiu(startx, starty, endx, endy);//移动珠子

									cct_showstr(0, hang * 2 + 2, " ", COLOR_BLACK, COLOR_WHITE, 1);
									return 0;
									break;
								}
							}
						}
					}
				}
			}
		}
		cct_gotoxy(z1, z);

	}
	cct_disable_mouse();//取消使用鼠标
	return 0;
}
void five_game::Score()
{
	//打印得分框以及分数
	cct_showstr(PLACE_1, 1, "╔═════╗", COLOR_HCYAN, COLOR_BLACK, 1);
	cct_showstr(PLACE_1, 2, "║得分：", COLOR_HCYAN, COLOR_BLACK, 1);
	if (total_score >= 10)
	{
		cct_showint(PLACE_1 + 8, 2, total_score, COLOR_HCYAN, COLOR_BLACK);
	}
	else
	{
		cct_showint(PLACE_1 + 8, 2, 0, COLOR_HCYAN, COLOR_BLACK);
		cct_showint(49, 2, total_score, COLOR_HCYAN, COLOR_BLACK);
	}
	cct_showstr(PLACE_1 + 10, 2, "  ║", COLOR_HCYAN, COLOR_BLACK, 1);
	cct_showstr(PLACE_1, 3, "╚═════╝", COLOR_HCYAN, COLOR_BLACK, 1);

}
void five_game::Color(int next_3[3])
{
	//打印出下一轮会增加的三个珠子
	cct_showstr(PLACE_1, 5, "╔═╦═╦═╗", COLOR_HCYAN, COLOR_BLACK, 1);
	cct_showstr(PLACE_1, 6, "║", COLOR_HCYAN, COLOR_BLACK, 1);
	cct_showstr(PLACE_1 + 2, 6, "○", next_3[0], COLOR_WHITE);
	cct_showstr(PLACE_1 + 4, 6, "║", COLOR_HCYAN, COLOR_BLACK, 1);
	cct_showstr(PLACE_1 + 6, 6, "○", next_3[1], COLOR_WHITE);
	cct_showstr(PLACE_1 + 8, 6, "║", COLOR_HCYAN, COLOR_BLACK, 1);
	cct_showstr(PLACE_1 + 10, 6, "○", next_3[2], COLOR_WHITE);
	cct_showstr(PLACE_1 + 12, 6, "║", COLOR_HCYAN, COLOR_BLACK, 1);
	cct_showstr(PLACE_1, 7, "╚═╩═╩═╝", COLOR_HCYAN, COLOR_BLACK, 1);
}
void five_game::find_num(int now[7])
{
	//统计当前各颜色珠子的个数
	for (int i = 0; i < 7; i++)
	{
		now[i] = 0;
	}
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			now[map[i][j]]++;
		}
	}
}
void five_game::fenjie(float i, int& a1, int& a2, int& a3, int& a4)
{
	i = i * 100;
	a1 = int(i / 1000);
	i = i - 1000 * a1;
	a2 = int(i / 100);
	i = i - 100 * a2;
	a3 = int(i / 10);
	i = i - 10 * a3;
	a4 = int(i);
}
void five_game::xinxi(int now[7], int die[7])
{
	cct_showstr(40, 9, "╔════════════╗", COLOR_HCYAN, COLOR_BLACK, 1);
	int t = 0;
	for (int i = 1; i < 7; i++)
	{
		t += now[i];
	}
	//打印出各个珠子的数量，占比，以及消除的个数
	for (int i = 1; i <= 6; i++)
	{
		cct_showstr(PLACE_1, 9 + i, "║", COLOR_HCYAN, COLOR_BLACK, 1);
		cct_showstr(PLACE_1 + 2, 9 + i, "○", i, COLOR_WHITE, 1);
		cct_showstr(PLACE_1 + 4, 9 + i, "：", COLOR_HCYAN, COLOR_BLACK, 1);
		if (now[i] >= 10)
		{
			cct_showint(PLACE_1 + 6, 9 + i, now[i], COLOR_HCYAN, COLOR_BLACK);
		}
		else
		{
			cct_showint(PLACE_1 + 6, 9 + i, 0, COLOR_HCYAN, COLOR_BLACK);
			cct_showint(PLACE_1 + 7, 9 + i, now[i], COLOR_HCYAN, COLOR_BLACK);
		}
		cct_showstr(PLACE_1 + 8, 9 + i, "/(", COLOR_HCYAN, COLOR_BLACK, 1);
		float z = now[i] * 1.0 / t;
		z = z * 100;
		int a1, a2, a3, a4;
		fenjie(z, a1, a2, a3, a4);
		cct_showint(PLACE_2, 9 + i, a1, COLOR_HCYAN, COLOR_BLACK);
		cct_showint(PLACE_2 + 1, 9 + i, a2, COLOR_HCYAN, COLOR_BLACK);
		cct_showstr(PLACE_2 + 2, 9 + i, ".", COLOR_HCYAN, COLOR_BLACK, 1);
		cct_showint(PLACE_2 + 3, 9 + i, a3, COLOR_HCYAN, COLOR_BLACK);
		cct_showint(PLACE_2 + 4, 9 + i, a4, COLOR_HCYAN, COLOR_BLACK);
		cct_showstr(PLACE_2 + 5, 9 + i, "%) ", COLOR_HCYAN, COLOR_BLACK, 1);
		cct_showstr(PLACE_2 + 7, 9 + i, "消除-", COLOR_HCYAN, COLOR_BLACK, 1);
		if (die[i] >= 10)
		{
			cct_showint(PLACE_3 + 2, 9 + i, die[i], COLOR_HCYAN, COLOR_BLACK);
		}
		else
		{
			cct_showint(PLACE_3 + 2, 9 + i, 0, COLOR_HCYAN, COLOR_BLACK);
			cct_showint(PLACE_3 + 3, 9 + i, die[i], COLOR_HCYAN, COLOR_BLACK);
		}
		cct_showstr(PLACE_3 + 4, 9 + i, "   ", COLOR_HCYAN, COLOR_WHITE, 1);
		cct_showstr(PLACE_3 + 6, 9 + i, "║", COLOR_HCYAN, COLOR_BLACK, 1);

	}
	cct_showstr(PLACE_1, 16, "╚════════════╝", COLOR_HCYAN, COLOR_BLACK, 1);
	cct_showstr(0, 20, " ", COLOR_BLACK, COLOR_WHITE, 1);
}
void five_game::add( int next_3[3])
{
	//为三个珠子随机产生三个位置
	for (int i = 0; i < 3; i++)
	{
		while (1)
		{
			int xx = rand() % hang;
			int yy = rand() % lie;
			if (map[xx][yy] == 0)
			{
				map[xx][yy] = next_3[i];
				break;
			}
		}
	}
}
int five_game::Isfull()
{
	//返回空余的位置个数
	int n = 0;
	for (int i = 0; i < hang; i++)
	{
		for (int j = 0; j < lie; j++)
		{
			if (map[i][j] == 0)
			{
				n++;
			}
		}
	}
	return n;
}
int  five_game::fun()
{
	srand((unsigned)time(NULL));
	int remove[8]={0};
	shape2();//打印出棋盘
	int next_3[3];//下三个颜色
	for (int i = 0; i < 3; i++)
		next_3[i] = rand() % 6 + 1;
	Score();//更新得分栏
	Color(next_3);//下三个珠子的信息框
	int now[7] = { 0 };//六种颜色现在的个数,0下标不用
	int die[7] = { 0 };//六种颜色珠子消去的个数，下标为1到6代表6种珠子
	find_num(now); //当前各颜色珠子的个数
	xinxi(now, die);//场上各种颜色的珠子的信息
	int ju = mouse();//进行鼠标操作
	if (ju == 1)//鼠标点击了右键，退出本轮游戏
	{
		return 0;
	}
	int temp1 = judge( remove);//判断有没有珠子可以消除
	if (temp1 == 0)//没有珠子可以消除
	{
		int opp = Isfull();
		if (opp <= 3)
			return 0;
		add(next_3);
	}
	else//有珠子可以消除，记录消除珠子的颜色和数量，对die数组进行更新
	{
		total_score += temp1;
		die[remove[0]] += remove[1];
		die[remove[2]] += remove[3];
		die[remove[4]] += remove[5];
		die[remove[6]] += remove[7];
	}
	//通过循环来控制游戏过程
	while (1)
	{
		shape2();
		find_num(now);
		for (int i = 0; i < 3; i++)
			next_3[i] = rand() % 6 + 1;
		Score();//更新得分栏
		Color(next_3);//下三个珠子的信息框
		xinxi(now, die);//场上各种颜色的珠子的信息
		int juu = mouse();//鼠标操作
		if (juu == 1)//点击了右键
		{
			break;
		}
		for (int i = 0; i < 8; i++)//remove数组清空
			remove[i] = 0;
		int temp = judge(remove);
		if (temp == 0)
		{
			int  ff = Isfull();
			if (ff <= 3)//小于等于三个空位置，游戏结束
			{
				cct_showstr(0, 2 * hang + 4, "   游戏结束   " , COLOR_HCYAN, COLOR_BLACK, 1);
				cct_showstr(0, 2 * hang + 4, "", COLOR_BLACK, COLOR_BLACK, 1);
				Sleep(Sleep_time*100);
				return 0;
			}
			add(next_3);
			//添加三个元素之后再次判断
			for (int i = 0; i < 8; i++)//remove数组清空
				remove[i] = 0;
			int temp1 = judge(remove);
			if (temp1 != 0)
			{
				total_score += temp1;
				die[remove[0]] += remove[1];
				die[remove[2]] += remove[3];
				die[remove[4]] += remove[5];
				die[remove[6]] += remove[7];
			}
		}
		else
		{
			total_score += temp;
			die[remove[0]] += remove[1];
			die[remove[2]] += remove[3];
			die[remove[4]] += remove[5];
			die[remove[6]] += remove[7];
		}

	}
	return 0;
}
void five_game::Strat()
{
	srand((unsigned)time(NULL));
	Music_num = rand() % 2;
	Music();
	srand((unsigned)time(NULL));
	cct_cls();
	int col, lin, b_col, b_lin;//记录当前的cmd参数
	cct_getconsoleborder(col, lin, b_col, b_lin);
	
	//初始化数组，打印最初的5个珠子
	int a[7] = { 0 };
	//珠子的颜色
	for (int i = 0; i <7; i++)
	{
		a[i] = rand() % 6 + 1;
	}
	//珠子的位置
	for (int i = 0; i < 7; i++)
	{
		while (1)
		{
			int xx = rand() % hang;
			int yy = rand() % lie;
			if (map[xx][yy] == 0)
			{
				map[xx][yy] = a[i];
				break;
			}
		}
	}
	//进行游戏
	fun();
	cct_setconsoleborder(col, lin, b_col, b_lin);
	cct_setfontsize("新宋体", 16);
}

int five_game::judge(int remove[])
{
	int mark[9][9] = { 0 };
	int score = 0;
	// 检查横向消除
	for (int i = 0; i < hang; i++)
	{
		int j = 0;
		while (map[i][j] == 0&&j<=lie)
		{
			j++;
		}//找到第一个珠子
		int count = 1;//第一个珠子
		for (j = j + 1; j < lie; j++)
		{
			if (map[i][j] == map[i][j - 1] && map[i][j] != 0)
			{
				count++;
			}
			else
			{
				if (count >= 5)
				{
					score += count * 2;
					remove[0] = map[i][j - 1];
					remove[1] += count;
					for (int k = 0; k < count; k++)
					{
						mark[i][j - 1 - k] ++;
					}
				}
				count = 1;
			}
		}
		//这时为当前行的最右边的几个珠子可以消除
		if (count >= 5)
		{
			score += count * 2;
			remove[0] = map[i][j - 1];
			remove[1] += count;
			for (int k = 0; k < count; k++)
			{
				mark[i][lie - 1 - k] ++;
			}
		}
	}
	//检查纵向消除
	for (int j = 0; j < lie; j++)//按列寻找
	{
		int i = 0;
		while (map[i][j] == 0&&i<=hang)
		{
			i++;
		}//找到第一个珠子
		int count = 1;//第一个珠子
		for (i = i + 1; i < hang; i++)
		{
			if (map[i][j] == map[i - 1][j] && map[i][j] != 0)
			{
				count++;
			}
			else
			{
				if (count >= 5)
				{
					score += count * 2;
					remove[2] = map[i-1][j ];
					remove[3] += count;
					for (int k = 0; k < count; k++)
					{
						mark[i - 1 - k][j] ++;
					}
				}
				count = 1;
			}
		}
		//这时为当前列的最下面几个珠子可以消除
		if (count >= 5)
		{
			score += count * 2;
			remove[2] = map[i - 1][j];
			remove[3] += count;
			for (int k = 0; k < count; k++)
			{
				mark[hang - 1 - k][j] ++;
			}
		}
	}
	// 检查主对角线消除
	int i, j;
	for (int k = 0; k < 9; k++)
	{
		//考虑9*9的棋盘，最多只有9条主对角线可能有5个或以上的珠子
		if (k >= 0 && k <= 4)
		{
			i = 0;
			j = 4 - k;//j为0,1,2,3,4
		}
		else
		{
			j = 0;
			i = k - 5;//i为1,2,3,4
		}
		while (map[i][j] == 0 && i < 9 && j < 9)//找到第一个珠子
		{
			i++;
			j++;
		}
		int count = 1;
		i++;
		j++;
		while (i < 9 && j < 9)
		{
			if (map[i][j] == map[i - 1][j - 1] && map[i][j] != 0)
			{
				count++;
			}
			else
			{
				if (count >= 5)
				{
					score += count * 2;
					remove[4] = map[i-1][j - 1];
					remove[5] += count;
					for (int m = 0; m < count; m++)
					{
						mark[i - 1 - m][j - 1 - m] ++;
					}
				}
				count = 1;
			}
			i++;
			j++;
		}
		if (count >= 5)
		{
			score += count * 2;
			remove[4] = map[i - 1][j - 1];
			remove[5] += count;
			for (int m = 0; m < count; m++)
			{
				mark[i - 1 - m][j - 1 - m] ++;
			}
		}
	}

	// 检查副对角线消除
	for (int k = 0; k < 9; k++)
	{
		//考虑9*9的棋盘，最多只有9条副对角线可能有5个或以上的珠子
		if (k >= 0 && k <= 4)
		{
			i = 0;
			j = 4 + k;//j为4,5,6,7,8
		}
		else
		{
			j = 8;
			i = k - 4;//i为1,2,3,4
		}
		while (map[i][j] == 0 && i < 9 && j>=0)//找到第一个珠子
		{
			i++;
			j--;
		}
		int count = 1;
		i++;
		j--;
		while (i >= 0 && i < 9 && j < 9 && j >= 0)
		{
			if (map[i][j] == map[i - 1][j + 1] && map[i][j] != 0)
			{
				count++;
			}
			else
			{
				if (count >= 5)
				{
					score += count * 2;
					remove[6] = map[i - 1][j+1];
					remove[7] += count;
					for (int m = 0; m < count; m++)
					{
						mark[i - 1 - m][j + 1 + m]++;
					}
				}
				count = 1;
			}
			i++;
			j--;
		}
		if (count >= 5)
		{
			score += count * 2;
			remove[6] = map[i - 1][j + 1];
			remove[7] += count;
			for (int m = 0; m < count; m++)
			{
				mark[i - 1 - m][j + 1 + m] ++;
			}
		}
	}
	//将所有可以消除的小珠子消除
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			if (mark[i][j] != 0)
			{
				map[i][j] = 0;
			}
		}
	}
	//对重复删除的珠子进行删除数量的更新，即若有两个方向的交叉，则把一个方向的删除珠子数由5变成4.
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			if (mark[i][j] == 2)//此位置的珠子要消除两次
			{
				for (int k = 0; k < 4; k++)
				{
					if (map[i][j] == remove[2 * k])
					{
						remove[2 * k + 1]--;//实际的删除数量-1
						break;
					}
				}
			}
			else if (mark[i][j] == 3)//此位置的珠子要消除三次
			{
				for (int k = 0; k < 4; k++)
				{
					if (map[i][j] == remove[2 * k])
					{
						remove[2 * k + 1]-=2;//实际的删除数量-2
						break;
					}
				}
			}
			else if (mark[i][j] == 4)//此位置的珠子要消除四次
			{
				for (int k = 0; k < 4; k++)
				{
					if (map[i][j] == remove[2 * k])
					{
						remove[2 * k + 1]-=3;//实际的删除数量-3
						break;
					}
				}
			}
		}
	}
	return score;
}

int main()
{
	five_game A;//生成类的对象
	while (1)
	{
		A.Strat();
		int temp=A.restore();
		if (temp == 0)
			break;
	}
	return 0;
}
