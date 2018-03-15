#include <stdio.h>
#include <stdlib.h>
#include <cv.h>
#include <highgui.h>


#define threshold  125

int main()
{
	IplImage *InImage, *MidImage,*OutImage,*OutImage1,*OutImage1x,*OutImage1y,*OutImage2,*OutImage3;
	int label=1,**Imagelabel,x,y,max,flag=0,i,j,*Imagelabelnum,**Imagelabelx,**Imagelabely,big=0,small=0,maxx=0,minn=0,sum=0,sumn=0;
	char InputFileName[50];


	printf("Enter file name.\n");
	scanf("%s", &InputFileName);

	InImage = cvLoadImage(InputFileName, CV_LOAD_IMAGE_COLOR);
	if(!InImage)
	{
		printf("Error: Couldn't open the image file.\n");
		system("pause");
		return -1;
	}


	
	MidImage = cvCreateImage(cvSize(InImage->width, InImage->height), 8, 3);
	OutImage = cvCreateImage(cvSize(InImage->width, InImage->height), 8, 3);
	OutImage1 = cvCreateImage(cvSize(InImage->width, InImage->height), 8, 3);
	OutImage1x = cvCreateImage(cvSize(InImage->width, InImage->height), 8, 3);
	OutImage1y = cvCreateImage(cvSize(InImage->width, InImage->height), 8, 3);
	OutImage2 = cvCreateImage(cvSize(InImage->width, InImage->height), 8, 3);
	OutImage3 = cvCreateImage(cvSize(2*(InImage->width), 2*(InImage->height)), 8, 3);
	
	cvNamedWindow("initial", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("灰階化", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("孤立點", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("邊緣", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("邊緣x", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("邊緣y", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("基本整體門檻化", CV_WINDOW_AUTOSIZE);
	

	y=InImage -> width;
	x=InImage -> height;
	
	Imagelabel = (int**)malloc(x*sizeof(int *)); 
	for (i=0;i<x;i++)
	{
		Imagelabel[i]=(int*)malloc(y*sizeof(int));
	}
		Imagelabelx = (int**)malloc(x*sizeof(int *)); 
	for (i=0;i<x;i++)
	{
		Imagelabelx[i]=(int*)malloc(y*sizeof(int));
	}	
		Imagelabely = (int**)malloc(x*sizeof(int *)); 
	for (i=0;i<x;i++)
	{
		Imagelabely[i]=(int*)malloc(y*sizeof(int));
	}
	//讀取圖
	for (i=0;i<x;i++)
	{
		for(j=0;j<y;j++)
		{
			Imagelabel[i][j]=0;
			Imagelabelx[i][j]=0;
			Imagelabely[i][j]=0;
		}

	}

	//灰階化
	cvShowImage("initial", InImage);

	for(i=0; i<InImage->height; i++){
		for(j=0; j<InImage->width; j++){
			CvScalar x = cvGet2D(InImage, i, j);
		    int y = 0.299*x.val[2]+0.587*x.val[1]+0.114*x.val[0];
			cvSet2D(MidImage, i, j, CV_RGB(y,y,y) );
			Imagelabel[i][j]=y;
			Imagelabelx[i][j]=y;
			Imagelabely[i][j]=y;
		}
	}
		
	
	
	
	
		//孤立點   T==800  拉普拉絲
	cvShowImage("灰階化", MidImage);
	for(i=0; i<(MidImage->height); i++)
	{
		for(j=0; j<(MidImage->width)-1; j++)
		{
			if (i>0 &&j!=0&&i!=(MidImage->height)-1&&j!=(MidImage->width)-1)
			{
				int y=Imagelabel[i-1][j-1]+Imagelabel[i-1][j]+Imagelabel[i-1][j+1]+Imagelabel[i][j-1]+-8*Imagelabel[i][j]+Imagelabel[i][j+1]+Imagelabel[i+1][j+1]+Imagelabel[i+1][j]+Imagelabel[i+1][j-1];
			//	printf("%d\n",y);
				if (y>800)
					cvSet2D(OutImage, i, j, CV_RGB(255, 255, 255));
				else
					cvSet2D(OutImage, i, j, CV_RGB(0, 0, 0));
			}
			else
			{
				cvSet2D(OutImage, i, j, CV_RGB(Imagelabel[i][j], Imagelabel[i][j], Imagelabel[i][j]));
			}
		}
	}
	


		cvShowImage("孤立點", OutImage);
	//邊緣-x
			
	for(i=0; i<(MidImage->height); i++)
	{
		for(j=0; (j<MidImage->width); j++)
		{
			if (i>0 && j>0 &&(i<(MidImage->height)-1)&&(j<(MidImage->width)-1))
			{
				Imagelabelx[i][j]=-1*Imagelabel[i-1][j-1]+-2*Imagelabel[i-1][j]+-1*Imagelabel[i-1][j+1]+Imagelabel[i+1][j+1]+2*Imagelabel[i+1][j]+Imagelabel[i+1][j-1];
				cvSet2D(OutImage1x, i, j, CV_RGB(Imagelabelx[i][j], Imagelabelx[i][j], Imagelabelx[i][j]));
			}
			else
			{
				cvSet2D(OutImage1x, i, j, CV_RGB(Imagelabelx[i][j], Imagelabelx[i][j], Imagelabelx[i][j]));
			}
		}
	}

		cvShowImage("邊緣x", OutImage1x);
	//邊緣-y
		
	for(i=0; i<(MidImage->height); i++)
	{
		for(j=0; (j<MidImage->width); j++)
		{
			if (i>0 && j>0 &&(i<(MidImage->height)-1)&&(j<(MidImage->width)-1))
				Imagelabely[i][j]=-1*Imagelabel[i-1][j-1]+-2*Imagelabel[i][j-1]+-1*Imagelabel[i+1][j-1]+Imagelabel[i+1][j+1]+2*Imagelabel[i][j+1]+Imagelabel[i-1][j+1];
			cvSet2D(OutImage1y, i, j, CV_RGB(Imagelabely[i][j], Imagelabely[i][j], Imagelabely[i][j]));
		}
	}

		cvShowImage("邊緣y", OutImage1y);
	//邊緣

	for(i=0; i<(MidImage->height); i++)
	{
		for(j=0; (j<MidImage->width); j++)
		{
			int y=abs(Imagelabelx[i][j])+abs(Imagelabely[i][j]);
			cvSet2D(OutImage1, i, j, CV_RGB(y, y, y));
		}
	}


		cvShowImage("邊緣", OutImage1);
		
	//基本整體門檻化
	int m1=0,m2=0,m1n=0,m2n=0,T=127,bT=0;
	printf("%d\n",T);
	while(bT!=T)
	{
		m1=0;
		m2=0;
		m1n=0;
		m2n=0;
		bT=T;
		for(i=0; i<(MidImage->height); i++)
		{
			for(j=0; (j<MidImage->width); j++)
			{
				if (Imagelabel[i][j]>127)
				{
					m1=m1+Imagelabel[i][j];
					m1n++;
				}
				else
				{
					m2=m2+Imagelabel[i][j];
					m2n++;
				}
			}
		}
		T=(((m1/m1n)+(m2/m2n))/2);
		printf("%d\n",T);
	}
	for(i=0; i<(MidImage->height); i++)
	{
		for(j=0; (j<MidImage->width); j++)
		{
			if (Imagelabel[i][j]>T)
				cvSet2D(OutImage2, i, j, CV_RGB(255, 255, 255));
			else
				cvSet2D(OutImage2, i, j, CV_RGB(0, 0, 0));
		}
	}
		cvShowImage("基本整體門檻化", OutImage2);
		
	while(1)
	{
		char c;
		c = cvWaitKey(1);
		if(c == 'q') break;
	}
	cvReleaseImage(&InImage);
	cvReleaseImage(&MidImage);
	cvReleaseImage(&OutImage);
	cvReleaseImage(&OutImage1x);
	cvReleaseImage(&OutImage1y);
	cvReleaseImage(&OutImage1);
	cvReleaseImage(&OutImage2);
	cvDestroyWindow("initial");    // 關閉視窗
	cvDestroyWindow("灰階化");
	cvDestroyWindow("孤立點");
	cvDestroyWindow("邊緣x");
	cvDestroyWindow("邊緣");
	cvDestroyWindow("邊緣y");
	cvDestroyWindow("基本整體門檻化");
	
	for (i=0;i<x;i++)
	{
		free(Imagelabelx[i]);
		free(Imagelabely[i]);
		free(Imagelabel[i]);
	}
	
	free(Imagelabel);
	free(Imagelabelx);
	free(Imagelabely);
	
	system("pause");
	return 0;
}