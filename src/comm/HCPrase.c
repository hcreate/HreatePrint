#include"HCPrase.h"
void printHC(char *p)
{
  FILE *file=open(p);
  Queue* queue=  readToQueue(file);
  pageConfig *page=NULL;
  page= prase(queue);
  

}
FILE* open(char *filename)
{
      FILE *file = NULL;
      file = fopen(filename, "rb");
      if (file == NULL)
    {
        printf("open file fail\n");
        return -1;
    }
  return file;
}


char* readTagFile(FILE* file) 
{

    if (file == NULL) {
        perror("Error opening file");
        return NULL;
    }
 
    // 获取文件大小
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);
 
    // 分配内存
    char* content = (char*)malloc(size + 1);
    if (content == NULL) {
        fclose(file);
        perror("Error allocating memory");
        return NULL;
    }
 
    // 读取文件内容
    fread(content, 1, size, file);
    content[size] = '\0'; // 添加字符串结束标记
 
    fclose(file);
    return content;
}
int getCurrentElementSize(char *buf)
{
    int  icnt=0;
    while (*buf!='>')
    {
        /* code */
        buf++;
        icnt++;
    }
    return icnt+1;
}
int getDataSize(char *data)
{
    int icnt=0;
     while (*data!='<')
    {
        /* code */
        data++;
        icnt++;
    }
    return icnt;

}

Queue* readToQueue(FILE *file)
{
  uint8_t c = ' ';
  Queue* queue;
  queue=createQueue();
  uint8_t isElement=-1;//0 开始 1  结束

   char *buffer=readTagFile(file);
   char* element;
   char* data1;
   char* data2;
   int i=0;
   char *buf=buffer;
   size_t len = strlen(buffer);

   while (i<len)//
    {

    // c = buf;
      int size= getCurrentElementSize(buf);
      //分割
       if(size<=0)
       {
          buf++;
          i++;
        }
       else
        {
          element= (char*)malloc(size + 1);
          for(int j=0;j<size;j++)
          {
          element[j]=*(buf++);
          i++;

          }
        }
      
       element[size]='\0';
      //分离中间值
      int dataLength=getDataSize(element);
      if(dataLength>0)
      {
        if(element[0]=='\r'&&element[1]=='\n')
        {
          
        }else
        {
          data1=(char*)malloc(dataLength + 1);
          for(int m=0;m<dataLength;m++)
            data1[m]=element[m];
          data1[dataLength]='\0';
          enqueue(queue,data1);
        }
         

      }
      
      data2=(char*)malloc(size-dataLength+1);
      for(int m=dataLength;m<size;m++)
             data2[m-dataLength]=element[m];
      data2[size-dataLength]='\0';
          enqueue(queue,data2);

      free(element);//释放临时数据内存

    }
    //printf("%s",buffer);
    //解析XML
    //prase(queue);
    
   // printf("parse ok!\n");
     
    free(buffer);
    fclose(file);

    return queue;
}

