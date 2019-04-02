#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <string.h>
#include <windows.h>
#include "mtg.h"
#include "inc\fmod.h"

#define Ver "1.01"

FMOD_SYSTEM *g_System;		// ������� ����� ���� �Լ�
FMOD_SOUND *g_Sound[6];		// �������(ȿ����)�� ����
FMOD_CHANNEL *g_Channel[2]; // ��������� ����

#define WIDTH_S 0			// ���� ������ǥ x
#define HEIGHT_S 8			// ���� ������ǥ y
#define WIDTH 38+2			// ���� �ʺ�
#define HEIGHT 21+2			// ���� ����

#define MY_WALL 0			// �� = 0
#define MY_BLANK 1			// �� ���� = 1
#define MY_STAR 2			// �� = 2
#define MY_PLAYER 3			// �÷��̾� = 3
#define MY_BLOCK 4			// ��ֹ� = 4
#define MY_DEAD 5			// ��ü;; = 5

#define PLAYER_X 4			// �÷��̾� �⺻ x��ǥ = 4
#define PLAYER_Y 21			// �÷��̾� �⺻ y��ǥ = 23

#define SPEED 38			// ���� �⺻ �ӵ�
//#define PLAYTIME 1000		// ���� �÷��� �ð� ( 40 ��)

typedef struct 
{
	char n_char[7];			// ��¿�
	int n_int;				// ��� �� �����	
	char name[20];
}PRINT_NUM;

typedef struct
{	
	PRINT_NUM score;
	PRINT_NUM times;
}RECORD;

int map[WIDTH][HEIGHT];		// ��ü ��
int jump = 0;
int jumpcount = 0, jumpcount2 = 0;
int gameover = 0;
RECORD user;
RECORD record;

////////////////////////////////////// �ʱ�ȭ ///////////////////////////////////////

void GameStart(void);		// ���� ���� ȭ��
void GameMain(void);		// ���� ����
void GameIni(void);			// ���� ���� �ʱ�ȭ
void SoundIni(void);		// ���� �Ҹ� �ʱ�ȭ
void Release(void);			// ������� �� ����
void Draw(void);			// ���� ��üȭ�� ���
void DrawMap(void);			// �� ���
void DrawHelp(void);		// ���� �� ���� ǥ��
int Jump(void);				// ����
int DrawBlock(void);		// �� �̵�
void GetNewBlock(void);		// ���ο� �� ����
void MapIni(void);			// �� �ʱ�ȭ
void GameOver(void);		// ���� ����
void PrintInt(int);			// ����� ���� �ڷ��� ����
void GetRecord(void);		// �ְ��� �ҷ���
void WriteRecord(void);		// �ְ��� ����
void GetName(void);			// �÷��̾� �̸� �Է¹���
void DrawTitle(void);		// ����ȭ�� ���
void Admin(void);			// ������ ���
int CheckPlayer(void);		// �÷��̾� ���� ��ġ ã�� (���� �÷��̾� y ��ǥ)
int CheckGameOver(int);		// �÷��̾� ���� ���� Ȯ�� (�Ű����� : ���� �÷��̾� y ��ǥ)
void CheckBlock(void);		// �Ұ����� �� ���� ����
int RandomBGM(void);		// BGM �������� ���

//////////////////////////////////////  �Լ���  ///////////////////////////////////////

