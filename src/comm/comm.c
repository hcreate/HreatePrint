#include"comm.h"

void printCOMM(char *p)
{
    printf("%s",p);
}
//判断字符串是否为空 1为空 0不为空
int isStringEmpty(const char* str) 
{
     if (str == NULL)
        return 1;
    if (strlen(str) == 0)
        return 1;
    else
        return 0;
}
char *findData(char *key,char *data)
{
    
     char *midResult=strstr(data,key);
     int keyLength=strlen(key);
    
     if(midResult!=NULL)
     {
        int strLength=strlen(midResult);
        int icnt=0;
        while (icnt<strLength)
        {
            /* code */
            if(midResult[icnt]==' '||midResult[icnt]=='>')
               break;
            icnt++;
        }

       char *result=(char *)malloc(sizeof(char)*(icnt+1-keyLength-2));
       for(int i=0;i<icnt-keyLength-2;i++)
           result[i]=midResult[i+keyLength+1];
        result[icnt-keyLength-2]='\0';
       return result;

     }
     return NULL;
     
}

float pixelToMillimeter(float dpi,int pixel)
{
    float mill=pixel*MILLIMETERTOPIXEL/dpi;
    return mill;
}
float pixelToInch(float dpi,int pixel)
{
    float inch=(pixel*MILLIMETERTOPIXEL/dpi)/MILLIMETERTOPIXEL;
    return inch;
}
int  millimeterToPixel(float dpi,float length)
{
     int pixel=(int)((dpi/MILLIMETERTOPIXEL)*length);
     return pixel;
}

int HCreateDPIChange(int oldDPI,int newDPI,int pos)
{
    float width=pixelToMillimeter(oldDPI,pos);
    int newPos=millimeterToPixel(newDPI,width);
    return newPos;

}

const char * base64char  = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"; 
char * base64_encode( const unsigned char * originChar, char * base64, int originlength )//base64编码
{
    int i, j;
    unsigned char current;
 
    for ( i = 0, j = 0 ; i < originlength ; i += 3 )
    {
        current = (originChar[i] >> 2) ;
        current &= (unsigned char)0x3F;
        base64[j++] = base64char[(int)current];
 
        current = ( (unsigned char)(originChar[i] << 4 ) ) & ( (unsigned char)0x30 ) ;
        if ( i + 1 >= originlength )
        {
            base64[j++] = base64char[(int)current];
            base64[j++] = '=';
            base64[j++] = '=';
            break;
        }
        current |= ( (unsigned char)(originChar[i+1] >> 4) ) & ( (unsigned char) 0x0F );
        base64[j++] = base64char[(int)current];
 
        current = ( (unsigned char)(originChar[i+1] << 2) ) & ( (unsigned char)0x3C ) ;
        if ( i + 2 >= originlength )
        {
            base64[j++] = base64char[(int)current];
            base64[j++] = '=';
            break;
        }
        current |= ( (unsigned char)(originChar[i+2] >> 6) ) & ( (unsigned char) 0x03 );
        base64[j++] = base64char[(int)current];
 
        current = ( (unsigned char)originChar[i+2] ) & ( (unsigned char)0x3F ) ;
        base64[j++] = base64char[(int)current];
    }
    base64[j] = '\0';
    return base64;
}
int base64_decode( const char * base64, unsigned char * originChar )//base64解码
{
    int i, j;
    unsigned char k;
    unsigned char temp[4];
    for ( i = 0, j = 0; base64[i] != '\0' ; i += 4 )
    {
        memset( temp, 0xFF, sizeof(temp) );
        for ( k = 0 ; k < 64 ; k ++ )
        {
            if ( base64char[k] == base64[i] )
                temp[0]= k;
        }
        for ( k = 0 ; k < 64 ; k ++ )
        {
            if ( base64char[k] == base64[i+1] )
                temp[1]= k;
        }
        for ( k = 0 ; k < 64 ; k ++ )
        {
            if ( base64char[k] == base64[i+2] )
                temp[2]= k;
        }
        for ( k = 0 ; k < 64 ; k ++ )
        {
            if ( base64char[k] == base64[i+3] )
                temp[3]= k;
        }
 
        originChar[j++] = ((unsigned char)(((unsigned char)(temp[0] << 2))&0xFC)) |
                ((unsigned char)((unsigned char)(temp[1]>>4)&0x03));
        if ( base64[i+2] == '=' )
            break;
 
        originChar[j++] = ((unsigned char)(((unsigned char)(temp[1] << 4))&0xF0)) |
                ((unsigned char)((unsigned char)(temp[2]>>2)&0x0F));
        if ( base64[i+3] == '=' )
            break;
 
        originChar[j++] = ((unsigned char)(((unsigned char)(temp[2] << 6))&0xF0)) |
                ((unsigned char)(temp[3]&0x3F));
    }
    return j;
}


