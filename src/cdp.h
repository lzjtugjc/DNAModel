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

#define __MOVELEFT__	1				//�����ƶ�
#define __MOVERIGHT__	2				//�����ƶ�

#define __DNAMOVEOffset__ 1



using namespace std;

//DNAClass*		  g_DNAPoint;					//DNAָ��
vector<DNAClass*> g_DNA;						//DNA��
int				  g_UsingDNA;					//����ʹ�õ�DNA��
int				  g_numOfDNA;					//DNA����
unsigned long	  g_numOfTimePerLoop = 0;		//ÿ��ѭ�����õ�ʱ��
unsigned long	  g_FPS = 0;					//ÿ��֡��
HINSTANCE		  g_Hinstance;					//������
HWND			  g_Hwnd;						//������
HDC				  g_HDC;
bool			  g_Multisample = false;
bool			  g_MoveDNA = false;			//DNA�Ƿ����л�;
int				  g_ObjectDNA = 0;
char			  g_path[_MAX_PATH]; 

struct WindowInfo
{
	int Width;
	int Height;
	int PosX;
	int PosY;
};

void RenderDNA();				//��ȾDNA
void InitDNA(int numOfDNA);
void KeyControl(unsigned char key,int x,int y);
void MainLoop();
void InitApplication(int* argc,char** argv);
void InitMaterial();
void Destory();


void RenderDNA()
{


	//��ȾDNA
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();


	//�õ�FPS
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
		MessageBox(NULL,"���������ļ�δ�ҵ�������DNACTRL����","����",MB_OK);
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
	//���̿��Ʋ���
	unsigned char t_key = tolower(key);

	switch(t_key)
	{
	case 27:
		//�˳�,��ESC��ִ��
		exit(0);
		break;

	case 'w':
		//��ǰ��ת
		g_DNA.at(g_UsingDNA)->RotateAdd(-1,0,0);
		break;
	case 's':
		//�����ת
		g_DNA.at(g_UsingDNA)->RotateAdd(1,0,0);
		break;
	case 'a':
		//������ת
		g_DNA.at(g_UsingDNA)->RotateAdd(0,0,1);
		break;
	case 'd':
		//������ת
		g_DNA.at(g_UsingDNA)->RotateAdd(0,0,-1);
		break;
	case 'q' :
		//������ת�ٶ�
		if(g_DNA.at(g_UsingDNA)->GetRotate().y < 1)
			g_DNA.at(g_UsingDNA)->RotateAdd(0,0.1,0);
		break;
	case 'e' :
		//������ת�ٶ�
		if(g_DNA.at(g_UsingDNA)->GetRotate().y > -1)
			g_DNA.at(g_UsingDNA)->RotateAdd(0,-0.1,0);
		break;
	case 'i' :
		//��СDNA
		g_DNA.at(g_UsingDNA)->Move(0,0,0.05);
		break;
	case 'k' :
		//�Ŵ�DNA
		g_DNA.at(g_UsingDNA)->Move(0,0,-0.05);
		break;
	case 'z' :
		//������ͼ1
		g_DNA.at(g_UsingDNA)->SetRotateXZ(0,-90);
		break;
	case 'x' :
		//������ͼ2
		g_DNA.at(g_UsingDNA)->SetRotateXZ(-90,0);
		break;
	case 'c' :
		//������ͼ3
		g_DNA.at(g_UsingDNA)->SetRotateXZ(0,0);
		break;


	}
}

void MainLoop()
{
	//�������ѭ��
	unsigned long t_NowTime = GetTickCount();
	unsigned long t_UseTime;


	//��ʾDNA
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
	//��ʼ��

	//�õ�����Ŀ¼
	getcwd( g_path,_MAX_PATH);

	strcat(g_path,"\\");

	glutInit(argc, argv); 
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE); 
	glutInitWindowPosition(100,100); 
	glutInitWindowSize(500,500); 
	glutCreateWindow("DNA");

	//��ó�����
	g_Hinstance = GetModuleHandle(NULL);
	g_Hwnd = GetActiveWindow();
	g_HDC = wglGetCurrentDC();

	//����ѭ������
	glutDisplayFunc(MainLoop); 
	glutIdleFunc(MainLoop);

	//���ü��̺���
	glutKeyboardFunc(KeyControl);

	//������ͼ����
	glMatrixMode(GL_PROJECTION);
	gluPerspective(45, 1.0f, 1.0f, 50.0f);


	//���ù��շ���
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