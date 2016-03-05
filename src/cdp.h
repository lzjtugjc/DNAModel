//By MatrixC

#ifndef __CDP_H__
#define __CDP_H__

#include <vector>
#include <direct.h>   
#include <gl/glut.h>
#include <stdio.h>
#include <shlwapi.h>
#include <iostream>
#include <ctype.h>
#include <windows.h>
#include "DNAClass.h"

#define __MOVELEFT__	1				//向左移动
#define __MOVERIGHT__	2				//向友移动

#define __DNAMOVEOffset__ 1



using namespace std;

//DNAClass*		  g_DNAPoint;					//DNA指针
vector<DNAClass*> g_DNA;						//DNA链
int				  g_UsingDNA;					//正在使用的DNA链
int				  g_numOfDNA;					//DNA链数
unsigned long	  g_numOfTimePerLoop = 0;		//每次循环所用的时间
unsigned long	  g_FPS = 0;					//每秒帧数
HINSTANCE		  g_Hinstance;					//程序句柄
HWND			  g_Hwnd;						//程序句柄
HDC				  g_HDC;
bool			  g_Multisample = false;
bool			  g_MoveDNA = false;			//DNA是否在切换;
int				  g_ObjectDNA = 0;
char			  g_path[_MAX_PATH]; 

struct WindowInfo
{
	int Width;
	int Height;
	int PosX;
	int PosY;
};

void RenderDNA();				//渲染DNA
void InitDNA(int numOfDNA);
void KeyControl(unsigned char key,int x,int y);
void MainLoop();
void InitApplication(int* argc,char** argv);
void InitMaterial();
void Destory();


void RenderDNA()
{


	//渲染DNA
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();


	//得到FPS
	char t_buf[256];
	itoa(g_FPS,t_buf,10);


	for(vector<DNAClass*>::iterator iter = g_DNA.begin();iter != g_DNA.end();iter++)
	{
		DNAClass* t_iter;
		t_iter = *iter;
		if(t_iter->GetEnable())
		{
			t_iter->RenderAll();
		}

	}

	glFlush();
	glutSwapBuffers();

}

void InitDNA(int numOfDNA)
{
//	g_DNA.reserve(g_numOfDNA + 1);
	g_DNA.reserve(1);
	DNAClass* t_DNAClass;
	
	char t_runfile[_MAX_PATH];
	memset(t_runfile,0,_MAX_PATH);

	char t_filepath[256+1];
	memset(t_filepath,0,256);

	strcat(t_filepath,g_path);
	strcat(t_filepath,"RunDNA.dat");

	FILE *fp;
	fp = fopen(t_filepath,"r");
	if(!fp)
	{
		MessageBox(NULL,"错误，运行文件未找到，请用DNACTRL建立","错误",MB_OK);
		exit(0);
	}

	fgets(t_runfile,256,fp);
	fclose(fp);

	t_DNAClass = new DNAClass(t_runfile);
	t_DNAClass->SetBasePosition(0,0,-15);
	g_DNA.push_back(t_DNAClass);

}



void KeyControl(unsigned char key,int x,int y)
{
	//键盘控制部分
	unsigned char t_key = tolower(key);

	switch(t_key)
	{
	case 27:
		//退出,按ESC后执行
		exit(0);
		break;

	case 'w':
		//向前旋转
		g_DNA.at(g_UsingDNA)->RotateAdd(-1,0,0);
		break;
	case 's':
		//向后旋转
		g_DNA.at(g_UsingDNA)->RotateAdd(1,0,0);
		break;
	case 'a':
		//向左旋转
		g_DNA.at(g_UsingDNA)->RotateAdd(0,0,1);
		break;
	case 'd':
		//向右旋转
		g_DNA.at(g_UsingDNA)->RotateAdd(0,0,-1);
		break;
	case 'q' :
		//增加旋转速度
		if(g_DNA.at(g_UsingDNA)->GetRotate().y < 1)
			g_DNA.at(g_UsingDNA)->RotateAdd(0,0.1,0);
		break;
	case 'e' :
		//降低旋转速度
		if(g_DNA.at(g_UsingDNA)->GetRotate().y > -1)
			g_DNA.at(g_UsingDNA)->RotateAdd(0,-0.1,0);
		break;
	case 'i' :
		//缩小DNA
		g_DNA.at(g_UsingDNA)->Move(0,0,0.05);
		break;
	case 'k' :
		//放大DNA
		g_DNA.at(g_UsingDNA)->Move(0,0,-0.05);
		break;
	case 'z' :
		//设置视图1
		g_DNA.at(g_UsingDNA)->SetRotateXZ(0,-90);
		break;
	case 'x' :
		//设置视图2
		g_DNA.at(g_UsingDNA)->SetRotateXZ(-90,0);
		break;
	case 'c' :
		//设置视图3
		g_DNA.at(g_UsingDNA)->SetRotateXZ(0,0);
		break;


	}
}

void MainLoop()
{
	//程序的主循环
	unsigned long t_NowTime = GetTickCount();
	unsigned long t_UseTime;


	//显示DNA
	RenderDNA();

	t_UseTime = GetTickCount() - t_NowTime;
	g_numOfTimePerLoop = t_UseTime;
	if(t_UseTime!=0)
		g_FPS = 1 / (t_UseTime*0.001);

}

void Destory()
{
	for(vector<DNAClass*>::iterator iter = g_DNA.begin();iter != g_DNA.end();iter++)
		delete (*iter);
}

void InitApplication(int* argc,char** argv)
{
	//初始化

	//得到运行目录
	getcwd( g_path,_MAX_PATH);

	strcat(g_path,"\\");

	glutInit(argc, argv); 
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE); 
	glutInitWindowPosition(100,100); 
	glutInitWindowSize(500,500); 
	glutCreateWindow("DNA");

	//获得程序句柄
	g_Hinstance = GetModuleHandle(NULL);
	g_Hwnd = GetActiveWindow();
	g_HDC = wglGetCurrentDC();

	//设置循环函数
	glutDisplayFunc(MainLoop); 
	glutIdleFunc(MainLoop);

	//设置键盘函数
	glutKeyboardFunc(KeyControl);

	//设置视图矩阵
	glMatrixMode(GL_PROJECTION);
	gluPerspective(45, 1.0f, 1.0f, 50.0f);


	//设置光照方法
	glShadeModel(GL_FLAT);  
	glDepthFunc(GL_LEQUAL); 

 	glEnable(GL_LINE_SMOOTH);

	g_numOfDNA = 3;
	InitDNA(g_numOfDNA);
	g_UsingDNA = 0;


	glutMainLoop(); 
	Destory();

}

void InitMaterial()
{

}

#endif