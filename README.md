# Toon-Shading
:octocat:Free Style(optional)<br>
__________________________________________________________________________________________
Author:赵明福                                        Student ID：201400301087                            E-mail:1109646702@qq.com<br>
## Toon Shading
　　卡通渲染属于non photorealistic rendering或简称NPR中的一种。卡通渲染一个物体看起来的效果就像卡通绘画一般，这里介绍一种简单的渲染卡通效果的方法。和一般光照渲染不同的是，卡通效果渲染的物体像素之间颜色过渡是一种阶梯函数形式的，而一般的光照渲染的物体颜色过渡则是平滑的连续函数形式。<br> 
　要实现Toon Shading的效果，可在Fragment shader中测试每个像素漫反射diffuse中的NdotL值，让漫反射形成一个阶梯函数，不同的NdotL区域对应不同的颜色，下图显示出不同部分不同的漫反射强度值，不同的漫反射值分别指定了不同的像素颜色。<br> 
　　![](https://github.com/Chicharito999/ImageCache/raw/master/image/图片54.png)<br>
　但是仅仅这样渲染出来的物体是没有轮廓的，为了给茶壶添加轮廓，还要考虑观察向量V和法线N的关系。将V和N进行点乘，如果这个值比较小，比如0.2，我们就可以认为观察到的物体的边缘。<br>  
　　![](https://github.com/Chicharito999/ImageCache/raw/master/image/图片55.png)<br>  
　　在上图中，P1和P2代表相机坐在位置，S是物体表面上一点，N是点S的法线。当我们在点P1观察物体的时候，S点很明显不能看成物体的轮廓，这个时候N和V1的点积值比较大，而在P2点观察的时候，S点左边的部分几乎是看不到的，这个时候N和V2的点积值很小，接近0，于是可以把S看成是物体的轮廓。<br> 
　　虽然这种Toon shading的方法很简单，但是也有不足的地方。如果构成物体的多边形之间的棱角很分明，比如正方体，那么采用这种方法的话，会出现物体闪烁的情况，因为在物体边缘的地方法线变化很大。<br> 
## Code
* 逐像素着色<br>
```cg
   if( NdotL<0.5)    color = float3(0.16,0.14,0.129);//设置不同漫反射角度区域的颜色值
	  else if(NdotL>=0.5 && NdotL<0.85)    color =float3(1.0,0.379,0);
	  else if(NdotL>=0.85)    color = float3(1.0,0.6,0.07);
	  if(NdotV<0.2)    color = float3(0,0,0);//轮廓线条设为黑色
```
　　在顶点着色器中对所有顶点进行法向量计算，得到每个顶点的法向量，在将输出的顶点法向量作为参数输入片段着色器，片段着色器根据每个三角形片插值得到所有像素的法向量，计算所有像素点的法向量和反射光线的夹角，根据夹角的大小设置不同的颜色值。再通过观察向量和法线的夹角大小设置轮廓。<br>

## Display
截图：<br>
![](https://github.com/Chicharito999/ImageCache/raw/master/image/Gouraud.png)<br>
                                    　　　　　　　　　Gouraud Shading<br>
![](https://github.com/Chicharito999/ImageCache/raw/master/image/Phong.png)<br>
                                　  　　　　　　　　  Phong Shading<br>
![](https://github.com/Chicharito999/ImageCache/raw/master/image/图片56.png) <br>
                                   　　　　　　　　　 Toon Shading<br>
　　通过Gouraud Shading和Phong Shading的对比可以看出，由于Gouraud Shading是基于顶点计算的光照，其余光照的元素由顶点插值得到，这样插值后的光照相比于基于片元的Phong Shading显得不是很真实，Phong Shading能够获得更为平滑的光照效果。<br>
　　通过Phong Shading和BlinnPhong Shading的对比可以看出，BlinnPhong Shading在明暗交接处的变化是渐变的不像Phong Shading那样变化得那么突然，这是因为在计算镜面反射光时用到的角度不同。　
 
 