void DrawTitle(void)
{
	_DrawTextColorOrg(10,5," I N F I N I T E ",0xf9);
	_DrawTextColorOrg(30,5,"       ",0x22);
	_DrawTextColorOrg(30,6," ",0x22);
	_DrawTextColorOrg(36,6,"  ",0x22);
	_DrawTextColorOrg(30,7,"       ",0x22);
	_DrawTextColorOrg(30,8," ",0x22);
	_DrawTextColorOrg(35,8," ",0x22);
	_DrawTextColorOrg(30,9," ",0x22);
	_DrawTextColorOrg(36,9,"  ",0x22);				// R
	_DrawTextColorOrg(39,7," ",0x22);
	_DrawTextColorOrg(39,8," ",0x22);
	_DrawTextColorOrg(40,9,"   ",0x22);
	_DrawTextColorOrg(43,8," ",0x22);
	_DrawTextColorOrg(43,7," ",0x22);
	_DrawTextColorOrg(44,9," ",0x22);				// u
	_DrawTextColorOrg(46,9," ",0x22);
	_DrawTextColorOrg(46,8," ",0x22);
	_DrawTextColorOrg(46,7,"    ",0x22);
	_DrawTextColorOrg(50,8," ",0x22);
	_DrawTextColorOrg(50,9," ",0x22);				// n
	_DrawTextColorOrg(53,4,"            ",0x00);
	_DrawTextColorOrg(61,5,"   ",0x00);
	_DrawTextColorOrg(59,6,"   ",0x00);
	_DrawTextColorOrg(57,7,"   ",0x00);
	_DrawTextColorOrg(55,8,"   ",0x00);
	_DrawTextColorOrg(53,9,"                ",0x00); // Z
	_DrawTextColorOrg(1,11,"������������������������������������������������������������������������������",0xf9);
	_DrawTextColorOrg(70,8,"��",0xfd);
	_DrawTextColorOrg(40,13,"1 s t   E D I T I O N  ver",0xf1);
	_DrawTextColorOrg(68,13,Ver,0xf0);

	_DrawTextColorOrg(62,1,"CODING By. Z",0xf0);
}

void GetName(void) // �÷��̾� �̸� �Է¹ޱ�
{
	FMOD_System_PlaySound(g_System,FMOD_CHANNEL_FREE,g_Sound[0],0,&g_Channel[0]); // Ÿ��Ʋ ȭ�� BGM ���

	DrawTitle();
	printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n  \t\t\t\t\t   ");
	_DrawTextColorOrg(21,15,"��������������������������������������",0xfc);
	_DrawTextColorOrg(21,16,"��                                  ��",0xfc);					
	_DrawTextColorOrg(21,17,"�� Input Your Name :                ��",0xfc);
	_DrawTextColorOrg(21,18,"��                                  ��",0xfc);	
	_DrawTextColorOrg(21,19,"��������������������������������������",0xfc);

	gets(user.score.name);
	if(!strcmp(user.score.name,"Admin#"))
		Admin();
	else if(!strcmp(user.score.name,"exit#"))
		exit(1);
	else if(!strcmp(user.score.name,""))  // �̸��� �Է����� �ʾ��� �� ����� �Է��ϰ� �����
	{
		while(1)
		{
			system("cls");
			_DrawTextColorOrg(25,21,"Input Your Name Correctly!!",0xfc);
			DrawTitle();

			printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n  \t\t\t\t\t   ");
			_DrawTextColorOrg(21,15,"��������������������������������������",0xfc);
			_DrawTextColorOrg(21,16,"��                                  ��",0xfc);					
			_DrawTextColorOrg(21,17,"�� Input Your Name :                ��",0xfc);
			_DrawTextColorOrg(21,18,"��                                  ��",0xfc);	
			_DrawTextColorOrg(21,19,"��������������������������������������",0xfc);
			gets(user.score.name);
			fflush(stdin);
			if(strcmp(user.score.name,""))
				break;
		}
	}
	else
	{
		strcpy(user.times.name,user.score.name);
	}
}

void GameStart(void)
{
	system("cls");
	DrawTitle();
	_DrawTextColorOrg(20,15,"��������������������������������������������",0xfc);	
	_DrawTextColorOrg(20,16,"��                                        ��",0xfc);	
	_DrawTextColorOrg(20,17,"��  Press any key to start Infinite RunZ! ��",0xfc);
	_DrawTextColorOrg(20,18,"��                                        ��",0xfc);
	_DrawTextColorOrg(20,19,"��������������������������������������������",0xfc);
	_DrawTextColorOrg(25,21,"Welcome!",0xfc);
	_DrawTextColorOrg(34,21,user.score.name,0xf0);

	while (!kbhit());

	FMOD_Channel_Stop(g_Channel[0]);
	FMOD_System_PlaySound(g_System,FMOD_CHANNEL_FREE,g_Sound[RandomBGM()],0,&g_Channel[0]); // ���� ȭ�� BGM ���
}

