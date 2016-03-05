//By MatrixC

#include "DNAClass.h"
#define PI 3.14159265354

void DNAClass::ComputePoint(int numOfPointPerCircle,float numOfCircle,float Rad,float NRad,float longOfDNA)
{
	
	//计算螺旋方程
	double t_stepOfPoint;
	double t_stepOfY;
	
	//根据参数给出的数据算出螺旋方程计算所用参数
	t_stepOfPoint=2.0 * PI / numOfPointPerCircle;
	t_stepOfY=2.0 / numOfPointPerCircle * longOfDNA;

	for(int i=0;i<(int)(numOfCircle*numOfPointPerCircle);i++)
	{
		CHammer t_Hammer;
		CVertex t_Vertex;

		float t_Radio;
		float t_Degree;

		float t_Rad = t_stepOfPoint * i;
		
		float x = Rad * cos(t_Rad);
		float z = Rad * sin(t_Rad);
		float y = t_stepOfY*i;

		//所处碱基旋转角度
		t_Radio = atan(z/x);
		t_Degree = t_Radio / PI *180;

		if(x==0 && z>0)
			t_Degree = 90;
		if(x==0 && z<0)
			t_Degree = 270;
		if(z==0 && x>0)
			t_Degree = 0;
		if(z==0 && x<0)
			t_Degree = 180;

		if(x<0 && z>0)												//第二象限
			t_Degree = 180 + t_Degree;
		if(x<0 && z<0)
			t_Degree = 180 + t_Degree;
		if(x>0 && z<0)
			t_Degree = 360 + t_Degree;

		t_Hammer.m_Degree = t_Degree;

		t_Vertex.x = x;
		t_Vertex.z = z *-1;
		t_Vertex.y = y - ((numOfCircle*numOfPointPerCircle-1) * t_stepOfY) / 2 ; //将螺旋的中点定位坐标系中点

		t_Hammer.m_Point1[0] = t_Vertex;

		t_Vertex.x = t_Vertex.x * -1;
		t_Vertex.z = t_Vertex.z * -1;

		t_Hammer.m_Point1[1] = t_Vertex;


		//算出第二个螺旋的各点的坐标
		x = NRad * cos(t_Rad);
		z = NRad * sin(t_Rad);

		t_Vertex.x = x;
		t_Vertex.z = z*-1;

		t_Hammer.m_Point2[0] = t_Vertex;

		t_Vertex.x = t_Vertex.x * -1;
		t_Vertex.z = t_Vertex.z * -1;

		t_Hammer.m_Point2[1] = t_Vertex;

		//t_Hammer.m_Type=_AT;

		m_Hammer.push_back(t_Hammer);

				
	}
}

void DNAClass::ComputeUsePoint(int k)
{
	//计算磷酸基团所使用的点的坐标
	unsigned long t_t = 0;

	CHammer t_Hammer;

	//创建所有点的迭代器
	vector<CHammer>::iterator t_iter;
	vector<int>::iterator i_iter;
	t_iter = m_Hammer.begin();
	i_iter = m_DNAList.begin();

	while(t_iter != m_Hammer.end())
//	while(i_iter != m_DNAList.end())
	{
		if(t_t == 0)
		{
			//t_iter->m_Type = 
			/*
			此处加入从文件中读取的DNA序列
			*/
			t_Hammer = *t_iter;
			//t_Hammer.m_Type = (*i_iter);
			m_UseHammer.push_back(t_Hammer);
		//	t_iter++;
		}

		t_t ++;
		t_iter ++;
		if(t_t == k)
			t_t = 0;
	}


}



void DNAClass::RenderSphere(GLfloat rad)
{
	//渲染磷酸基团

	//给出材质
	RunMaterial(__PMATERIAL__);

	//RunMaterial(t_mat1);

	for(vector<CHammer>::iterator iter = m_UseHammer.begin();iter != m_UseHammer.end();iter++)
	{
		//渲染左边的磷酸
		glPushMatrix();
		//给定位置
		glTranslatef(iter->m_Point1[0].x,iter->m_Point1[0].y,iter->m_Point1[0].z);
		//glColor3f(0.5,0,0.5);
		glutSolidSphere(rad,15,15);
		glPopMatrix();

		//渲染右边的磷酸
		glPushMatrix();
		glTranslatef(iter->m_Point1[1].x,iter->m_Point1[1].y,iter->m_Point1[1].z);
		//glColor3f(0.5,0,0.5);
		glutSolidSphere(rad,15,15);
		glPopMatrix();

	}
}

