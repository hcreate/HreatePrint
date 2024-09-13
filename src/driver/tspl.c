#include"tspl.h"

void printTSPL(char *p)
{
  // printf("%s",p);
  Queue* TSPLqueue=createQueue();
  pageConfig *page=getPageConfigTSPL(p);
  HCreateDirectionTSPL(TSPLqueue,page,1);
  HCreateClearBufferTSPL(TSPLqueue,page);
  HCreateSizeTSPL(TSPLqueue,page,0);
  HCreateGapTSPL(TSPLqueue,page,0);
  HCreateObjectTSPL(TSPLqueue,page);
  HCreatePrintTSPL(TSPLqueue,page,1);
//输出打印，并释放内存
  while(!isEmpty(TSPLqueue))
   {
     char *data=dequeue(TSPLqueue);
     printf("%s",data);
     free(data);

   }
   free(TSPLqueue);
   freePage(page);
  //printf("%s\n",cmds);
}
pageConfig *getPageConfigTSPL(char *p)
{

  FILE *file=open(p);
  Queue* queue=  readToQueue(file);
  pageConfig *page=NULL;
  page= prase(queue);
  return page;
}
void HCreateDirectionTSPL(Queue*TSPLqueue,pageConfig *page,u_int8_t n)
{
    char *cmds=(char *)malloc(sizeof(char)*100);

    strcat(cmds,"DIRECTION");
    char n_s[10];
    sprintf(n_s, " %d\n", n);
    strcat(cmds,n_s);
    enqueue(TSPLqueue,cmds);
}
void HCreatePrintTSPL(Queue*TSPLqueue,pageConfig *page,u_int8_t m)
{
    char *cmds=(char *)malloc(sizeof(char)*100);

    strcat(cmds,"PRINT");
    char m_s[10];
    sprintf(m_s, " %d\n", m);
    strcat(cmds,m_s);
    enqueue(TSPLqueue,cmds);

}
void HCreateClearBufferTSPL(Queue*TSPLqueue,pageConfig *page)
{
   char *cmds=(char *)malloc(sizeof(char)*100);
   strcat(cmds,"CLS\n");
    enqueue(TSPLqueue,cmds);
}
void HCreateSizeTSPL(Queue*TSPLqueue,pageConfig *page,u_int8_t type)
{
  
  float m=pixelToMillimeter(page->dpiX,page->pageWidth);
  float n=pixelToMillimeter(page->dpiY,page->pageHeight);
  
  char *cmds=(char *)malloc(sizeof(char)*100);
  char m_s[20];
  char n_s[20];
  switch (type)
  {
  case 0:
    /* code */
    strcat(cmds,"SIZE");

    sprintf(m_s, " %.2f mm,", m);
    sprintf(n_s, " %.2f mm", n);
    strcat(cmds,m_s);
    strcat(cmds,n_s);
    strcat(cmds,"\n");

    break;
      case 1:
    /* code */
    strcat(cmds,"SIZE");

    sprintf(m_s, " %.2f,", m);
    sprintf(n_s, " %.2f", n);
    strcat(cmds,m_s);
    strcat(cmds,n_s);
    strcat(cmds,"\n");
    break;
      case 2:
    /* code */
    strcat(cmds,"SIZE");
    int w=HCreateDPIChange(page->dpiX,DPI,page->pageWidth);
    int h=HCreateDPIChange(page->dpiY,DPI,page->pageHeight);
    sprintf(m_s, " %d dot,", w);
    sprintf(n_s, " %d dot", h);
    strcat(cmds,m_s);
    strcat(cmds,n_s);
    strcat(cmds,"\n");
    break;
  
  default:
    break;
  }
  enqueue(TSPLqueue,cmds);
}
void HCreateGapTSPL(Queue*TSPLqueue,pageConfig *page,u_int8_t type)
{
    
  float m=pixelToMillimeter(page->dpiX,page->pageVerticalSpacing);
  float n=pixelToMillimeter(page->dpiY,page->pageHorizontalSpacing);

  char *cmds=(char *)malloc(sizeof(char)*100);
  char m_s[20];
  char n_s[20];
  if(m==0&&n==0)
  {
    strcat(cmds,"GAP");
    sprintf(m_s, " %.2f,", m);
    sprintf(n_s, " %.2f", n);
    strcat(cmds,m_s);
    strcat(cmds,n_s);
    strcat(cmds,"\n");
  }else
  {
    switch (type)
    {
      case 0:
    /* code */
      strcat(cmds,"GAP");

      sprintf(m_s, " %d mm,", m);
      sprintf(n_s, " %d mm", n);
      strcat(cmds,m_s);
      strcat(cmds,n_s);
      strcat(cmds,"\n");

      break;
      case 1:
    /* code */
      strcat(cmds,"GAP");

      sprintf(m_s, " %.2f", m);
      sprintf(n_s, " %.2f", n);
      strcat(cmds,m_s);
      strcat(cmds,n_s);
      strcat(cmds,"\n");
      break;
      case 2:
    /* code */
      strcat(cmds,"GAP");
      int w=HCreateDPIChange(page->dpiX,DPI,page->pageVerticalSpacing);
      int h=HCreateDPIChange(page->dpiY,DPI,page->pageHorizontalSpacing);
      sprintf(m_s, " %d dot,", w);
      sprintf(n_s, " %d dot", h);
      strcat(cmds,m_s);
      strcat(cmds,n_s);
      strcat(cmds,"\n");
      break;
  
    default:
      break;
    }
   
  }

  enqueue(TSPLqueue,cmds);
}
void HCreateObjectTSPL(Queue*TSPLqueue,pageConfig *page)
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
      HCreateTextTSPL(TSPLqueue,page,current->layerData);

      break;
    case Barcode:
     //BarcodeLayer *barcodeObject=(BarcodeLayer *)current->layerData;
      /* code */
      HCreateBarcodeTSPL(TSPLqueue,page,current->layerData);
    break;
    case Rfid:
    // RfidLayer *rfidObject=(RfidLayer *)current->layerData;
     HCreateRfidTSPL(TSPLqueue,page,current->layerData);
      /* code */
    break;
    case Image:
     //ImageLayer *imageObject=(ImageLayer *)current->layerData;
      /* code */
      HCreateImageTSPL(TSPLqueue,page,current->layerData);
    break;
    case Line:
     //LineLayer *lineObject=(LineLayer *)current->layerData;
      HCreateLineTSPL(TSPLqueue,page,current->layerData);
      /* code */
    break;
    case Rectangle:
    // RectangleLayer *rectangleObject=(RectangleLayer *)current->layerData;
      /* code */
      HCreateRectangleTSPL(TSPLqueue,page,current->layerData);
    break;
    case Ellipse:
    //EllipseLayer *ellipseObject=(EllipseLayer *)current->layerData;
     HCreateEllipseTSPL(TSPLqueue,page,current->layerData);
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
void HCreateBarcodeTSPL(Queue*TSPLqueue,pageConfig *page,BarcodeLayer *barcodeLayer)
{
   char *cmds= (char *)malloc(sizeof(char)*5000);
   int alignment=0;
   char xpos[10];
   int x=HCreateDPIChange(page->dpiX,DPI,barcodeLayer->baseLayer->x);
   sprintf(xpos, "%d,", x);
   char ypos[10];
   int y=HCreateDPIChange(page->dpiY,DPI,barcodeLayer->baseLayer->y);
   sprintf(ypos, "%d,", y);
   char xdim[10];
   int xdimension=millimeterToPixel(DPI,barcodeLayer->XDimension);
   sprintf(xdim, "%d,", xdimension);
   int h=HCreateDPIChange(page->dpiY,DPI,barcodeLayer->baseLayer->height);
   char height[10];
   sprintf(height, "%d,", h);
   int w=HCreateDPIChange(page->dpiY,DPI,barcodeLayer->baseLayer->width);
   char width[10];
   sprintf(width,"%d,",w);

   char errorLevel[10];
   sprintf(errorLevel, "%d,", barcodeLayer->errorLevel);
  

   unsigned char extAttr[10];
     //QR码字段定义
   char modlparam[10];
   char elevel[10];
   char maskno[10];
   char charactermode[10];
   char inputmodel[10];
   int count=base64_decode(barcodeLayer->ExtAttr,extAttr);

   int readable_aglinment=2;
   int narrow=2;
   int wide=2;
   int ratio=0;
   switch (barcodeLayer->baseLayer->Angle)
   {
    case 0:
    /* code */
    ratio=0;
    break;
     case 1:
    /* code */
    ratio=90;
    break;
     case 2:
    /* code */
    ratio=180;
    break;
     case 3:
    /* code */
    ratio=270;
    break;
   
   default:
       ratio=0;
    break;
   }
   if(barcodeLayer->isShowText==1)
   {
      switch (barcodeLayer->ta)
      {
      case Left:
        /* code */
      readable_aglinment=1;
        break;
      case Right:
        /* code */
      readable_aglinment=3;
        break;
      case Center:
        /* code */
        readable_aglinment=2;
        break;
      case Stretched:
        /* code */
       readable_aglinment=2;
        break;
      
      default:
        break;
      }

      switch (extAttr[0])
      {
       case 0x00:
        narrow=xdimension;
        wide=xdimension;
       break;
       case 0x01:
        narrow=xdimension;
        wide=(int)(xdimension*2.5);
       break;
       case 0x02:
        narrow=xdimension;
        wide=(int)(xdimension*3);
       break;
       case 0x03:
       break;
       case 0x04:
        narrow=xdimension;
        wide=xdimension*2;
       break;
       case 0x05:
       break;
       case 0x06:
       break;
       case 0x07:
       break;
       case 0x08:
       break;
       case 0x09:
       break;
       case 0x0a:
       break;
       case 0x0b:
       break;
       case 0x0c:
       break;
       case 0x0d:
       break;
       case 0x0e:
       break;
   
   
       default:
       narrow=xdimension;
        wide=xdimension;
       break;
      }
   }else
   {
    readable_aglinment=0;
   }
    char s_readable_aglinment[10];
    char s_narrow[10];
    char s_wide[10];
    char s_ratio[10];
    char s_alignment[10];
    sprintf(s_readable_aglinment, "%d,", readable_aglinment);
    sprintf(s_narrow, "%d,", narrow);
    sprintf(s_wide, "%d,", wide);
    sprintf(s_ratio, "%d,", ratio);
    sprintf(s_alignment, "%d,", alignment);


    char s_temp[10];
    int  b_temp;
   strcat(cmds,"BARCODE ");
   switch (barcodeLayer->codeType)
   {
    case 0://ean13
       if(isStringEmpty(barcodeLayer->supplenmentText)==1)//无supplenmentText
       {
        
         strcat(cmds,xpos);
         strcat(cmds,ypos);
         strcat(cmds,"\"EAN13\",");
         strcat(cmds,height);
         strcat(cmds,s_readable_aglinment);
         strcat(cmds,s_ratio);
         strcat(cmds,s_narrow);
         strcat(cmds,s_wide);
         
         if(alignment>0)
         strcat(cmds,s_alignment);
         strcat(cmds,"\"");
         strcat(cmds,barcodeLayer->content);
         strcat(cmds,"\"");
         strcat(cmds,"\n");

       }else if(strlen(barcodeLayer->supplenmentText)==2)
       {
         strcat(cmds,xpos);
         strcat(cmds,ypos);
         strcat(cmds,"\"EAN13+2\",");
         strcat(cmds,height);
         strcat(cmds,s_readable_aglinment);
         strcat(cmds,s_ratio);
         strcat(cmds,s_narrow);
         strcat(cmds,s_wide);

         if(alignment>0)
         strcat(cmds,s_alignment);
         strcat(cmds,"\"");
         strcat(cmds,barcodeLayer->content);
         strcat(cmds,barcodeLayer->supplenmentText);
         strcat(cmds,"\"");
         strcat(cmds,"\n"); 
       }else if(strlen(barcodeLayer->supplenmentText)==5)
       {
         strcat(cmds,xpos);
         strcat(cmds,ypos);
         strcat(cmds,"\"EAN13+5\",");
         strcat(cmds,height);
         strcat(cmds,s_readable_aglinment);
         strcat(cmds,s_ratio);
         strcat(cmds,s_narrow);
         strcat(cmds,s_wide);
   
         if(alignment>0)
         strcat(cmds,s_alignment);
         strcat(cmds,"\"");
         strcat(cmds,barcodeLayer->content);
         strcat(cmds,"\"");
         strcat(cmds,barcodeLayer->supplenmentText);
         strcat(cmds,"\n"); 
       }

    break;
  case 2://upc
    if(isStringEmpty(barcodeLayer->supplenmentText)==1)//无supplenmentText
       {
        
         strcat(cmds,xpos);
         strcat(cmds,ypos);
         strcat(cmds,"\"UPCA\",");
         strcat(cmds,height);
         strcat(cmds,s_readable_aglinment);
         strcat(cmds,s_ratio);
         strcat(cmds,s_narrow);
         strcat(cmds,s_wide);
       
         if(alignment>0)
         strcat(cmds,s_alignment);
         strcat(cmds,"\"");
         strcat(cmds,barcodeLayer->content);
         strcat(cmds,"\"");
         strcat(cmds,"\n");

       }else if(strlen(barcodeLayer->supplenmentText)==2)
       {
         strcat(cmds,xpos);
         strcat(cmds,ypos);
         strcat(cmds,"\"UPCA+2\",");
         strcat(cmds,height);
         strcat(cmds,s_readable_aglinment);
         strcat(cmds,s_ratio);
         strcat(cmds,s_narrow);
         strcat(cmds,s_wide);
      
         if(alignment>0)
         strcat(cmds,s_alignment);
         strcat(cmds,"\"");
         strcat(cmds,barcodeLayer->content);
         strcat(cmds,barcodeLayer->supplenmentText);
         strcat(cmds,"\"");
         strcat(cmds,"\n"); 
       }else if(strlen(barcodeLayer->supplenmentText)==5)
       {
         strcat(cmds,xpos);
         strcat(cmds,ypos);
         strcat(cmds,"\"UPCA+5\",");
         strcat(cmds,height);
         strcat(cmds,s_readable_aglinment);
         strcat(cmds,s_ratio);
         strcat(cmds,s_narrow);
         strcat(cmds,s_wide);
        
         if(alignment>0)
         strcat(cmds,s_alignment);
         strcat(cmds,"\"");
         strcat(cmds,barcodeLayer->content);
         strcat(cmds,barcodeLayer->supplenmentText);
         strcat(cmds,"\"");
         strcat(cmds,"\n"); 
       }

  break;
  case 3:
       if(isStringEmpty(barcodeLayer->supplenmentText)==1)//无supplenmentText
       {
        
         strcat(cmds,xpos);
         strcat(cmds,ypos);
         strcat(cmds,"\"UPCE\",");
         strcat(cmds,height);
         strcat(cmds,s_readable_aglinment);
         strcat(cmds,s_ratio);
         strcat(cmds,s_narrow);
         strcat(cmds,s_wide);
  
         if(alignment>0)
         strcat(cmds,s_alignment);
         strcat(cmds,"\"");
         strcat(cmds,barcodeLayer->content);
         strcat(cmds,"\"");
         strcat(cmds,"\n");

       }else if(strlen(barcodeLayer->supplenmentText)==2)
       {
         strcat(cmds,xpos);
         strcat(cmds,ypos);
         strcat(cmds,"\"UPCE+2\",");
         strcat(cmds,height);
         strcat(cmds,s_readable_aglinment);
         strcat(cmds,s_ratio);
         strcat(cmds,s_narrow);
         strcat(cmds,s_wide);
 
         if(alignment>0)
         strcat(cmds,s_alignment);
         strcat(cmds,"\"");
         strcat(cmds,barcodeLayer->content);
         strcat(cmds,barcodeLayer->supplenmentText);
         strcat(cmds,"\"");
         strcat(cmds,"\n"); 
       }else if(strlen(barcodeLayer->supplenmentText)==5)
       {
         strcat(cmds,xpos);
         strcat(cmds,ypos);
         strcat(cmds,"\"UPCE+5\",");
         strcat(cmds,height);
         strcat(cmds,s_readable_aglinment);
         strcat(cmds,s_ratio);
         strcat(cmds,s_narrow);
         strcat(cmds,s_wide);

         if(alignment>0)
         strcat(cmds,s_alignment);
         strcat(cmds,"\"");
         strcat(cmds,barcodeLayer->content);
         strcat(cmds,barcodeLayer->supplenmentText);
         strcat(cmds,"\"");
         strcat(cmds,"\n"); 
       }

  break;
  case 1:
   if(isStringEmpty(barcodeLayer->supplenmentText)==1)//无supplenmentText
       {
        
         strcat(cmds,xpos);
         strcat(cmds,ypos);
         strcat(cmds,"\"EAN8\",");
         strcat(cmds,height);
         strcat(cmds,s_readable_aglinment);
         strcat(cmds,s_ratio);
         strcat(cmds,s_narrow);
         strcat(cmds,s_wide);
    
         if(alignment>0)
         strcat(cmds,s_alignment);
         strcat(cmds,"\"");
         strcat(cmds,barcodeLayer->content);
         strcat(cmds,"\"");
         strcat(cmds,"\n");

       }else if(strlen(barcodeLayer->supplenmentText)==2)
       {
         strcat(cmds,xpos);
         strcat(cmds,ypos);
         strcat(cmds,"\"EAN8+2\",");
         strcat(cmds,height);
         strcat(cmds,s_readable_aglinment);
         strcat(cmds,s_ratio);
         strcat(cmds,s_narrow);
         strcat(cmds,s_wide);

         if(alignment>0)
         strcat(cmds,s_alignment);
         strcat(cmds,"\"");
         strcat(cmds,barcodeLayer->content);
         strcat(cmds,barcodeLayer->supplenmentText);
         strcat(cmds,"\"");
         strcat(cmds,"\n"); 
       }else if(strlen(barcodeLayer->supplenmentText)==5)
       {
         strcat(cmds,xpos);
         strcat(cmds,ypos);
         strcat(cmds,"\"EAN8+5\",");
         strcat(cmds,height);
         strcat(cmds,s_readable_aglinment);
         strcat(cmds,s_ratio);
         strcat(cmds,s_narrow);
         strcat(cmds,s_wide);
      
         if(alignment>0)
         strcat(cmds,s_alignment);
         strcat(cmds,"\"");
         strcat(cmds,barcodeLayer->content);
         strcat(cmds,barcodeLayer->supplenmentText);
         strcat(cmds,"\"");
         strcat(cmds,"\n"); 
       }

  break;
  case 4:
         strcat(cmds,xpos);
         strcat(cmds,ypos);
         strcat(cmds,"\"ITF14\",");
         strcat(cmds,height);
         strcat(cmds,s_readable_aglinment);
         strcat(cmds,s_ratio);
         strcat(cmds,s_narrow);
         strcat(cmds,s_wide);
    
         if(alignment>0)
          strcat(cmds,s_alignment);
         strcat(cmds,"\"");
         strcat(cmds,barcodeLayer->content);
         strcat(cmds,"\"");
         strcat(cmds,"\n");
  break;
  case 11:
         strcat(cmds,xpos);
         strcat(cmds,ypos);
         strcat(cmds,"\"39\",");
         strcat(cmds,height);
         strcat(cmds,s_readable_aglinment);
         strcat(cmds,s_ratio);
         strcat(cmds,s_narrow);
         strcat(cmds,s_wide);
  
         if(alignment>0)
         strcat(cmds,s_alignment);
         strcat(cmds,"\"");
         strcat(cmds,barcodeLayer->content);
         strcat(cmds,"\"");
         strcat(cmds,"\n");
  break;
  case 31:
         strcat(cmds,xpos);
         strcat(cmds,ypos);
         strcat(cmds,"\"TELEPEN\",");
         strcat(cmds,height);
         strcat(cmds,s_readable_aglinment);
         strcat(cmds,s_ratio);
         strcat(cmds,s_narrow);
         strcat(cmds,s_wide);

         if(alignment>0)
         strcat(cmds,s_alignment);
         strcat(cmds,"\"");
         strcat(cmds,barcodeLayer->content);
         strcat(cmds,"\"");
         strcat(cmds,"\n");
  break;
    case 32:
         strcat(cmds,xpos);
         strcat(cmds,ypos);
         strcat(cmds,"\"TELEPENN\",");
         strcat(cmds,height);
         strcat(cmds,s_readable_aglinment);
         strcat(cmds,s_ratio);
         strcat(cmds,s_narrow);
         strcat(cmds,s_wide);
       
         if(alignment>0)
         strcat(cmds,s_alignment);
         strcat(cmds,"\"");
         strcat(cmds,barcodeLayer->content);
         strcat(cmds,"\"");
         strcat(cmds,"\n");
  break;
    case 72:
         strcat(cmds,xpos);
         strcat(cmds,ypos);
         strcat(cmds,"\"PLANET\",");
         strcat(cmds,height);
         strcat(cmds,s_readable_aglinment);
         strcat(cmds,s_ratio);
         strcat(cmds,s_narrow);
         strcat(cmds,s_wide);
         
         if(alignment>0)
         strcat(cmds,s_alignment);
         strcat(cmds,"\"");
         strcat(cmds,barcodeLayer->content);
         strcat(cmds,"\"");
         strcat(cmds,"\n");
  break;
  case 39:
         strcat(cmds,xpos);
         strcat(cmds,ypos);
         strcat(cmds,"\"11\",");
         strcat(cmds,height);
         strcat(cmds,s_readable_aglinment);
         strcat(cmds,s_ratio);
         strcat(cmds,s_narrow);
         strcat(cmds,s_wide);
         
         if(alignment>0)
         strcat(cmds,s_alignment);
         strcat(cmds,"\"");
         strcat(cmds,barcodeLayer->content);
         strcat(cmds,"\"");
         strcat(cmds,"\n");
  break;
  case 9:
         strcat(cmds,xpos);
         strcat(cmds,ypos);
         strcat(cmds,"\"93\",");
         strcat(cmds,height);
         strcat(cmds,s_readable_aglinment);
         strcat(cmds,s_ratio);
         strcat(cmds,s_narrow);
         strcat(cmds,s_wide);
         
         if(alignment>0)
         strcat(cmds,s_alignment);
         strcat(cmds,"\"");
         strcat(cmds,barcodeLayer->content);
         strcat(cmds,"\"");
         strcat(cmds,"\n");
  break;
  case 7:
         strcat(cmds,xpos);
         strcat(cmds,ypos);
         strcat(cmds,"\"EAN128\",");
         strcat(cmds,height);
         strcat(cmds,s_readable_aglinment);
         strcat(cmds,s_ratio);
         strcat(cmds,s_narrow);
         strcat(cmds,s_wide);
         
         if(alignment>0)
         strcat(cmds,s_alignment);
         strcat(cmds,"\"");
         strcat(cmds,barcodeLayer->content);
         strcat(cmds,"\"");
         strcat(cmds,"\n");
  break;
  case 42:
         strcat(cmds,xpos);
         strcat(cmds,ypos);
         strcat(cmds,"\"25\",");
         strcat(cmds,height);
         strcat(cmds,s_readable_aglinment);
         strcat(cmds,s_ratio);
         strcat(cmds,s_narrow);
         strcat(cmds,s_wide);
         
         if(alignment>0)
         strcat(cmds,s_alignment);
         strcat(cmds,"\"");
         strcat(cmds,barcodeLayer->content);
         strcat(cmds,"\"");
         strcat(cmds,"\n");
  break;
  case 25:
         strcat(cmds,xpos);
         strcat(cmds,ypos);
         strcat(cmds,"\"CODE49\",");
         strcat(cmds,height);
         strcat(cmds,s_readable_aglinment);
         strcat(cmds,s_ratio);
         strcat(cmds,s_narrow);
         strcat(cmds,s_wide);
         
         if(alignment>0)
         strcat(cmds,s_alignment);
         strcat(cmds,"\"");
         strcat(cmds,barcodeLayer->content);
         strcat(cmds,"\"");
         strcat(cmds,"\n");
  break;
  case 44:
         strcat(cmds,xpos);
         strcat(cmds,ypos);
         strcat(cmds,"\"25C\",");
         strcat(cmds,height);
         strcat(cmds,s_readable_aglinment);
         strcat(cmds,s_ratio);
         strcat(cmds,s_narrow);
         strcat(cmds,s_wide);
         
         if(alignment>0)
         strcat(cmds,s_alignment);
         strcat(cmds,"\"");
         strcat(cmds,barcodeLayer->content);
         strcat(cmds,"\"");
         strcat(cmds,"\n");
  break;
  case 62:
         strcat(cmds,xpos);
         strcat(cmds,ypos);
         strcat(cmds,"\"DPI\",");
         strcat(cmds,height);
         strcat(cmds,s_readable_aglinment);
         strcat(cmds,s_ratio);
         strcat(cmds,s_narrow);
         strcat(cmds,s_wide);
         
         if(alignment>0)
         strcat(cmds,s_alignment);
         strcat(cmds,"\"");
         strcat(cmds,barcodeLayer->content);
         strcat(cmds,"\"");
         strcat(cmds,"\n");
  break;
    case 63:
         strcat(cmds,xpos);
         strcat(cmds,ypos);
         strcat(cmds,"\"DPL\",");
         strcat(cmds,height);
         strcat(cmds,s_readable_aglinment);
         strcat(cmds,s_ratio);
         strcat(cmds,s_narrow);
         strcat(cmds,s_wide);
         
         if(alignment>0)
         strcat(cmds,s_alignment);
         strcat(cmds,"\"");
         strcat(cmds,barcodeLayer->content);
         strcat(cmds,"\"");
         strcat(cmds,"\n");
  break;
    case 12: case 13: case 14:
         strcat(cmds,xpos);
         strcat(cmds,ypos);
         strcat(cmds,"\"128\",");
         strcat(cmds,height);
         strcat(cmds,s_readable_aglinment);
         strcat(cmds,s_ratio);
         strcat(cmds,s_narrow);
         strcat(cmds,s_wide);
         
         if(alignment>0)
         strcat(cmds,s_alignment);
         strcat(cmds,"\"");
         strcat(cmds,barcodeLayer->content);
         strcat(cmds,"\"");
         strcat(cmds,"\n");
  break;
   case 18: 
         strcat(cmds,xpos);
         strcat(cmds,ypos);
         strcat(cmds,"\"CODA\",");
         strcat(cmds,height);
         strcat(cmds,s_readable_aglinment);
         strcat(cmds,s_ratio);
         strcat(cmds,s_narrow);
         strcat(cmds,s_wide);
         
         if(alignment>0)
         strcat(cmds,s_alignment);
         strcat(cmds,"\"");
         strcat(cmds,barcodeLayer->content);
         strcat(cmds,"\"");
         strcat(cmds,"\n");
   break;
   case 73:
         strcat(cmds,xpos);
         strcat(cmds,ypos);
         strcat(cmds,"\"POST\",");
         strcat(cmds,height);
         strcat(cmds,s_readable_aglinment);
         strcat(cmds,s_ratio);
         strcat(cmds,s_narrow);
         strcat(cmds,s_wide);
         
         if(alignment>0)
         strcat(cmds,s_alignment);
         strcat(cmds,"\"");
         strcat(cmds,barcodeLayer->content);
         strcat(cmds,"\"");
         strcat(cmds,"\n");
   break;
   case 70:
         strcat(cmds,xpos);
         strcat(cmds,ypos);
         strcat(cmds,"\"MSI\",");
         strcat(cmds,height);
         strcat(cmds,s_readable_aglinment);
         strcat(cmds,s_ratio);
         strcat(cmds,s_narrow);
         strcat(cmds,s_wide);
         
         if(alignment>0)
         strcat(cmds,s_alignment);
         strcat(cmds,"\"");
         strcat(cmds,barcodeLayer->content);
         strcat(cmds,"\"");
         strcat(cmds,"\n");
   break;
   case 21:case 71:
     strcat(cmds,"\"PDF417\",");
     strcat(cmds,xpos);
     strcat(cmds,ypos);
     strcat(cmds,width);
     strcat(cmds,height);
     strcat(cmds,s_ratio);
     strcat(cmds,"P0,");
     strcat(cmds,errorLevel);
     strcat(cmds,"W");
     strcat(cmds,xdim);
     strcat(cmds,"H");
     strcat(cmds,xdim);
     memset(s_temp,0,sizeof(s_temp));
     sprintf(s_temp, "R%d,", extAttr[1]);
     strcat(cmds,s_temp);
     memset(s_temp,0,sizeof(s_temp));
     sprintf(s_temp, "C%d,", extAttr[0]);
     strcat(cmds,s_temp);
     memset(s_temp,0,sizeof(s_temp));
     sprintf(s_temp, "T%d,", extAttr[3]);
     strcat(cmds,s_temp);
     strcat(cmds,"\"");
     strcat(cmds,barcodeLayer->content);
     strcat(cmds,"\"");
    strcat(cmds,"\n");

   break;
   case 29:
     strcat(cmds,"\"MAXICODE\",");
     strcat(cmds,xpos);
     strcat(cmds,ypos);   
     strcat(cmds,"\"");
     strcat(cmds,barcodeLayer->content);
     strcat(cmds,"\"");
     strcat(cmds,"\n");
   break;
   case 45:
         strcat(cmds,xpos);
         strcat(cmds,ypos);
         strcat(cmds,"\"LOGMARS\",");
         strcat(cmds,height);
         strcat(cmds,s_readable_aglinment);
         strcat(cmds,s_ratio);
         strcat(cmds,s_narrow);
         strcat(cmds,s_wide);
         
         if(alignment>0)
         strcat(cmds,s_alignment);
         strcat(cmds,"\"");
         strcat(cmds,barcodeLayer->content);
         strcat(cmds,"\"");
         strcat(cmds,"\n");
   break;
   case 33:
         strcat(cmds,xpos);
         strcat(cmds,ypos);
         strcat(cmds,"\"EAN14\",");
         strcat(cmds,height);
         strcat(cmds,s_readable_aglinment);
         strcat(cmds,s_ratio);
         strcat(cmds,s_narrow);
         strcat(cmds,s_wide);
        
         if(alignment>0)
         strcat(cmds,s_alignment);
         strcat(cmds,"\"");
         strcat(cmds,barcodeLayer->content);
         strcat(cmds,"\"");
         strcat(cmds,"\n");
   break;
   case 17:
     strcat(cmds,"\"AZTEC\",");
     strcat(cmds,xpos);
     strcat(cmds,ypos);
     strcat(cmds,s_ratio);
     strcat(cmds,xdim);
     strcat(cmds,"0,");
    
     memset(s_temp,0,sizeof(s_temp));
     sprintf(s_temp, "%d,", extAttr[2]);//FLG
     strcat(cmds,s_temp);

     memset(s_temp,0,sizeof(s_temp));
     sprintf(s_temp, "%d,", extAttr[0]);//MENU
     strcat(cmds,s_temp);

     memset(s_temp,0,sizeof(s_temp));
     sprintf(s_temp, "%d,", extAttr[3]);//MULTI
     strcat(cmds,s_temp);

     memset(s_temp,0,sizeof(s_temp));
     sprintf(s_temp, "%d,", extAttr[1]);//REV
     strcat(cmds,s_temp);
     strcat(cmds,"\"");
     strcat(cmds,barcodeLayer->content);
     strcat(cmds,"\"");
     strcat(cmds,"\n");

   break;
   case 38:
     strcat(cmds,"\"CODABLOCK\",");
     strcat(cmds,xpos);
     strcat(cmds,ypos);
     strcat(cmds,s_ratio);
     strcat(cmds,xdim);//ROWHEIGHT
     strcat(cmds,xdim);//MODULEWIDTH
     strcat(cmds,"\"");
     strcat(cmds,barcodeLayer->content);
     strcat(cmds,"\"");
     strcat(cmds,"\n");
   break;
   case 16:case 23: case 69: case 74: case 76: case 84: case 86: case 78: case 87:
     strcat(cmds,"\"QRCODE\",");
     strcat(cmds,xpos);
     strcat(cmds,ypos);
     switch (barcodeLayer->errorLevel)
          {
          case 0:
            sprintf(elevel, "%s,", "L");
            break;
          case 1:
           sprintf(elevel, "%s,", "M");
          break;
           case 2:
           sprintf(elevel, "%s,", "Q");
          break;
           case 3:
           sprintf(elevel, "%s,", "H");
          break;
          
          default:
           sprintf(elevel, "%s,", "L");
            break;
          }
     strcat(cmds,elevel);
     strcat(cmds,xdim);
     if(extAttr[1]=0x01)
        sprintf(inputmodel, "%s,", "A");
     else
        sprintf(inputmodel, "%s,", "M");
     strcat(cmds,inputmodel);
     strcat(cmds,s_ratio);
     strcat(cmds,"\"");
     strcat(cmds,barcodeLayer->content);
     strcat(cmds,"\"");
     strcat(cmds,"\n");
         

   break;
   case 8: case 20:case 79: case 85: case 88:
     strcat(cmds,"\"DMATRIX\",");
     strcat(cmds,xpos);
     strcat(cmds,ypos);
     strcat(cmds,width);
     strcat(cmds,height);
  



     memset(s_temp,0,sizeof(s_temp));
     sprintf(s_temp, "X%d,", barcodeLayer->XDimension);//REV
     strcat(cmds,s_temp);
    

     memset(s_temp,0,sizeof(s_temp));
     switch (barcodeLayer->baseLayer->Angle)
     {
     case 0:
      strcat(cmds,"r0,");
      break;
     case 1:
      strcat(cmds,"r90,");
     break;
     case 2:
      strcat(cmds,"r180,");
     break;
     case 3:
      strcat(cmds,"r270,");
     break;

     
     default:
      break;
     }
     if(extAttr[0]=0x00)
       strcat(cmds,"a0,");
     else
       strcat(cmds,"a1,");
     switch (extAttr[3])
     {
     case 0:
      strcat(cmds,"48,");
      strcat(cmds,"48,");
      break;
    case 1:
       strcat(cmds,"10,");
       strcat(cmds,"10,");
      break;
    case 2:
        strcat(cmds,"12,");
        strcat(cmds,"12,");
      break;
       case 3:
        strcat(cmds,"14,");
      strcat(cmds,"14,");
      break;
       case 4:
        strcat(cmds,"16,");
      strcat(cmds,"26,");
      break;
       case 5:
        strcat(cmds,"18,");
      strcat(cmds,"18,");
      break;
       case 6:
        strcat(cmds,"20,");
      strcat(cmds,"20,");
      break;
       case 7:
        strcat(cmds,"22,");
      strcat(cmds,"22,");
      break;
       case 8:
        strcat(cmds,"24,");
      strcat(cmds,"24,");
      break;
       case 9:
        strcat(cmds,"26,");
      strcat(cmds,"26,");
      break;
       case 10:
        strcat(cmds,"32,");
      strcat(cmds,"32,");
      break;
       case 11:
        strcat(cmds,"36,");
      strcat(cmds,"36,");
      break;
       case 12:
        strcat(cmds,"40,");
      strcat(cmds,"40,");
      break;
       case 13:
        strcat(cmds,"44,");
      strcat(cmds,"44,");
      break;
       case 14:
        strcat(cmds,"48,");
      strcat(cmds,"48,");
      break;
       case 15:
        strcat(cmds,"52,");
      strcat(cmds,"52,");
      break;
       case 16:
        strcat(cmds,"64,");
      strcat(cmds,"64,");
      break;
       case 17:
        strcat(cmds,"72,");
      strcat(cmds,"72,");
      break;
       case 18:
        strcat(cmds,"80,");
      strcat(cmds,"80,");
      break;
       case 19:
        strcat(cmds,"88,");
      strcat(cmds,"88,");
      break;
       case 20:
        strcat(cmds,"96,");
      strcat(cmds,"96,");
      break;
        case 21:
         strcat(cmds,"104,");
      strcat(cmds,"104,");
      break;
        case 22:
         strcat(cmds,"120,");
      strcat(cmds,"120,");
      break;
        case 23:
         strcat(cmds,"132,");
      strcat(cmds,"132,");
      break;
        case 24:
         strcat(cmds,"144,");
      strcat(cmds,"144,");
      break;
        case 25:
         strcat(cmds,"8,");
      strcat(cmds,"18,");
      break;
        case 26:
         strcat(cmds,"8,");
      strcat(cmds,"32,");
      break;
        case 27:
         strcat(cmds,"12,");
      strcat(cmds,"26,");
      break;
        case 28:
         strcat(cmds,"12,");
      strcat(cmds,"36,");
      break;
        case 29:
         strcat(cmds,"16,");
      strcat(cmds,"36,");
      break;
        case 30:
         strcat(cmds,"16,");
      strcat(cmds,"48,");
      break;
         
     default:
      strcat(cmds,"80,");
      strcat(cmds,"80,");
      break;
     }
     strcat(cmds,"\"");
     strcat(cmds,barcodeLayer->content);
     strcat(cmds,"\"");
     strcat(cmds,"\n");
   break;


  

  
   
   default:
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

  enqueue(TSPLqueue,cmds);
}
void HCreateTextTSPL(Queue*TSPLqueue,pageConfig *page,TextLayer *textLayer)
{

  char *cmds= (char *)malloc(sizeof(char)*5000);
  char *fName =(char *)malloc(sizeof(char)*20);
  sprintf(fName, "\"%s\",", FONTNAME);

  int FontSize;
  FontSize=textLayer->font.FontSize;
  FontSize=FontSize/10;
  int xmul,ymul;
  int rotation=0;
  if(FontSize==0)
  {
    xmul=1;
    ymul=1;
  }else
  {
    xmul=FontSize;
    ymul=FontSize;
  }
  char fsize[10];
  sprintf(fsize, "%d,", FontSize);

  char xpos[10];
  int x=HCreateDPIChange(page->dpiX,DPI,textLayer->baseLayer->x);
  sprintf(xpos, "%d,", x);
  char ypos[10];
  int y=HCreateDPIChange(page->dpiY,DPI,textLayer->baseLayer->y);
  sprintf(ypos, "%d,", y);



  switch (textLayer->baseLayer->Angle)
  {
    case 0:
    /* code */
    rotation=0;
    break;
    case 1:
     rotation=90;
    break;
    case 2:
      rotation=180;
    break;
    case 3:
        rotation=270;
    break;

  
  default:
    rotation=0;
    break;
  }
  char angle[10];
  sprintf(angle, "%d,", rotation);
  
  strcat(cmds,"TEXT ");
  strcat(cmds,xpos);
  strcat(cmds,ypos);
  strcat(cmds,fName);
  strcat(cmds,angle);
  strcat(cmds,fsize);
  strcat(cmds,fsize);
  strcat(cmds,"\"");
  strcat(cmds,textLayer->content);
  strcat(cmds,"\"");
  strcat(cmds,"\r\n");
  free(textLayer->font.Style);
  free(textLayer->font.FontName);
  free(textLayer->content);
  free(textLayer->baseLayer->layerName);
  free(textLayer->baseLayer);
  free(fName);
  free(textLayer);

  enqueue(TSPLqueue,cmds);
}
void HCreateLineTSPL(Queue*TSPLqueue,pageConfig *page,LineLayer *lineLayer)
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
     strcat(cmds,"BAR ");
     strcat(cmds,xpos);
     strcat(cmds,ypos);
     strcat(cmds,lineWidth);
     strcat(cmds,lineHeight);
     strcat(cmds,"\n");




  free(lineLayer->baseLayer->layerName);
  
  free(lineLayer->baseLayer);

  free(lineLayer);
  enqueue(TSPLqueue,cmds);



}
void HCreateRectangleTSPL(Queue*TSPLqueue,pageConfig *page,RectangleLayer *rectLayer)
{
  char *cmds= (char *)malloc(sizeof(char)*5000);
   char xpos[10];
   int x=HCreateDPIChange(page->dpiX,DPI,rectLayer->baseLayer->x);
   sprintf(xpos, "%d,", x);
   char ypos[10];
   int y=HCreateDPIChange(page->dpiY,DPI,rectLayer->baseLayer->y);
   sprintf(ypos, "%d,", y);

   char rectWidth[10];
   int width=HCreateDPIChange(page->dpiX,DPI,rectLayer->RectangWidth);
   sprintf(rectWidth, "%d,", width);

   char rectHeight[10];
   int height=HCreateDPIChange(page->dpiY,DPI,rectLayer->RectangHeight);
   sprintf(rectHeight, "%d,", height);

   char lineLength[10];
   int length=HCreateDPIChange(page->dpiY,DPI,rectLayer->lineWidth);
   sprintf(lineLength, "%d", length);

   char xend[10];
   char yend[10];
   int x1=x+width;
   int y1=y+height;
   sprintf(xend, "%d,", x1);
   sprintf(yend, "%d,", y1);

   strcat(cmds,"BOX  ");
   strcat(cmds,xpos);
   strcat(cmds,ypos);
   strcat(cmds,xend);
   strcat(cmds,yend);
   strcat(cmds,lineLength);
   if(rectLayer->RectanRadius>0)
   {
     strcat(cmds,",");
     char s_temp[10];
     sprintf(s_temp, "%d", rectLayer->RectanRadius);
     strcat(cmds,s_temp);
   }

   strcat(cmds,"\n");
  free(rectLayer->baseLayer->layerName);
  free(rectLayer->baseLayer);
  free(rectLayer);

  enqueue(TSPLqueue,cmds);

}
void HCreateEllipseTSPL(Queue*TSPLqueue,pageConfig *page,EllipseLayer *ellipseLayer)
{
   char *cmds= (char *)malloc(sizeof(char)*5000);
   char xpos[10];
   int x=HCreateDPIChange(page->dpiX,DPI,ellipseLayer->baseLayer->x);
   sprintf(xpos, "%d,", x);
   char ypos[10];
   int y=HCreateDPIChange(page->dpiY,DPI,ellipseLayer->baseLayer->y);
   sprintf(ypos, "%d,", y);

   char rectWidth[10];
   int width=HCreateDPIChange(page->dpiX,DPI,ellipseLayer->EllipseWidth);
   sprintf(rectWidth, "%d,", width);

   char rectHeight[10];
   int height=HCreateDPIChange(page->dpiY,DPI,ellipseLayer->EllipseHeight);
   sprintf(rectHeight, "%d,", height);

   char lineLength[10];
   int length=HCreateDPIChange(page->dpiY,DPI,ellipseLayer->lineWidth);
   sprintf(lineLength, "%d", length);

   char xend[10];
   char yend[10];
   int x1=x+width;
   int y1=y+height;
   sprintf(xend, "%d,", x1);
   sprintf(yend, "%d,", y1);

   strcat(cmds,"ELLIPSE ");
   strcat(cmds,xpos);
   strcat(cmds,ypos);
   strcat(cmds,xend);
   strcat(cmds,yend);
   strcat(cmds,lineLength);
   strcat(cmds,"\n");
  free(ellipseLayer->baseLayer->layerName);
  free(ellipseLayer->baseLayer);
  free(ellipseLayer);

  enqueue(TSPLqueue,cmds);
}
void HCreateImageTSPL(Queue*TSPLqueue,pageConfig *page,ImageLayer *imageLayer)
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
//图像数据处理
   unsigned char buffer[5000];
   int icnt=base64_decode(imageLayer->img,buffer);
     //构建bmp文件结构体和信息结构体
    BMPFileHeader bmpHeader;
    BMPInfoHeader bmpInfoHeader;
    getBMPFileHeader(&bmpHeader,buffer);
    getBMPInfoHeader(&bmpInfoHeader,buffer);
 
    saveByteArrayToFile("test.bmp",buffer,icnt);




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
  enqueue(TSPLqueue,cmds);

}
void HCreateRfidTSPL(Queue*TSPLqueue,pageConfig *page,RfidLayer *rfidLayer)
{
    free(rfidLayer->baseLayer->layerName);
    free(rfidLayer->RfidContent);
    free(rfidLayer->baseLayer);
    free(rfidLayer);
}