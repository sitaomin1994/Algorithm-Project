#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include<time.h>
#include<Windows.h>
typedef int ElementType;
typedef struct Rect *PtrToRect;
typedef PtrToRect Rectangular;
typedef struct Contour *PtrToContour;
typedef PtrToContour ContourList;
typedef PtrToContour contour;
int N;     
struct Rect{
	int length;
	int width;
	int x;																						//x,y the axis value of the rectangular left-bottom
	int y;
	int rotation;																				//rotation--the angle the rectangular rotate, it can be 0 or 90
	int area;																					//area---the area of the rectangular
};
struct Contour{
	int x;																						//axis value of each level contour
	int y;
	int width;
	contour Next;
};

int FindFitRect(struct Rect *rect,contour CLowest,contour CLowestPre,int height);               //funciton to find the most fited rectangular every time
void combineC(struct Contour *CLowestPre,struct Contour *CLowest,struct Contour *Cnew);         //function to combine the contour if some contour level have the same height 
contour FindLowestCPre(ContourList C);                                                          //find the contour of the smallest height in the contour list
void Insert( ElementType x,ElementType y,ElementType width, ContourList L, contour P );         //insert a contour to contour list
void DeleteList( ContourList L );                                                               //delete the contour list

int main(){
	clock_t start,stop;
	double duration=0;
	srand(time(0));
	/*Defination of the variation*/
	int width,minheight,i,j;                                                                    //width--given width of the stripe
	int *RecWidth,*RecHeight;                                                                   //array to store the input length and width of rectangular
	int height=0;                                                                               //height---the height of the all rectangular having been inserted in the stripe
	int rectID;																					//rectID---the ID of the rect which will be inserted
	int times=1;
	int temp;
	struct Rect rect[10000];	                                                                //store the information of each rectangular
	contour CLowestPre,CLowest;
	ContourList C;
	contour TmpCell;
	
	RecWidth=(int *)malloc(sizeof(int)*N);
	RecHeight=(int *)malloc(sizeof(int)*N);	
	printf("Input width,N:\n");
	scanf("%d",&width);
	scanf("%d",&N);
	
		C=(struct Contour *)malloc(sizeof(struct Contour));	
		TmpCell =(struct Contour*) malloc( sizeof( struct Contour ) );
		height=0;
		rectID=0;
		
		/*InPUT:*/
		/*Initialization*/
		for(i = 0;i<N;i++){
			rect[i].length=rand()%width+1;
			rect[i].width=rand()%width+1;
			if(rect[i].length<rect[i].width){
				temp=rect[i].length;
				rect[i].length=rect[i].width;
				rect[i].width=temp;
			}
			rect[i].x=-1;
			rect[i].y=-1;
			rect[i].rotation=0;
			rect[i].area=rect[i].width*rect[i].length;
			//printf("%d %d\n",rect[i].length,rect[i].width);
			}
		/*Packing Process*/
		
		start=clock();
		
		/*Initial Contour*/
		C->Next=NULL;
		C->x=-1;
		C->y=width*N;
		C->width=0;
		TmpCell->x = 0;
		TmpCell->y = 0;
		TmpCell->width=width;
		TmpCell->Next = C->Next;
		C->Next = TmpCell;
		TmpCell=NULL;
		/*Insert N rect to the given stripe*/		
		for(i=0;i<N;i++){
			TmpCell =(struct Contour*) malloc( sizeof( struct Contour ) );
			/*找到最低的轮廓水平线*/
			CLowestPre=FindLowestCPre(C); 
			CLowest=CLowestPre->Next;
			/*优先匹配算法填充矩形*/
			rectID=FindFitRect(rect,CLowest,CLowestPre,height);											//find the most fit rectangular
			//printf("%d %d %d\n",CLowest->width,CLowest->x,CLowest->y);
			//printf("%d %d\n",rectID,rect[rectID].rotation);
			while(rectID==-1){																			//if can not find the fit rect, update the contour and find again
				CLowest->y=CLowestPre->y;
				combineC(CLowestPre,CLowest,CLowest->Next);
				CLowestPre=FindLowestCPre(C); 
				CLowest=CLowestPre->Next;
				rectID=FindFitRect(rect,CLowest,CLowestPre,height);
				}
			//printf("%d %d %d\n",CLowest->width,CLowest->x,CLowest->y);
			/*填入矩形*/
			rect[rectID].x=CLowest->x;                                                                  //insert the rectangular and update the rectangular information
			rect[rectID].y=CLowest->y; 
		
			/*更新contour信息*/	
			if(rect[rectID].rotation==0){																//check the rotation,so it have rotation and not rotation 2 cases 	
				CLowest->x=rect[rectID].x;																//because lowest level contour divide in 2,so update related value
				CLowest->y=rect[rectID].y+rect[rectID].width;
				//Insert(CLowest->x+rect[rectID].width,rect[rectID].y,CLowest->width-rect[rectID].width,C,CLowest);	
				TmpCell->x = CLowest->x+rect[rectID].length;
				TmpCell->y=rect[rectID].y;
				TmpCell->width=CLowest->width-rect[rectID].length;
				TmpCell->Next = CLowest->Next;
				CLowest->Next = TmpCell;
				CLowest->width=rect[rectID].length;
				}
			else{
				CLowest->x=rect[rectID].x;																//because lowest level contour divide in 2,so update related value
				CLowest->y=rect[rectID].y+rect[rectID].length;
				//Insert(CLowest->x+rect[rectID].length,rect[rectID].y,CLowest->width-rect[rectID].length,C,CLowest);
				TmpCell->x = CLowest->x+rect[rectID].width;
				TmpCell->y = rect[rectID].y;
				TmpCell->width=CLowest->width-rect[rectID].width;
				TmpCell->Next = CLowest->Next;
				CLowest->Next = TmpCell;
				CLowest->width=rect[rectID].width;		
				}	
			//printf("%d %d %d %d\n",rectID,rect[rectID].rotation,rect[rectID].x,rect[rectID].y);	
			/*合并高度相同的contour*/
			combineC(CLowestPre,CLowest,CLowest->Next);												//combine the contour have the same height
		   	
			/*更新高度*/
			if(rect[rectID].rotation==0){
				if(rect[rectID].y+rect[rectID].width>height)										  //if height exceed the former height aftervthe rectangular  adding
					height = rect[rectID].y+rect[rectID].width;
		 		}
				else{
					if(rect[rectID].y+rect[rectID].length>height)
					height = rect[rectID].y+rect[rectID].length;
				}
			}
	
		/*找到最小高度*/
		minheight=height;
	
		stop=clock();
		duration+=((double)stop-start)/CLK_TCK;

		/*Output*/
		printf("output:minimum height:\n");
		printf("%d\n",minheight);
		
		/*free the memory*/
		DeleteList( C );
	
	printf("the duration time:%lf\n",duration);	
	return 0;
} 

