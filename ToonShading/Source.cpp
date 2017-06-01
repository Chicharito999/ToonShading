#include <gl/glut.h>
#include <cg/cg.h>
#include <Cg/cgGL.h>
#include <stdio.h>

int ww = GetSystemMetrics(SM_CXSCREEN), hh = GetSystemMetrics(SM_CYSCREEN);//���ڳߴ�

void render();//���ƺ���
void reshape(int w, int h);//���ܴ���

static CGcontext   myCgContext;//װ��shader������
static CGprofile   myCgVertexProfile;
static CGprogram   myCgVertexProgram;
static CGprofile   myCgFragmentProfile;
static CGprogram   myCgFragmentProgram;


// 03vs.cg 03fs.cg ��ͨ��ɫ
static const char *myProgramName = "shading";
static const char *myVertexProgramFileName = "03vs.cg";
static const char *myFragmentProgramFileName = "03fs.cg";
static const char *myVertexProgramName = "vs_main";
static const char *myFragmentProgramName = "fs_main";


int main(int argc, char** argv)
{
	glutInit(&argc, argv);//��ʼ��glut��
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);//��ʼ��Ϊ˫���壬RGB��ɫģʽ�Ĵ��� 
	glutInitWindowSize(ww, hh);//���ó�ʼ���ڵĴ�С
	glutCreateWindow(myProgramName);//����һ������ΪShading�Ĵ���

	glutDisplayFunc(render);//����
	glutReshapeFunc(reshape);//���ܴ���

	glEnable(GL_DEPTH_TEST);//������Ȳ��ԣ����������Զ���Զ����ر���ס��ͼ��

	myCgContext = cgCreateContext();//����һ��װ��shader������
	cgGLSetDebugMode(CG_FALSE);
	cgSetParameterSettingMode(myCgContext, CG_DEFERRED_PARAMETER_SETTING);

	myCgVertexProfile = cgGLGetLatestProfile(CG_GL_VERTEX);//��ȡ���µı��붥��Shader�����profile�İ汾
	cgGLSetOptimalOptions(myCgVertexProfile);
	myCgFragmentProfile = cgGLGetLatestProfile(CG_GL_FRAGMENT);//��ȡ���µı���Ƭ��Shader�����profile�İ汾
	cgGLSetOptimalOptions(myCgFragmentProfile);

	myCgVertexProgram =
		cgCreateProgramFromFile(      //��������ɫ��������myCgContext��������
			myCgContext,
			CG_SOURCE,                // ����shader�����Դ��������
			myVertexProgramFileName,  // ����shaderԴ�����ļ�������
			myCgVertexProfile,        // ����ö���shader�����profile�İ汾
			myVertexProgramName,      // ����shader�������ں���
			NULL);

	cgGLLoadProgram(myCgVertexProgram);//��myCgContext�м��ظö���shader����
	myCgFragmentProgram =
		cgCreateProgramFromFile(      //��Ƭ����ɫ��������myCgContext��������
			myCgContext,
			CG_SOURCE,                  // Ƭ��shader�����Դ��������
			myFragmentProgramFileName,  // Ƭ��shaderԴ�����ļ�������
			myCgFragmentProfile,        // �����Ƭ��shader�����profile�İ汾
			myFragmentProgramName,      // Ƭ��shader�������ں���
			NULL);
	cgGLLoadProgram(myCgFragmentProgram);//��myCgContext�м��ظ�Ƭ��shader����


	glutMainLoop();//���������¼��йصĺ�������ѭ��
	return 0;
}

void reshape(int w, int h)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(55, (float)w / (float)h, 0.1, 100);
	glViewport(0, 0, w, h);
	//glViewport(w / 2, 0, w / 2, h / 2);
	ww = w;
	hh = h;
}

void render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//�����һ��ѭ��ִ�����µĻ�������ɫ
	glClearColor(.0f, .0f, 0.3f, 1.0f);//���屳����ɫΪ��ɫ
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();//ԭ�������������
	gluLookAt(.0, 6.0, 6.0, .0, .0, .0, .0, 1.0, .0);//���ô��ĸ�����

	glRotatef(90.0, -1.0, 0.0, 0.0);//������ת90��
	static float angle;
	glRotatef(angle, 0.0, 0.0, -1.0);//��ͣ��˳ʱ����ת

	cgGLBindProgram(myCgVertexProgram);//�󶨶�����ɫ��������Ҫ���Ƶ�������
	cgGLEnableProfile(myCgVertexProfile);//����Profile����
	cgGLBindProgram(myCgFragmentProgram);//��Ƭ����ɫ��������Ҫ���Ƶ�������
	cgGLEnableProfile(myCgFragmentProfile);//����Profile����


										   //��ModelViewProjection������shader
	CGparameter mv = cgGetNamedParameter(myCgVertexProgram, "MV");
	cgGLSetStateMatrixParameter(mv, CG_GL_MODELVIEW_MATRIX, CG_GL_MATRIX_IDENTITY);
	CGparameter mvp = cgGetNamedParameter(myCgVertexProgram, "MVP");
	cgGLSetStateMatrixParameter(mvp, CG_GL_MODELVIEW_PROJECTION_MATRIX, CG_GL_MATRIX_IDENTITY);

	//CGparameter mv = cgGetNamedParameter(myCgFragmentProgram, "MV");
	//cgGLSetStateMatrixParameter(mv, CG_GL_MODELVIEW_MATRIX, CG_GL_MATRIX_IDENTITY);
	//CGparameter mvp = cgGetNamedParameter(myCgFragmentProgram, "MVP");
	//cgGLSetStateMatrixParameter(mvp, CG_GL_MODELVIEW_PROJECTION_MATRIX, CG_GL_MATRIX_IDENTITY);


	//glutSolidTorus(0.3, 1.0, 30, 30);
	//glutSolidSphere(0.9, 30, 30);//����UFO
	//glTranslatef(ww/2, 0, 0);
	glutSolidTeapot(1);//���Ʋ��

	cgGLDisableProfile(myCgVertexProfile);//�رյ�ǰ��Profile
	cgGLDisableProfile(myCgFragmentProfile);//�رյ�ǰ��Profile

	angle += 0.5;
	if (angle >= 360) angle = 0.0f;//��ͣ����ת

	glutSwapBuffers();
	glutPostRedisplay();
}