void Admin(void)  // ������ ���
{
	char command[10]="0";
	int switch1=0, switch2=0;;

	system("cls");
	FMOD_Channel_Stop(g_Channel[0]);	

	while(1)
	{
		DrawTitle();
		printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n  \t\t\t\t\t   ");
		_DrawTextColorOrg(16,13,"Admin Mode!!",0xfc);		
		_DrawTextColorOrg(21,15,"��������������������������������������",0xfc);
		_DrawTextColorOrg(21,16,"��                                  ��",0xfc);		
		_DrawTextColorOrg(21,17,"�� Input Command :                  ��",0xfc);
		_DrawTextColorOrg(21,18,"��                                  ��",0xfc);
		_DrawTextColorOrg(21,19,"��������������������������������������",0xfc);

		if(!strcmp(command,"reset#"))			
			_DrawTextColorOrg(25,21,"Reset Record Data!!",0xfc);
		else if(switch1)
			_DrawTextColorOrg(25,21,"Set Record Data!!",0xfc);
		else if(switch2)
			_DrawTextColorOrg(25,21,"Set Speed Data!!",0xfc);

		gets(command);

		if(!strcmp(command,"reset#"))
		{
			FILE* fp1=fopen("data\\hi-score.sav","wb");
			FILE* fp2=fopen("data\\best-time.sav","wb");

			record.score.n_int = 0;
			record.times.n_int = 0;

			PrintInt(0);

			strcpy(record.score.name,"NULL");			// ���̽��ھ �̸��� ����
			strcpy(record.times.name,"NULL");			// �ð� �ű�Ͽ�  �̸��� ����

			fprintf(fp1,"%d %s",record.score.n_int,record.score.name);
			fprintf(fp2,"%d %s",record.times.n_int,record.times.name);
			
			fclose(fp1);
			fclose(fp2);	
		}
		else if(!strcmp(command,"exit#"))
		{
			system("cls");
			GetName();
			break;
		}

		system("cls");
	}
}

void CheckBlock(void)		// �Ұ����� �� ���� ����
{
	int i;

	if(map[WIDTH - 2][PLAYER_Y] == MY_BLOCK)
	{
		for(i = 6; i < 18; i++)
		{
			if(map[WIDTH - i][PLAYER_Y] == MY_BLOCK || map[WIDTH - i][PLAYER_Y - 1] == MY_BLOCK)
			{
				map[WIDTH - 2][PLAYER_Y] = MY_BLANK;
				map[WIDTH - 2][PLAYER_Y - 1] = MY_BLANK;
			}
		}
	}
}

void GetNewBlock(void)
{	
	if(rand()%100+1 <= 25)		// 25%
	{				
		map[WIDTH-2][PLAYER_Y-rand()%5] = MY_STAR;
	}

	if(rand()%100+1 <= 10)		// 10%
	{			
		map[WIDTH-2][PLAYER_Y] = MY_BLOCK;
	}
	else if(rand()%100+1 <= 10)		// 10%
	{
		map[WIDTH-2][PLAYER_Y] = MY_BLOCK;
		map[WIDTH-2][PLAYER_Y-1] = MY_BLOCK;
	}

	CheckBlock();
}

int DrawBlock(void)		// �� ��ü�� �������� �̵�
{
	int x, y;

	GetNewBlock();

	if(CheckGameOver(CheckPlayer()) == -1)
	{
		return -1;
	}
	for(x=1; x<WIDTH-2; x++)
	{
		for(y=HEIGHT_S+1; y<HEIGHT-1; y++)
		{
			if(map[x][y] == MY_BLOCK && map[x-1][y] == MY_PLAYER)	// ���� ������ ���ӿ���
			{
				return -1;
			}
			else if(map[x][y] == MY_STAR && map[x-1][y] == MY_PLAYER)		// �� �Ծ��� ��
			{
				user.score.n_int+=10;
				map[x][y] = map[x+1][y];
			}
			else if(map[x][y] == MY_STAR && map[x+1][y] == MY_PLAYER)
				map[x][y] = MY_BLANK;
			else if(map[x][y] == MY_BLOCK && map[x+1][y] == MY_PLAYER)
				map[x][y] = MY_BLANK;
			else if(map[x][y] != MY_PLAYER && map[x+1][y] != MY_PLAYER)
				map[x][y] = map[x+1][y];
		}
	}
	for(y=HEIGHT_S+1; y<HEIGHT-1; y++)
	{
		map[WIDTH-2][y] = MY_BLANK;
	}
	return 0;
}