int FindFitRect(struct Rect *rect,contour CLowest,contour CLowestPre,int height){
	int ID=-1;
	int MaxHeight=0;
	int MaxWidth=0;
	int MaxArea=0;
	int rotationFlag;
	int i;	
	/*宽度优先适配，找到宽度或高度等于最低水平线的矩形*/
	/*找高度等于水平线的矩形*/
	for(i=0;i<N;i++){
		/*如果矩形已经被填充那么跳过*/
		if(rect[i].x>-1) continue; 
		/*找到宽度或高度匹配的矩形*/
		if(rect[i].length==CLowest->width){
		   if(rect[i].width>MaxHeight){
		   		ID=i;
		   		rotationFlag=0; 
				MaxHeight=rect[i].width;	
			}
		} 
		/*找到宽度或高度匹配的矩形*/
		if(rect[i].width==CLowest->width){
		   if(rect[i].length>MaxHeight){
		   		ID=i;
		   		rotationFlag=90; 
				MaxHeight=rect[i].length;	
			}
		}
	}
	/*如果找到那么返回ID */
	if(ID>=0){
		if(rotationFlag==0){	
			rect[ID].rotation=0;
			return ID;
		}
		if(rotationFlag==90){
			rect[ID].rotation=90;
			return ID;	
		}
	}
	MaxHeight=0;
	
	
	
	/*高度优先匹配，找到高度和最低水平线齐平的矩形*/ 
	/*矩形的长度和高度匹配*/ 
	for(i=0;i<N;i++){
		/*如果矩形已经被填充那么跳过*/
		if(rect[i].x>-1) continue;
		/*找到矩形长度和高度匹配并且面积最大的矩形*/
		if((rect[i].length<CLowest->width)&&(rect[i].width==(CLowestPre->y-CLowest->y))){
		   if(rect[i].length>MaxWidth){
		   		ID=i;
		   		rotationFlag=0;
				MaxWidth=rect[i].length;
			}
		} 	
		/*找到矩形宽度和高度匹配并且面积最大的矩形*/
		if((rect[i].width<CLowest->width)&&(rect[i].length==(CLowestPre->y-CLowest->y))){
		   if(rect[i].width>MaxWidth){
		   		ID=i;
		   		rotationFlag=90;
				MaxWidth=rect[i].width;
			}
		} 
	}
	/*如果找到返回矩形的序号*/ 
	if(ID>=0){
		if(rotationFlag==0){	
			rect[ID].rotation=0;
			return ID;
		}
		if(rotationFlag==90){
			rect[ID].rotation=90;
			return ID;	
		}
	}
	MaxWidth=0;
	


	/*最优可装入匹配*/
	for(i=0;i<N;i++){
		/*如果矩形已经被填充那么跳过*/
		if(rect[i].x>-1) continue;
		/*找到匹配矩形并且面积最大的矩形*/	
		if(rect[i].length<CLowest->width&&rect[i].width<(CLowestPre->y-CLowest->y)){
			if(rect[i].area>MaxArea){
				MaxArea=rect[i].area;
				rotationFlag=0;
				ID=i;
			}
		}
		if(rect[i].width<CLowest->width&&rect[i].length<(CLowestPre->y-CLowest->y)){
			if(rect[i].area>MaxArea){
				MaxArea=rect[i].area;
				rotationFlag=90;
				ID=i;
			}
		}					
	}
	/*找到矩形则返回ID*/
	if(ID>=0){
		if(rotationFlag==0){	
			rect[ID].rotation=0;
			return ID;
		}
		if(rotationFlag==90){
			rect[ID].rotation=90;
			return ID;	
		}
	}
	MaxArea=0;
	
	
	/*高度超出范围最优匹配*/
	for(i=0;i<N;i++){
		/*如果矩形已经被填充那么跳过*/
		if(rect[i].x>-1) continue;
		/*高度超出的情况*/
		if((rect[i].width<CLowest->width)&&(rect[i].length>(CLowestPre->y-CLowest->y))){
			if(rect[i].length+CLowest->y<height+rect[i].width){
				if(rect[i].area>MaxArea){
					MaxArea=rect[i].area;
					rotationFlag=90;
					ID=i;
				}
			}
		}
		if(rect[i].length<CLowest->width&&rect[i].width>(CLowestPre->y-CLowest->y)){
			if(rect[i].width+CLowest->y<height+rect[i].length){
				if(rect[i].area>MaxArea){
					MaxArea=rect[i].area;
					rotationFlag=0;
					ID=i;
				}
			}
		}
	}
	/*找到矩形则返回ID*/
	if(ID>=0){
		if(rotationFlag==0){	
			rect[ID].rotation=0;
			return ID;
		}
		if(rotationFlag==90){
			rect[ID].rotation=90;
			return ID;	
		}
	}
	MaxArea=0;
	
	
	/**找不到匹配的矩形返回ID值等于-1**/
	return ID;
}