void DNAClass::RenderAlkali()
{
	//渲染含氮碱基
	CHammer t_Hammer;
	for(vector<CHammer>::iterator iter = m_UseHammer.begin();iter != m_UseHammer.end();iter++)
	{
		//得到碱基的坐标
		t_Hammer = *iter;
		float t_distance = sqrt((t_Hammer.m_Point2[0].x - t_Hammer.m_Point2[1].x) * (t_Hammer.m_Point2[0].x - t_Hammer.m_Point2[1].x) + (t_Hammer.m_Point2[0].z - t_Hammer.m_Point2[1].z) * (t_Hammer.m_Point2[0].z - t_Hammer.m_Point2[1].z));
		glPushMatrix();
		float x = (t_Hammer.m_Point2[0].x + t_Hammer.m_Point2[1].x)/2;
		float y = t_Hammer.m_Point2[0].y;
		float z = (t_Hammer.m_Point2[0].z + t_Hammer.m_Point2[1].z)/2;
		//移动碱基
		glTranslatef(x,y,z);
		glRotatef(t_Hammer.m_Degree,0,1,0);

		//渲染A -- T的碱基
		if(t_Hammer.m_Type==_AT)
		{

			RunMaterial(__AMATERIAL__);
			glPushMatrix();
			glTranslatef(0.1875f*t_distance*-1,0,0);
			//glColor3f(0,0,1);							///A蓝色 T 红色 C黄色 G绿色
			RenderCube(t_distance,__Long__);
			glPopMatrix();
			

			RunMaterial(__TMATERIAL__);
			glPushMatrix();
			glTranslatef(0.375*t_distance,0,0);
			RenderCube(t_distance,__Short__);
			glPopMatrix();
			
		
		}

		//渲染T -- A的碱基
		if(t_Hammer.m_Type==_TA)
		{
			//给出材质和位置
			RunMaterial(__TMATERIAL__);
			glPushMatrix();
			glTranslatef(0.375*t_distance*-1,0,0);
			RenderCube(t_distance,__Short__);
			glPopMatrix();

			RunMaterial(__AMATERIAL__);
			glPushMatrix();
			//glTranslatef(3/16*t_distance*-1,0,0);
			glTranslatef(0.1875f*t_distance,0,0);
			//glColor3f(0,0,1);
//			RunMaterial(m_MaterialInfo.m_A);
			RenderCube(t_distance,__Long__);
			glPopMatrix();

		}

		//渲染C -- G的碱基
		if(t_Hammer.m_Type==_CG)
		{
			//给出材质和位置
			RunMaterial(__CMATERIAL__);
			glPushMatrix();
			//glTranslatef(3/16*t_distance*-1,0,0);
			glTranslatef(0.1875f*t_distance*-1,0,0);

			RenderCube(t_distance,__Long__);
			glPopMatrix();

			RunMaterial(__GMATERIAL__);
			glPushMatrix();
			glTranslatef(0.375*t_distance,0,0);
			//glColor3f(0,1,0);
//			RunMaterial(m_MaterialInfo.m_G);
			RenderCube(t_distance,__Short__);
			glPopMatrix();

		}

		//渲染G -- C的碱基
		if(t_Hammer.m_Type==_GC)
		{
			//给出材质和位置
			RunMaterial(__GMATERIAL__);
			glPushMatrix();
			glTranslatef(0.375*t_distance*-1,0,0);
			//glColor3f(1,0,0);
//			RunMaterial(m_MaterialInfo.m_G);
			RenderCube(t_distance,__Short__);
			glPopMatrix();

			RunMaterial(__CMATERIAL__);
			glPushMatrix();
			//glTranslatef(3/16*t_distance*-1,0,0);
			glTranslatef(0.1875f*t_distance,0,0);
			//glColor3f(0,0,1);
//			RunMaterial(m_MaterialInfo.m_C);
			RenderCube(t_distance,__Long__);
			glPopMatrix();

		}


		glPopMatrix();
		

	}
}