int Jump(void)
{
	if(CheckGameOver(CheckPlayer()) == -1)
	{
		return -1;
	}
	if(jump && jumpcount<8)
	{
		if(jumpcount%2==0)
		{
			if(map[PLAYER_X][PLAYER_Y-jumpcount2-1] == MY_STAR)
			{
				user.score.n_int+=10;
			}
			else if(map[PLAYER_X][PLAYER_Y-jumpcount2-1] == MY_BLOCK)		// �����ϴٰ� ���
			{
				//map[PLAYER_X][PLAYER_Y-jumpcount2] = MY_DEAD;
				return -1;
			}
			map[PLAYER_X][PLAYER_Y-jumpcount2-1] = MY_PLAYER;
			map[PLAYER_X][PLAYER_Y-jumpcount2] = MY_BLANK;
			jumpcount++;
			jumpcount2++;
		}
		else
			jumpcount++;
	}
	else if(jump && jumpcount==16)
	{		
		jump=0;
		jumpcount=0;
	}
	else if(jump && jumpcount>=8)
	{
		if(jumpcount%2==0)
		{			
			jumpcount2--;
			if(map[PLAYER_X][PLAYER_Y-jumpcount2] == MY_STAR)
			{
				user.score.n_int+=10;
			}
			else if(map[PLAYER_X][PLAYER_Y-jumpcount2] == MY_BLOCK)		// �������ٰ� ���
			{
				if(map[PLAYER_X][PLAYER_Y-jumpcount2 - 1] == MY_BLOCK)
				{
					map[PLAYER_X][PLAYER_Y-jumpcount2 - 1] = MY_BLANK;
					map[PLAYER_X + 1][PLAYER_Y-jumpcount2 - 1] = MY_BLOCK;
				}
				return -1;
			}
			map[PLAYER_X][PLAYER_Y-jumpcount2] = MY_PLAYER;
			map[PLAYER_X][PLAYER_Y-jumpcount2-1] = MY_BLANK;
			jumpcount++;			
		}
		else
			jumpcount++;
	}
	return 0;
}

void DrawMap(void)
{
	int x, y;

	for(y=HEIGHT_S; y<HEIGHT; y++)
	{
		for(x=WIDTH_S; x<WIDTH; x++)
		{
			if(map[x][y]==MY_WALL)
			{
				if(x==WIDTH_S && y==HEIGHT_S)
					_DrawTextColor(x*2,y,"��",0xf8);
				else if(x==WIDTH-1 && y==HEIGHT-1)
					_DrawTextColor(x*2,y,"��",0xf8);
				else if(x==WIDTH_S && y==HEIGHT-1)
					_DrawTextColor(x*2,y,"��",0xf8);
				else if(x==WIDTH-1 && y==HEIGHT_S)
					_DrawTextColor(x*2,y,"��",0xf8);				
				else if(y==HEIGHT-1 || y==HEIGHT_S)
					_DrawTextColor(x*2,y,"��",0xf8);				
				else
					_DrawTextColor(x*2,y,"��",0xf8);
			}			
			else if(map[x][y]==MY_BLANK)
				_DrawTextColor(x*2,y,"  ",0xf0);

			else if(map[x][y]==MY_BLOCK)
				_DrawTextColor(x*2,y,"��",0xf0);
						
			else if(map[x][y]==MY_STAR)		
				_DrawTextColor(x*2, y,"��",0xfc);

			else if(map[x][y]==MY_PLAYER)
				_DrawTextColor(x*2,y,"��",0xf9);

			else if(map[x][y]==MY_DEAD)
			{
				if(map[x][y - 1]==MY_PLAYER)
					_DrawTextColor(x*2,y - 1,"��",0xfc);
				else
					_DrawTextColor(x*2,y,"��",0xfc);
			}
		}
	}
}

