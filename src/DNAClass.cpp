//By MatrixC

#include "DNAClass.h"
#define PI 3.14159265354

void DNAClass::ComputePoint(int numOfPointPerCircle,float numOfCircle,float Rad,float NRad,float longOfDNA)
{
	
	//������������
	double t_stepOfPoint;
	double t_stepOfY;
	
	//���ݲ�����������������������̼������ò���
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

		//���������ת�Ƕ�
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

		if(x<0 && z>0)												//�ڶ�����
			t_Degree = 180 + t_Degree;
		if(x<0 && z<0)
			t_Degree = 180 + t_Degree;
		if(x>0 && z<0)
			t_Degree = 360 + t_Degree;

		t_Hammer.m_Degree = t_Degree;

		t_Vertex.x = x;
		t_Vertex.z = z *-1;
		t_Vertex.y = y - ((numOfCircle*numOfPointPerCircle-1) * t_stepOfY) / 2 ; //���������е㶨λ����ϵ�е�

		t_Hammer.m_Point1[0] = t_Vertex;

		t_Vertex.x = t_Vertex.x * -1;
		t_Vertex.z = t_Vertex.z * -1;

		t_Hammer.m_Point1[1] = t_Vertex;


		//����ڶ��������ĸ��������
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
	//�������������ʹ�õĵ������
	unsigned long t_t = 0;

	CHammer t_Hammer;

	//�������е�ĵ�����
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
			�˴�������ļ��ж�ȡ��DNA����
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
	//��Ⱦ�������

	//��������
	RunMaterial(__PMATERIAL__);

	//RunMaterial(t_mat1);

	for(vector<CHammer>::iterator iter = m_UseHammer.begin();iter != m_UseHammer.end();iter++)
	{
		//��Ⱦ��ߵ�����
		glPushMatrix();
		//����λ��
		glTranslatef(iter->m_Point1[0].x,iter->m_Point1[0].y,iter->m_Point1[0].z);
		//glColor3f(0.5,0,0.5);
		glutSolidSphere(rad,15,15);
		glPopMatrix();

		//��Ⱦ�ұߵ�����
		glPushMatrix();
		glTranslatef(iter->m_Point1[1].x,iter->m_Point1[1].y,iter->m_Point1[1].z);
		//glColor3f(0.5,0,0.5);
		glutSolidSphere(rad,15,15);
		glPopMatrix();

	}
}