void DNAClass::RenderCube(float slong,int type)
{
	//渲染不同的方块，用与渲染含氮碱基
	//RunMaterical(0);
	//长方块
	if(type==__Long__)
	{
		//glScalef(5/8*slong,0.1,0.1);
		glScalef(0.625*slong,0.2,0.2);
		glutSolidCube(1);
	}

	//短方块
	if(type==__Short__)
	{
		glScalef(slong*0.25,0.2,0.2);
		glutSolidCube(1);
	}

}

void DNAClass::RenderHKey()
{
	//渲染氢键
	for(vector<CHammer>::iterator iter = m_UseHammer.begin();iter!=m_UseHammer.end();iter++)
	{
		float x = (iter->m_Point1[0].x + iter->m_Point1[1].x) * 0.5;
		float y = iter->m_Point1[0].y;
		float z = (iter->m_Point1[0].z + iter->m_Point1[1].z) * 0.5;

		//给出材质

		RunMaterial(__HKEYMATERIAL__);


		//渲染氢氢三建
		if(iter->m_Type == _GC || iter->m_Type == _CG)
		{
			//渲染第一个键
			glPushMatrix();
			glTranslatef(x,y,z);
			glRotatef(iter->m_Degree,0,1,0);
			glScalef(1.5,0.05,0.05);
			glutSolidCube(1);
			glPopMatrix();

			//渲染第二个键
			glPushMatrix();
			glTranslatef(x,y+0.07,z);
			glRotatef(iter->m_Degree,0,1,0);
			glScalef(1.5,0.05,0.05);
			glutSolidCube(1);
			glPopMatrix();

			//渲染第三个键
			glPushMatrix();
			glTranslatef(x,y-0.07,z);
			glRotatef(iter->m_Degree,0,1,0);
			glScalef(1.5,0.05,0.05);
			glutSolidCube(1);
			glPopMatrix();
		}
		else
			//渲染氢氢双建
		{
			//渲染第一个键
			glPushMatrix();
			glTranslatef(x,y+0.07,z);
			glRotatef(iter->m_Degree,0,1,0);
			glScalef(1.5,0.05,0.05);
			glutSolidCube(1);
			glPopMatrix();

			//渲染第二个键
			glPushMatrix();
			glTranslatef(x,y-0.07,z);
			glRotatef(iter->m_Degree,0,1,0);
			glScalef(1.5,0.05,0.05);
			glutSolidCube(1);
			glPopMatrix();

		}

	}

}

void DNAClass::RenderLine()
{
	//渲染磷酸基团之间的连线
	vector<CHammer>::iterator iter,t_iter;
	glLineWidth(1);
	glColor3f(0,0,1);
	glBegin(GL_LINES);
	//开始画线
		for(iter = m_UseHammer.begin();iter!=m_UseHammer.end();iter++)
		{
			glVertex3f(iter->m_Point1[0].x,iter->m_Point1[0].y,iter->m_Point1[0].z);
			if(iter!=m_UseHammer.begin())
			{
				t_iter = iter - 1;
				glVertex3f(t_iter->m_Point1[0].x,t_iter->m_Point1[0].y,t_iter->m_Point1[0].z);

			}
			
		}
	glEnd();
	
	//开始画第二条线
	glBegin(GL_LINES);
		for(iter = m_UseHammer.begin();iter!=m_UseHammer.end();iter++)
		{
			glVertex3f(iter->m_Point1[1].x,iter->m_Point1[1].y,iter->m_Point1[1].z);
			if(iter!=m_UseHammer.begin())
			{
				t_iter = iter - 1;
				glVertex3f(t_iter->m_Point1[1].x,t_iter->m_Point1[1].y,t_iter->m_Point1[1].z);
			}
		}
	glEnd();
		
	

	
}