void DrawHelp(void)
{
	_DrawTextColor(2,1,"Infinite RunZ  1st EDITION ver",0xf2);				// ���� ���� ���
	_DrawTextColor(34,1,Ver,0xf0);
	_DrawTextColor(65,1,"CODING By. Z",0xf3);
	_DrawTextColor(2,3,"SCORE : ",0xf0);
	_DrawTextColor(14,3,user.score.n_char,0xf8);
	_DrawTextColor(25,3,"DISTANCE : ",0xf0);
	_DrawTextColor(40,3,user.times.n_char,0xf8);
	_DrawTextColor(2,5,"Hi-SCORE : ",0xf1);
	_DrawTextColor(14,5,record.score.n_char,0xf0);
	_DrawTextColor(25,5,"BEST-RECORD : ",0xf1);
	_DrawTextColor(40,5,record.times.n_char,0xf0);
	_DrawTextColor(46,5,"m",0xf0);
	_DrawTextColor(46,3,"m",0xf0);
	_DrawTextColor(14,7,record.score.name,0xf0);
	_DrawTextColor(38,7,record.times.name,0xf0);	
	_DrawTextColor(48,0,"��",0xf8);
	_DrawTextColor(48,1,"��",0xf8);
	_DrawTextColor(48,2,"��",0xf8);
	_DrawTextColor(48,3,"��",0xf8);
	_DrawTextColor(48,4,"��",0xf8);
	_DrawTextColor(48,5,"��",0xf8);
	_DrawTextColor(48,6,"��",0xf8);
	_DrawTextColor(48,7,"��",0xf8);
	_DrawTextColor(48,8,"��",0xf8);
	_DrawTextColor(53,3,"Space : Jump",0xf0);
	_DrawTextColor(53,5,"P : pause / Q : quit",0xf0);
	_DrawTextColor(45,23,"copyright_2014_zeikar@naver.com",0xf0);

	if(gameover)
	{
		_DrawTextColor(30,9,"GAME OVER!!!!",0xfc);
		_DrawTextColor(20,11,"Press 'q' key to exit / 'r' key to restart",0xfc);
	}

	if(user.times.n_int / 1000 > 0 && user.times.n_int % 1000 <= 50)
	{
		_DrawTextColor(22,7,"SPEED UP!",0xfc);
	}
}

void Draw(void)
{	
	DrawMap();		// �� ��ü Ʋ ���	
	DrawHelp();
}

void PrintInt(int n)  // int���� char������ ��ȯ������
{	
	if(n)		// �÷��̾�
	{
		user.score.n_char[0] = user.score.n_int/100000+48;
		user.score.n_char[1] = (user.score.n_int%100000)/10000+48;
		user.score.n_char[2] = (user.score.n_int%10000)/1000+48;
		user.score.n_char[3] = (user.score.n_int%1000)/100+48;
		user.score.n_char[4] = (user.score.n_int%100)/10+48;
		user.score.n_char[5] = (user.score.n_int%10)+48;

		user.times.n_char[0] = user.times.n_int/100000+48;
		user.times.n_char[1] = (user.times.n_int%100000)/10000+48;
		user.times.n_char[2] = (user.times.n_int%10000)/1000+48;
		user.times.n_char[3] = (user.times.n_int%1000)/100+48;
		user.times.n_char[4] = (user.times.n_int%100)/10+48;
		user.times.n_char[5] = (user.times.n_int%10)+48;
	}
	else		// �ְ���
	{
		record.score.n_char[0] = record.score.n_int/100000+48;
		record.score.n_char[1] = (record.score.n_int%100000)/10000+48;
		record.score.n_char[2] = (record.score.n_int%10000)/1000+48;
		record.score.n_char[3] = (record.score.n_int%1000)/100+48;
		record.score.n_char[4] = (record.score.n_int%100)/10+48;
		record.score.n_char[5] = (record.score.n_int%10)+48;

		record.times.n_char[0] = record.times.n_int/100000+48;
		record.times.n_char[1] = (record.times.n_int%100000)/10000+48;
		record.times.n_char[2] = (record.times.n_int%10000)/1000+48;
		record.times.n_char[3] = (record.times.n_int%1000)/100+48;
		record.times.n_char[4] = (record.times.n_int%100)/10+48;
		record.times.n_char[5] = (record.times.n_int%10)+48;
	}
}

