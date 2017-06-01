#include <gl/glut.h>
#include <cg/cg.h>
#include <Cg/cgGL.h>
#include <stdio.h>

int ww = GetSystemMetrics(SM_CXSCREEN), hh = GetSystemMetrics(SM_CYSCREEN);//窗口尺寸

void render();//绘制函数
void reshape(int w, int h);//重塑窗口

static CGcontext   myCgContext;//装载shader的容器
static CGprofile   myCgVertexProfile;
static CGprogram   myCgVertexProgram;
static CGprofile   myCgFragmentProfile;
static CGprogram   myCgFragmentProgram;


// 03vs.cg 03fs.cg 卡通着色
static const char *myProgramName = "shading";
static const char *myVertexProgramFileName = "03vs.cg";
static const char *myFragmentProgramFileName = "03fs.cg";
static const char *myVertexProgramName = "vs_main";
static const char *myFragmentProgramName = "fs_main";


int main(int argc, char** argv)
{
	glutInit(&argc, argv);//初始化glut库
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);//初始化为双缓冲，RGB颜色模式的窗口 
	glutInitWindowSize(ww, hh);//设置初始窗口的大小
	glutCreateWindow(myProgramName);//创建一个标题为Shading的窗口

	glutDisplayFunc(render);//绘制
	glutReshapeFunc(reshape);//重塑窗口

	glEnable(GL_DEPTH_TEST);//启用深度测试，根据坐标的远近自动隐藏被遮住的图形

	myCgContext = cgCreateContext();//定义一个装载shader的容器
	cgGLSetDebugMode(CG_FALSE);
	cgSetParameterSettingMode(myCgContext, CG_DEFERRED_PARAMETER_SETTING);

	myCgVertexProfile = cgGLGetLatestProfile(CG_GL_VERTEX);//获取最新的编译顶点Shader程序的profile的版本
	cgGLSetOptimalOptions(myCgVertexProfile);
	myCgFragmentProfile = cgGLGetLatestProfile(CG_GL_FRAGMENT);//获取最新的编译片段Shader程序的profile的版本
	cgGLSetOptimalOptions(myCgFragmentProfile);

	myCgVertexProgram =
		cgCreateProgramFromFile(      //将顶点着色器程序与myCgContext连接起来
			myCgContext,
			CG_SOURCE,                // 顶点shader程序的源代码类型
			myVertexProgramFileName,  // 顶点shader源代码文件的名字
			myCgVertexProfile,        // 编译该顶点shader程序的profile的版本
			myVertexProgramName,      // 顶点shader程序的入口函数
			NULL);

	cgGLLoadProgram(myCgVertexProgram);//在myCgContext中加载该顶点shader程序
	myCgFragmentProgram =
		cgCreateProgramFromFile(      //将片段着色器程序与myCgContext连接起来
			myCgContext,
			CG_SOURCE,                  // 片段shader程序的源代码类型
			myFragmentProgramFileName,  // 片段shader源代码文件的名字
			myCgFragmentProfile,        // 编译该片段shader程序的profile的版本
			myFragmentProgramName,      // 片段shader程序的入口函数
			NULL);
	cgGLLoadProgram(myCgFragmentProgram);//在myCgContext中加载该片段shader程序


	glutMainLoop();//让所有与事件有关的函数无限循环
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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//清除上一次循环执行留下的缓冲区颜色
	glClearColor(.0f, .0f, 0.3f, 1.0f);//定义背景颜色为黑色
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();//原点放在坐标中心
	gluLookAt(.0, 6.0, 6.0, .0, .0, .0, .0, 1.0, .0);//设置从哪个方向看

	glRotatef(90.0, -1.0, 0.0, 0.0);//向外旋转90度
	static float angle;
	glRotatef(angle, 0.0, 0.0, -1.0);//不停地顺时针旋转

	cgGLBindProgram(myCgVertexProgram);//绑定顶点着色程序到下面要绘制的物体上
	cgGLEnableProfile(myCgVertexProfile);//激活Profile编译
	cgGLBindProgram(myCgFragmentProgram);//绑定片段着色程序到下面要绘制的物体上
	cgGLEnableProfile(myCgFragmentProfile);//激活Profile编译


										   //将ModelViewProjection矩阵传入shader
	CGparameter mv = cgGetNamedParameter(myCgVertexProgram, "MV");
	cgGLSetStateMatrixParameter(mv, CG_GL_MODELVIEW_MATRIX, CG_GL_MATRIX_IDENTITY);
	CGparameter mvp = cgGetNamedParameter(myCgVertexProgram, "MVP");
	cgGLSetStateMatrixParameter(mvp, CG_GL_MODELVIEW_PROJECTION_MATRIX, CG_GL_MATRIX_IDENTITY);

	//CGparameter mv = cgGetNamedParameter(myCgFragmentProgram, "MV");
	//cgGLSetStateMatrixParameter(mv, CG_GL_MODELVIEW_MATRIX, CG_GL_MATRIX_IDENTITY);
	//CGparameter mvp = cgGetNamedParameter(myCgFragmentProgram, "MVP");
	//cgGLSetStateMatrixParameter(mvp, CG_GL_MODELVIEW_PROJECTION_MATRIX, CG_GL_MATRIX_IDENTITY);


	//glutSolidTorus(0.3, 1.0, 30, 30);
	//glutSolidSphere(0.9, 30, 30);//绘制UFO
	//glTranslatef(ww/2, 0, 0);
	glutSolidTeapot(1);//绘制茶壶

	cgGLDisableProfile(myCgVertexProfile);//关闭当前的Profile
	cgGLDisableProfile(myCgFragmentProfile);//关闭当前的Profile

	angle += 0.5;
	if (angle >= 360) angle = 0.0f;//不停地旋转

	glutSwapBuffers();
	glutPostRedisplay();
}