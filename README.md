# ply_processor

- 由于要做成通用的PLY解析和压缩结构，因此每处理一个PLY格式的文件时，需要先生成一个config文件，指定哪些属性要当成一个vector，然后做向量的量化或者做标量的量化。本项目已经给出了一个3dgs的样例config文件。
- 标量量化的过程，我这里假设分别对每一维分别进行量化，例如x,y,z三个维度分别量化到uint8，而不是将x,y,z三个维度一起量化到uint8。四元数也是同理。

## Introduction
- 实现了ply文件读取、压缩、解压等功能
- 针对向量（例如球谐函数）使用codebook压缩，同时储存codebook，并提供相应的解码函数
- 针对标量（例如坐标、法向量）使用向量量化的方法，float32压缩到uint8，并提供相应的解码函数
- 针对字符串（例如comments）使用霍夫曼编码，同时储存频率表，并提供相应的解码函数

## 项目结构
- config
    - 保存了常用的PLY文件格式，例如3dgs（点云，球谐函数，optical等），pointcloudXYZRGB（点云，XYZRGB）


    https://blog.playcanvas.com/compressing-gaussian-splats/

## config参数解析
- 根据config的设置，将某几个name的数据放到一个vector中。例如，x,y,z要放到xyz_vec中，nx,ny,nz要放到normal_vec中。

## 整体流程
### 压缩流程
- 加载对应的config文件，确认哪些需要使用向量压缩，哪些需要使用标量压缩，哪些需要使用字符串压缩
- 读取PLY文件并解析ply文件，根据PLY的header，保存相应的属性信息
- 根据config内容，分别对不同的属性进行压缩，并保存相应的压缩信息和属性信息（除了压缩后的数据，还有codebook等信息）
- 把属性信息写入bin的头文件，压缩后的数据写入bin的内容

### 解压流程
- 读取bin文件头，根据header信息，读取相应的属性信息，包括属性名称、类型、压缩类型、codebook等
- 根据属性信息，对属性进行解压
- 利用成员函数访问数据
- 或者保存成新的ply格式