int RunKey() // Ű �Է¹޴� �Լ�
{

	char k;

	k=_GetKey();
	if(-1==k) return 0;

	if('q'==k) // q �Է¹����� -1 (����) ��ȯ
	{
		return -1;
	}
	else if ('p'==k)  // ���� �Ͻ�����
	{
		_MessageBoxColor(23,9,34,2," Paused. press any key to resume",0xf4); 
	}
	else if(32 ==k) // �����̽��� ������ ����
	{
		if(!jump)
			FMOD_System_PlaySound(g_System,FMOD_CHANNEL_FREE,g_Sound[5],0,&g_Channel[1]);  // ������ �� ȿ����
		jump=1;
	}
	_Invalidate();

	return 0;
}

void RunTimer() // Ÿ�̸�
{
	static int speed=SPEED;
	static long oldT=0;
	long newT;
	static int killTimer=0;

	if(-1==killTimer)
		return;

	newT=_GetTickCount();
	if(abs(newT-oldT)<speed) // �ӵ�
	{
		return;
	}
	else
	{
		oldT=newT;
	}

	if(-1==Jump())				// �����ϴٰ� ���� ����
	{
		int y;//= CheckPlayer();
		for(y = 0; y <= CheckPlayer(); y++)
		{
			if(map[PLAYER_X][y] == MY_PLAYER)
				map[PLAYER_X][y] = MY_DEAD;
		}

		WriteRecord();
		gameover=1;
		killTimer=-1;
		_Invalidate();
		GameOver();
		
		GameIni();
		killTimer=0;
		user.times.n_int=0;
	}

	else if(-1==DrawBlock())				// ���� ����
	{
		WriteRecord();
		gameover=1;
		killTimer=-1;
		map[PLAYER_X][PLAYER_Y] = MY_DEAD;
		_Invalidate();
		
		GameOver();
		
		GameIni();
		killTimer=0;
		//user.times.n_int=0;
	}
	
	user.times.n_int++;
	
	if(user.times.n_int % 1000 == 0)		// 1000�� ���� ���ǵ� ��!
	{
		if(speed > 7)
		{
			speed -= 2;
		}
	}
	PrintInt(1);
	_Invalidate();
}

void GameMain(void)
{
	while(1)
	{
		RunTimer();
		if(-1==RunKey()) // ��ȯ���� -1 �̸� ���� ����
		{
			break;
		}
	}
}

void MapIni(void)
{
	int x, y;

	for(y=0; y<HEIGHT; y++)
	{
		for(x=0; x<WIDTH; x++)
		{
			if(x==WIDTH_S || y==HEIGHT_S || x==WIDTH-1 || y==HEIGHT-1)
				map[x][y]=MY_WALL;
			else
				map[x][y]=MY_BLANK;
		}
	}
}

void GameOver(void)
{	
	char key;

	FMOD_System_PlaySound(g_System,FMOD_CHANNEL_FREE,g_Sound[4],0,&g_Channel[1]); // ���� ���� ���� ���

	while(1)
	{
		if(kbhit())
		{
			key=getch();
			if(key=='q')		// qŰ ������ ���� ����
				exit(0);

			else if(key=='r')	// rŰ ������ ���� �����
			{
				jump=0;
				jumpcount=0;
				jumpcount2=0;
				gameover=0;
				break;
			}
		}
	}
}

int CheckPlayer(void)
{
	int y;
	for(y = PLAYER_Y; y < PLAYER_Y+6; y++)
	{
		if(map[PLAYER_X][y] == MY_PLAYER)
			return y;
	}
}

int CheckGameOver(int current)
{
	if(map[PLAYER_X + 1][current] == MY_BLOCK)
	{
		return -1;
	}
	return 0;
}