void DNAClass::RenderFC()
{
	//渲染五碳糖
	float t_FCoffert = 0.1;
	CVertex t_pos1,t_pos2;
	for(vector<CHammer>::iterator iter = m_UseHammer.begin();iter != m_UseHammer.end();iter++)
	{
		//找出中点坐标
		t_pos1.x = (iter->m_Point1[0].x + iter->m_Point2[0].x) * 0.5;
		t_pos1.y = iter->m_Point1[0].y;
		t_pos1.z = (iter->m_Point1[0].z + iter->m_Point2[0].z) * 0.5;

		//算出与之对应的五碳糖的坐标
		t_pos2.x = t_pos1.x * -1;
		t_pos2.y = t_pos1.y;
		t_pos2.z = t_pos1.z * -1;

		glPushMatrix();

		//渲染第一个五碳糖
		RunMaterial(__FCMATERIAL__);
		glTranslatef(t_pos1.x ,t_pos1.y,t_pos1.z);
		glRotatef(iter->m_Degree,0,1,0);
		
		glPushMatrix();
		glTranslatef(t_FCoffert * -1,0,0);
		//glScalef(0.15,0.15,0.15);
		glScalef(0.22,0.22,0.22);
		glutSolidOctahedron();
		glPopMatrix();

		//渲染之间的键
		glPushMatrix();
		RunMaterial(__SPCMATERIAL__);
		//glTranslatef(t_FCoffert * -1,0,0);
		glScalef(0.6,0.05,0.05);
		glutSolidCube(1);
		glPopMatrix();

		glPopMatrix();


		//渲染第二个五碳糖
		glPushMatrix();

		RunMaterial(__FCMATERIAL__);
		glTranslatef(t_pos2.x,t_pos2.y,t_pos2.z);
		glRotatef(iter->m_Degree,0,1,0);

		glPushMatrix();
		glTranslatef(t_FCoffert,0,0);
		//glScalef(0.15,0.15,0.15);
		glScalef(0.22,0.22,0.22);
		glutSolidOctahedron();
		glPopMatrix();

		//渲染之间的键
		glPushMatrix();
		RunMaterial(__SPCMATERIAL__);
		//glTranslatef(t_FCoffert,0,0);
		glScalef(0.6,0.05,0.05);
		glutSolidCube(1);
		glPopMatrix();

		glPopMatrix();

	}
}

void DNAClass::RenderAll()
{

	//渲染DNA内所有的成员
	glPushMatrix();
	//glTranslatef(0.0f, -4, -15.0f);
	glTranslatef(m_BasePosition.x ,m_BasePosition.y,m_BasePosition.z);


	static float t_RotateY = 0;		//Y轴自动旋转角度

	//对其成员进行旋转
	glRotatef(m_Rotate.x/* * m_TimePerLoop*/,1,0,0);
	glRotatef(m_Rotate.z/* * m_TimePerLoop*/,0,0,1);
	glRotatef(t_RotateY/* * m_TimePerLoop*/,0,1,0);

	//Y轴自动旋转
	t_RotateY += m_Rotate.y;

	if(t_RotateY>=3600 || t_RotateY<=-3600)
		t_RotateY=0;


	//开启深度检测
	glEnable(GL_DEPTH_TEST);

	//执行显示列表
	glCallList(m_RenderList);

	glDisable(GL_DEPTH_TEST);
	glPopMatrix();


}

void DNAClass::CompileRenderList()
{
	//生成显示列表
	m_RenderList = glGenLists(1);
	glNewList(m_RenderList,GL_COMPILE);

	//渲染各个成员
	//RenderSphere(0.2);
	RenderSphere(0.15);
	RenderAlkali();
	//RenderLine();
	RenderHKey();
	RenderFC();
	glEndList();

	
}