pageConfig *prase(Queue* queue)
{
   


   // layerTreeNode *root=createLayerTreeNode();
    //travel(root,queue);
    pageConfig* page= queueCoventToPage(queue);
    free(queue);

    return page;//返回页面

}
pageConfig * queueCoventToPage(Queue* queue)
{
       pageConfig *page=createPage();
        while(!isEmpty(queue))
        {
          char *data=dequeue(queue);
          if(strstr(data,"<DpiX>"))
          {
              free(data);
              data=dequeue(queue);
              page->dpiX=atoi(data);
              free(data);
              data=dequeue(queue);
          }
          if(strstr(data,"<DpiY>"))
          {
              free(data);
              data=dequeue(queue);
              page->dpiY=atoi(data);
              free(data);
              data=dequeue(queue);
          }
          if(strstr(data,"<PageWidth>"))
          {
              free(data);
              data=dequeue(queue);
              page->pageWidth=atoi(data);
              free(data);
              data=dequeue(queue);
          }
          if(strstr(data,"<PageHeight>"))
          {
              free(data);
              data=dequeue(queue);
              page->pageHeight=atoi(data);
              free(data);
              data=dequeue(queue);
          }
          if(strstr(data,"<PagePadding>"))
          {
              free(data);
              data=dequeue(queue);
              page->pagePadding=atoi(data);
              free(data);
              data=dequeue(queue);
          }
          if(strstr(data,"<PageHorizontalSpacing>"))
          {
              free(data);
              data=dequeue(queue);
              page->pageHorizontalSpacing=atoi(data);
              free(data);
              data=dequeue(queue);
          }
          if(strstr(data,"<PageVerticalSpacing>"))
          {
              free(data);
              data=dequeue(queue);
              page->pageVerticalSpacing=atoi(data);
              free(data);
              data=dequeue(queue);
          }
          if(strstr(data,"RfidLayerConfig"))
          {
              layerNode *node=(layerNode *)malloc(sizeof(layerNode));
              RfidLayer *rfidObject=(RfidLayer *)malloc(sizeof(RfidLayer));

              char *value=findData("RfidContent=",data);
              rfidObject->RfidContent=value;
              rfidObject->baseLayer=(BaseImageLayer*)malloc(sizeof(BaseImageLayer));
              value=findData("X=",data);
              rfidObject->baseLayer->x=atoi(value);
              free(value);
               value=findData("Y=",data);
              rfidObject->baseLayer->y=atoi(value);
              free(value);

            //  rfidObject->RfidContent="123456";
            
              node->layerData=rfidObject;
              node->layerType=Rfid;
              node->next=NULL;
              layerNode *currentNode=page->layers;
              while (currentNode->next!=NULL)
              {
                /* code */
                currentNode=currentNode->next;
              }
              currentNode->next=node;
              free(data);
              data=dequeue(queue);
          }
          if(strstr(data,"ImageLayerConfig"))
           {
             layerNode *node=(layerNode *)malloc(sizeof(layerNode));
              ImageLayer *imageObject=(ImageLayer *)malloc(sizeof(ImageLayer));
              char *value=findData("ImageBase64=",data);
              imageObject->img=value;
              imageObject->baseLayer=(BaseImageLayer*)malloc(sizeof(BaseImageLayer));
              value=findData("Rotat=",data);
              imageObject->baseLayer->Angle=atoi(value);
              free(value);
              value=findData("X=",data);
              imageObject->baseLayer->x=atoi(value);
              free(value);
               value=findData("Y=",data);
              imageObject->baseLayer->y=atoi(value);
              free(value);
              value=findData("Height=",data);
              imageObject->baseLayer->height=atoi(value);
              free(value);
               value=findData("Width=",data);
              imageObject->baseLayer->width=atoi(value);
              free(value);

              //imageObject->img="testdata";
              node->layerData=imageObject;
              node->layerType=Image;
              node->next=NULL;
              layerNode *currentNode=page->layers;
              while (currentNode->next!=NULL)
              {
                /* code */
                currentNode=currentNode->next;
              }
              currentNode->next=node;

              free(data);
              data=dequeue(queue);
          }
          if(strstr(data,"BarCodeLayerConfig"))
          {
             layerNode *node=(layerNode *)malloc(sizeof(layerNode));
              BarcodeLayer *barcodeObject=(BarcodeLayer *)malloc(sizeof(BarcodeLayer));
              char *value=findData("Content=",data);
              barcodeObject->content=value;
              value=findData("CodeType=",data);
              barcodeObject->codeType=atoi(value);
              free(value);
              value=findData("CodeSize=",data);
              barcodeObject->codeSize=atoi(value);
              free(value);
              value=findData("ECIMode=",data);
              barcodeObject->ECIMode=atoi(value);
              free(value);
               
              value=findData("EncodingMode=",data);
              barcodeObject->EncodingMode=value;
              value=findData("DataMode=",data);
              barcodeObject->DataMode=value;
              


              value=findData("XDimension=",data);
              barcodeObject->XDimension=atof(value);
              free(value);

              value=findData("ErrorLevel=",data);
              barcodeObject->errorLevel=atoi(value);
              free(value);


               value=findData("supplementText=",data);
              barcodeObject->supplenmentText=value;
             // free(value);

              value=findData("ExtAttr=",data);
              barcodeObject->ExtAttr=value;
             // free(value);

              value=findData("IsShowText=",data);
              if(strcmp(value,"true")==0)
                barcodeObject->isShowText=1;
              else
                barcodeObject->isShowText=0;
              free(value);
 
              barcodeObject->baseLayer=(BaseImageLayer*)malloc(sizeof(BaseImageLayer));
              value=findData("Rotat=",data);
              barcodeObject->baseLayer->Angle=atoi(value);
              free(value);

                value=findData("X=",data);
              barcodeObject->baseLayer->x=atoi(value);
              free(value);
               value=findData("Y=",data);
              barcodeObject->baseLayer->y=atoi(value);
              free(value);

              value=findData("Height=",data);
              barcodeObject->baseLayer->height=atoi(value);
              free(value);
               value=findData("Width=",data);
              barcodeObject->baseLayer->width=atoi(value);
              free(value);
          


              node->layerData=barcodeObject;
              node->layerType=Barcode;
              node->next=NULL;
              layerNode *currentNode=page->layers;
              while (currentNode->next!=NULL)
              {
                /* code */
                currentNode=currentNode->next;
              }
              currentNode->next=node;
              free(data);
              //color1
              data=dequeue(queue);//<Color1>
              free(data);
              data=dequeue(queue);//NamedColor:Black
              barcodeObject->tm_t.r=0;
              barcodeObject->tm_t.g=0;
              barcodeObject->tm_t.b=0;
              free(data);
              data=dequeue(queue);//</Color1>
              free(data);
              //color2
              data=dequeue(queue);//<Color2>
              free(data);
              data=dequeue(queue);//NamedColor:White
              barcodeObject->tm_k.r=255;
              barcodeObject->tm_k.g=255;
              barcodeObject->tm_k.b=255;
              free(data);
              data=dequeue(queue);//</Color2>
              free(data);

              //color3
              data=dequeue(queue);//<Color3>
              free(data);
              data=dequeue(queue);//NamedColor:Black
              barcodeObject->tm_zt.r=0;
              barcodeObject->tm_zt.g=0;
              barcodeObject->tm_zt.b=0;
              free(data);
              data=dequeue(queue);//</Color3>
              free(data);

              //   <XmlFontObject>
              data=dequeue(queue);//<XmlFontObject>
              free(data);
              data=dequeue(queue);//<FontFamily>
              free(data);
              data=dequeue(queue);//OCRB
              barcodeObject->zt_font.FontName=data;
              data=dequeue(queue);//</FontFamily>
              free(data);
                 data=dequeue(queue);//<GraphicsUnit>
              free(data);
                 data=dequeue(queue);//Point
              free(data);
                 data=dequeue(queue);//</GraphicsUnit>
              free(data);

               data=dequeue(queue);//<Size>
              free(data);
              data=dequeue(queue);//80
               barcodeObject->zt_font.FontSize=atoi(data);
              free(data);
              data=dequeue(queue);//</Size>
              free(data);


                data=dequeue(queue);//<Style>
              free(data);
                 data=dequeue(queue);//Bold
                  barcodeObject->zt_font.Style=data;
             // free(data);
                 data=dequeue(queue);//</Style>
              free(data);


                data=dequeue(queue);// </XmlFontObject>
              free(data);
                 data=dequeue(queue);// </anyType>

          }
          if(strstr(data,"TextLayerConfig"))
          {
              layerNode *node=(layerNode *)malloc(sizeof(layerNode));
              TextLayer *textObject=(TextLayer *)malloc(sizeof(TextLayer));

              char *value=findData("Content=",data);
              textObject->content=value;
              textObject->baseLayer=(BaseImageLayer*)malloc(sizeof(BaseImageLayer));
              value=findData("Angle=",data);
              textObject->baseLayer->Angle=atoi(value);
              free(value);
              value=findData("X=",data);
              textObject->baseLayer->x=atoi(value);
              free(value);
               value=findData("Y=",data);
              textObject->baseLayer->y=atoi(value);
              free(value);

              node->layerData=textObject;
              node->layerType=Text;
              node->next=NULL;
              layerNode *currentNode=page->layers;
              while (currentNode->next!=NULL)
              {
                /* code */
                currentNode=currentNode->next;
              }
              currentNode->next=node;
              free(data);

              data=dequeue(queue);//<Font>
              free(data);
              data=dequeue(queue);// <FontFamily>
              free(data);
              data=dequeue(queue);// 宋体
              textObject->font.FontName=data;
              data=dequeue(queue);// </FontFamily>
              free(data);
              data=dequeue(queue);// <GraphicsUnit>
              free(data);
              data=dequeue(queue);// Point
              free(data);
              data=dequeue(queue);// </GraphicsUnit>
              free(data);

              data=dequeue(queue);//  <Size>
              free(data);
              data=dequeue(queue);// 12
              textObject->font.FontSize=atoi(data);
              free(data);
              data=dequeue(queue);//  </Size>
              free(data);

              data=dequeue(queue);//  <Style>
              free(data);
              data=dequeue(queue);// Regular
              textObject->font.Style=data;
             // free(data);
              data=dequeue(queue);//  </Style>
              free(data);
               data=dequeue(queue);//  </Font>
              free(data);
              data=dequeue(queue);//  <Color>
              free(data);

              data=dequeue(queue);//NamedColor:Black
              if(strcmp(data,"NamedColor:White")==0)
              textObject->isReverse=1;
              else
               textObject->isReverse=0;
              free(data);
              data=dequeue(queue);//</Color>


          }
          if(strstr(data,"LineLayerConfig"))
           {
              layerNode *node=(layerNode *)malloc(sizeof(layerNode));
              LineLayer *lineObject=(LineLayer *)malloc(sizeof(LineLayer));
              lineObject->baseLayer=(BaseImageLayer*)malloc(sizeof(BaseImageLayer));
              char *value=findData("lineDirect=",data); 
              lineObject->linDirect=atoi(value);
              free(value);
              value=findData("lineWidth=",data); 
              lineObject->lineWidth=atoi(value);
              free(value);
              value=findData("lineLength=",data); 
              lineObject->lineLength=atoi(value);
              free(value);
              value=findData("isDash=",data); 
              if(strcmp(value,"false"))
              lineObject->isDash=0;
              else
              lineObject->isDash=1;
              free(value);

               value=findData("DashLength=",data); 
              lineObject->dashLength=atoi(value);
              free(value);

              value=findData("X=",data);
              lineObject->baseLayer->x=atoi(value);
              free(value);
               value=findData("Y=",data);
              lineObject->baseLayer->y=atoi(value);
            //  free(value);

              node->layerData=lineObject;
              node->layerType=Line;
              node->next=NULL;
              layerNode *currentNode=page->layers;
              while (currentNode->next!=NULL)
              {
                /* code */
                currentNode=currentNode->next;
              }
              currentNode->next=node;


           }
          if(strstr(data,"RectangleLayerConfig")&&!strstr(data,"RectangleLayerConfig"))
           {
             layerNode *node=(layerNode *)malloc(sizeof(layerNode));
              RectangleLayer *rectObject=(RectangleLayer *)malloc(sizeof(RectangleLayer));
              rectObject->baseLayer=(BaseImageLayer*)malloc(sizeof(BaseImageLayer));
              rectObject->RectanRadius=0;
              char *value=findData("LineWidth=",data); 
              rectObject->lineWidth=atoi(value);
              free(value);
              value=findData("RectangWidth=",data); 
              rectObject->RectangWidth=atoi(value);
              free(value);
               value=findData("RectangHeight=",data); 
              rectObject->RectangHeight=atoi(value);
              free(value);

              value=findData("X=",data);
              rectObject->baseLayer->x=atoi(value);
              free(value);
              value=findData("Y=",data);
              rectObject->baseLayer->y=atoi(value);


              node->layerData=rectObject;
              node->layerType=Rectangle;
              node->next=NULL;
              layerNode *currentNode=page->layers;
              while (currentNode->next!=NULL)
              {
                /* code */
                currentNode=currentNode->next;
              }
              currentNode->next=node;

           }
          if(strstr(data,"RoundRectangleLayerConfig"))
           {
             layerNode *node=(layerNode *)malloc(sizeof(layerNode));
              RectangleLayer *rectObject=(RectangleLayer *)malloc(sizeof(RectangleLayer));
              rectObject->baseLayer=(BaseImageLayer*)malloc(sizeof(BaseImageLayer));
              char *value=findData("LineWidth=",data); 
              rectObject->lineWidth=atoi(value);
              free(value);
              value=findData("RoundRectangleWidth=",data); 
              rectObject->RectangWidth=atoi(value);
              free(value);
              value=findData("RoundRectangleHeight=",data); 
              rectObject->RectangHeight=atoi(value);
              free(value);

              value=findData("RoundRectangRadius=",data); 
              rectObject->RectanRadius=atoi(value);
              free(value);

              value=findData("X=",data);
              rectObject->baseLayer->x=atoi(value);
              free(value);
              value=findData("Y=",data);
              rectObject->baseLayer->y=atoi(value);


              node->layerData=rectObject;
              node->layerType=Rectangle;
              node->next=NULL;
              layerNode *currentNode=page->layers;
              while (currentNode->next!=NULL)
              {
                /* code */
                currentNode=currentNode->next;
              }
              currentNode->next=node;

           }        
          if(strstr(data,"EllipseLayerConfig"))
           {
             layerNode *node=(layerNode *)malloc(sizeof(layerNode));
              EllipseLayer *ellipseObject=(EllipseLayer *)malloc(sizeof(EllipseLayer));
              ellipseObject->baseLayer=(BaseImageLayer*)malloc(sizeof(BaseImageLayer));
              char *value=findData("LineWidth=",data); 
              ellipseObject->lineWidth=atoi(value);
              free(value);
              value=findData("EllipseWidth=",data); 
              ellipseObject->EllipseWidth=atoi(value);
              free(value);
              value=findData("EllipseHeight=",data); 
              ellipseObject->EllipseHeight=atoi(value);
              free(value);
               value=findData("X=",data);
              ellipseObject->baseLayer->x=atoi(value);
              free(value);
              value=findData("Y=",data);
              ellipseObject->baseLayer->y=atoi(value);


               node->layerData=ellipseObject;
              node->layerType=Ellipse;
              node->next=NULL;
              layerNode *currentNode=page->layers;
              while (currentNode->next!=NULL)
              {
                /* code */
                currentNode=currentNode->next;
              }
              currentNode->next=node;
           }
           free(data);
        }
       return page;

}