contour FindLowestCPre(ContourList C){
	contour MinPre=C;
	int Min=C->y;
	while(C->Next!=NULL){
		if(C->Next->y<Min){
				MinPre=C;
				Min=C->Next->y;
		}
		C=C->Next;
	}
	return MinPre;
}

void combineC(struct Contour *CLowestPre,struct Contour *CLowest,struct Contour *Cnew){
	  contour tmp; 
	 if(CLowest->Next!=NULL){	 
	 	if(Cnew->width==0){                                         		//if Cnew width=0 ,delete it
	 		CLowest->Next=Cnew->Next;
	 		free(Cnew);
	 	}
	}
	if(CLowest->Next!=NULL){
	 	if(CLowest->y==(CLowest->Next->y)){                           	//if CLowest and CLowestNext have ssame height combine them
	 		CLowest->width+=CLowest->Next->width;
			tmp=CLowest->Next;
			CLowest->Next=CLowest->Next->Next;
			free(tmp);  
	 	} 	
	 }

	 if(CLowest->y==CLowestPre->y){                              		//if CLowest and CLowestPre have same height combine them
	 	CLowestPre->width+=CLowest->width;
		CLowestPre->Next=CLowest->Next;
		free(CLowest);
	 }
}

void Insert( ElementType x,ElementType y,ElementType width, ContourList L, contour P ) {
	contour TmpCell;
	TmpCell =(struct Contour*) malloc( sizeof( struct Contour ) );
	TmpCell->x = x;
	TmpCell->y=y;
	TmpCell->width=width;
	TmpCell->Next = P->Next;
	P->Next = TmpCell;
}

void DeleteList( ContourList L ){
	contour P,tmp;
	P = L->Next;  /* Header assumed */

	L->Next = NULL;
	while( P != NULL ){
		tmp=P->Next;
		free( P );
		P = tmp;
	}
 }