#include"cpcl.h"
void printCPCL(char *p)
{
   // printf("%s",p);
  Queue* CPCLqueue=createQueue();
  pageConfig *page=getPageConfig(p);
  HCreateOpen(CPCLqueue,page);
  HCreateObject(CPCLqueue,page);
  HCreateClose(CPCLqueue,page);
  HCreateFormFeed(CPCLqueue,page);
  //输出打印，并释放内存
  while(!isEmpty(CPCLqueue))
   {
     char *data=dequeue(CPCLqueue);
     printf("%s",data);
     free(data);

   }
  free(CPCLqueue);
  freePage(page);
  //printf("%s\n",cmds);
}
pageConfig *getPageConfig(char *p)
{

  FILE *file=open(p);
  Queue* queue=  readToQueue(file);
  pageConfig *page=NULL;
  page= prase(queue);
  return page;
}

void HCreateSetPageSize(Queue*CPCLqueue,pageConfig *page)
{
    
  float PageHeight=pixelToMillimeter(page->dpiX,page->pageHeight);
  float PageWidth=pixelToMillimeter(page->dpiY,page->pageWidth);
  float Offset=pixelToMillimeter(page->dpiX,page->pagePadding);
  int height=millimeterToPixel(DPI,PageHeight);
  int width=millimeterToPixel(DPI,PageWidth);
  int padding=millimeterToPixel(DPI,Offset);

  char *cmds=(char *)malloc(sizeof(char)*100);
 // char temps[2]="!";
  strcat(cmds,"!");
  char str[4];
  sprintf(str, " %d", padding);
  strcat(cmds,str);
  char dpiX[10];
  sprintf(dpiX, " %d", DPI);
  strcat(cmds,dpiX);
  char dpiY[10];
  sprintf(dpiY, " %d", DPI);
  strcat(cmds,dpiY);
  char pageHeight[10];

  sprintf(pageHeight, " %d", height);
  strcat(cmds,pageHeight);
  char pageWidth[10];
  sprintf(pageWidth, " %d 1\n", width);
  strcat(cmds,pageWidth);
  
  enqueue(CPCLqueue,cmds);

}
void HCreateUnitSet(int unitType,Queue*CPCLqueue,pageConfig *page)
{
  char *cmds=(char *)malloc(sizeof(char)*100);
  //char buff[20];
   // char buff1[20]="N-INCHES\r\n";
   // char buff2[20]="N-CENTIMETERS\r\n";
   // char buff3[20]="N-MILLIMETER\r\n";
   // char buff4[20]="N-DOTS\r\n";
    switch (unitType)
    {
    case 0:
      /* code */
      //sprintf(buff,"%s\r\n","N-INCHES");
    
      strcat(cmds,"N-INCHES\r\n");

      break;
      case 1:
      /* code */
     // cmds="N-CENTIMETERS\r\n";
      //sprintf(buff,"%s\r\n","N-CENTIMETERS");
    
      strcat(cmds,"N-CENTIMETERS\r\n");
      break;
      case 2:
      /* code */
     // cmds="N-MILLIMETER\r\n";
       //sprintf(buff,"%s\r\n","N-MILLIMETER");
       // char buff[20]={"N-MILLIMETER\r\n"};
     
      strcat(cmds,"N-MILLIMETER\r\n");

      break;
        case 3:
      /* code */
     // cmds="N-DOTS\r\n";
      //sprintf(buff,"%s\r\n","N-DOTS");
   
      strcat(cmds,"N-DOTS\r\n");
      break;
    
    
    default:
    //char buff[20]={"N-DOTS\r\n"};
    // sprintf(buff,"%s\r\n","N-DOTS");
 
      strcat(cmds,"N-DOTS\r\n");
   // cmds="N-DOTS\r\n";
      break;
    }
  enqueue(CPCLqueue,cmds);
}
void HCreatePageWidth(Queue*CPCLqueue,pageConfig *page)
{
  float PageWidth=pixelToMillimeter(page->dpiY,page->pageWidth);
  int width=millimeterToPixel(DPI,PageWidth);
  char *cmds=(char*)malloc(sizeof(char)*100);
 // char cmds[100]="PAGE-WIDTH ";
  //char temps[20]={"PAGE-WIDTH "};
  strcat(cmds,"PAGE-WIDTH ");
  char str[10];
  sprintf(str, "%d\r\n", width);
  strcat(cmds,str);
  enqueue(CPCLqueue,cmds);
}
void HCreateOpen(Queue*CPCLqueue,pageConfig *page)
{
 HCreateSetPageSize(CPCLqueue,page);
 HCreateUnitSet(3,CPCLqueue,page);
 HCreatePageWidth(CPCLqueue,page);

}
void HCreateObject(Queue*CPCLqueue,pageConfig *page)
{
 layerNode *current=page->layers;
  while (current!=NULL)
  {
    /* code */

    switch (current->layerType)
    {
    case Text:
      /* code */
      //TextLayer *textObject=(TextLayer *)current->layerData;
      HCreateText(CPCLqueue,page,current->layerData);

      break;
    case Barcode:
     //BarcodeLayer *barcodeObject=(BarcodeLayer *)current->layerData;
      /* code */
      HCreateBarcode(CPCLqueue,page,current->layerData);
    break;
    case Rfid:
     HCreateRfid(CPCLqueue,page,current->layerData);
      /* code */
    break;
    case Image:
     //ImageLayer *imageObject=(ImageLayer *)current->layerData;
      /* code */
      HCreateImage(CPCLqueue,page,current->layerData);
    break;
    case Line:
     //LineLayer *lineObject=(LineLayer *)current->layerData;
      HCreateLine(CPCLqueue,page,current->layerData);
      /* code */
    break;
    case Rectangle:
    // RectangleLayer *rectangleObject=(RectangleLayer *)current->layerData;
      /* code */
      HCreateRectangle(CPCLqueue,page,current->layerData);
    break;
    case Ellipse:
    //EllipseLayer *ellipseObject=(EllipseLayer *)current->layerData;
      /* code */
      HCreateEllipse(CPCLqueue,page,current->layerData);

    break;
 

    default:
      break;
    }
    page->layers=current->next;
    free(current);
    current=page->layers;


  }
}