void DNAClass::RunMaterial(int mat)
{

	//执行材质
	GLfloat t_mat_ambient[] = {0.5,0.5,0.5,1};
	GLfloat t_mat_diffuse[] = {1,1,1,1};

	switch(mat)
	{
		//设置A的材质
		case __AMATERIAL__ :
		//	t_mat_diffuse[] = { 0.8,0,0,1 };
			t_mat_diffuse[0] = 1;
			t_mat_diffuse[1] = 0;
			t_mat_diffuse[2] = 0;
			t_mat_diffuse[3] = 1;
			glMaterialfv(GL_FRONT,GL_AMBIENT,t_mat_ambient);
			glMaterialfv(GL_FRONT,GL_DIFFUSE,t_mat_diffuse);
			break;
		case __TMATERIAL__ :
			//设置T的材质
			t_mat_diffuse[0] = 0;
			t_mat_diffuse[1] = 0;
			t_mat_diffuse[2] = 1;
			t_mat_diffuse[3] = 1;
			glMaterialfv(GL_FRONT,GL_AMBIENT,t_mat_ambient);
			glMaterialfv(GL_FRONT,GL_DIFFUSE,t_mat_diffuse);
			break;
		case __CMATERIAL__ :
			//设置C的材质
			t_mat_diffuse[0] = 0;
			t_mat_diffuse[1] = 1;
			t_mat_diffuse[2] = 0;
			t_mat_diffuse[3] = 1;
			glMaterialfv(GL_FRONT,GL_AMBIENT,t_mat_ambient);
			glMaterialfv(GL_FRONT,GL_DIFFUSE,t_mat_diffuse);
			break;
		case __GMATERIAL__ :
			//设置G的材质
			t_mat_diffuse[0] = 1;
			t_mat_diffuse[1] = 1;
			t_mat_diffuse[2] = 0;
			t_mat_diffuse[3] = 1;
			glMaterialfv(GL_FRONT,GL_AMBIENT,t_mat_ambient);
			glMaterialfv(GL_FRONT,GL_DIFFUSE,t_mat_diffuse);
			break;
		case __PMATERIAL__ :
			//设置磷酸基团的材质
			t_mat_diffuse[0] = 1;
			t_mat_diffuse[1] = 0;
			t_mat_diffuse[2] = 1;
			t_mat_diffuse[3] = 1;
			glMaterialfv(GL_FRONT,GL_AMBIENT,t_mat_ambient);
			glMaterialfv(GL_FRONT,GL_DIFFUSE,t_mat_diffuse);
			break;
		case __FCMATERIAL__ :
			//设置五碳糖的材质
			t_mat_diffuse[0] = 1;
			t_mat_diffuse[1] = 1;
			t_mat_diffuse[2] = 1;
			t_mat_diffuse[3] = 1;
			glMaterialfv(GL_FRONT,GL_AMBIENT,t_mat_ambient);
			glMaterialfv(GL_FRONT,GL_DIFFUSE,t_mat_diffuse);
			break;
		case __HKEYMATERIAL__ :
			//设置氢键的材质
			t_mat_diffuse[0] = 1;
			t_mat_diffuse[1] = 1;
			t_mat_diffuse[2] = 0;
			t_mat_diffuse[3] = 1;
			glMaterialfv(GL_FRONT,GL_AMBIENT,t_mat_ambient);
			glMaterialfv(GL_FRONT,GL_DIFFUSE,t_mat_diffuse);
			break;
		case __SPCMATERIAL__ :
			//设置特殊键的材质
			t_mat_diffuse[0] = 0;
			t_mat_diffuse[1] = 1;
			t_mat_diffuse[2] = 1;
			t_mat_diffuse[3] = 1;
			glMaterialfv(GL_FRONT,GL_AMBIENT,t_mat_ambient);
			glMaterialfv(GL_FRONT,GL_DIFFUSE,t_mat_diffuse);
		
	}

}


void DNAClass::Init(const char* filename)
{
	//设置初始位置
	SetBasePosition(0,0,-15);

	//设置Y轴初始自动旋转角度
	RotateAdd(0,0.1,0);


	//开启阴影平滑
	glShadeModel(GL_SMOOTH);
	//设置灯光
	SetLight();

	//从文件中读取DNA序列
	int numOfDNA = ReadDNAListFromFile(filename);


	//计算所有点
	ComputePoint(100,numOfDNA * 0.04,1.8,1.2,5);
	//计算使用的点
	ComputeUsePoint(__POINTSTEP__);


	for(int i=0;i<numOfDNA;i++)
	{
		m_UseHammer[i].m_Type = m_DNAList[i];
	}

	//计算渲染列表

	CompileRenderList();


	//设置其为有效
	m_Enable = true;

}

