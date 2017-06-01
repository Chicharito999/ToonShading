# Toon-Shading
:sunny:Realize at least two different method such as Phong model, Gouraud model<br>
__________________________________________________________________________________________
Author:赵明福                                        Student ID：201400301087                            E-mail:1109646702@qq.com<br>
## Assignment
Here comes the new project.This project may be a little difficult.You have 2 choices: <br>
1:Based on last project:Let's your sitck man move<br>
2:Create your own scene,you can place anything in it<br>
Choose one and use at least two different method such as Phong model, Gouraud model to complete this project.<br>
![](https://github.com/Chicharito999/ImageCache/raw/master/image/图片24.png) 
![](https://github.com/Chicharito999/ImageCache/raw/master/image/图片25.png)
## Analysis
### 冯氏反射模型：
　　冯氏反射模型是基于单平行光源，不考虑光的连续折射变化。<br>
　　光的强度由R、G、B 3个分量组成，每个分量在[0,1]之间变化；物体表面的材质也由R、G、B 3分量表示，每个分量表示反射光的比例。两者的点乘才表示光照射物体后反射的光进入眼睛感受到的颜色。<br>
　　Phong Reflection Model是经典的光照模型，它计算光照包括三个部分：环境光+漫反射光+镜面光。<br>
　　所以，要展现出经过光的照射后物体表面的颜色，要先计算这3类光的RGB分量，然后再与物体表面反射光的RGB分量相乘的总和得到照射后的颜色。注意，对于不同类别的光，物体表面反射的RGB分量不一定相同。<br>
* 环境光：环境光是场景中光源给定或者全局给定的一个光照常量，它一般很小。环境光假设入射光均匀地从周围环境入射至物体表面并等量地向各个方向反射出去。<br>
* 漫反射光：表示特定点光源在物体表面某一点的反射光中那些向空间各个方向均匀反射出去的光。漫反射光的亮度与入射光线与表面法向量的余弦成正比。<br>
* 镜面反射光：镜面光成分模拟的是物体表面光滑时反射的高亮的光，镜面光反映的通常是光的颜色，而不是物体的颜色。镜面反射光的亮度与反射光线和人的眼睛与物体表面的连线成正比。<br>
![](https://github.com/Chicharito999/ImageCache/raw/master/image/图片27.png)<br>
![](https://github.com/Chicharito999/ImageCache/raw/master/image/图片26.png)<br>
得到3类光的RGB分量之后，与物体表面材质的RGB反射分量相乘即可获得光照射之后物体表面颜色。<br>
 
### CG编写Shader：
　　如果在openGL中使用CG(C for Graphic)语言，首先要下载并安装 NVIDIA的Cg Toolkit，然后在项目中的附加包含目录中添加头文件目录，在附加库目录中添加库文件目录，在附加依赖项中添加cg.lib cgGL.lib就可以在程序中使用了。<br>
　　着色器可以分为顶点着色器和片段着色器。顾名思义，顶点着色器是处理各个顶点的程序，而片段着色器处理片段，负责输出每个呈现三角形的最终像素颜色。<br>
　　顶点着色器的输出参数可以说是直接作为了片段着色器的形参传递过来。它的工作原理如下：片段着色器以输入的形式收到顶点着色器传递的所有这些片段，所以到达片段着色器的片段是顶点着色器的顶点属性输出的插值版本。<br>

### 古罗着色 冯氏着色 Blinn冯氏着色：
* 古罗着色：又叫做逐顶点着色，故名思意跟顶点有关，也就是在我们的顶点着色器中根据每个顶点上的入射向量L、法向量N、观察向量V等直接计算出每个顶点该有的颜色，然后传递给片段着色器进行插值着色，可想而知由于顶点是离散的，片段是连续的，所以引起着色效果的不光滑很容易理解。<br> 
* 冯氏着色：与古罗着色对应即是在片段着色器中，对法向量与坐标进行插值，然后再通过冯氏反射模型计算出每个像素点的颜色值，从而使离散的顶点计算出来的离散的颜色变得连续而光滑。我们直接把环境光、漫反射光、镜面反射光的计算拿到片段着色器中计算即可完成修改，那么法向量、观察向量、入射向量同理需要传递给片段着色器，而不再是直接传递一个颜色。<br> 
* Blinn冯氏着色：类似于与冯氏着色模型，只是Blinn-Phong模型镜面光的计算，采用了半角向量，这个向量是光照向量L和观察向量V的取中向量H，通过计算这个取中向量H与法向量N的夹角来得到镜面发射光强度，如下图所示：<br> 
![](https://github.com/Chicharito999/ImageCache/raw/master/image/图片28.png)<br>
## Code
* 光照计算
```cg
      output OUT;
      float3 N = normalize(IN.normal);//计算法向量
      float3 P = IN.objectPos;
      float3 L = normalize(LightPosition - P);//计算入射向量
      float NdotL = max(dot(N,L),0);//入射向量与法向量夹角
      float3 ambient = Ka * I;//环境光
      float3 diffuse = Kd * I * NdotL;//漫反射光
      float3 V = normalize(eyePosition - P);//眼睛-物体连线向量  

 //Phong光照模型
      float3 R=reflect(-L,N);//计算反射光线向量
      R=normalize(R);
      float NdotH = pow(max(dot(V,R), 0), shininess);//反射光与视角的夹角
 //BlinnPhong光照模型
      //  float3 H = normalize(L+V);
      //  float NdotH = pow(max(dot(N,H), 0), shininess);

      if(NdotL<=0)
           NdotH = 0.0;
      float3 specular = Ks*I*NdotH;//镜面发射光
      float3 color = ambient + diffuse + specular;//所有成分相加
      OUT.color.xyz= color;
      OUT.color.w = 1.0;
      return OUT;
```
* 古罗着色<br>
01vs.cg:<br>
```cg
struct output
{
      float4 position : POSITION; 
      float4 color     : COLOR; 
};
 
output vs_main( float4 position : POSITION,
                     float3 normal   : NORMAL,
                     uniform float4x4 MV, // 在相机坐标系中计算，所以要用到ModelView变换矩阵
                     uniform float4x4 MVP // ModelViewProjection变换矩阵
                    )
{

   //光照计算

}
```
01fs.cg:<br>
```cg
float4 fs_main( float4 color    : COLOR ) : COLOR
{
      return color;
}  
```
　　在顶点着色器中对所有顶点进行光照计算，得到每个顶点的颜色，在将输出的所有顶点颜色作为参数输入片段着色器，片段着色器根据每个三角形片的顶点颜色差值得到所有像素的颜色，从而实现了古罗着色模型。<br>
* 冯氏着色<br>
02vs.cg:<br>
```cg
struct output
{
      float4 position  : POSITION;    
      float3 objectPos : TEXCOORD0;  //顶点坐标传入fragment shader ，然后插值
      float3 normal     : TEXCOORD1;//法向量传入fragment shader，然后插值
};
 
output vs_main( float4 position : POSITION,
                  float3 normal   : NORMAL,
                  uniform float4x4 MV,
                  uniform float4x4 MVP
                    )
{
      output OUT;
      OUT.position = mul(MVP, position);//顶点位置转换到裁剪坐标
      OUT.objectPos = mul(MV, position).xyz;//转换到相机坐标
      OUT.normal = mul(MV, float4(normal,0.0)).xyz;//计算相机坐标下的法向量
 
      return OUT;
}
```
02fs.cg:<br>
```cg
struct input{//传入每个顶点的在相机坐标的位置信息和法向量，插值得到每个像素点的
      float3 objectPos: TEXCOORD0;   
      float3 normal   : TEXCOORD1;
};
 
struct output{
      float4 color     : COLOR;
};
 
output fs_main( in input IN )//为每个像素点计算color
{
     //光照计算

}
```
　　在顶点着色器中将所有顶点的坐标转换到相机坐标、计算出每个顶点的法向量并将它们传入片段着色器，片段着色器自动插值得到所有像素点的相机坐标和法向量，最后通过光照计算得到每个像素点的颜色<br>
* Blinn冯氏着色<br>
　　实现方式类似于冯氏着色，只是在计算镜面反射时用到的夹角不同<br>

## Display
视频链接：http://www.bilibili.com/video/av9903444/<br>
截图：<br>
![](https://github.com/Chicharito999/ImageCache/raw/master/image/Gouraud.png)<br>
                                    　　　　　　　　　Gouraud Shading<br>
![](https://github.com/Chicharito999/ImageCache/raw/master/image/Phong.png)<br>
                                　  　　　　　　　　  Phong Shading<br>
![](https://github.com/Chicharito999/ImageCache/raw/master/image/BlinnPhong.png) <br>
                                   　　　　　　　　　 BlinnPhong Shading<br>
　　通过Gouraud Shading和Phong Shading的对比可以看出，由于Gouraud Shading是基于顶点计算的光照，其余光照的元素由顶点插值得到，这样插值后的光照相比于基于片元的Phong Shading显得不是很真实，Phong Shading能够获得更为平滑的光照效果。<br>
　　通过Phong Shading和BlinnPhong Shading的对比可以看出，BlinnPhong Shading在明暗交接处的变化是渐变的不像Phong Shading那样变化得那么突然，这是因为在计算镜面反射光时用到的角度不同。　