void GetRecord(void)		// �ְ��� �ε�
{
	FILE* fp1=fopen("data\\hi-score.sav","rb");
	FILE* fp2=fopen("data\\best-time.sav","rb");
	if(fp1==NULL || fp2==NULL)
	{
		return;
	}	
	fscanf(fp1,"%d %s",&record.score.n_int,record.score.name);
	fscanf(fp2,"%d %s",&record.times.n_int,record.times.name);

	fclose(fp1);
	fclose(fp2);

	PrintInt(0);
}

void WriteRecord(void)		// �ְ��� ����
{
	if (user.score.n_int > record.score.n_int) // ���� ������ ��Ϻ��� ���� ��!! ���̽��ھ� �޼�~!!
	{
		FILE* fp=fopen("data\\hi-score.sav","wb");
		record.score.n_int = user.score.n_int;

		PrintInt(0);

		strcpy(record.score.name,user.score.name);  // ���̽��ھ ���� �̸��� ����

		fprintf(fp,"%d %s",record.score.n_int,record.score.name);
		fclose(fp);
	}
	if (user.times.n_int > record.times.n_int)		// �ð� �ű��
	{
		FILE* fp=fopen("data\\best-time.sav","wb");
		record.times.n_int = user.times.n_int;

		PrintInt(0);

		strcpy(record.times.name,user.times.name);  // �ð� �ű�Ͽ� ���� �̸��� ����

		fprintf(fp,"%d %s",record.times.n_int,record.times.name);
		fclose(fp);
	}
}

void GameIni(void)		// ���� �ʱ�ȭ
{	
	MapIni();

	map[PLAYER_X][PLAYER_Y] = MY_PLAYER;
	user.score.n_int = 0;
	user.times.n_int = 0;	
	PrintInt(1);

	GetNewBlock();
}

int RandomBGM(void)		// BGM �������� ���
{
	srand(time(NULL));

	return rand() % 3 + 1;
}

void SoundIni(void)
{
	FMOD_System_Create(&g_System);
	FMOD_System_Init(g_System,10,FMOD_INIT_NORMAL,NULL);

	FMOD_System_CreateSound(g_System,"data\\bgm_lobby.mp3",FMOD_LOOP_NORMAL,0,&g_Sound[0]); // Ÿ��Ʋ ȭ�� BGM
	FMOD_System_CreateSound(g_System,"data\\bgm_main.ogg",FMOD_LOOP_NORMAL,0,&g_Sound[1]);  // ���� ȭ�� BGM
	FMOD_System_CreateSound(g_System,"data\\bgm_main2.ogg",FMOD_LOOP_NORMAL,0,&g_Sound[2]);  // ���� ȭ��2 BGM
	FMOD_System_CreateSound(g_System,"data\\bgm_main3.mp3",FMOD_LOOP_NORMAL,0,&g_Sound[3]);  // ���� ȭ��3 BGM
	FMOD_System_CreateSound(g_System,"data\\g_end.mp3",FMOD_DEFAULT,0,&g_Sound[4]);         // ���ӿ��� ȿ����
	FMOD_System_CreateSound(g_System,"data\\g_jelly.mp3",FMOD_DEFAULT,0,&g_Sound[5]);       // ���� ȿ����
}

void Release(void) // ������� �� ����
{
	FMOD_Sound_Release(g_Sound[0]);
	FMOD_Sound_Release(g_Sound[1]);
	FMOD_Sound_Release(g_Sound[2]);
	FMOD_Sound_Release(g_Sound[3]);
	FMOD_Sound_Release(g_Sound[4]);
	FMOD_Sound_Release(g_Sound[5]);
	FMOD_System_Close(g_System);
	FMOD_System_Release(g_System);
}

void main(void)
{
	system("color f0");
	puts("\n * Loading...Please Wait..");

	SoundIni();
	FMOD_System_Update(g_System);

	srand(time(NULL));
	GameIni();

	system("cls");

	GetName();
	_BeginWindow();
	GetRecord();
	GameStart();
	GameMain();
	Release();
}