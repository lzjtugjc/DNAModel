//By MatrixC

#ifndef __DNACLASS__
#define __DNACLASS__


#define _AT 1
#define _TA 2
#define _CG 3
#define _GC 4

#define __Long__ 1			//长碱基
#define __Short__ 2			//短碱基

#define __POINTSTEP__	4


//各个单位的材质序号
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

//颜色结构
struct ColorRGBA
{
	float r;
	float g;
	float b;
	float a;
};

//材质结构
struct CMaterialInformation
{
#define __MaterNameLong__	100

	char m_HKey[__MaterNameLong__];
	char m_A[__MaterNameLong__];
	char m_T[__MaterNameLong__];
	char m_G[__MaterNameLong__];
	char m_C[__MaterNameLong__];
	char m_P[__MaterNameLong__];						//磷酸基团
	char m_PPP[__MaterNameLong__];						//五碳糖

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

//材质
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


//顶点信息
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
	CVertex m_Point1[2];	//磷点
	CVertex m_Point2[2];	//碱基点


	int m_Type;				//碱基类型 (1:A T 2:T A 3 C G 4:GC)
	float m_Degree;			//与Z轴所成的夹角。
};


class DNAClass
{
private:

	vector<CHammer> m_Hammer;				//所有的点
	vector<CHammer> m_UseHammer;			//磷连的点

	CVertex m_Rotate;						//旋转控制
	CVertex m_BasePosition;					//初始位置

	vector<int> m_DNAList;					//DNA核苷酸列表
	vector<CMaterial> m_Material;			//材质
	//map<int,CMaterial> m_Material;
	//map<int,int> aaa;

	bool m_UsingMaterial;					//是否使用材质
	GLuint m_RenderList;					//渲染列表指针
	CMaterialInformation m_MatInfo;			//材质名称
	CMatInfo m_MaterialInfo;

	int m_numOfAT;							//AT总数
	int m_numOfCG;							//CG总数

	bool m_Enable;							//是否可用
	float m_TimePerLoop;


public:
	DNAClass(const char* filename);
	void Init(const char* filename);
	void ComputePoint(int numOfPointPerCircle,float numOfCircle,float Rad,float NRad,float longOfDNA);			//计算图形中所用的点的坐标
	void ComputeUsePoint(int k);																				//计算将要使用的点
	void RenderSphere(GLfloat rad);																				//渲染磷酸基团
	void RenderCube(float slong,int type);																		//渲染碱基
	void RenderAlkali();																						//渲染核苷酸
	void RenderLine();																							//磷酸基团之间的线，用圆柱代替！！！！
	void RenderHKey();																							//显示氢键
	void RenderFC();																							//渲染五碳糖
	void CompileRenderList();																					//建立显示列表，提高渲染速度

	CVertex GetPosition();																						//获取位置
	CVertex GetRotate();																						//获取角度
	bool	GetEnable();																						//获取是否有效
	


	void RenderAll();																							//渲染所有
	void SetLight();																							//设置灯光

	int ReadDNAListFromFile(const char* filename);																//从文件里读取DNA序列
	void ReadMaterialFromFile(const char* filename);															//从文件中读取材质
	void RunMaterial(int mat);																					//执行材质

	void RotateAdd(float x,float y,float z);																	//旋转角度增加
	void SetRotateXZ(float x,float z);																			//设置旋转角度
	void SetBasePosition(float x,float y,float z);																//设置位置
	void SetEnable(bool m);																						//设置是否有效
//	void SetTimePerLoop(unsigned long t);
	void Move(float x,float y,float z);																			//移动函数


	void InitMaterial();



};

#endif