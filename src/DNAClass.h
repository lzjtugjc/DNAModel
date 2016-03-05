//By MatrixC

#ifndef __DNACLASS__
#define __DNACLASS__


#define _AT 1
#define _TA 2
#define _CG 3
#define _GC 4

#define __Long__ 1			//�����
#define __Short__ 2			//�̼��

#define __POINTSTEP__	4


//������λ�Ĳ������
#define __HKEYMATERIAL__ 1
#define __PMATERIAL__ 2
#define __AMATERIAL__ 3
#define __TMATERIAL__ 4
#define __CMATERIAL__ 5
#define __GMATERIAL__ 6
#define __FCMATERIAL__ 7
#define __SPCMATERIAL__ 8

//#define __AMATERIAL__
#define _MAX_PATH   260
extern char g_path[_MAX_PATH ];


#include <gl/glut.h>
#include <stdio.h>
#include <math.h>
#include <vector>
#include <deque>
#include <windows.h>



using namespace std;

//��ɫ�ṹ
struct ColorRGBA
{
	float r;
	float g;
	float b;
	float a;
};

//���ʽṹ
struct CMaterialInformation
{
#define __MaterNameLong__	100

	char m_HKey[__MaterNameLong__];
	char m_A[__MaterNameLong__];
	char m_T[__MaterNameLong__];
	char m_G[__MaterNameLong__];
	char m_C[__MaterNameLong__];
	char m_P[__MaterNameLong__];						//�������
	char m_PPP[__MaterNameLong__];						//��̼��

	CMaterialInformation()
	{
		memset(m_HKey,NULL,__MaterNameLong__);
		memset(m_A,NULL,__MaterNameLong__);
		memset(m_T,NULL,__MaterNameLong__);
		memset(m_C,NULL,__MaterNameLong__);
		memset(m_G,NULL,__MaterNameLong__);
		memset(m_P,NULL,__MaterNameLong__);
		memset(m_PPP,NULL,__MaterNameLong__);

	}


};

struct CMatInfo
{
	int m_HKey;
	int m_A;
	int m_T;
	int m_G;
	int m_C;
	int m_P;
	int m_PPP;

	CMatInfo()
	{
		m_HKey = 0;
		m_A = 0;
		m_T = 0;
		m_C = 0;
		m_G = 0;
		m_P = 0;
		m_PPP = 0;
	}
};

//����
struct CMaterial
{
	ColorRGBA m_Material_Ambient;
	ColorRGBA m_Material_Diffuse;
	ColorRGBA m_Material_Specular;

	//char m_Name[__MaterNameLong__];
	CMaterial()
	{
		m_ID = 0;
	}
	int m_ID;

};


//������Ϣ
class CVertex
{
public:
	double x;
	double y;
	double z;

	CVertex()
	{
		x = 0;
		y = 0;
		z = 0;
	}
};

class CHammer
{
public:
	CVertex m_Point1[2];	//�׵�
	CVertex m_Point2[2];	//�����


	int m_Type;				//������� (1:A T 2:T A 3 C G 4:GC)
	float m_Degree;			//��Z�����ɵļнǡ�
};


class DNAClass
{
private:

	vector<CHammer> m_Hammer;				//���еĵ�
	vector<CHammer> m_UseHammer;			//�����ĵ�

	CVertex m_Rotate;						//��ת����
	CVertex m_BasePosition;					//��ʼλ��

	vector<int> m_DNAList;					//DNA�������б�
	vector<CMaterial> m_Material;			//����
	//map<int,CMaterial> m_Material;
	//map<int,int> aaa;

	bool m_UsingMaterial;					//�Ƿ�ʹ�ò���
	GLuint m_RenderList;					//��Ⱦ�б�ָ��
	CMaterialInformation m_MatInfo;			//��������
	CMatInfo m_MaterialInfo;

	int m_numOfAT;							//AT����
	int m_numOfCG;							//CG����

	bool m_Enable;							//�Ƿ����
	float m_TimePerLoop;


public:
	DNAClass(const char* filename);
	void Init(const char* filename);
	void ComputePoint(int numOfPointPerCircle,float numOfCircle,float Rad,float NRad,float longOfDNA);			//����ͼ�������õĵ������
	void ComputeUsePoint(int k);																				//���㽫Ҫʹ�õĵ�
	void RenderSphere(GLfloat rad);																				//��Ⱦ�������
	void RenderCube(float slong,int type);																		//��Ⱦ���
	void RenderAlkali();																						//��Ⱦ������
	void RenderLine();																							//�������֮����ߣ���Բ�����棡������
	void RenderHKey();																							//��ʾ���
	void RenderFC();																							//��Ⱦ��̼��
	void CompileRenderList();																					//������ʾ�б������Ⱦ�ٶ�

	CVertex GetPosition();																						//��ȡλ��
	CVertex GetRotate();																						//��ȡ�Ƕ�
	bool	GetEnable();																						//��ȡ�Ƿ���Ч
	


	void RenderAll();																							//��Ⱦ����
	void SetLight();																							//���õƹ�

	int ReadDNAListFromFile(const char* filename);																//���ļ����ȡDNA����
	void ReadMaterialFromFile(const char* filename);															//���ļ��ж�ȡ����
	void RunMaterial(int mat);																					//ִ�в���

	void RotateAdd(float x,float y,float z);																	//��ת�Ƕ�����
	void SetRotateXZ(float x,float z);																			//������ת�Ƕ�
	void SetBasePosition(float x,float y,float z);																//����λ��
	void SetEnable(bool m);																						//�����Ƿ���Ч
//	void SetTimePerLoop(unsigned long t);
	void Move(float x,float y,float z);																			//�ƶ�����


	void InitMaterial();



};

#endif