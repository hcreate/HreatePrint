#include"epl.h"
void printEPL(char *p)
{
  Queue* EPLqueue=createQueue();
  pageConfig *page=getPageConfigEPL(p);
  HCreateClearBufferEPL(EPLqueue,page);
  HCreateSetPageHeightAndGapAndOffsetEPL(EPLqueue,page);
  HCreateSetPageWidthEPL(EPLqueue,page);
  HCreateObjectEPL(EPLqueue,page);
  HCreatePrintEPL(EPLqueue,page,1);
//输出打印，并释放内存
  while(!isEmpty(EPLqueue))
   {
     char *data=dequeue(EPLqueue);
     printf("%s",data);
     free(data);

   }
  free(EPLqueue);
  freePage(page);
  //printf("%s\n",cmds);
}
pageConfig *getPageConfigEPL(char *p)
{
  FILE *file=open(p);
  Queue* queue=  readToQueue(file);
  pageConfig *page=NULL;
  page= prase(queue);
  return page;
}
void HCreateSetPageHeightAndGapAndOffsetEPL(Queue*EPLqueue,pageConfig *page)
{
     char *cmds=(char *)malloc(sizeof(char)*100);
     int w=HCreateDPIChange(page->dpiX,DPI,page->pageWidth);
     int h=HCreateDPIChange(page->dpiY,DPI,page->pageHeight);
     int vspacing=HCreateDPIChange(page->dpiX,DPI,page->pageVerticalSpacing);
     int padding=HCreateDPIChange(page->dpiX,DPI,page->pagePadding);

    char height[10];
    char gap[10];
    char offset[10];

    strcat(cmds,"Q");

    sprintf(height, "%d", h);
    sprintf(gap, "%d", vspacing);
    sprintf(offset, "%d", padding);

    strcat(cmds,height);
    strcat(cmds,",");
    strcat(cmds,gap);
    strcat(cmds,",");
    strcat(cmds,offset);
    strcat(cmds,"\n");
    enqueue(EPLqueue,cmds);
}
void HCreateSetPageWidthEPL(Queue*EPLqueue,pageConfig *page)
{
     char *cmds=(char *)malloc(sizeof(char)*100);
     int w=HCreateDPIChange(page->dpiX,DPI,page->pageWidth);
     char width[10];
     sprintf(width, "%d", w);
     strcat(cmds,"q");
     strcat(cmds,width);
     strcat(cmds,"\n");
     enqueue(EPLqueue,cmds);
}
void HCreateClearBufferEPL(Queue*EPLqueue,pageConfig *page)
{
    char *cmds=(char *)malloc(sizeof(char)*100);

    strcat(cmds,"N\n");
    enqueue(EPLqueue,cmds);
}
void HCreateObjectEPL(Queue*EPLqueue,pageConfig *page)
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
      HCreateTextEPL(EPLqueue,page,current->layerData);

      break;
    case Barcode:
     //BarcodeLayer *barcodeObject=(BarcodeLayer *)current->layerData;
      /* code */
      HCreateBarcodeEPL(EPLqueue,page,current->layerData);
    break;
    case Rfid:
    // RfidLayer *rfidObject=(RfidLayer *)current->layerData;
      HCreateRfidEPL(EPLqueue,page,current->layerData);
      /* code */
    break;
    case Image:
     //ImageLayer *imageObject=(ImageLayer *)current->layerData;
      /* code */
      HCreateImageEPL(EPLqueue,page,current->layerData);
    break;
    case Line:
     //LineLayer *lineObject=(LineLayer *)current->layerData;
      HCreateLineEPL(EPLqueue,page,current->layerData);
      /* code */
    break;
    case Rectangle:
    // RectangleLayer *rectangleObject=(RectangleLayer *)current->layerData;
      /* code */
      HCreateRectangleEPL(EPLqueue,page,current->layerData);
    break;
    case Ellipse:
    //EllipseLayer *ellipseObject=(EllipseLayer *)current->layerData;
     HCreateEllipseEPL(EPLqueue,page,current->layerData);
      /* code */
    break;
    default:
      break;
    }
    page->layers=current->next;
    free(current);
    current=page->layers;


  }

}
void HCreateBarcodeEPL(Queue*EPLqueue,pageConfig *page,BarcodeLayer *barcodeLayer)
{
  char *cmds= (char *)malloc(sizeof(char)*5000);
  char xpos[10];
  int x=HCreateDPIChange(page->dpiX,DPI,barcodeLayer->baseLayer->x);
  sprintf(xpos, "%d", x);
  char ypos[10];
  int y=HCreateDPIChange(page->dpiY,DPI,barcodeLayer->baseLayer->y);
  sprintf(ypos, "%d", y);
  int h;
  int w;
  char width[10];
  char height[10];
  if(barcodeLayer->baseLayer->Angle%2==0)
  {
    h=HCreateDPIChange(page->dpiY,DPI,barcodeLayer->baseLayer->height);
    w=HCreateDPIChange(page->dpiX,DPI,barcodeLayer->baseLayer->width);
  }else
  { 
    h=HCreateDPIChange(page->dpiX,DPI,barcodeLayer->baseLayer->height);
    w=HCreateDPIChange(page->dpiY,DPI,barcodeLayer->baseLayer->width);
  }

  sprintf(width, "%d", w);
  sprintf(height, "%d", h);

   char errorLevel[10];
   sprintf(errorLevel, "%d,", barcodeLayer->errorLevel);
  

   unsigned char extAttr[10];

   int count=base64_decode(barcodeLayer->ExtAttr,extAttr);


  int narrow_bar_width=2*(int)millimeterToPixel(DPI,barcodeLayer->XDimension);
  int wide_bar_width=narrow_bar_width;
  char narrow[10];
  char wide[10];
  char isShowText[10];
  char angle[10];
  if(barcodeLayer->isShowText==1)
    sprintf(isShowText, "%s", "B");
  else
    sprintf(isShowText, "%s", "N");

  sprintf(angle, "%d", barcodeLayer->baseLayer->Angle);
  sprintf(narrow, "%d", narrow_bar_width);
  //aztec param
  int symbolScaling=3;
  char Scaling[10];
  char ErrorLevels[10];
  char Enablesmenu[10];
  char Inverseimage[10];
  char flg[10];

  switch (extAttr[0])
  {
  case 0:
    wide_bar_width=(int)(1.5*narrow_bar_width);
    break;
  case 1:
    wide_bar_width=(int)(2.5*narrow_bar_width);
  break;
  case 2:
   wide_bar_width=(int)(3*narrow_bar_width);
  break;
  case 3:
   wide_bar_width=(int)(3.5*narrow_bar_width);
  break;
  case 4:
   wide_bar_width=(int)(2*narrow_bar_width);
  break;
  case 5:case 6:case 7:case 8:
  wide_bar_width=(int)(2*narrow_bar_width);
  break;
  case 9:case 10:case 11:case 12:
   wide_bar_width=(int)(2.5*narrow_bar_width);
  break;
  case 13: case 14:
   wide_bar_width=(int)(2.5*narrow_bar_width);
  break;
  
  default:
    break;
  }
  sprintf(wide, "%d", wide_bar_width);
 
  switch (barcodeLayer->codeType)
  {
  case 0://ean13

     if(isStringEmpty(barcodeLayer->supplenmentText)==1)//无supplenmentText
       {
         strcat(cmds,"B");
         strcat(cmds,xpos);
         strcat(cmds,",");
         strcat(cmds,ypos);
         strcat(cmds,",");
         strcat(cmds,angle);
         strcat(cmds,",");
         strcat(cmds,"E30,");
         strcat(cmds,narrow);
         strcat(cmds,",");
         strcat(cmds,wide);
         strcat(cmds,",");
         strcat(cmds,height);
         strcat(cmds,",");
         strcat(cmds,isShowText);
         strcat(cmds,",");
         strcat(cmds,"\"");
         strcat(cmds,barcodeLayer->content);
         strcat(cmds,"\"");
         strcat(cmds,"\n");

       }else if(strlen(barcodeLayer->supplenmentText)==2)
       {
          strcat(cmds,"B");
         strcat(cmds,xpos);
         strcat(cmds,",");
         strcat(cmds,ypos);
         strcat(cmds,",");
         strcat(cmds,angle);
         strcat(cmds,",");
         strcat(cmds,"E32,");
         strcat(cmds,narrow);
         strcat(cmds,",");
         strcat(cmds,wide);
         strcat(cmds,",");
         strcat(cmds,height);
         strcat(cmds,",");
         strcat(cmds,isShowText);
         strcat(cmds,",");
         strcat(cmds,"\"");
         strcat(cmds,barcodeLayer->content);
         strcat(cmds,barcodeLayer->supplenmentText);
         strcat(cmds,"\"");
         strcat(cmds,"\n");
       }else if(strlen(barcodeLayer->supplenmentText)==5)
       {
          strcat(cmds,"B");
         strcat(cmds,xpos);
         strcat(cmds,",");
         strcat(cmds,ypos);
         strcat(cmds,",");
         strcat(cmds,angle);
         strcat(cmds,",");
         strcat(cmds,"E35,");
         strcat(cmds,narrow);
         strcat(cmds,",");
         strcat(cmds,wide);
         strcat(cmds,",");
         strcat(cmds,height);
         strcat(cmds,",");
         strcat(cmds,isShowText);
         strcat(cmds,",");
         strcat(cmds,"\"");
         strcat(cmds,barcodeLayer->content);
         strcat(cmds,barcodeLayer->supplenmentText);
         strcat(cmds,"\"");
         strcat(cmds,"\n");
       }

    break;
  case 1://ean8
  if(isStringEmpty(barcodeLayer->supplenmentText)==1)//无supplenmentText
       {
         strcat(cmds,"B");
         strcat(cmds,xpos);
         strcat(cmds,",");
         strcat(cmds,ypos);
         strcat(cmds,",");
         strcat(cmds,angle);
         strcat(cmds,",");
         strcat(cmds,"E80,");
         strcat(cmds,narrow);
         strcat(cmds,",");
         strcat(cmds,wide);
         strcat(cmds,",");
         strcat(cmds,height);
         strcat(cmds,",");
         strcat(cmds,isShowText);
         strcat(cmds,",");
         strcat(cmds,"\"");
         strcat(cmds,barcodeLayer->content);
         strcat(cmds,"\"");
         strcat(cmds,"\n");

       }else if(strlen(barcodeLayer->supplenmentText)==2)
       {
          strcat(cmds,"B");
         strcat(cmds,xpos);
         strcat(cmds,",");
         strcat(cmds,ypos);
         strcat(cmds,",");
         strcat(cmds,angle);
         strcat(cmds,",");
         strcat(cmds,"E82,");
         strcat(cmds,narrow);
         strcat(cmds,",");
         strcat(cmds,wide);
         strcat(cmds,",");
         strcat(cmds,height);
         strcat(cmds,",");
         strcat(cmds,isShowText);
         strcat(cmds,",");
         strcat(cmds,"\"");
         strcat(cmds,barcodeLayer->content);
         strcat(cmds,barcodeLayer->supplenmentText);
         strcat(cmds,"\"");
         strcat(cmds,"\n");
       }else if(strlen(barcodeLayer->supplenmentText)==5)
       {
          strcat(cmds,"B");
         strcat(cmds,xpos);
         strcat(cmds,",");
         strcat(cmds,ypos);
         strcat(cmds,",");
         strcat(cmds,angle);
         strcat(cmds,",");
         strcat(cmds,"E85,");
         strcat(cmds,narrow);
         strcat(cmds,",");
         strcat(cmds,wide);
         strcat(cmds,",");
         strcat(cmds,height);
         strcat(cmds,",");
         strcat(cmds,isShowText);
         strcat(cmds,",");
         strcat(cmds,"\"");
         strcat(cmds,barcodeLayer->content);
         strcat(cmds,barcodeLayer->supplenmentText);
         strcat(cmds,"\"");
         strcat(cmds,"\n");
       }

  break;
  case 2://upca
  if(isStringEmpty(barcodeLayer->supplenmentText)==1)//无supplenmentText
       {
         strcat(cmds,"B");
         strcat(cmds,xpos);
         strcat(cmds,",");
         strcat(cmds,ypos);
         strcat(cmds,",");
         strcat(cmds,angle);
         strcat(cmds,",");
         strcat(cmds,"UA0,");
         strcat(cmds,narrow);
         strcat(cmds,",");
         strcat(cmds,wide);
         strcat(cmds,",");
         strcat(cmds,height);
         strcat(cmds,",");
         strcat(cmds,isShowText);
         strcat(cmds,",");
         strcat(cmds,"\"");
         strcat(cmds,barcodeLayer->content);
         strcat(cmds,"\"");
         strcat(cmds,"\n");

       }else if(strlen(barcodeLayer->supplenmentText)==2)
       {
          strcat(cmds,"B");
         strcat(cmds,xpos);
         strcat(cmds,",");
         strcat(cmds,ypos);
         strcat(cmds,",");
         strcat(cmds,angle);
         strcat(cmds,",");
         strcat(cmds,"UA2,");
         strcat(cmds,narrow);
         strcat(cmds,",");
         strcat(cmds,wide);
         strcat(cmds,",");
         strcat(cmds,height);
         strcat(cmds,",");
         strcat(cmds,isShowText);
         strcat(cmds,",");
         strcat(cmds,"\"");
         strcat(cmds,barcodeLayer->content);
         strcat(cmds,barcodeLayer->supplenmentText);
         strcat(cmds,"\"");
         strcat(cmds,"\n");
       }else if(strlen(barcodeLayer->supplenmentText)==5)
       {
          strcat(cmds,"B");
         strcat(cmds,xpos);
         strcat(cmds,",");
         strcat(cmds,ypos);
         strcat(cmds,",");
         strcat(cmds,angle);
         strcat(cmds,",");
         strcat(cmds,"UA5,");
         strcat(cmds,narrow);
         strcat(cmds,",");
         strcat(cmds,wide);
         strcat(cmds,",");
         strcat(cmds,height);
         strcat(cmds,",");
         strcat(cmds,isShowText);
         strcat(cmds,",");
         strcat(cmds,"\"");
         strcat(cmds,barcodeLayer->content);
         strcat(cmds,barcodeLayer->supplenmentText);
         strcat(cmds,"\"");
         strcat(cmds,"\n");
       }

  break;
  case 3://UPCE
  if(isStringEmpty(barcodeLayer->supplenmentText)==1)//无supplenmentText
       {
         strcat(cmds,"B");
         strcat(cmds,xpos);
         strcat(cmds,",");
         strcat(cmds,ypos);
         strcat(cmds,",");
         strcat(cmds,angle);
         strcat(cmds,",");
         strcat(cmds,"UE0,");
         strcat(cmds,narrow);
         strcat(cmds,",");
         strcat(cmds,wide);
         strcat(cmds,",");
         strcat(cmds,height);
         strcat(cmds,",");
         strcat(cmds,isShowText);
         strcat(cmds,",");
         strcat(cmds,"\"");
         strcat(cmds,barcodeLayer->content);
         strcat(cmds,"\"");
         strcat(cmds,"\n");

       }else if(strlen(barcodeLayer->supplenmentText)==2)
       {
          strcat(cmds,"B");
         strcat(cmds,xpos);
         strcat(cmds,",");
         strcat(cmds,ypos);
         strcat(cmds,",");
         strcat(cmds,angle);
         strcat(cmds,",");
         strcat(cmds,"UE2,");
         strcat(cmds,narrow);
         strcat(cmds,",");
         strcat(cmds,wide);
         strcat(cmds,",");
         strcat(cmds,height);
         strcat(cmds,",");
         strcat(cmds,isShowText);
         strcat(cmds,",");
         strcat(cmds,"\"");
         strcat(cmds,barcodeLayer->content);
         strcat(cmds,barcodeLayer->supplenmentText);
         strcat(cmds,"\"");
         strcat(cmds,"\n");
       }else if(strlen(barcodeLayer->supplenmentText)==5)
       {
          strcat(cmds,"B");
         strcat(cmds,xpos);
         strcat(cmds,",");
         strcat(cmds,ypos);
         strcat(cmds,",");
         strcat(cmds,angle);
         strcat(cmds,",");
         strcat(cmds,"UE5,");
         strcat(cmds,narrow);
         strcat(cmds,",");
         strcat(cmds,wide);
         strcat(cmds,",");
         strcat(cmds,height);
         strcat(cmds,",");
         strcat(cmds,isShowText);
         strcat(cmds,",");
         strcat(cmds,"\"");
         strcat(cmds,barcodeLayer->content);
         strcat(cmds,barcodeLayer->supplenmentText);
         strcat(cmds,"\"");
         strcat(cmds,"\n");
       }

  break;
  case 4://ITF14
  break;
  case 5: case 6://ISBN ISSN
  if(isStringEmpty(barcodeLayer->supplenmentText)==1)//无supplenmentText
       {
         strcat(cmds,"B");
         strcat(cmds,xpos);
         strcat(cmds,",");
         strcat(cmds,ypos);
         strcat(cmds,",");
         strcat(cmds,angle);
         strcat(cmds,",");
         strcat(cmds,"E30,");
         strcat(cmds,narrow);
         strcat(cmds,",");
         strcat(cmds,wide);
         strcat(cmds,",");
         strcat(cmds,height);
         strcat(cmds,",");
         strcat(cmds,isShowText);
         strcat(cmds,",");
         strcat(cmds,"\"");
         strcat(cmds,barcodeLayer->content);
         strcat(cmds,"\"");
         strcat(cmds,"\n");

       }else if(strlen(barcodeLayer->supplenmentText)==2)
       {
          strcat(cmds,"B");
         strcat(cmds,xpos);
         strcat(cmds,",");
         strcat(cmds,ypos);
         strcat(cmds,",");
         strcat(cmds,angle);
         strcat(cmds,",");
         strcat(cmds,"E32,");
         strcat(cmds,narrow);
         strcat(cmds,",");
         strcat(cmds,wide);
         strcat(cmds,",");
         strcat(cmds,height);
         strcat(cmds,",");
         strcat(cmds,isShowText);
         strcat(cmds,",");
         strcat(cmds,"\"");
         strcat(cmds,barcodeLayer->content);
         strcat(cmds,barcodeLayer->supplenmentText);
         strcat(cmds,"\"");
         strcat(cmds,"\n");
       }else if(strlen(barcodeLayer->supplenmentText)==5)
       {
          strcat(cmds,"B");
         strcat(cmds,xpos);
         strcat(cmds,",");
         strcat(cmds,ypos);
         strcat(cmds,",");
         strcat(cmds,angle);
         strcat(cmds,",");
         strcat(cmds,"E35,");
         strcat(cmds,narrow);
         strcat(cmds,",");
         strcat(cmds,wide);
         strcat(cmds,",");
         strcat(cmds,height);
         strcat(cmds,",");
         strcat(cmds,isShowText);
         strcat(cmds,",");
         strcat(cmds,"\"");
         strcat(cmds,barcodeLayer->content);
         strcat(cmds,barcodeLayer->supplenmentText);
         strcat(cmds,"\"");
         strcat(cmds,"\n");
       }

  break;
  case 7://GS128
         strcat(cmds,"B");
         strcat(cmds,xpos);
         strcat(cmds,",");
         strcat(cmds,ypos);
         strcat(cmds,",");
         strcat(cmds,angle);
         strcat(cmds,",");
         strcat(cmds,"1E,");
         strcat(cmds,narrow);
         strcat(cmds,",");
         strcat(cmds,wide);
         strcat(cmds,",");
         strcat(cmds,height);
         strcat(cmds,",");
         strcat(cmds,isShowText);
         strcat(cmds,",");
         strcat(cmds,"\"");
         strcat(cmds,barcodeLayer->content);
         strcat(cmds,"\"");
         strcat(cmds,"\n");
  break;
  case 8://gs1dm
         strcat(cmds,"b");
         strcat(cmds,xpos);
         strcat(cmds,",");
         strcat(cmds,ypos);
         strcat(cmds,",D,");
         strcat(cmds,"\"");
         strcat(cmds,barcodeLayer->content);
         strcat(cmds,"\"");
         strcat(cmds,"\n");
  break;
  case 9:
         strcat(cmds,"B");
         strcat(cmds,xpos);
         strcat(cmds,",");
         strcat(cmds,ypos);
         strcat(cmds,",");
         strcat(cmds,angle);
         strcat(cmds,",");
         strcat(cmds,"9,");
         strcat(cmds,narrow);
         strcat(cmds,",");
         strcat(cmds,wide);
         strcat(cmds,",");
         strcat(cmds,height);
         strcat(cmds,",");
         strcat(cmds,isShowText);
         strcat(cmds,",");
         strcat(cmds,"\"");
         strcat(cmds,barcodeLayer->content);
         strcat(cmds,"\"");
         strcat(cmds,"\n");
  break;
  case 10://twofive
         strcat(cmds,"B");
         strcat(cmds,xpos);
         strcat(cmds,",");
         strcat(cmds,ypos);
         strcat(cmds,",");
         strcat(cmds,angle);
         strcat(cmds,",");
         strcat(cmds,"2,");
         strcat(cmds,narrow);
         strcat(cmds,",");
         strcat(cmds,wide);
         strcat(cmds,",");
         strcat(cmds,height);
         strcat(cmds,",");
         strcat(cmds,isShowText);
         strcat(cmds,",");
         strcat(cmds,"\"");
         strcat(cmds,barcodeLayer->content);
         strcat(cmds,"\"");
         strcat(cmds,"\n");
  break;
  case 11://code39
         strcat(cmds,"B");
         strcat(cmds,xpos);
         strcat(cmds,",");
         strcat(cmds,ypos);
         strcat(cmds,",");
         strcat(cmds,angle);
         strcat(cmds,",");
         strcat(cmds,"3,");
         strcat(cmds,narrow);
         strcat(cmds,",");
         strcat(cmds,wide);
         strcat(cmds,",");
         strcat(cmds,height);
         strcat(cmds,",");
         strcat(cmds,isShowText);
         strcat(cmds,",");
         strcat(cmds,"\"");
         strcat(cmds,barcodeLayer->content);
         strcat(cmds,"\"");
         strcat(cmds,"\n");
  break;
  case 12://code128a
         strcat(cmds,"B");
         strcat(cmds,xpos);
         strcat(cmds,",");
         strcat(cmds,ypos);
         strcat(cmds,",");
         strcat(cmds,angle);
         strcat(cmds,",");
         strcat(cmds,"1A,");
         strcat(cmds,narrow);
         strcat(cmds,",");
         strcat(cmds,wide);
         strcat(cmds,",");
         strcat(cmds,height);
         strcat(cmds,",");
         strcat(cmds,isShowText);
         strcat(cmds,",");
         strcat(cmds,"\"");
         strcat(cmds,barcodeLayer->content);
         strcat(cmds,"\"");
         strcat(cmds,"\n");
  break;
  case 13://code128b
         strcat(cmds,"B");
         strcat(cmds,xpos);
         strcat(cmds,",");
         strcat(cmds,ypos);
         strcat(cmds,",");
         strcat(cmds,angle);
         strcat(cmds,",");
         strcat(cmds,"1B,");
         strcat(cmds,narrow);
         strcat(cmds,",");
         strcat(cmds,wide);
         strcat(cmds,",");
         strcat(cmds,height);
         strcat(cmds,",");
         strcat(cmds,isShowText);
         strcat(cmds,",");
         strcat(cmds,"\"");
         strcat(cmds,barcodeLayer->content);
         strcat(cmds,"\"");
         strcat(cmds,"\n");
  break;
  case14://code128c
         strcat(cmds,"B");
         strcat(cmds,xpos);
         strcat(cmds,",");
         strcat(cmds,ypos);
         strcat(cmds,",");
         strcat(cmds,angle);
         strcat(cmds,",");
         strcat(cmds,"1C,");
         strcat(cmds,narrow);
         strcat(cmds,",");
         strcat(cmds,wide);
         strcat(cmds,",");
         strcat(cmds,height);
         strcat(cmds,",");
         strcat(cmds,isShowText);
         strcat(cmds,",");
         strcat(cmds,"\"");
         strcat(cmds,barcodeLayer->content);
         strcat(cmds,"\"");
         strcat(cmds,"\n");
  break;
  case 15://kix
  break;
  case 16://qr
         strcat(cmds,"b");
         strcat(cmds,xpos);
         strcat(cmds,",");
         strcat(cmds,ypos);
         strcat(cmds,",Q,");
         strcat(cmds,"\"");
         strcat(cmds,barcodeLayer->content);
         strcat(cmds,"\"");
         strcat(cmds,"\n");
  break;
  case 17://aztec
   sprintf(Scaling,"%d",(int)extAttr[3]);
   sprintf(ErrorLevels,"e%d",barcodeLayer->errorLevel);
   strcat(cmds,"b");
   strcat(cmds,xpos);
   strcat(cmds,",");
   strcat(cmds,ypos);
   strcat(cmds,",A,");
   if(extAttr[3]>0)
   {
    strcat(cmds,Scaling);
    strcat(cmds,",");
   }
   if(barcodeLayer->errorLevel>0)
   {
    strcat(cmds,ErrorLevels);
    strcat(cmds,",");
   }
   if(extAttr[2]>0)
    strcat(cmds,"f,");
   if(extAttr[0])
    strcat(cmds,"m,");
   if(extAttr[1]>0)
    strcat(cmds,"r,");
   
   strcat(cmds,"\"");
   strcat(cmds,barcodeLayer->content);
   strcat(cmds,"\"");
   strcat(cmds,"\n");


  break;
  case 18://codabar
         strcat(cmds,"B");
         strcat(cmds,xpos);
         strcat(cmds,",");
         strcat(cmds,ypos);
         strcat(cmds,",");
         strcat(cmds,angle);
         strcat(cmds,",");
         strcat(cmds,"K,");
         strcat(cmds,narrow);
         strcat(cmds,",");
         strcat(cmds,wide);
         strcat(cmds,",");
         strcat(cmds,height);
         strcat(cmds,",");
         strcat(cmds,isShowText);
         strcat(cmds,",");
         strcat(cmds,"\"");
         strcat(cmds,barcodeLayer->content);
         strcat(cmds,"\"");
         strcat(cmds,"\n");

  break;
  case 19://royalmail
  break;
  case 20://dm
         strcat(cmds,"b");
         strcat(cmds,xpos);
         strcat(cmds,",");
         strcat(cmds,ypos);
         strcat(cmds,",D,");
         strcat(cmds,"\"");
         strcat(cmds,barcodeLayer->content);
         strcat(cmds,"\"");
         strcat(cmds,"\n");
  break;
  case 21://pdf
         strcat(cmds,"b");
         strcat(cmds,xpos);
         strcat(cmds,",");
         strcat(cmds,ypos);
         strcat(cmds,",P,");
         strcat(cmds,width);
         strcat(cmds,",");
         strcat(cmds,height);
         strcat(cmds,",");
      
         strcat(cmds,"\"");
         strcat(cmds,barcodeLayer->content);
         strcat(cmds,"\"");
         strcat(cmds,"\n");
  break;
  case 22://hx
  break;
  case 23://gsqr
         strcat(cmds,"b");
         strcat(cmds,xpos);
         strcat(cmds,",");
         strcat(cmds,ypos);
         strcat(cmds,",Q,");
         strcat(cmds,"\"");
         strcat(cmds,barcodeLayer->content);
         strcat(cmds,"\"");
         strcat(cmds,"\n");
  break;
  case 24://code16k
  break;
  case 25://code49
  break;
  case 26://codeone
  break;
  case 27://dotcode
  break;
  case 28://gridmatrix
  break;
  case 29://maxicode
         strcat(cmds,"b");
         strcat(cmds,xpos);
         strcat(cmds,",");
         strcat(cmds,ypos);
         strcat(cmds,",M,");
         strcat(cmds,"\"");
         strcat(cmds,barcodeLayer->content);
         strcat(cmds,"\"");
         strcat(cmds,"\n");
  break;
  case 34://ukplessey
         strcat(cmds,"B");
         strcat(cmds,xpos);
         strcat(cmds,",");
         strcat(cmds,ypos);
         strcat(cmds,",");
         strcat(cmds,angle);
         strcat(cmds,",");
         strcat(cmds,"L,");
         strcat(cmds,narrow);
         strcat(cmds,",");
         strcat(cmds,wide);
         strcat(cmds,",");
         strcat(cmds,height);
         strcat(cmds,",");
         strcat(cmds,isShowText);
         strcat(cmds,",");
         strcat(cmds,"\"");
         strcat(cmds,barcodeLayer->content);
         strcat(cmds,"\"");
         strcat(cmds,"\n");
  break;
  case 42://I2OF5
         strcat(cmds,"B");
         strcat(cmds,xpos);
         strcat(cmds,",");
         strcat(cmds,ypos);
         strcat(cmds,",");
         strcat(cmds,angle);
         strcat(cmds,",");
         strcat(cmds,"2,");
         strcat(cmds,narrow);
         strcat(cmds,",");
         strcat(cmds,wide);
         strcat(cmds,",");
         strcat(cmds,height);
         strcat(cmds,",");
         strcat(cmds,isShowText);
         strcat(cmds,",");
         strcat(cmds,"\"");
         strcat(cmds,barcodeLayer->content);
         strcat(cmds,"\"");
         strcat(cmds,"\n");
  break;
  case 44://STAND2OF5
         strcat(cmds,"B");
         strcat(cmds,xpos);
         strcat(cmds,",");
         strcat(cmds,ypos);
         strcat(cmds,",");
         strcat(cmds,angle);
         strcat(cmds,",");
         strcat(cmds,"K,");
         strcat(cmds,narrow);
         strcat(cmds,",");
         strcat(cmds,wide);
         strcat(cmds,",");
         strcat(cmds,height);
         strcat(cmds,",");
         strcat(cmds,isShowText);
         strcat(cmds,",");
         strcat(cmds,"\"");
         strcat(cmds,barcodeLayer->content);
         strcat(cmds,"\"");
         strcat(cmds,"\n");
  break;
  case 47: case 48: case 49: case 50://AUSPOST
         strcat(cmds,"B");
         strcat(cmds,xpos);
         strcat(cmds,",");
         strcat(cmds,ypos);
         strcat(cmds,",");
         strcat(cmds,angle);
         strcat(cmds,",");
         strcat(cmds,"1D,");
         strcat(cmds,narrow);
         strcat(cmds,",");
         strcat(cmds,wide);
         strcat(cmds,",");
         strcat(cmds,height);
         strcat(cmds,",");
         strcat(cmds,isShowText);
         strcat(cmds,",");
         strcat(cmds,"\"");
         strcat(cmds,barcodeLayer->content);
         strcat(cmds,"\"");
         strcat(cmds,"\n");
  break;
  case 51://AZTECRUNES
         strcat(cmds,"b");
         strcat(cmds,xpos);
         strcat(cmds,",");
         strcat(cmds,ypos);
         strcat(cmds,",A,");
         strcat(cmds,"\"");
         strcat(cmds,barcodeLayer->content);
         strcat(cmds,"\"");
         strcat(cmds,"\n");

  break;
  case 53://CODE39EXTENDED
         strcat(cmds,"B");
         strcat(cmds,xpos);
         strcat(cmds,",");
         strcat(cmds,ypos);
         strcat(cmds,",");
         strcat(cmds,angle);
         strcat(cmds,",");
         strcat(cmds,"3,");
         strcat(cmds,narrow);
         strcat(cmds,",");
         strcat(cmds,wide);
         strcat(cmds,",");
         strcat(cmds,height);
         strcat(cmds,",");
         strcat(cmds,isShowText);
         strcat(cmds,",");
         strcat(cmds,"\"");
         strcat(cmds,barcodeLayer->content);
         strcat(cmds,"\"");
         strcat(cmds,"\n");
  break;
  case 55://DATABAREXPANDED
         strcat(cmds,"B");
         strcat(cmds,xpos);
         strcat(cmds,",");
         strcat(cmds,ypos);
         strcat(cmds,",");
         strcat(cmds,angle);
         strcat(cmds,",");
         strcat(cmds,"R14,");
         strcat(cmds,narrow);
         strcat(cmds,",");
         strcat(cmds,wide);
         strcat(cmds,",");
         strcat(cmds,height);
         strcat(cmds,",");
         strcat(cmds,isShowText);
         strcat(cmds,",");
         strcat(cmds,"\"");
         strcat(cmds,barcodeLayer->content);
         strcat(cmds,"\"");
         strcat(cmds,"\n");
  break;
  case 56://DATABAREXPANDEDSTACKED
         strcat(cmds,"B");
         strcat(cmds,xpos);
         strcat(cmds,",");
         strcat(cmds,ypos);
         strcat(cmds,",");
         strcat(cmds,angle);
         strcat(cmds,",");
         strcat(cmds,"RS,");
         strcat(cmds,narrow);
         strcat(cmds,",");
         strcat(cmds,wide);
         strcat(cmds,",");
         strcat(cmds,height);
         strcat(cmds,",");
         strcat(cmds,isShowText);
         strcat(cmds,",");
         strcat(cmds,"\"");
         strcat(cmds,barcodeLayer->content);
         strcat(cmds,"\"");
         strcat(cmds,"\n");
  break;
  case 57://DATABARLIMITED
         strcat(cmds,"B");
         strcat(cmds,xpos);
         strcat(cmds,",");
         strcat(cmds,ypos);
         strcat(cmds,",");
         strcat(cmds,angle);
         strcat(cmds,",");
         strcat(cmds,"RL,");
         strcat(cmds,narrow);
         strcat(cmds,",");
         strcat(cmds,wide);
         strcat(cmds,",");
         strcat(cmds,height);
         strcat(cmds,",");
         strcat(cmds,isShowText);
         strcat(cmds,",");
         strcat(cmds,"\"");
         strcat(cmds,barcodeLayer->content);
         strcat(cmds,"\"");
         strcat(cmds,"\n");
  break;
  case 59:case 60://DATABAROMNISTACKED DATABARSTACKED
         strcat(cmds,"B");
         strcat(cmds,xpos);
         strcat(cmds,",");
         strcat(cmds,ypos);
         strcat(cmds,",");
         strcat(cmds,angle);
         strcat(cmds,",");
         strcat(cmds,"RS,");
         strcat(cmds,narrow);
         strcat(cmds,",");
         strcat(cmds,wide);
         strcat(cmds,",");
         strcat(cmds,height);
         strcat(cmds,",");
         strcat(cmds,isShowText);
         strcat(cmds,",");
         strcat(cmds,"\"");
         strcat(cmds,barcodeLayer->content);
         strcat(cmds,"\"");
         strcat(cmds,"\n");
  break;
  case 61://DATABARTRUNCATED
         strcat(cmds,"B");
         strcat(cmds,xpos);
         strcat(cmds,",");
         strcat(cmds,ypos);
         strcat(cmds,",");
         strcat(cmds,angle);
         strcat(cmds,",");
         strcat(cmds,"RT,");
         strcat(cmds,narrow);
         strcat(cmds,",");
         strcat(cmds,wide);
         strcat(cmds,",");
         strcat(cmds,height);
         strcat(cmds,",");
         strcat(cmds,isShowText);
         strcat(cmds,",");
         strcat(cmds,"\"");
         strcat(cmds,barcodeLayer->content);
         strcat(cmds,"\"");
         strcat(cmds,"\n");
  break;
  case 72://PLANET
         strcat(cmds,"B");
         strcat(cmds,xpos);
         strcat(cmds,",");
         strcat(cmds,ypos);
         strcat(cmds,",");
         strcat(cmds,angle);
         strcat(cmds,",");
         strcat(cmds,"PL,");
         strcat(cmds,narrow);
         strcat(cmds,",");
         strcat(cmds,wide);
         strcat(cmds,",");
         strcat(cmds,height);
         strcat(cmds,",");
         strcat(cmds,isShowText);
         strcat(cmds,",");
         strcat(cmds,"\"");
         strcat(cmds,barcodeLayer->content);
         strcat(cmds,"\"");
         strcat(cmds,"\n");
  break;
  case 73://POSTNET
         strcat(cmds,"B");
         strcat(cmds,xpos);
         strcat(cmds,",");
         strcat(cmds,ypos);
         strcat(cmds,",");
         strcat(cmds,angle);
         strcat(cmds,",");
         strcat(cmds,"P,");
         strcat(cmds,narrow);
         strcat(cmds,",");
         strcat(cmds,wide);
         strcat(cmds,",");
         strcat(cmds,height);
         strcat(cmds,",");
         strcat(cmds,isShowText);
         strcat(cmds,",");
         strcat(cmds,"\"");
         strcat(cmds,barcodeLayer->content);
         strcat(cmds,"\"");
         strcat(cmds,"\n");
  break;
  case 74: case 76: case 78://MICROQRCODE
         strcat(cmds,"b");
         strcat(cmds,xpos);
         strcat(cmds,",");
         strcat(cmds,ypos);
         strcat(cmds,",Q,");
         strcat(cmds,"\"");
         strcat(cmds,barcodeLayer->content);
         strcat(cmds,"\"");
         strcat(cmds,"\n");
  break;
  case 79://HIBDM
         strcat(cmds,"b");
         strcat(cmds,xpos);
         strcat(cmds,",");
         strcat(cmds,ypos);
         strcat(cmds,",D,");
         strcat(cmds,"\"");
         strcat(cmds,barcodeLayer->content);
         strcat(cmds,"\"");
         strcat(cmds,"\n");
  break;
  case 82: case 83://AZTEC
         strcat(cmds,"b");
         strcat(cmds,xpos);
         strcat(cmds,",");
         strcat(cmds,ypos);
         strcat(cmds,",A,");
         strcat(cmds,"\"");
         strcat(cmds,barcodeLayer->content);
         strcat(cmds,"\"");
         strcat(cmds,"\n");
  break;
  case 84://FUN1QR
         strcat(cmds,"b");
         strcat(cmds,xpos);
         strcat(cmds,",");
         strcat(cmds,ypos);
         strcat(cmds,",Q,");
         strcat(cmds,"\"");
         strcat(cmds,barcodeLayer->content);
         strcat(cmds,"\"");
         strcat(cmds,"\n");
  break;
  case 85://FNC1DM
         strcat(cmds,"b");
         strcat(cmds,xpos);
         strcat(cmds,",");
         strcat(cmds,ypos);
         strcat(cmds,",D,");
         strcat(cmds,"\"");
         strcat(cmds,barcodeLayer->content);
         strcat(cmds,"\"");
         strcat(cmds,"\n");
  break;
  case 86://GS128
         strcat(cmds,"B");
         strcat(cmds,xpos);
         strcat(cmds,",");
         strcat(cmds,ypos);
         strcat(cmds,",");
         strcat(cmds,angle);
         strcat(cmds,",");
         strcat(cmds,"1E,");
         strcat(cmds,narrow);
         strcat(cmds,",");
         strcat(cmds,wide);
         strcat(cmds,",");
         strcat(cmds,height);
         strcat(cmds,",");
         strcat(cmds,isShowText);
         strcat(cmds,",");
         strcat(cmds,"\"");
         strcat(cmds,barcodeLayer->content);
         strcat(cmds,"\"");
         strcat(cmds,"\n");
  break;
  case 87://GS1QR
         strcat(cmds,"b");
         strcat(cmds,xpos);
         strcat(cmds,",");
         strcat(cmds,ypos);
         strcat(cmds,",Q,");
         strcat(cmds,"\"");
         strcat(cmds,barcodeLayer->content);
         strcat(cmds,"\"");
         strcat(cmds,"\n");
  break;
  case 88://GS1DM
         strcat(cmds,"b");
         strcat(cmds,xpos);
         strcat(cmds,",");
         strcat(cmds,ypos);
         strcat(cmds,",D,");
         strcat(cmds,"\"");
         strcat(cmds,barcodeLayer->content);
         strcat(cmds,"\"");
         strcat(cmds,"\n");

  break;
  case 89://GS1AZTEC
         strcat(cmds,"b");
         strcat(cmds,xpos);
         strcat(cmds,",");
         strcat(cmds,ypos);
         strcat(cmds,",A,");
         strcat(cmds,"\"");
         strcat(cmds,barcodeLayer->content);
         strcat(cmds,"\"");
         strcat(cmds,"\n");
  break;



  
  default:
         strcat(cmds,"B");
         strcat(cmds,xpos);
         strcat(cmds,",");
         strcat(cmds,ypos);
         strcat(cmds,",");
         strcat(cmds,angle);
         strcat(cmds,",");
         strcat(cmds,"1B,");
         strcat(cmds,narrow);
         strcat(cmds,",");
         strcat(cmds,wide);
         strcat(cmds,",");
         strcat(cmds,height);
         strcat(cmds,",");
         strcat(cmds,isShowText);
         strcat(cmds,",");
         strcat(cmds,"\"");
         strcat(cmds,barcodeLayer->content);
         strcat(cmds,"\"");
         strcat(cmds,"\n");
    break;
  }
  
  free(barcodeLayer->EncodingMode);
  free(barcodeLayer->DataMode);
  free(barcodeLayer->ExtAttr);
  free(barcodeLayer->supplenmentText);
  free(barcodeLayer->content);
  free(barcodeLayer->baseLayer->layerName);
  free(barcodeLayer->baseLayer);
  free(barcodeLayer);

  enqueue(EPLqueue,cmds);

}
void HCreateTextEPL(Queue*EPLqueue,pageConfig *page,TextLayer *textLayer)
{
  char *cmds= (char *)malloc(sizeof(char)*5000);
  char *fName =(char *)malloc(sizeof(char)*20);
  sprintf(fName, "\"%s\"", FONTNAME);

  char xpos[10];
  int x=HCreateDPIChange(page->dpiX,DPI,textLayer->baseLayer->x);
  sprintf(xpos, "%d", x);
  char ypos[10];
  int y=HCreateDPIChange(page->dpiY,DPI,textLayer->baseLayer->y);
  sprintf(ypos, "%d", y);
  int xmul=(int)(textLayer->font.FontSize/10);
  int ymul=(int)(textLayer->font.FontSize/10);
  if(xmul+ymul<=0)
  {
    xmul=1;
    ymul=1;
  }
  char angle[10];
  char xmultiplier[10];
  char ymultiplier[10];
  sprintf(angle, "%d", textLayer->baseLayer->Angle);
  sprintf(xmultiplier, "%d", xmul);
  sprintf(ymultiplier, "%d", ymul);
  strcat(cmds,"A");
  strcat(cmds,xpos);
  strcat(cmds,",");
  strcat(cmds,ypos);
  strcat(cmds,",");
  strcat(cmds,angle);
  strcat(cmds,",");
  strcat(cmds,fName);
  strcat(cmds,",");
  strcat(cmds,xmultiplier);
  strcat(cmds,",");
  strcat(cmds,ymultiplier);
   if(textLayer->color.b==255&&textLayer->color.r==255&&textLayer->color.g==255)//white
   {
    //reverse
     strcat(cmds,",R,");
   }
   else
   {
      strcat(cmds,",N,");
   }
  strcat(cmds,"\"");
  strcat(cmds,textLayer->content);
  strcat(cmds,"\"");
  strcat(cmds,"\n");
  free(textLayer->font.Style);
  free(textLayer->font.FontName);
  free(textLayer->content);
  free(textLayer->baseLayer->layerName);
  free(textLayer->baseLayer);
  free(fName);
  free(textLayer);

  enqueue(EPLqueue,cmds);

}
void HCreateLineEPL(Queue*EPLqueue,pageConfig *page,LineLayer *lineLayer)
{
   char *cmds= (char *)malloc(sizeof(char)*5000);

   char xpos[10];
   int x=HCreateDPIChange(page->dpiX,DPI,lineLayer->baseLayer->x);
   sprintf(xpos, "%d,", x);
   char ypos[10];
   int y=HCreateDPIChange(page->dpiY,DPI,lineLayer->baseLayer->y);
   sprintf(ypos, "%d,", y);
    char lineWidth[10];
    char lineHeight[10];
    if(lineLayer->linDirect==1)
    {
      int width=HCreateDPIChange(page->dpiX,DPI,lineLayer->lineLength);
      sprintf(lineWidth, "%d,", width);
      int height=HCreateDPIChange(page->dpiY,DPI,lineLayer->lineWidth);
      sprintf(lineHeight, "%d,", height);
    }else
    {
      int width=HCreateDPIChange(page->dpiX,DPI,lineLayer->lineWidth);
      sprintf(lineWidth, "%d,", width);
      int height=HCreateDPIChange(page->dpiY,DPI,lineLayer->lineLength);
      sprintf(lineHeight, "%d,", height);
    }
     strcat(cmds,"LO");
     strcat(cmds,xpos);
     strcat(cmds,",");
     strcat(cmds,ypos);
     strcat(cmds,",");
     strcat(cmds,lineWidth);
     strcat(cmds,",");
     strcat(cmds,lineHeight);
     strcat(cmds,"\n");


  free(lineLayer->baseLayer->layerName);
  
  free(lineLayer->baseLayer);

  free(lineLayer);
  enqueue(EPLqueue,cmds);
}
void HCreateRectangleEPL(Queue*EPLqueue,pageConfig *page,RectangleLayer *rectLayer)
{
  free(rectLayer->baseLayer->layerName);
  free(rectLayer->baseLayer);
  free(rectLayer);
}
void HCreateEllipseEPL(Queue*EPLqueue,pageConfig *page,EllipseLayer *ellipseLayer)
{
   free(ellipseLayer->baseLayer->layerName);
   free(ellipseLayer->baseLayer);
   free(ellipseLayer);
}
void HCreateImageEPL(Queue*EPLqueue,pageConfig *page,ImageLayer *imageLayer)
{
  free(imageLayer->img);
  free(imageLayer->baseLayer->layerName);
  
  free(imageLayer->baseLayer);

  free(imageLayer);
}
void HCreateRfidEPL(Queue*EPLqueue,pageConfig *page,RfidLayer *rfidLayer)
{
    free(rfidLayer->baseLayer->layerName);
    free(rfidLayer->RfidContent);
    free(rfidLayer->baseLayer);
    free(rfidLayer);
}
void HCreatePrintEPL(Queue*EPLqueue,pageConfig *page,u_int32_t n)
{
    char *cmds=(char *)malloc(sizeof(char)*100);

    strcat(cmds,"P");
    char m_s[10];
    sprintf(m_s, " %d\n", n);
    strcat(cmds,m_s);
    enqueue(EPLqueue,cmds);
}