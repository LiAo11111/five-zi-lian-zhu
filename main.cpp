#include"cmd_console_tools.h"
#include<iostream>
#include <conio.h>
#include <fstream>
#include <Windows.h>
#include<vector>
#include <stdio.h>
#include <stdlib.h>
#pragma comment(lib, "winmm.lib")//�������ִ���
using namespace std;

struct direction//��¼ȥĳ������ʱ����������ı仯ֵ
{
	int a;//������仯��
	int b;//������仯��
};
//��Ϸ��
class five_game
{
private:
	int map[9][9];//�����9*9������
	int record[3];//��¼������ʷ��õ�ʮ�γɼ�
	int hang;//ʵ����������7/8/9��ѡ��
	int lie;//ʵ����������7/8/9��ѡ��
	int Music_num;//������Ϸ�����֣���ʱ��0��1��������
	int total_score;//������Ϸ�ĳɼ�
public:
	five_game();//���캯��
	void show_music();//��ʾ��ǰ�ı�������
	void best_record(int a);//��¼������Ϸ�ĳɼ����������а�
	bool restore();//���ڿ�ʼ��һ����Ϸʱ��ĳЩ���ݻָ�����ʼֵ
	void Strat();//��Ϸ��ʼ����
	int  fun();//��Ϸ���̿��ƺ���
	void Music();//���ֲ��ź���
	void show_score();//��ӡ���ɼ����а�
	bool choose(int flag);//ѡ�����̵Ĺ���Լ��Ƿ��˳���Ϸ
	void Color(int next_3[3]);//������һ�ּ���������3�����ӵ���ɫ
	void Score();//��ʾ���÷���
	void xinxi(int now[10], int die[10]);//��ӡ�������и�����ɫ�����ӵ���Ϣ����������������ռ�ȣ��Ѿ���ȥ������
	void fenjie(float i, int& a1, int& a2, int& a3, int& a4);//��float���͵�С������������ռ�ȣ��ֽ����λ����
	int mouse();//�����ƺ���
	int judge(int remove[]);//�ж��ĸ������ܷ�ɾ���������ص÷֣�����ͨ���������鷵����ȥ�����ӵľ�����ɫ�;�������
	void find_num(int now[7]);//ͳ��6�����ӵ�����
	int Isfull();//�ж������ǲ������ģ���Ϸ����ֹ����֮һ
	void writeArrayToFile(const std::string& filename);//����Ϸ�÷�д���ļ�
	void readArrayFromFile(const std::string& filename);//���ļ��ж�ȡ���а�����
	void shape2();//��ӡ���������������
	void add( int next_3[3]);//�����������3������
	void donghua(int startx, int starty, int printff[]);//��̬ģ�������ƶ�����
	void move_qiu( int startx, int starty, int endx, int endy);//�����ƶ��Ŀ��ƺ���
	int movepath1( int mark[9][9], int printff[50], int num, int startx, int starty, int s, int t, int endx, int endy, direction move[]);//���ݷ�Ѱ��·��
};
five_game::five_game()
{
	total_score = 0;
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
			map[i][j] = 0;
	}
	choose(0);//������Ϸ��ʼ����
}
void five_game::show_music()
{
	cct_showstr(PLACE_1 + 20, 3, "��������", COLOR_HCYAN, COLOR_BLACK, 1);
	if (Music_num == 0)
	{
		cct_showstr(PLACE_1 + 19, 4, "<<��ѿ֮��>>", COLOR_HCYAN, COLOR_BLACK, 1);
	}
	else 
	{
		cct_showstr(PLACE_1 + 19, 4, "<<���֮��>>", COLOR_HCYAN, COLOR_BLACK, 1);
	}
	
}
void five_game::show_score()
{
	int  choose_lie = 40;
	cct_showstr(choose_lie, 5, "�X�T�T�T�T�T�T�T�[", COLOR_HCYAN, COLOR_BLACK, 1);
	cct_showstr(choose_lie, 6, "�U  �ɼ����а�  �U", COLOR_HCYAN, COLOR_BLACK, 1);
	cct_showstr(choose_lie, 7, "�UBest��        �U", COLOR_HCYAN, COLOR_BLACK, 1);
	cct_showstr(choose_lie, 8, "�USecond��      �U", COLOR_HCYAN, COLOR_BLACK, 1);
	cct_showstr(choose_lie, 9, "�UThird��       �U", COLOR_HCYAN, COLOR_BLACK, 1);
	cct_showstr(choose_lie, 10,"�^�T�T�T�T�T�T�T�a", COLOR_HCYAN, COLOR_BLACK, 1);
	cct_showint(choose_lie + 8, 7, record[0], COLOR_HCYAN, COLOR_BLACK, 1);
	cct_showint(choose_lie + 10, 8, record[1], COLOR_HCYAN, COLOR_BLACK, 1);
	cct_showint(choose_lie + 9, 9, record[2], COLOR_HCYAN, COLOR_BLACK, 1);
}
void five_game::writeArrayToFile( const std::string& filename)
{
	//д�ļ���record����
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
	//���ļ���record����
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

	// ����������д���ļ�
	readArrayFromFile(filename);
	//�����µĳɼ���ֻ�б���õ����γɼ��òŻ�������а�
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
	//�����º��������д���ļ�
	writeArrayToFile( filename);

	// ��ȡ�ļ��е�����Ԫ��
}
void five_game::Music()
{
	// ��������,ֱ���������
	if (Music_num == 0)
	{
		PlaySound(TEXT("��ѿ֮��.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
	}
	else
	{
		PlaySound(TEXT("���֮��.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
	}
}
bool five_game::restore()
{
	//�÷����㣬����map������Ϣ
	total_score = 0;
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
			map[i][j] = 0;
	}
	//�ص���ʼѡ�����
	int temp=choose(1);
	if (temp == 0)
	{
		cct_showstr(0,0, "0", COLOR_BLACK, COLOR_WHITE, 1);//�ָ�Ϊ�ڵװ���
		cct_cls();
		return 0;
	}
	return 1;
}
void five_game::shape2()
{
	//��ӡ����*�е�����
	cct_cls();
	int cols, lines, buffer_cols, buffer_lines;
	cct_setconsoleborder(SIZE5, SIZE4, SIZE5, SIZE4);
	cct_getconsoleborder(cols, lines, buffer_cols, buffer_lines);
	cct_setfontsize("������", ZITI, ZITI);
	cct_showstr(0,0, " ", COLOR_BLACK, COLOR_WHITE, 1);//�ָ���ɫΪ�ڵװ���
	cout << "     ���ѡ���Ҽ��˳�";
	cout << endl;
	cct_showstr(0, 1, "�X", COLOR_HCYAN, COLOR_BLACK, 1);
	for (int i = 0; i < lie * 2 - 1; i++)
	{
		if (i % 2 == 0)
		{
			cct_showstr(i * 2 + 2, 1, "�T", COLOR_HCYAN, COLOR_BLACK, 1);
		}
		else
		{
			cct_showstr(i * 2 + 2, 1, "�j", COLOR_HCYAN, COLOR_BLACK, 1);
		}
	}
	cct_showstr(lie * 2 * 2, 1, "�[", COLOR_HCYAN, COLOR_BLACK, 1);
	int ii = 0;
	for (int i = 0; i < hang * 2 - 1; i++)
	{
		if (i % 2 == 0)
		{
			int jj = 0;
			cct_showstr(0, i + 2, "�U", COLOR_HCYAN, COLOR_BLACK, 1);
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
						cct_showstr(j * 2 + 2, i + 2, "��", map[ii][jj], COLOR_WHITE, 1);
					}
					jj++;
				}
				else
				{
					cct_showstr(j * 2 + 2, i + 2, "�U", COLOR_HCYAN, COLOR_BLACK, 1);
				}
			}
			ii++;
		}
		else
		{
			cct_showstr(0, i + 2, "�d", COLOR_HCYAN, COLOR_BLACK, 1);
			for (int j = 0; j < lie * 2 - 1; j++)
			{
				if (j % 2 == 0)
				{
					cct_showstr(j * 2 + 2, i + 2, "�T", COLOR_HCYAN, COLOR_BLACK, 1);
				}
				else
				{
					cct_showstr(j * 2 + 2, i + 2, "�p", COLOR_HCYAN, COLOR_BLACK, 1);
				}

			}
			cct_showstr(4 * lie, i + 2, "�g", COLOR_HCYAN, COLOR_BLACK, 1);
		}
	}
	cct_showstr(0, 2 * hang + 1, "�^", COLOR_HCYAN, COLOR_BLACK, 1);
	for (int i = 0; i < lie * 2 - 1; i++)
	{
		if (i % 2 == 0)
		{
			cct_showstr(i * 2 + 2, 2 * hang + 1, "�T", COLOR_HCYAN, COLOR_BLACK, 1);
		}
		else
		{
			cct_showstr(i * 2 + 2, 2 * hang + 1, "�m", COLOR_HCYAN, COLOR_BLACK, 1);
		}
	}
	cct_showstr(lie * 4, 2 * hang + 1, "�a", COLOR_HCYAN, COLOR_BLACK, 1);
	show_music();//��ʾ��ǰ����
	cct_showstr(lie * 4 + 2, 2 * hang + 1, "  ", COLOR_BLACK, COLOR_WHITE, 1);

}
void five_game::donghua(int startx, int starty, int printff[])
{
	//���ع��
	cct_setcursor(CURSOR_INVISIBLE);
	int i = 0;
	int x = 0, y = 0, x1 = 0, y1 = 0;
	//�����λ��ת���ɾ���λ��
	x = starty * 4 + 2;
	y = startx * 2 + 2;
	x1 = printff[1] * 4 + 2;
	y1 = printff[0] * 2 + 2;
	cct_showstr(x, y, "��", map[startx][starty], COLOR_WHITE, 1);
	Sleep(Sleep_time);
	if ((x + x1) / 2 != x)//�����ƶ�
	{
		cct_showstr(x, y, "  ", COLOR_HCYAN, COLOR_HCYAN, 1);
		cct_showstr((x + x1) / 2, y, "��", map[startx][starty], COLOR_WHITE, 1);
		Sleep(Sleep_time);
		cct_showstr((x + x1) / 2, y, "�U", COLOR_HCYAN, COLOR_BLACK, 1);
		cct_showstr(x1, y, "��", map[startx][starty], COLOR_WHITE, 1);
		Sleep(Sleep_time);
	}
	else//�����ƶ�
	{
		cct_showstr(x, y, "  ", COLOR_HCYAN, COLOR_HCYAN, 1);
		cct_showstr(x, (y + y1) / 2, "��", map[startx][starty], COLOR_WHITE, 1);
		Sleep(Sleep_time);
		cct_showstr(x, (y + y1) / 2, "�T", COLOR_HCYAN, COLOR_BLACK, 1);
		cct_showstr(x, y1, "��", map[startx][starty], COLOR_WHITE, 1);
		Sleep(Sleep_time);
	}
	//ͨ��ѭ������̬����ʾ���ƶ�����
	i = 2;
	while (printff[i] != 99)
	{
		x1 = printff[i + 1] * 4 + 2;
		y1 = printff[i] * 2 + 2;
		x = printff[i - 1] * 4 + 2;
		y = printff[i - 2] * 2 + 2;
		if ((x + x1) / 2 != x)//�����ƶ�
		{
			cct_showstr(x, y, "  ", COLOR_HCYAN, COLOR_HCYAN, 1);
			cct_showstr((x + x1) / 2, y, "��", map[startx][starty], COLOR_WHITE, 1);
			Sleep(Sleep_time);
			cct_showstr((x + x1) / 2, y, "�U", COLOR_HCYAN, COLOR_BLACK, 1);
			cct_showstr(x1, y, "��", map[startx][starty], COLOR_WHITE, 1);
			Sleep(Sleep_time);
		}
		else//�����ƶ�
		{
			cct_showstr(x, y, "  ", COLOR_HCYAN, COLOR_HCYAN, 1);
			cct_showstr(x, (y + y1) / 2, "��", map[startx][starty], COLOR_WHITE, 1);
			Sleep(Sleep_time);
			cct_showstr(x, (y + y1) / 2, "�T", COLOR_HCYAN, COLOR_BLACK, 1);
			cct_showstr(x, y1, "��", map[startx][starty], COLOR_WHITE, 1);
			Sleep(Sleep_time);
		}
		i = i + 2;
	}
	cct_setcursor(CURSOR_VISIBLE_NORMAL);
}
int  five_game::movepath1(int mark[9][9], int printff[MAX], int num, int startx, int starty, int s, int t, int endx, int endy, direction move[])
{
	int i, m, p;
	if (startx == endx && starty == endy)//�Ѿ������յ�
	{
		return 1;
	}
	else//δ�����յ�
	{
		for (i = 0; i < 4; i++)//���ĸ��������ν�����̽
		{
			m = startx + move[i].a;
			p = starty + move[i].b;//����仯
			if (map[m][p] == 0 && mark[m][p] == 0 && 0 <= m && m < hang && p < lie && 0 <= p)//����������
			{
				mark[startx][starty] = 1;
				int temp = movepath1( mark, printff, num + 2, m, p, s, t, endx, endy, move);
				if (temp != 0)
				{
					printff[num++] = m;
					printff[num++] = p;//��¼·��
					return 1;
				}
				else
				{
					continue;
				}
			}
		}
		if (i == 4)//�ĸ������Ѿ���̽���
		{
			return 0;
		}
	}
	return 0;
}
void five_game::move_qiu( int startx, int starty, int endx, int endy)
{

	int printff[MAX];//��¼�ƶ�˳�򣬲��������
	for (int i = 0; i < MAX; i++)
	{
		printff[i] = 99;
	}
	int mark[9][9] = { 0 };//������顣ֵΪ0˵���������ߣ�ֵΪ1��9������
	int start = startx, start1 = starty, end = endx, end1 = endy;//�������������յ������
	direction move[4] = { {-1,0},{0,1},{0,-1},{1,0} };//�ĸ����������仯ֵ
	//printff[]�е���·����Ѱ��·��
	movepath1(mark,printff, 0, start, start1, start, start1, end, end1, move);

	if (printff[0] == 99)
	{
		cct_showstr(0, 2 * hang + 4, "�޷��ҵ��ƶ�·��", COLOR_HCYAN, COLOR_BLACK, 1);
		Sleep(Sleep_time*100);
	}
	else
	{
		//��̬�ƶ�����
		donghua(startx, starty, printff);
		//��������ӵ�λ�øĵ��յ㣬������ݸĳ�0
		map[endx][endy] = map[startx][starty];
		map[startx][starty] = 0;
	}
}
bool five_game::choose(int flag)
{
	//��ʼѡ�����
	cct_showint(0, 0, ' ', COLOR_BLACK, COLOR_BLACK, 1);
	cct_cls();
	//��ȡ�������а��ļ�
	best_record(0);
	//��ӡ����õ����γɼ�
	show_score();
	int choose_lie = 20;
	cct_setcursor(CURSOR_INVISIBLE);//���ù�겻�ɼ�
	cct_showstr(choose_lie, 5, "Ĭ��Ϊ��9��9��   ", COLOR_HCYAN, COLOR_BLACK);
	cct_showstr(choose_lie, 6, "��ѡ��������7 8 9", COLOR_HCYAN, COLOR_BLACK);
	cct_showstr(choose_lie, 7, "��ѡ��������7 8 9", COLOR_HCYAN, COLOR_BLACK);
	cct_showstr(choose_lie, 9, "      ȷ��       ", COLOR_HCYAN, COLOR_BLACK);
	if (flag == 1)//��һ����Ϸ���֮��Ż���֡��˳���Ϸ��
	{
		cct_showstr(choose_lie, 11, "    �˳���Ϸ    ", COLOR_HCYAN, COLOR_BLACK);
	}
	cct_enable_mouse();//����ʹ�����
	int MX, MY, MAction, keycode1, keycode2;//ǰ������¼����λ�úͰ���ֵ
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
					//���ϴ�ѡ���λ�ûָ���δѡ��״̬
					if (h == 7)
					{
						cct_showstr(choose_lie+h * 2 - 2, y, "7", COLOR_HCYAN, COLOR_BLACK);//�ϴ�ѡ�е�7�ص�����״̬
					}
					else if (h == 8)
					{
						cct_showstr(choose_lie+h * 2 - 2, y, "8", COLOR_HCYAN, COLOR_BLACK);//�ϴ�ѡ�е�8�ص�����״̬
					}
					else if (h == 9)
					{
						cct_showstr(choose_lie+h * 2 - 2, y, "9", COLOR_HCYAN, COLOR_BLACK);//�ϴ�ѡ�е�9�ص�����״̬
					}
					//ѡ����λ��
					if (x == 12+ choose_lie || x == 13+ choose_lie)
					{
						cct_showstr(choose_lie+12, y, "7", COLOR_RED, COLOR_BLACK);//��ѡ�е�ͻ��
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
					//���ϴ�ѡ���λ�ûָ���δѡ��״̬
					if (l == 7)
					{
						cct_showstr(choose_lie+l * 2 - 2, y, "7", COLOR_HCYAN, COLOR_BLACK);//�ϴ�ѡ�е�7�ص�����״̬
					}
					else if (l == 8)
					{
						cct_showstr(choose_lie+l * 2 - 2, y, "8", COLOR_HCYAN, COLOR_BLACK);//�ϴ�ѡ�е�8�ص�����״̬
					}
					else if (l == 9)
					{
						cct_showstr(choose_lie+l * 2 - 2, y, "9", COLOR_HCYAN, COLOR_BLACK);//�ϴ�ѡ�е�9�ص�����״̬
					}
					//ѡ����λ��
					if (x == 12+ choose_lie || x == 13 + choose_lie)
					{
						cct_showstr(choose_lie+12, y, "7", COLOR_RED, COLOR_BLACK);//��ѡ�е�ͻ��
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
			else if (y == 9)//ȷ�Ͻ�����Ϸ
			{
				if (x >= choose_lie + 6 && x <= choose_lie + 9)
				{
					cct_showstr(0,0, " ", COLOR_BLACK, COLOR_BLACK);//�ָ���ɫ
					hang = h;
					lie = l;
					cct_disable_mouse();//ȡ��ʹ�����
					return 1;
				}
			}
			else if (y == 11 && flag == 1)//�˳���Ϸ
			{
				if (x >= choose_lie + 4 && x <= choose_lie + 11)
				{
					cct_showstr(0, 0, " ", COLOR_BLACK, COLOR_BLACK);//�ָ���ɫ
					cct_disable_mouse();//ȡ��ʹ�����
					return 0;
				}
			}
		}
	}
}
int five_game::mouse()
{
	cct_setcursor(CURSOR_INVISIBLE);//���ù�겻�ɼ�
	int MX, MY, MAction, keycode1, keycode2;//ǰ������¼����λ�úͰ���ֵ
	int z1, z;
	cct_enable_mouse();//����ʹ�����
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
		//������ǰ������ڵ�λ��
		x = MX;
		y = MY;
		if (y % 2 == 0 && y / 2 <= hang)
		{
			if ((x + 2) % 4 == 0 && (x + 2) / 4 <= lie)
			{
				cct_gotoxy(0, 2 * hang + 2);
				cout << "      [��ǰ���]";
				cout << char(64 + MY / 2) << "��";
				cout << (x + 2) / 4 << "��";
			}
		}
		//����Ҽ�ʱ���˳�
		if (MAction == MOUSE_RIGHT_BUTTON_CLICK)
		{
			cct_showstr(0, hang * 2 + 2, "        ������Ϸ����               ", COLOR_BLACK, COLOR_WHITE);
			best_record(total_score);
			Sleep(Sleep_time*100);
			return 1;
			break;
		}
		//������ʱѡ������
		else if (MAction == MOUSE_LEFT_BUTTON_CLICK)
		{
			if (y % 2 == 0 && y / 2 <= hang && (x + 2) % 4 == 0 && (x + 2) / 4 <= lie)
			{
				if (map[y / 2 - 1][(x + 2) / 4 - 1] != 0)//ѡ�����ӣ����ӵ���ʽ�����仯
				{
					cct_showstr(x, y, "��", map[y / 2 - 1][(x + 2) / 4 - 1], COLOR_WHITE, 1);
					cct_showstr(0, hang * 2 + 2, "         �����ѱ�ѡ��             ", COLOR_BLACK, COLOR_WHITE);
					while (1)
					{
						int MX1, MY1, MAction1, keycode11, keycode21;
						cct_read_keyboard_and_mouse(MX1, MY1, MAction1, keycode11, keycode21);
						if (MAction1 == MOUSE_LEFT_BUTTON_CLICK)
						{
							if (MY1 % 2 == 0 && MY1 / 2 <= hang && (MX1 + 2) % 4 == 0 && (MX1 + 2) / 4 <= lie)
							{
								if (map[MY1 / 2 - 1][(MX1 + 2) / 4 - 1] != 0)//ѡ������������
								{
									cct_showstr(x, y, "��", map[y / 2 - 1][(x + 2) / 4 - 1], COLOR_WHITE, 1);
									cct_showstr(MX1, MY1, "��", map[MY1 / 2 - 1][(MX1 + 2) / 4 - 1], COLOR_WHITE, 1);
									x = MX1;
									y = MY1;
								}
								else//ѡ�񵽿յ�λ�ã�׼�������ƶ�
								{
									//��map�ж�Ӧ�������յ�����
									int startx = y / 2 - 1;
									int starty = (x + 2) / 4 - 1;
									int endx = MY1 / 2 - 1;
									int endy = (MX1 + 2) / 4 - 1;
									cct_showstr(0, hang * 2 + 2, "           ���������ƶ�               ", COLOR_BLACK, COLOR_WHITE);
									move_qiu(startx, starty, endx, endy);//�ƶ�����

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
	cct_disable_mouse();//ȡ��ʹ�����
	return 0;
}
void five_game::Score()
{
	//��ӡ�÷ֿ��Լ�����
	cct_showstr(PLACE_1, 1, "�X�T�T�T�T�T�[", COLOR_HCYAN, COLOR_BLACK, 1);
	cct_showstr(PLACE_1, 2, "�U�÷֣�", COLOR_HCYAN, COLOR_BLACK, 1);
	if (total_score >= 10)
	{
		cct_showint(PLACE_1 + 8, 2, total_score, COLOR_HCYAN, COLOR_BLACK);
	}
	else
	{
		cct_showint(PLACE_1 + 8, 2, 0, COLOR_HCYAN, COLOR_BLACK);
		cct_showint(49, 2, total_score, COLOR_HCYAN, COLOR_BLACK);
	}
	cct_showstr(PLACE_1 + 10, 2, "  �U", COLOR_HCYAN, COLOR_BLACK, 1);
	cct_showstr(PLACE_1, 3, "�^�T�T�T�T�T�a", COLOR_HCYAN, COLOR_BLACK, 1);

}
void five_game::Color(int next_3[3])
{
	//��ӡ����һ�ֻ����ӵ���������
	cct_showstr(PLACE_1, 5, "�X�T�j�T�j�T�[", COLOR_HCYAN, COLOR_BLACK, 1);
	cct_showstr(PLACE_1, 6, "�U", COLOR_HCYAN, COLOR_BLACK, 1);
	cct_showstr(PLACE_1 + 2, 6, "��", next_3[0], COLOR_WHITE);
	cct_showstr(PLACE_1 + 4, 6, "�U", COLOR_HCYAN, COLOR_BLACK, 1);
	cct_showstr(PLACE_1 + 6, 6, "��", next_3[1], COLOR_WHITE);
	cct_showstr(PLACE_1 + 8, 6, "�U", COLOR_HCYAN, COLOR_BLACK, 1);
	cct_showstr(PLACE_1 + 10, 6, "��", next_3[2], COLOR_WHITE);
	cct_showstr(PLACE_1 + 12, 6, "�U", COLOR_HCYAN, COLOR_BLACK, 1);
	cct_showstr(PLACE_1, 7, "�^�T�m�T�m�T�a", COLOR_HCYAN, COLOR_BLACK, 1);
}
void five_game::find_num(int now[7])
{
	//ͳ�Ƶ�ǰ����ɫ���ӵĸ���
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
	cct_showstr(40, 9, "�X�T�T�T�T�T�T�T�T�T�T�T�T�[", COLOR_HCYAN, COLOR_BLACK, 1);
	int t = 0;
	for (int i = 1; i < 7; i++)
	{
		t += now[i];
	}
	//��ӡ���������ӵ�������ռ�ȣ��Լ������ĸ���
	for (int i = 1; i <= 6; i++)
	{
		cct_showstr(PLACE_1, 9 + i, "�U", COLOR_HCYAN, COLOR_BLACK, 1);
		cct_showstr(PLACE_1 + 2, 9 + i, "��", i, COLOR_WHITE, 1);
		cct_showstr(PLACE_1 + 4, 9 + i, "��", COLOR_HCYAN, COLOR_BLACK, 1);
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
		cct_showstr(PLACE_2 + 7, 9 + i, "����-", COLOR_HCYAN, COLOR_BLACK, 1);
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
		cct_showstr(PLACE_3 + 6, 9 + i, "�U", COLOR_HCYAN, COLOR_BLACK, 1);

	}
	cct_showstr(PLACE_1, 16, "�^�T�T�T�T�T�T�T�T�T�T�T�T�a", COLOR_HCYAN, COLOR_BLACK, 1);
	cct_showstr(0, 20, " ", COLOR_BLACK, COLOR_WHITE, 1);
}
void five_game::add( int next_3[3])
{
	//Ϊ�������������������λ��
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
	//���ؿ����λ�ø���
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
	shape2();//��ӡ������
	int next_3[3];//��������ɫ
	for (int i = 0; i < 3; i++)
		next_3[i] = rand() % 6 + 1;
	Score();//���µ÷���
	Color(next_3);//���������ӵ���Ϣ��
	int now[7] = { 0 };//������ɫ���ڵĸ���,0�±겻��
	int die[7] = { 0 };//������ɫ������ȥ�ĸ������±�Ϊ1��6����6������
	find_num(now); //��ǰ����ɫ���ӵĸ���
	xinxi(now, die);//���ϸ�����ɫ�����ӵ���Ϣ
	int ju = mouse();//����������
	if (ju == 1)//��������Ҽ����˳�������Ϸ
	{
		return 0;
	}
	int temp1 = judge( remove);//�ж���û�����ӿ�������
	if (temp1 == 0)//û�����ӿ�������
	{
		int opp = Isfull();
		if (opp <= 3)
			return 0;
		add(next_3);
	}
	else//�����ӿ�����������¼�������ӵ���ɫ����������die������и���
	{
		total_score += temp1;
		die[remove[0]] += remove[1];
		die[remove[2]] += remove[3];
		die[remove[4]] += remove[5];
		die[remove[6]] += remove[7];
	}
	//ͨ��ѭ����������Ϸ����
	while (1)
	{
		shape2();
		find_num(now);
		for (int i = 0; i < 3; i++)
			next_3[i] = rand() % 6 + 1;
		Score();//���µ÷���
		Color(next_3);//���������ӵ���Ϣ��
		xinxi(now, die);//���ϸ�����ɫ�����ӵ���Ϣ
		int juu = mouse();//������
		if (juu == 1)//������Ҽ�
		{
			break;
		}
		for (int i = 0; i < 8; i++)//remove�������
			remove[i] = 0;
		int temp = judge(remove);
		if (temp == 0)
		{
			int  ff = Isfull();
			if (ff <= 3)//С�ڵ���������λ�ã���Ϸ����
			{
				cct_showstr(0, 2 * hang + 4, "   ��Ϸ����   " , COLOR_HCYAN, COLOR_BLACK, 1);
				cct_showstr(0, 2 * hang + 4, "", COLOR_BLACK, COLOR_BLACK, 1);
				Sleep(Sleep_time*100);
				return 0;
			}
			add(next_3);
			//�������Ԫ��֮���ٴ��ж�
			for (int i = 0; i < 8; i++)//remove�������
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
	int col, lin, b_col, b_lin;//��¼��ǰ��cmd����
	cct_getconsoleborder(col, lin, b_col, b_lin);
	
	//��ʼ�����飬��ӡ�����5������
	int a[7] = { 0 };
	//���ӵ���ɫ
	for (int i = 0; i <7; i++)
	{
		a[i] = rand() % 6 + 1;
	}
	//���ӵ�λ��
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
	//������Ϸ
	fun();
	cct_setconsoleborder(col, lin, b_col, b_lin);
	cct_setfontsize("������", 16);
}

int five_game::judge(int remove[])
{
	int mark[9][9] = { 0 };
	int score = 0;
	// ����������
	for (int i = 0; i < hang; i++)
	{
		int j = 0;
		while (map[i][j] == 0&&j<=lie)
		{
			j++;
		}//�ҵ���һ������
		int count = 1;//��һ������
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
		//��ʱΪ��ǰ�е����ұߵļ������ӿ�������
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
	//�����������
	for (int j = 0; j < lie; j++)//����Ѱ��
	{
		int i = 0;
		while (map[i][j] == 0&&i<=hang)
		{
			i++;
		}//�ҵ���һ������
		int count = 1;//��һ������
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
		//��ʱΪ��ǰ�е������漸�����ӿ�������
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
	// ������Խ�������
	int i, j;
	for (int k = 0; k < 9; k++)
	{
		//����9*9�����̣����ֻ��9�����Խ��߿�����5�������ϵ�����
		if (k >= 0 && k <= 4)
		{
			i = 0;
			j = 4 - k;//jΪ0,1,2,3,4
		}
		else
		{
			j = 0;
			i = k - 5;//iΪ1,2,3,4
		}
		while (map[i][j] == 0 && i < 9 && j < 9)//�ҵ���һ������
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

	// ��鸱�Խ�������
	for (int k = 0; k < 9; k++)
	{
		//����9*9�����̣����ֻ��9�����Խ��߿�����5�������ϵ�����
		if (k >= 0 && k <= 4)
		{
			i = 0;
			j = 4 + k;//jΪ4,5,6,7,8
		}
		else
		{
			j = 8;
			i = k - 4;//iΪ1,2,3,4
		}
		while (map[i][j] == 0 && i < 9 && j>=0)//�ҵ���һ������
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
	//�����п���������С��������
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
	//���ظ�ɾ�������ӽ���ɾ�������ĸ��£���������������Ľ��棬���һ�������ɾ����������5���4.
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			if (mark[i][j] == 2)//��λ�õ�����Ҫ��������
			{
				for (int k = 0; k < 4; k++)
				{
					if (map[i][j] == remove[2 * k])
					{
						remove[2 * k + 1]--;//ʵ�ʵ�ɾ������-1
						break;
					}
				}
			}
			else if (mark[i][j] == 3)//��λ�õ�����Ҫ��������
			{
				for (int k = 0; k < 4; k++)
				{
					if (map[i][j] == remove[2 * k])
					{
						remove[2 * k + 1]-=2;//ʵ�ʵ�ɾ������-2
						break;
					}
				}
			}
			else if (mark[i][j] == 4)//��λ�õ�����Ҫ�����Ĵ�
			{
				for (int k = 0; k < 4; k++)
				{
					if (map[i][j] == remove[2 * k])
					{
						remove[2 * k + 1]-=3;//ʵ�ʵ�ɾ������-3
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
	five_game A;//������Ķ���
	while (1)
	{
		A.Strat();
		int temp=A.restore();
		if (temp == 0)
			break;
	}
	return 0;
}
