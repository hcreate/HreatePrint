#include"bitmap.h"

void getBMPFileHeader(BMPFileHeader *header,char* data)
{

   // BMPFileHeader.signature=buffer[0]<<8&buffer[1];
    header->signature=data[0];
    header->signature=header->signature<<8;
    header->signature=header->signature|data[1];
    u_int32_t filesize=0;
    filesize=data[5];
    filesize=filesize<<24;

    header->filesize=header->filesize|filesize;
    filesize=0;
    filesize=data[4];
    filesize=filesize<<16;
    header->filesize=header->filesize|filesize;
    filesize=0;
    filesize=data[3];
    filesize=filesize<<8;
    header->filesize=header->filesize|filesize;
    filesize=0;
    filesize=data[2];
    header->filesize=header->filesize|filesize;


    header->reserved1=data[7];
    header->reserved1=header->reserved1<<8;
    header->reserved1=header->reserved1|data[6];

    header->reserved2=data[9];
    header->reserved2=header->reserved2<<8;
    header->reserved2=header->reserved2|data[8];




    filesize=0;
    filesize=data[13];
    filesize=filesize<<24;

    header->dataoffset=header->dataoffset|filesize;
    filesize=0;
    filesize=data[12];
    filesize=filesize<<16;
    header->dataoffset=header->dataoffset|filesize;
    filesize=0;
    filesize=data[11];
    filesize=filesize<<8;
    header->dataoffset=header->dataoffset|filesize;
    filesize=0;
    filesize=data[9];
    header->dataoffset=header->dataoffset|filesize;



    // BMPFileHeader.signature=BMPFileHeader.signature|buffer[1];
    // BMPFileHeader.filesize=buffer[2]<<24&buffer[3]<<16*buffer[4]<<8&buffer[5];
    // BMPFileHeader.reserved1=buffer[6]<<8&buffer[7];
    // BMPFileHeader.reserved2=buffer[8]<<8&buffer[9];
    // BMPFileHeader.dataoffset=buffer[10]<<24&buffer[11]<<16*buffer[12]<<8&buffer[13];
    // memcpy(&BMPFileHeader,buffer,sizeof(BMPFileHeader));
    //memcpy(&BMPInfoHeader,buffer+sizeof(BMPFileHeader),sizeof(BMPInfoHeader));


}
void getBMPInfoHeader(BMPInfoHeader *info,char* data)
{
    //headersize
    info->headersize=0;
    u_int32_t tempInt32=0;
    tempInt32=data[17];
    tempInt32=tempInt32<<24;
    info->headersize=info->headersize|tempInt32;
    tempInt32=0;
    tempInt32=data[16];
    tempInt32=tempInt32<<16;
    info->headersize=info->headersize|tempInt32;
    tempInt32=0;
    tempInt32=data[15];
    tempInt32=tempInt32<<8;
    info->headersize=info->headersize|tempInt32;
    tempInt32=0;
    tempInt32=data[14];
    info->headersize=info->headersize|tempInt32;

    //width int
    info->width=0;
    tempInt32=0;
    tempInt32=data[21];
    tempInt32=tempInt32<<24;
    info->width=info->width|tempInt32;
    tempInt32=0;
    tempInt32=data[20];
    tempInt32=tempInt32<<16;
    info->width=info->width|tempInt32;
    tempInt32=0;
    tempInt32=data[19];
    tempInt32=tempInt32<<8;
    info->width=info->width|tempInt32;
    tempInt32=0;
    tempInt32=data[18];
    info->width=info->width|tempInt32;

    //height
    info->height=0;
    tempInt32=0;
    tempInt32=data[25];
    tempInt32=tempInt32<<24;
    info->height=info->height|tempInt32;
    tempInt32=0;
    tempInt32=data[24];
    tempInt32=tempInt32<<16;
    info->height=info->height|tempInt32;
    tempInt32=0;
    tempInt32=data[23];
    tempInt32=tempInt32<<8;
    info->height=info->height|tempInt32;
    tempInt32=0;
    tempInt32=data[22];
    info->height=info->height|tempInt32;
    //planes
    info->planes=0;
     info->planes=data[27];
     info->planes=info->planes<<8;
     info->planes=info->planes|data[26];
    //bitdepth
    info->bitdepth=0;
     info->bitdepth=data[29];
     info->bitdepth=info->bitdepth<<8;
     info->bitdepth=info->bitdepth|data[28];
    //compression
     info->compression=0;
    tempInt32=0;
    tempInt32=data[33];
    tempInt32=tempInt32<<24;
    info->compression=info->compression|tempInt32;
    tempInt32=0;
    tempInt32=data[32];
    tempInt32=tempInt32<<16;
    info->compression=info->compression|tempInt32;
    tempInt32=0;
    tempInt32=data[31];
    tempInt32=tempInt32<<8;
    info->compression=info->compression|tempInt32;
    tempInt32=0;
    tempInt32=data[30];
    info->compression=info->compression|tempInt32;
    //imagesize
    info->imagesize=0;
    tempInt32=0;
    tempInt32=data[37];
    tempInt32=tempInt32<<24;
    info->imagesize=info->imagesize|tempInt32;
    tempInt32=0;
    tempInt32=data[36];
    tempInt32=tempInt32<<16;
    info->imagesize=info->imagesize|tempInt32;
    tempInt32=0;
    tempInt32=data[35];
    tempInt32=tempInt32<<8;
    info->imagesize=info->imagesize|tempInt32;
    tempInt32=0;
    tempInt32=data[34];
    info->imagesize=info->imagesize|tempInt32;
    //horizontalres
    info->horizontalres=0;
    tempInt32=0;
    tempInt32=data[41];
    tempInt32=tempInt32<<24;
    info->horizontalres=info->horizontalres|tempInt32;
    tempInt32=0;
    tempInt32=data[40];
    tempInt32=tempInt32<<16;
    info->horizontalres=info->horizontalres|tempInt32;
    tempInt32=0;
    tempInt32=data[39];
    tempInt32=tempInt32<<8;
    info->horizontalres=info->horizontalres|tempInt32;
    tempInt32=0;
    tempInt32=data[38];
    info->horizontalres=info->horizontalres|tempInt32;
    //verticalres
    info->verticalres=0;
    tempInt32=0;
    tempInt32=data[45];
    tempInt32=tempInt32<<24;
    info->verticalres=info->verticalres|tempInt32;
    tempInt32=0;
    tempInt32=data[44];
    tempInt32=tempInt32<<16;
    info->verticalres=info->verticalres|tempInt32;
    tempInt32=0;
    tempInt32=data[43];
    tempInt32=tempInt32<<8;
    info->verticalres=info->verticalres|tempInt32;
    tempInt32=0;
    tempInt32=data[42];
    info->verticalres=info->verticalres|tempInt32;
    //numcolors
    info->numcolors=0;
    tempInt32=0;
    tempInt32=data[49];
    tempInt32=tempInt32<<24;
    info->numcolors=info->numcolors|tempInt32;
    tempInt32=0;
    tempInt32=data[48];
    tempInt32=tempInt32<<16;
    info->numcolors=info->numcolors|tempInt32;
    tempInt32=0;
    tempInt32=data[47];
    tempInt32=tempInt32<<8;
    info->numcolors=info->numcolors|tempInt32;
    tempInt32=0;
    tempInt32=data[46];
    info->numcolors=info->numcolors|tempInt32;
    //importantcolors
     info->importantcolors=0;
    tempInt32=0;
    tempInt32=data[53];
    tempInt32=tempInt32<<24;
    info->importantcolors=info->importantcolors|tempInt32;
    tempInt32=0;
    tempInt32=data[52];
    tempInt32=tempInt32<<16;
    info->importantcolors=info->importantcolors|tempInt32;
    tempInt32=0;
    tempInt32=data[51];
    tempInt32=tempInt32<<8;
    info->importantcolors=info->importantcolors|tempInt32;
    tempInt32=0;
    tempInt32=data[50];
    info->importantcolors=info->importantcolors|tempInt32;

}
//数据取反函数
void invertBytes(char* array, int length)
 {
  for (int i = 0; i < length; i++) {
    array[i] = ~array[i];
  }
}
void bmpToByteArray(const char* filename)
{
    //构建bmp文件结构体和信息结构体
    BMPFileHeader BMPFileHeader;
    BMPInfoHeader BMPInfoHeader;
 
    //打开文件
    FILE* file = fopen(filename, "rb");  // 以二进制只读模式打开文件
    if (!file)
    {
 
        printf("无法打开文件：%s\n", filename);
        return;
    }
    //读头文件结构体数据
    fread(&BMPFileHeader, 1,sizeof(BMPFileHeader), file);  // 读取BMP文件头
 
    // 移动文件指针到信息结构体
    fseek(file, sizeof(BMPFileHeader), SEEK_SET);
    fread(&BMPInfoHeader, 1,sizeof(BMPInfoHeader), file);
 
    if (BMPFileHeader.signature != 0x4D42 || BMPInfoHeader.planes != 1)
    {
        printf("不是单色BMP图片\n");
        fclose(file);
        return;
    }
    // 移动文件指针到像素数据的位置
    fseek(file, BMPFileHeader.dataoffset, SEEK_SET);
 
    // 计算像素数据所占字节数（win位图宽不足4个字节补齐4个字节）
    //如果是用bitmap指令打印 如果宽度像素不是32的倍速 自动补齐部分会是一条黑线
    rowSize = ((BMPInfoHeader.width + 31) / 32) * 4;
    colheigth=BMPInfoHeader.height;
 
    //数据的总字节数
    dataSize = rowSize * BMPInfoHeader.height;
 
 
    // 动态分配内存来存储像素数据
    Bitmapdata=(char*)malloc(sizeof(char)*dataSize);
 
 
    // 读取像素数据
    int readsize=fread(Bitmapdata,1,dataSize,file);
 
    //关闭文件
    fclose(file);
 
 
}

void saveByteArrayToFile(const char* filename, const u_int8_t* byteArray, int32_t size)
{
    FILE* file = fopen(filename, "w");  // 以写入模式打开文件
    if (!file)
    {
        printf("无法打开文件：%s\n", filename);
        perror("无法打开文件");
        return;
    }
 
    // 逐个字节写入文件
    for (int i = 0; i < size; i++)
    {
       // fprintf(file, "%d ", byteArray[i]);
        fwrite(byteArray, size, 1, file);
    }
 
    fclose(file);
}