void travel(layerTreeNode *root,Queue* queue)
{  
   
   while(!isEmpty(queue))
    {

      char *data=dequeue(queue);
      if(data[0]!='<')//字段值
      {
        layerTreeNode *treeNode=(layerTreeNode*) malloc(sizeof(layerTreeNode));
         treeNode->data=data;
         treeNode->nodeList=createLayerList();
      
         layerNode *node=(layerNode*) malloc(sizeof(layerNode));
         node->layerData=treeNode;
         layerNode *childs=root->nodeList;
         while (childs->next!=NULL)
         {
        /* code */
          childs=childs->next;
        }
         childs->next=node;
        travel(root,queue);
      }
       else if(strstr(root->data,data)==NULL)//和root不能对应，设置为当前节点的子节点
       {
         layerTreeNode *treeNode=(layerTreeNode*) malloc(sizeof(layerTreeNode));
         treeNode->data=data;
         treeNode->nodeList=createLayerList();
      
         layerNode *node=(layerNode*) malloc(sizeof(layerNode));
         node->layerData=treeNode;
         layerNode *childs=root->nodeList;
         while (childs->next!=NULL)
         {
        /* code */
          childs=childs->next;
        }
         childs->next=node;


         travel(treeNode,queue);
      

       }
       else if(strstr(root->data,data)!=NULL)//和当前节点对应
      {
         layerTreeNode *treeNode=(layerTreeNode*) malloc(sizeof(layerTreeNode));
         treeNode->data=data;
         treeNode->nodeList=createLayerList();
         layerNode *node=(layerNode*) malloc(sizeof(layerNode));
         node->layerData=treeNode;
         layerNode *childs=root->nodeList;
         while (childs->next!=NULL)
         {
        /* code */
          childs=childs->next;
        }
         childs->next=node;
         
         travel(root,queue);
      }
    
      
     

    }

}
void addTextLayer(pageConfig *page)
{
//在链表中添加一个节点
  if(page==NULL) 
      page=createPage();
  TextLayer *textLayer=(TextLayer*) malloc(sizeof(TextLayer));
  
  layerNode *node= (layerNode*) malloc(sizeof(layerNode));
  node->layerData= textLayer;
  node->layerType=Text;

  addLayerList(page->layers,node);

}