void DNAClass::SetLight()
{
	//设置灯光

	//灯光漫反射
	GLfloat light_diffuse[]= { 1.0, 1.0, 1.0, 1.0};
	//灯光位置
	GLfloat light_position[] = { 0, 3, 2.0, 0.0 };
	//灯光镜面反射
	GLfloat light_specular[] = { 0.8, 0.8,0.8, 1.0 };
	//灯光环境光
	GLfloat light_ambient[] = {0.5, 0.5, 0.5,1};

	//将值设置到LIGHT_0上
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular); 
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);    //如果什么都不设置，GL_LIGHT有默认的值。
	glCullFace(GL_BACK);
	
	
}

void DNAClass::SetBasePosition(float x,float y,float z)
{
	//设置DNA位置函数
	m_BasePosition.x = x;
	m_BasePosition.y = y;
	m_BasePosition.z = z;
}

void DNAClass::RotateAdd(float x,float y,float z)
{
	//设置DNA旋转角度函数
	m_Rotate.x += x;
	m_Rotate.z += z;
	m_Rotate.y += y;
}

void DNAClass::SetRotateXZ(float x,float z)
{
	//设置DNA角度
	m_Rotate.x = x;
	m_Rotate.z = z;
}

void DNAClass::Move(float x,float y,float z)
{
	//移动DNA
	m_BasePosition.x += x;
	m_BasePosition.y += y;
	m_BasePosition.z += z;
}

void DNAClass::SetEnable(bool m)
{
	//设置是否启用
	m_Enable = m;
}

int DNAClass::ReadDNAListFromFile(const char* filename)
{
	//从文件中读取DNA序列

	//文件句柄
	FILE *fp;

	//初始化DNA数
	int t_numOfDNA = 0;

	char t_filepath[256+1];
	memset(t_filepath,0,256);

	strcat(t_filepath,g_path);
	strcat(t_filepath,"RDNA.dat");

	fp = fopen(t_filepath,"r");
	
	if(!fp)
	{
		MessageBox(NULL,"DNA文件无法打开!","打开错误",MB_OK);
		exit(0);
	}
	


	char t_line[10];
	int t_DNA;
	while(!feof(fp))
	{
		//读取
		fgets(t_line,10,fp);
		t_DNA = atoi(t_line);
		m_DNAList.push_back(t_DNA);
		t_numOfDNA ++;
	}

	fclose(fp);
	t_numOfDNA --;

	return t_numOfDNA;

}

void DNAClass::ReadMaterialFromFile(const char* filename)
{
	//从文件中读取DNA材质
	FILE *fp;
	fp = fopen(filename,"rb");

	if(!fp)
	{
		MessageBox(NULL,"材质文件无法打开!","打开错误",MB_OK);
		exit(0);
	}

	int t_Count = 0;									//材质数
	fread(&t_Count,sizeof(int),1,fp);					//读取材质个数

	CMaterial* t_Material;
	t_Material = new CMaterial[t_Count];

	m_Material.reserve(t_Count);

	fread(t_Material,sizeof(CMaterial),t_Count,fp);

	for(int i=0;i<t_Count;i++)
	{
		m_Material.push_back(t_Material[i]);
	}
	fclose(fp);


}

//返回位置

CVertex DNAClass::GetPosition()
{
	return m_BasePosition;
}

//返回角度
CVertex DNAClass::GetRotate()
{
	return m_Rotate;
}

//返回是否有效
bool DNAClass::GetEnable()
{
	return m_Enable;
}

//DNA构造函数
DNAClass::DNAClass(const char* filename)
{
	char t_name[256];
	memset(t_name,0,256);
	strcat(t_name,filename);

	Init(t_name);
}