void DNAClass::RenderAlkali()
{
	//��Ⱦ�������
	CHammer t_Hammer;
	for(vector<CHammer>::iterator iter = m_UseHammer.begin();iter != m_UseHammer.end();iter++)
	{
		//�õ����������
		t_Hammer = *iter;
		float t_distance = sqrt((t_Hammer.m_Point2[0].x - t_Hammer.m_Point2[1].x) * (t_Hammer.m_Point2[0].x - t_Hammer.m_Point2[1].x) + (t_Hammer.m_Point2[0].z - t_Hammer.m_Point2[1].z) * (t_Hammer.m_Point2[0].z - t_Hammer.m_Point2[1].z));
		glPushMatrix();
		float x = (t_Hammer.m_Point2[0].x + t_Hammer.m_Point2[1].x)/2;
		float y = t_Hammer.m_Point2[0].y;
		float z = (t_Hammer.m_Point2[0].z + t_Hammer.m_Point2[1].z)/2;
		//�ƶ����
		glTranslatef(x,y,z);
		glRotatef(t_Hammer.m_Degree,0,1,0);

		//��ȾA -- T�ļ��
		if(t_Hammer.m_Type==_AT)
		{

			RunMaterial(__AMATERIAL__);
			glPushMatrix();
			glTranslatef(0.1875f*t_distance*-1,0,0);
			//glColor3f(0,0,1);							///A��ɫ T ��ɫ C��ɫ G��ɫ
			RenderCube(t_distance,__Long__);
			glPopMatrix();
			

			RunMaterial(__TMATERIAL__);
			glPushMatrix();
			glTranslatef(0.375*t_distance,0,0);
			RenderCube(t_distance,__Short__);
			glPopMatrix();
			
		
		}

		//��ȾT -- A�ļ��
		if(t_Hammer.m_Type==_TA)
		{
			//�������ʺ�λ��
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

		//��ȾC -- G�ļ��
		if(t_Hammer.m_Type==_CG)
		{
			//�������ʺ�λ��
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

		//��ȾG -- C�ļ��
		if(t_Hammer.m_Type==_GC)
		{
			//�������ʺ�λ��
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
	//��Ⱦ��ͬ�ķ��飬������Ⱦ�������
	//RunMaterical(0);
	//������
	if(type==__Long__)
	{
		//glScalef(5/8*slong,0.1,0.1);
		glScalef(0.625*slong,0.2,0.2);
		glutSolidCube(1);
	}

	//�̷���
	if(type==__Short__)
	{
		glScalef(slong*0.25,0.2,0.2);
		glutSolidCube(1);
	}

}

void DNAClass::RenderHKey()
{
	//��Ⱦ���
	for(vector<CHammer>::iterator iter = m_UseHammer.begin();iter!=m_UseHammer.end();iter++)
	{
		float x = (iter->m_Point1[0].x + iter->m_Point1[1].x) * 0.5;
		float y = iter->m_Point1[0].y;
		float z = (iter->m_Point1[0].z + iter->m_Point1[1].z) * 0.5;

		//��������

		RunMaterial(__HKEYMATERIAL__);


		//��Ⱦ��������
		if(iter->m_Type == _GC || iter->m_Type == _CG)
		{
			//��Ⱦ��һ����
			glPushMatrix();
			glTranslatef(x,y,z);
			glRotatef(iter->m_Degree,0,1,0);
			glScalef(1.5,0.05,0.05);
			glutSolidCube(1);
			glPopMatrix();

			//��Ⱦ�ڶ�����
			glPushMatrix();
			glTranslatef(x,y+0.07,z);
			glRotatef(iter->m_Degree,0,1,0);
			glScalef(1.5,0.05,0.05);
			glutSolidCube(1);
			glPopMatrix();

			//��Ⱦ��������
			glPushMatrix();
			glTranslatef(x,y-0.07,z);
			glRotatef(iter->m_Degree,0,1,0);
			glScalef(1.5,0.05,0.05);
			glutSolidCube(1);
			glPopMatrix();
		}
		else
			//��Ⱦ����˫��
		{
			//��Ⱦ��һ����
			glPushMatrix();
			glTranslatef(x,y+0.07,z);
			glRotatef(iter->m_Degree,0,1,0);
			glScalef(1.5,0.05,0.05);
			glutSolidCube(1);
			glPopMatrix();

			//��Ⱦ�ڶ�����
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
	//��Ⱦ�������֮�������
	vector<CHammer>::iterator iter,t_iter;
	glLineWidth(1);
	glColor3f(0,0,1);
	glBegin(GL_LINES);
	//��ʼ����
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
	
	//��ʼ���ڶ�����
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
	//��Ⱦ��̼��
	float t_FCoffert = 0.1;
	CVertex t_pos1,t_pos2;
	for(vector<CHammer>::iterator iter = m_UseHammer.begin();iter != m_UseHammer.end();iter++)
	{
		//�ҳ��е�����
		t_pos1.x = (iter->m_Point1[0].x + iter->m_Point2[0].x) * 0.5;
		t_pos1.y = iter->m_Point1[0].y;
		t_pos1.z = (iter->m_Point1[0].z + iter->m_Point2[0].z) * 0.5;

		//�����֮��Ӧ����̼�ǵ�����
		t_pos2.x = t_pos1.x * -1;
		t_pos2.y = t_pos1.y;
		t_pos2.z = t_pos1.z * -1;

		glPushMatrix();

		//��Ⱦ��һ����̼��
		RunMaterial(__FCMATERIAL__);
		glTranslatef(t_pos1.x ,t_pos1.y,t_pos1.z);
		glRotatef(iter->m_Degree,0,1,0);
		
		glPushMatrix();
		glTranslatef(t_FCoffert * -1,0,0);
		//glScalef(0.15,0.15,0.15);
		glScalef(0.22,0.22,0.22);
		glutSolidOctahedron();
		glPopMatrix();

		//��Ⱦ֮��ļ�
		glPushMatrix();
		RunMaterial(__SPCMATERIAL__);
		//glTranslatef(t_FCoffert * -1,0,0);
		glScalef(0.6,0.05,0.05);
		glutSolidCube(1);
		glPopMatrix();

		glPopMatrix();


		//��Ⱦ�ڶ�����̼��
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

		//��Ⱦ֮��ļ�
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

	//��ȾDNA�����еĳ�Ա
	glPushMatrix();
	//glTranslatef(0.0f, -4, -15.0f);
	glTranslatef(m_BasePosition.x ,m_BasePosition.y,m_BasePosition.z);


	static float t_RotateY = 0;		//Y���Զ���ת�Ƕ�

	//�����Ա������ת
	glRotatef(m_Rotate.x/* * m_TimePerLoop*/,1,0,0);
	glRotatef(m_Rotate.z/* * m_TimePerLoop*/,0,0,1);
	glRotatef(t_RotateY/* * m_TimePerLoop*/,0,1,0);

	//Y���Զ���ת
	t_RotateY += m_Rotate.y;

	if(t_RotateY>=3600 || t_RotateY<=-3600)
		t_RotateY=0;


	//������ȼ��
	glEnable(GL_DEPTH_TEST);

	//ִ����ʾ�б�
	glCallList(m_RenderList);

	glDisable(GL_DEPTH_TEST);
	glPopMatrix();


}

void DNAClass::CompileRenderList()
{
	//������ʾ�б�
	m_RenderList = glGenLists(1);
	glNewList(m_RenderList,GL_COMPILE);

	//��Ⱦ������Ա
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

	//ִ�в���
	GLfloat t_mat_ambient[] = {0.5,0.5,0.5,1};
	GLfloat t_mat_diffuse[] = {1,1,1,1};

	switch(mat)
	{
		//����A�Ĳ���
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
			//����T�Ĳ���
			t_mat_diffuse[0] = 0;
			t_mat_diffuse[1] = 0;
			t_mat_diffuse[2] = 1;
			t_mat_diffuse[3] = 1;
			glMaterialfv(GL_FRONT,GL_AMBIENT,t_mat_ambient);
			glMaterialfv(GL_FRONT,GL_DIFFUSE,t_mat_diffuse);
			break;
		case __CMATERIAL__ :
			//����C�Ĳ���
			t_mat_diffuse[0] = 0;
			t_mat_diffuse[1] = 1;
			t_mat_diffuse[2] = 0;
			t_mat_diffuse[3] = 1;
			glMaterialfv(GL_FRONT,GL_AMBIENT,t_mat_ambient);
			glMaterialfv(GL_FRONT,GL_DIFFUSE,t_mat_diffuse);
			break;
		case __GMATERIAL__ :
			//����G�Ĳ���
			t_mat_diffuse[0] = 1;
			t_mat_diffuse[1] = 1;
			t_mat_diffuse[2] = 0;
			t_mat_diffuse[3] = 1;
			glMaterialfv(GL_FRONT,GL_AMBIENT,t_mat_ambient);
			glMaterialfv(GL_FRONT,GL_DIFFUSE,t_mat_diffuse);
			break;
		case __PMATERIAL__ :
			//����������ŵĲ���
			t_mat_diffuse[0] = 1;
			t_mat_diffuse[1] = 0;
			t_mat_diffuse[2] = 1;
			t_mat_diffuse[3] = 1;
			glMaterialfv(GL_FRONT,GL_AMBIENT,t_mat_ambient);
			glMaterialfv(GL_FRONT,GL_DIFFUSE,t_mat_diffuse);
			break;
		case __FCMATERIAL__ :
			//������̼�ǵĲ���
			t_mat_diffuse[0] = 1;
			t_mat_diffuse[1] = 1;
			t_mat_diffuse[2] = 1;
			t_mat_diffuse[3] = 1;
			glMaterialfv(GL_FRONT,GL_AMBIENT,t_mat_ambient);
			glMaterialfv(GL_FRONT,GL_DIFFUSE,t_mat_diffuse);
			break;
		case __HKEYMATERIAL__ :
			//��������Ĳ���
			t_mat_diffuse[0] = 1;
			t_mat_diffuse[1] = 1;
			t_mat_diffuse[2] = 0;
			t_mat_diffuse[3] = 1;
			glMaterialfv(GL_FRONT,GL_AMBIENT,t_mat_ambient);
			glMaterialfv(GL_FRONT,GL_DIFFUSE,t_mat_diffuse);
			break;
		case __SPCMATERIAL__ :
			//����������Ĳ���
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
	//���ó�ʼλ��
	SetBasePosition(0,0,-15);

	//����Y���ʼ�Զ���ת�Ƕ�
	RotateAdd(0,0.1,0);


	//������Ӱƽ��
	glShadeModel(GL_SMOOTH);
	//���õƹ�
	SetLight();

	//���ļ��ж�ȡDNA����
	int numOfDNA = ReadDNAListFromFile(filename);


	//�������е�
	ComputePoint(100,numOfDNA * 0.04,1.8,1.2,5);
	//����ʹ�õĵ�
	ComputeUsePoint(__POINTSTEP__);


	for(int i=0;i<numOfDNA;i++)
	{
		m_UseHammer[i].m_Type = m_DNAList[i];
	}

	//������Ⱦ�б�

	CompileRenderList();


	//������Ϊ��Ч
	m_Enable = true;

}

void DNAClass::SetLight()
{
	//���õƹ�

	//�ƹ�������
	GLfloat light_diffuse[]= { 1.0, 1.0, 1.0, 1.0};
	//�ƹ�λ��
	GLfloat light_position[] = { 0, 3, 2.0, 0.0 };
	//�ƹ⾵�淴��
	GLfloat light_specular[] = { 0.8, 0.8,0.8, 1.0 };
	//�ƹ⻷����
	GLfloat light_ambient[] = {0.5, 0.5, 0.5,1};

	//��ֵ���õ�LIGHT_0��
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular); 
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);    //���ʲô�������ã�GL_LIGHT��Ĭ�ϵ�ֵ��
	glCullFace(GL_BACK);
	
	
}

void DNAClass::SetBasePosition(float x,float y,float z)
{
	//����DNAλ�ú���
	m_BasePosition.x = x;
	m_BasePosition.y = y;
	m_BasePosition.z = z;
}

void DNAClass::RotateAdd(float x,float y,float z)
{
	//����DNA��ת�ǶȺ���
	m_Rotate.x += x;
	m_Rotate.z += z;
	m_Rotate.y += y;
}

void DNAClass::SetRotateXZ(float x,float z)
{
	//����DNA�Ƕ�
	m_Rotate.x = x;
	m_Rotate.z = z;
}

void DNAClass::Move(float x,float y,float z)
{
	//�ƶ�DNA
	m_BasePosition.x += x;
	m_BasePosition.y += y;
	m_BasePosition.z += z;
}

void DNAClass::SetEnable(bool m)
{
	//�����Ƿ�����
	m_Enable = m;
}

int DNAClass::ReadDNAListFromFile(const char* filename)
{
	//���ļ��ж�ȡDNA����

	//�ļ����
	FILE *fp;

	//��ʼ��DNA��
	int t_numOfDNA = 0;

	char t_filepath[256+1];
	memset(t_filepath,0,256);

	strcat(t_filepath,g_path);
	strcat(t_filepath,"RDNA.dat");

	fp = fopen(t_filepath,"r");
	
	if(!fp)
	{
		MessageBox(NULL,"DNA�ļ��޷���!","�򿪴���",MB_OK);
		exit(0);
	}
	


	char t_line[10];
	int t_DNA;
	while(!feof(fp))
	{
		//��ȡ
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
	//���ļ��ж�ȡDNA����
	FILE *fp;
	fp = fopen(filename,"rb");

	if(!fp)
	{
		MessageBox(NULL,"�����ļ��޷���!","�򿪴���",MB_OK);
		exit(0);
	}

	int t_Count = 0;									//������
	fread(&t_Count,sizeof(int),1,fp);					//��ȡ���ʸ���

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

//����λ��

CVertex DNAClass::GetPosition()
{
	return m_BasePosition;
}

//���ؽǶ�
CVertex DNAClass::GetRotate()
{
	return m_Rotate;
}

//�����Ƿ���Ч
bool DNAClass::GetEnable()
{
	return m_Enable;
}

//DNA���캯��
DNAClass::DNAClass(const char* filename)
{
	char t_name[256];
	memset(t_name,0,256);
	strcat(t_name,filename);

	Init(t_name);
}