void HCreateClose(Queue*CPCLqueue,pageConfig *page)
{

}
void HCreateFormFeed(Queue*CPCLqueue,pageConfig *page)
{
 //char buff[20]="FORM\n";
 char *cmds=(char*)malloc(sizeof(char)*100);
 strcat(cmds,"FORM\n");
 enqueue(CPCLqueue,cmds);

}
void HCreateText(Queue*CPCLqueue,pageConfig *page,TextLayer *textLayer)
{
  char *cmds= (char *)malloc(sizeof(char)*5000);
  char *fName =(char *)malloc(sizeof(char)*20);
  sprintf(fName, "%s ", FONTNAME);


   int FontSize;
   FontSize=textLayer->font.FontSize;
   FontSize=FontSize/10;
   if(FontSize==0)
    FontSize=2;
   else if(FontSize==1)
    FontSize=0;
   else if(FontSize==2)
    FontSize=3;
   else if(FontSize==3)
    FontSize=1;
  char fsize[10];
  sprintf(fsize, "%d ", FontSize);

   char xpos[10];
   int x=HCreateDPIChange(page->dpiX,DPI,textLayer->baseLayer->x);
   sprintf(xpos, "%d ", x);
   char ypos[10];
   int y=HCreateDPIChange(page->dpiY,DPI,textLayer->baseLayer->y);
   sprintf(ypos, "%d ", y);

  switch (textLayer->baseLayer->Angle)
  {
  case 0:
    /* code */
    strcat(cmds,"TEXT ");
    break;
    case 1:
     strcat(cmds,"VTEXT90 ");
    break;
    case 2:
       strcat(cmds,"VTEXT180 ");
    break;
    case 3:
       strcat(cmds,"VTEXT270 ");
    break;

  
  default:
    strcat(cmds,"TEXT ");
    break;
  }
    strcat(cmds,fName);
    strcat(cmds,fsize);
    strcat(cmds,xpos);
    strcat(cmds,ypos);
    strcat(cmds,textLayer->content);
    strcat(cmds,"\r\n");
  free(fName);
  free(textLayer->font.Style);
  free(textLayer->font.FontName);
  free(textLayer->content);
  free(textLayer->baseLayer->layerName);
  free(textLayer->baseLayer);
  free(textLayer);

  enqueue(CPCLqueue,cmds);



 // enqueue(CPCLqueue,cmds);


}
void HCreateBarcode(Queue*CPCLqueue,pageConfig *page,BarcodeLayer *barcodeLayer)
{
   char *cmds= (char *)malloc(sizeof(char)*5000);
   char xpos[10];
   int x=HCreateDPIChange(page->dpiX,DPI,barcodeLayer->baseLayer->x);
   sprintf(xpos, "%d ", x);
   char ypos[10];
   int y=HCreateDPIChange(page->dpiY,DPI,barcodeLayer->baseLayer->y);
   sprintf(ypos, "%d ", y);
   char xdim[10];
   int xdimension=millimeterToPixel(DPI,barcodeLayer->XDimension);
   sprintf(xdim, "%d ", xdimension);
   int h=HCreateDPIChange(page->dpiY,DPI,barcodeLayer->baseLayer->height);
   char height[10];
   sprintf(height, "%d ", h);

   char errorLevel[10];
    sprintf(errorLevel, "%d ", barcodeLayer->errorLevel);
  
   unsigned char ratio;
   unsigned char extAttr[10];
   //QR码字段定义
   char modlparam[10];
   char elevel[10];
   char maskno[10];
   char charactermode[10];
   char inputmodel[10];
   u_int8_t e3;

   int count=base64_decode(barcodeLayer->ExtAttr,extAttr);

   //printf("%s",extAttr);
    if(barcodeLayer->baseLayer->Angle==0||barcodeLayer->baseLayer->Angle==2)
      strcat(cmds,"BARCODE ");
    else
      strcat(cmds,"VBARCODE ");

   
   if(barcodeLayer->codeType>=55&&barcodeLayer->codeType<=61)//rss条码
   {
    uint16_t line_height=extAttr[3]<<8;
    line_height=line_height+extAttr[2];
    char lineheight[10];
    sprintf(lineheight, "%d ", line_height);

    uint8_t sep_height=extAttr[0];
    uint8_t _segments=extAttr[1];
    _segments=(_segments+1)*2;

    char sepheight[10];
    char segents[10];
    sprintf(sepheight, "%d ", sep_height); 
    sprintf(segents, "%d ", _segments); 

      strcat(cmds,"RSS ");
      strcat(cmds,xpos);
      strcat(cmds,ypos);
      strcat(cmds,xdim);
      strcat(cmds,lineheight);
      strcat(cmds,segents);
     switch (barcodeLayer->codeType)
     {
     case 55:
      /* code */

      strcat(cmds,"6 ");

      break;
     case 56:
      /* code */
      strcat(cmds,"3 ");
      break;
      case 57:
      /* code */
        strcat(cmds,"5 ");
      break;
      case 58:
      case 59:
      /* code */
        strcat(cmds,"4 ");
      break;
      case 60:
      /* code */
        strcat(cmds,"3 ");
      break;
      case 61:
      /* code */
        strcat(cmds,"2 ");
      break;
  
     
     default:
      break;
     }
      strcat(cmds,barcodeLayer->content);
      strcat(cmds,"\n ");
   }else
   {
    switch (extAttr[0])
    {
    case 0x00:  case 0x01:  case 0x02:  case 0x03:  case 0x04:
      ratio=extAttr[0];
      break;
    case 0x05:case 0x06:case 0x07:case 0x08:case 0x09:case 0x0a:case 0x0b:case 0x0c:case 0x0d:case 0x0e:case 0x0f:case 0x10:
     ratio=extAttr[0]+0x0f;
    default:
      break;
    }

    char _ratio[10];
    sprintf(_ratio, "%d ", (uint8_t)ratio);
    switch (barcodeLayer->codeType)
    {
      case 0://ean13
    
      if(isStringEmpty(barcodeLayer->supplenmentText)==1)//无supplenmentText
      {
          strcat(cmds,"EAN13 ");
          strcat(cmds,xdim);
          strcat(cmds,_ratio);
          strcat(cmds,height);
          strcat(cmds,xpos);
          strcat(cmds,ypos);
          strcat(cmds,barcodeLayer->content);
           strcat(cmds,"\n");

      }else if(strlen(barcodeLayer->supplenmentText)==2)
      {
          strcat(cmds,"EAN132 ");
          strcat(cmds,xdim);
          strcat(cmds,_ratio);
          strcat(cmds,height);
          strcat(cmds,xpos);
          strcat(cmds,ypos);
          strcat(cmds,barcodeLayer->content);
           strcat(cmds,barcodeLayer->supplenmentText);
           strcat(cmds,"\n");
      }else if(strlen(barcodeLayer->supplenmentText)==5)
      {
          strcat(cmds,"EAN135 ");
          strcat(cmds,xdim);
          strcat(cmds,_ratio);
          strcat(cmds,height);
          strcat(cmds,xpos);
          strcat(cmds,ypos);
          strcat(cmds,barcodeLayer->content);
           strcat(cmds,barcodeLayer->supplenmentText);
           strcat(cmds,"\n");
      }

        break;
      case 2://upc
      /* code */
      if(isStringEmpty(barcodeLayer->supplenmentText)==1)//无supplenmentText
      {
          strcat(cmds,"UPCA ");
          strcat(cmds,xdim);
          strcat(cmds,_ratio);
          strcat(cmds,height);
          strcat(cmds,xpos);
          strcat(cmds,ypos);
          strcat(cmds,barcodeLayer->content);
           strcat(cmds,"\n");

      }else if(strlen(barcodeLayer->supplenmentText)==2)
      {
          strcat(cmds,"UPCA2 ");
          strcat(cmds,xdim);
          strcat(cmds,_ratio);
          strcat(cmds,height);
          strcat(cmds,xpos);
          strcat(cmds,ypos);
          strcat(cmds,barcodeLayer->content);
           strcat(cmds,barcodeLayer->supplenmentText);
           strcat(cmds,"\n");
      }else if(strlen(barcodeLayer->supplenmentText)==5)
      {
          strcat(cmds,"UPCA5 ");
          strcat(cmds,xdim);
          strcat(cmds,_ratio);
          strcat(cmds,height);
          strcat(cmds,xpos);
          strcat(cmds,ypos);
          strcat(cmds,barcodeLayer->content);
           strcat(cmds,barcodeLayer->supplenmentText);
           strcat(cmds,"\n");
      }

        break;
      case 3://upce
      if(isStringEmpty(barcodeLayer->supplenmentText)==1)//无supplenmentText
      {
          strcat(cmds,"UPCE ");
          strcat(cmds,xdim);
          strcat(cmds,_ratio);
          strcat(cmds,height);
          strcat(cmds,xpos);
          strcat(cmds,ypos);
          strcat(cmds,barcodeLayer->content);
           strcat(cmds,"\n");

      }else if(strlen(barcodeLayer->supplenmentText)==2)
      {
          strcat(cmds,"UPCE2 ");
          strcat(cmds,xdim);
          strcat(cmds,_ratio);
          strcat(cmds,height);
          strcat(cmds,xpos);
          strcat(cmds,ypos);
          strcat(cmds,barcodeLayer->content);
           strcat(cmds,barcodeLayer->supplenmentText);
           strcat(cmds,"\n");
      }else if(strlen(barcodeLayer->supplenmentText)==5)
      {
          strcat(cmds,"UPCE5 ");
          strcat(cmds,xdim);
          strcat(cmds,_ratio);
          strcat(cmds,height);
          strcat(cmds,xpos);
          strcat(cmds,ypos);
          strcat(cmds,barcodeLayer->content);
           strcat(cmds,barcodeLayer->supplenmentText);
           strcat(cmds,"\n");
      }

      break;
      case 1:
      if(isStringEmpty(barcodeLayer->supplenmentText)==1)//无supplenmentText
      {
          strcat(cmds,"EAN8 ");
          strcat(cmds,xdim);
          strcat(cmds,_ratio);
          strcat(cmds,height);
          strcat(cmds,xpos);
          strcat(cmds,ypos);
          strcat(cmds,barcodeLayer->content);
           strcat(cmds,"\n");

      }else if(strlen(barcodeLayer->supplenmentText)==2)
      {
          strcat(cmds,"EAN82 ");
          strcat(cmds,xdim);
          strcat(cmds,_ratio);
          strcat(cmds,height);
          strcat(cmds,xpos);
          strcat(cmds,ypos);
          strcat(cmds,barcodeLayer->content);
           strcat(cmds,barcodeLayer->supplenmentText);
           strcat(cmds,"\n");
      }else if(strlen(barcodeLayer->supplenmentText)==5)
      {
          strcat(cmds,"EAN85 ");
          strcat(cmds,xdim);
          strcat(cmds,_ratio);
          strcat(cmds,height);
          strcat(cmds,xpos);
          strcat(cmds,ypos);
          strcat(cmds,barcodeLayer->content);
           strcat(cmds,barcodeLayer->supplenmentText);
           strcat(cmds,"\n");
      }

      break;

      case 11://code39
          strcat(cmds,"39 ");
          strcat(cmds,xdim);
          strcat(cmds,_ratio);
          strcat(cmds,height);
          strcat(cmds,xpos);
          strcat(cmds,ypos);
          strcat(cmds,barcodeLayer->content);
           strcat(cmds,"\n");
      break;
      case 9://code93
         strcat(cmds,"93 ");
          strcat(cmds,xdim);
          strcat(cmds,_ratio);
          strcat(cmds,height);
          strcat(cmds,xpos);
          strcat(cmds,ypos);
          strcat(cmds,barcodeLayer->content);
           strcat(cmds,"\n");
      break;
      case 42://i2of5
         strcat(cmds,"I2OF5 ");
          strcat(cmds,xdim);
          strcat(cmds,_ratio);
          strcat(cmds,height);
          strcat(cmds,xpos);
          strcat(cmds,ypos);
          strcat(cmds,barcodeLayer->content);
           strcat(cmds,"\n");
      break;
      case 12:case 13:case 14://code128
       strcat(cmds,"128 ");
          strcat(cmds,xdim);
          strcat(cmds,_ratio);
          strcat(cmds,height);
          strcat(cmds,xpos);
          strcat(cmds,ypos);
          strcat(cmds,barcodeLayer->content);
           strcat(cmds,"\n");
      break;
      case 18://codabar
        strcat(cmds,"CODABAR ");
          strcat(cmds,xdim);
          strcat(cmds,_ratio);
          strcat(cmds,height);
          strcat(cmds,xpos);
          strcat(cmds,ypos);
          strcat(cmds,barcodeLayer->content);
           strcat(cmds,"\n");
      break;
      case 21:case 68:case 71://pdf-417
          strcat(cmds,"PDF-417 ");
          strcat(cmds,xpos);
          strcat(cmds,ypos);
          strcat(cmds,"XD ");
          strcat(cmds,xdim);
          strcat(cmds,"YD ");
          char tempint[10];
          sprintf(tempint, "%d ", (int)extAttr[1]);
          strcat(cmds,tempint);
          strcat(cmds,"C 3 S");
          strcat(cmds,errorLevel);
          strcat(cmds,"\n ");
          strcat(cmds,barcodeLayer->content);
          strcat(cmds,"\n ");
          strcat(cmds,"ENDPDF\n");
      break;
      case 17://aztec
          strcat(cmds,"AZTEC ");
          strcat(cmds,xpos);
          strcat(cmds,ypos);
          strcat(cmds,"XD ");
          strcat(cmds,xdim);
          strcat(cmds,"EC ");
          strcat(cmds,errorLevel);
          strcat(cmds,"\n");
      break;
      case 29://maxicode
          strcat(cmds,"MAXICODE ");
          strcat(cmds,xpos);
          strcat(cmds,ypos);
          strcat(cmds,"\n");
      break;
      case 16: case 87://qr
         
          e3=(int)extAttr[1];
          e3=e3+1;
          sprintf(modlparam, "%d ", e3);
       
          switch (barcodeLayer->errorLevel)
          {
          case 0:
            sprintf(elevel, "%s ", "L");
            break;
          case 1:
           sprintf(elevel, "%s ", "M");
          break;
           case 2:
           sprintf(elevel, "%s ", "Q");
          break;
           case 3:
           sprintf(elevel, "%s ", "H");
          break;
          
          default:
           sprintf(elevel, "%s ", "L");
            break;
          }
         
          sprintf(maskno, "%d ", (int)extAttr[0]);
     
       
          switch ((int)extAttr[2])
           {
                case 0:
                 sprintf(charactermode, "%s ", "N");
                break;
                 case 1:
                   sprintf(charactermode, "%s ", "A");
                break;
                 case 2:
                   sprintf(charactermode, "%s ", "B");
                break;
                 case 3:
                   sprintf(charactermode, "%s ", "K");
                break;
                default:
                   sprintf(charactermode, "%s ", "A");
                break;

           }
          
          if(extAttr[1]=0x01)
            sprintf(inputmodel, "%s ", "A");
          else
            sprintf(inputmodel, "%s ", "M");

          strcat(cmds,"QR ");
          strcat(cmds,xpos);
          strcat(cmds,ypos);
          strcat(cmds,"M ");
          strcat(cmds,modlparam);
          strcat(cmds,"U ");
          strcat(cmds,xdim);
          strcat(cmds,"\n ");
          strcat(cmds,elevel);
          strcat(cmds,maskno);
          strcat(cmds,inputmodel);
          strcat(cmds,", ");
          strcat(cmds,charactermode);
          strcat(cmds,barcodeLayer->content);
          strcat(cmds,"\n ");
          strcat(cmds,"ENDQR\n");





      break;
    
      default:
        break;
    }

   }

  free(barcodeLayer->EncodingMode);
  free(barcodeLayer->DataMode);
  free(barcodeLayer->ExtAttr);
  free(barcodeLayer->supplenmentText);
  free(barcodeLayer->content);
  free(barcodeLayer->baseLayer->layerName);
  free(barcodeLayer->baseLayer);
  free(barcodeLayer);

  enqueue(CPCLqueue,cmds);
}
void HCreateLine(Queue*CPCLqueue,pageConfig *page,LineLayer *lineLayer)
{
   char *cmds= (char *)malloc(sizeof(char)*5000);
   char xpos[10];
   int x=HCreateDPIChange(page->dpiX,DPI,lineLayer->baseLayer->x);
   sprintf(xpos, "%d ", x);
   char ypos[10];
   int y=HCreateDPIChange(page->dpiY,DPI,lineLayer->baseLayer->y);
   sprintf(ypos, "%d ", y);
   char lineWidth[10];
   int lwidth=HCreateDPIChange(page->dpiY,DPI,lineLayer->lineWidth);
   sprintf(lineWidth, "%d ", lwidth);

   char lineLength[10];
   int length=HCreateDPIChange(page->dpiY,DPI,lineLayer->lineLength);
   sprintf(lineLength, "%d ", length);
   strcat(cmds,"LINE  ");
   strcat(cmds,xpos);
   strcat(cmds,ypos);
   char xendpos[10];
   char yendpos[10];
   if(lineLayer->linDirect==1)
   {
    int x_endpos=x+length;
    int y_endpos=y;
    sprintf(xendpos, "%d ", x_endpos);
    sprintf(yendpos, "%d ", y_endpos);


   }else
   { 
    int x_endpos=x;
    int y_endpos=y+length;
    sprintf(xendpos, "%d ", x_endpos);
    sprintf(yendpos, "%d ", y_endpos);
   }
   strcat(cmds,xendpos);
   strcat(cmds,yendpos);
   strcat(cmds,lineLength);
   strcat(cmds,"\n");
  free(lineLayer->baseLayer->layerName);
  free(lineLayer->baseLayer);
  free(lineLayer);

  enqueue(CPCLqueue,cmds);
}
void HCreateRectangle(Queue*CPCLqueue,pageConfig *page,RectangleLayer *rectLayer)
{
  char *cmds= (char *)malloc(sizeof(char)*5000);
   char xpos[10];
   int x=HCreateDPIChange(page->dpiX,DPI,rectLayer->baseLayer->x);
   sprintf(xpos, "%d ", x);
   char ypos[10];
   int y=HCreateDPIChange(page->dpiY,DPI,rectLayer->baseLayer->y);
   sprintf(ypos, "%d ", y);

   char rectWidth[10];
   int width=HCreateDPIChange(page->dpiX,DPI,rectLayer->RectangWidth);
   sprintf(rectWidth, "%d ", width);

   char rectHeight[10];
   int height=HCreateDPIChange(page->dpiY,DPI,rectLayer->RectangHeight);
   sprintf(rectHeight, "%d ", height);

   char lineLength[10];
   int length=HCreateDPIChange(page->dpiY,DPI,rectLayer->lineWidth);
   sprintf(lineLength, "%d ", length);

   char xend[10];
   char yend[10];
   int x1=x+width;
   int y1=y+height;
   sprintf(xend, "%d ", x1);
   sprintf(yend, "%d ", y1);

   strcat(cmds,"BOX  ");
   strcat(cmds,xpos);
   strcat(cmds,ypos);
   strcat(cmds,xend);
   strcat(cmds,yend);
   strcat(cmds,lineLength);
   strcat(cmds,"\n");
  free(rectLayer->baseLayer->layerName);
  free(rectLayer->baseLayer);
  free(rectLayer);

  enqueue(CPCLqueue,cmds);
}
void HCreateImage(Queue*CPCLqueue,pageConfig *page,ImageLayer *imageLayer)
{
    char *cmds= (char *)malloc(sizeof(char)*(100+strlen(imageLayer->img)));



   char xpos[10];
   int x=HCreateDPIChange(page->dpiX,DPI,imageLayer->baseLayer->x);
   sprintf(xpos, "%d ", x);
   char ypos[10];
   int y=HCreateDPIChange(page->dpiY,DPI,imageLayer->baseLayer->y);
   sprintf(ypos, "%d ", y);

   char imgWidth[10];
   int width=HCreateDPIChange(page->dpiX,DPI,imageLayer->baseLayer->width);
   if(width%8>0)
     width=width/8+1;
   else
     width=width/8;

   sprintf(imgWidth, "%d ", width);

   char imgHeight[10];
   int height=HCreateDPIChange(page->dpiY,DPI,imageLayer->baseLayer->height);
   sprintf(imgHeight, "%d ", height);


   strcat(cmds,"EG ");
   strcat(cmds,imgWidth);
   strcat(cmds,imgHeight);
   strcat(cmds,xpos);
   strcat(cmds,ypos);
   strcat(cmds,imageLayer->img);
   strcat(cmds,"\n");

  free(imageLayer->img);
  free(imageLayer->baseLayer->layerName);
  
  free(imageLayer->baseLayer);

  free(imageLayer);
  enqueue(CPCLqueue,cmds);







}
void HCreateEllipse(Queue*CPCLqueue,pageConfig *page,EllipseLayer *ellipseLayer)
{
    free(ellipseLayer->baseLayer->layerName);
    free(ellipseLayer->baseLayer);
    free(ellipseLayer);
}
void HCreateRfid(Queue*CPCLqueue,pageConfig *page,RfidLayer *rfidLayer)
{
    free(rfidLayer->baseLayer->layerName);
    free(rfidLayer->RfidContent);
    free(rfidLayer->baseLayer);
    free(rfidLayer);
}