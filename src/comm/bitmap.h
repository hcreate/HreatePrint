
#ifndef BITMAP_H
#define BITMAP_H

#include<stdio.h>
#include <string.h>
#include<stdlib.h>
typedef struct {
    unsigned short   signature;      // 文件类型，必须为"BM"
    unsigned int    filesize;          // 文件大小，整个文件的大小
    unsigned short  reserved1;         // 保留字段1，一般为0
    unsigned short  reserved2;         // 保留字段2，一般为0
    unsigned int    dataoffset;        // 数据偏移量，即图像数据相对于文件起始处的偏移
} BMPFileHeader;
typedef struct {
    unsigned int    headersize;        // 头信息长度，对于Windows位图，一般为40  14~17
    signed int      width;             // 图像宽度，单位为像素   18~21
    signed int      height;            // 图像高度，单位为像素 22~25
    unsigned short  planes;            // 图像平面数，一般为1  26~27
    unsigned short  bitdepth;          // 每个像素所使用的位数，一般为24   28~29
    unsigned int    compression;       // 图像数据的压缩方式，一般为0（表示不压缩）
    unsigned int    imagesize;         // 图像数据的实际大小，以字节为单位
    signed int      horizontalres;     // 水平分辨率，单位为像素每米
    signed int      verticalres;       // 垂直分辨率，单位为像素每米
    unsigned int    numcolors;         // 所有颜色的数量，0表示使用默认取值
    unsigned int    importantcolors;   // 重要颜色的数量，0表示所有颜色都重要
} BMPInfoHeader;

// 将单色BMP图片转换为字节数组
int dataSize; // 图片数据大小
int rowSize;  //图片宽Bit
int colheigth; //图片像素高
char* Bitmapdata; //图片数据指针

void getBMPFileHeader(BMPFileHeader *header,char* data);
void getBMPInfoHeader(BMPInfoHeader *info,char* data);

void invertBytes(char* array, int length);
void bmpToByteArray(const char* filename);
void saveByteArrayToFile(const char* filename, const u_int8_t* byteArray, int32_t size);
#endif