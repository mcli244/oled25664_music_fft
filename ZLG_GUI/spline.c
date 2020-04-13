/****************************************************************************************
* �ļ�����SPLINE.C
* ���ܣ����β��������������ɳ���
* ���ߣ����ܱ�
* ���ڣ�2003.09.09
****************************************************************************************/
#include  "config.h"
#include  "math.h"


/****************************************************************************
* ���ƣ�max()
* ���ܣ���������������������ֵ��
* ��ڲ�����a			����1
*          b		����2
* ���ڲ�����������������ֵ
****************************************************************************/
int  max(int a, int b)
{  if(a>b) return(a);
     else  return(b);
}

/****************************************************************************
* ���ƣ�abs()
* ���ܣ���һ�������ľ���ֵ��
* ��ڲ�����a			ȡ����ֵ������
* ���ڲ���������a�ľ���ֵ
****************************************************************************/
int  abs(int a)
{  if(a>0) return(a);
     else  return(-a);
}



/* �������������غ��� */
typedef  struct
{  float  	Ax, Ay;			// ����A��B��C���������
   float  	Bx, By;
   float  	Cx, Cy;
   
   int    	Ndiv;			// ��������ƽ����������     
} CURVE;  

#define  DIV_FACTOR 		8.0				/* ƽ�����Ʋ��� */
/****************************************************************************
* ���ƣ�CURVE_SetCurve()
* ���ܣ������������߲�����
* ��ڲ�����	cl		Ҫ���������߶���CURVE�ṹ
*           ax		a��x����ֵ
*           ay		a��y����ֵ
*			bx		b��x����ֵ
*           by		b��y����ֵ
*			cx		c��x����ֵ
*           cy		c��y����ֵ
* ���ڲ�������
* ˵����
****************************************************************************/
void  CURVE_SetCurve(CURVE *cl,
                     float ax, float ay, 
               		 float bx, float by,
               		 float cx, float cy) 
{  cl->Ax = ax;
   cl->Ay = ay;
   cl->Bx = bx; 
   cl->By = by;
   cl->Cx = cx; 
   cl->Cy = cy;
   
   cl->Ndiv = (int)(max( abs((int)ax), abs((int)ay) ) / DIV_FACTOR);
}


/****************************************************************************
* ���ƣ�CURVE_GetCount()
* ���ܣ�ȡ��ƽ������Ndiv+1��ֵ��
* ��ڲ�����cl		Ҫ���������߶���CURVE�ṹ
* ���ڲ���������Ndiv+1��ֵ
* ˵������NdivΪ0������������Ϊ1��
****************************************************************************/
int  CURVE_GetCount(CURVE *cl)
{  if(0 == cl->Ndiv) cl->Ndiv=1;

   return(cl->Ndiv+1);
}


/****************************************************************************
* ���ƣ�CURVE_GetCurve()
* ���ܣ��������ߵ㵽points��
* ��ڲ�����cl			Ҫ���������߶���CURVE�ṹ
*          x			�������x����ֵ
*		   y 			�������y����ֵ
*		   points		���ߵ㻺����
*          PointCount	�������ĵ�ǰָ��
* ���ڲ�������
* ˵����
****************************************************************************/
void  CURVE_GetCurve(CURVE *cl, float x, float y, PointXY points[], int *PointCount)
{  int    X, Y;
   float  t, f, g, h;
   int    i;

   if(cl->Ndiv==0)  cl->Ndiv = 1;

    /* ����һ���㵽�ṹ */
	X = (int)x; 
	Y = (int)y;
	points[*PointCount].x = X;
	points[*PointCount].y = Y;
	(*PointCount)++;

    /* �任��ndiv���� */
	for(i=1; i<=cl->Ndiv; i++)
	{  t = 1.0f / (float)cl->Ndiv * (float)i;
	   f = t * t * (3.0f - 2.0f * t);
	   g = t * (t - 1.0f) * (t-1.0f);
	   h = t * t * (t-1.0f);
		
	   X = (int)(x + cl->Ax*f + cl->Bx*g + cl->Cx*h);
	   Y = (int)(y + cl->Ay*f + cl->By*g + cl->Cy*h);
		
	   points[*PointCount].x = X;
	   points[*PointCount].y = Y;
	   (*PointCount)++;
	}
}



/***************************************************************************/


/****************************************************************************
* ���ƣ�SPLINE_Spline()
* ���ܣ��������߳�ʼ�������������������뵽�������߶����С�
* ��ڲ�����sl			Ҫ�������������߶���SPLINE�ṹ
*		   pt			����������		
*          np			���������
* ���ڲ�������
* ˵����
****************************************************************************/
void  SPLINE_SetSpline(SPLINE *sl, PointXY pt[], int np)
{  int  i;

   sl->Np = np;

   /* �������ݸ��Ƶ�sl���� */
   for(i=0; i<sl->Np; i++) 
   {  sl->Px[i] = (float)pt[i].x;  
	  sl->Py[i] = (float)pt[i].y;
   }
}



/****************************************************************************
* ���ƣ�SPLINE_MatrixSolve()
* ���ܣ�������
* ��ڲ�����sl		Ҫ�������������߶���SPLINE�ṹ
*          B		�����B������(����)
* ���ڲ�������
* ˵������SPLINE_Generate()���ã�����ֵ������B���ء�
****************************************************************************/
void  SPLINE_MatrixSolve(SPLINE *sl, float B[]) 
{  float  Work[NPMAX];
   float  WorkB[NPMAX];
   int    i, j;
		
   for(i=0; i<=(sl->Np-1); i++) 
   {  Work[i] = B[i] / sl->Mat[1][i];
	  WorkB[i] = Work[i];
   }

   for(j=0; j<10; j++) 
   {  Work[0] = (B[0] - sl->Mat[2][0] * WorkB[1]) / sl->Mat[1][0];
	  for(i=1; i<(sl->Np-1); i++ ) 
  	  {  Work[i] = (B[i] - sl->Mat[0][i] * WorkB[i-1] - sl->Mat[2][i] * WorkB[i+1]) / sl->Mat[1][i];
	  }
	  Work[sl->Np-1] = (B[sl->Np-1] - sl->Mat[0][sl->Np-1] * WorkB[sl->Np-2]) / sl->Mat[1][sl->Np-1];

	  for(i=0; i<=(sl->Np-1); i++) 
	  {  WorkB[i] = Work[i];
	  }
   }
   
   for(i=0; i<=(sl->Np-1); i++) 
   {  B[i] = Work[i];
   }
   
}



/****************************************************************************
* ���ƣ�SPLINE_Generate()
* ���ܣ�������������ͼ�Ρ������ĸ����˵㱣�浽sl�Ľṹ�ڡ�
* ��ڲ�����sl		Ҫ�������������߶���SPLINE�ṹ
* ���ڲ�������
* ˵����slҪ��ʹ��SPLINE_SetSpline()���ø��������㡣
****************************************************************************/
void  SPLINE_Generate(SPLINE *sl) 
{  float  k[NPMAX];
   float  AMag , AMagOld;
   int    i;

   /* ����A��ֵ */
   for(i=0 ; i<=(sl->Np-2); i++ ) 
   {  sl->Ax[i] = sl->Px[i+1] - sl->Px[i];
	  sl->Ay[i] = sl->Py[i+1] - sl->Py[i];
   }
		
   /* ����k���� */
   AMagOld = (float)sqrt(sl->Ax[0] * sl->Ax[0] + sl->Ay[0] * sl->Ay[0]);
   for(i=0 ; i<=(sl->Np-3); i++) 
   {  AMag = (float)sqrt(sl->Ax[i+1] * sl->Ax[i+1] + sl->Ay[i+1] * sl->Ay[i+1]);
	  k[i] = AMagOld / AMag;
	  AMagOld = AMag;
   }
   k[sl->Np-2] = 1.0f;

   /* ������� */
   for(i=1; i<=(sl->Np-2); i++) 
   {  sl->Mat[0][i] = 1.0f;
	  sl->Mat[1][i] = 2.0f * k[i-1] * (1.0f + k[i-1]);
	  sl->Mat[2][i] = k[i-1] * k[i-1] * k[i];
   }
   sl->Mat[1][0] = 2.0f;
   sl->Mat[2][0] = k[0];
   sl->Mat[0][sl->Np-1] = 1.0f;
   sl->Mat[1][sl->Np-1] = 2.0f * k[sl->Np-2];
 
   for(i=1; i<=(sl->Np-2);i++) 
   {  sl->Bx[i] = 3.0f*(sl->Ax[i-1] + k[i-1] * k[i-1] * sl->Ax[i]);
	  sl->By[i] = 3.0f*(sl->Ay[i-1] + k[i-1] * k[i-1] * sl->Ay[i]);
   }
   sl->Bx[0] = 3.0f * sl->Ax[0];
   sl->By[0] = 3.0f * sl->Ay[0];
   sl->Bx[sl->Np-1] = 3.0f * sl->Ax[sl->Np-2];
   sl->By[sl->Np-1] = 3.0f * sl->Ay[sl->Np-2];

   SPLINE_MatrixSolve(sl, sl->Bx);
   SPLINE_MatrixSolve(sl, sl->By);

   for(i=0 ; i<=sl->Np-2 ; i++ ) 
   {  sl->Cx[i] = k[i] * sl->Bx[i+1];
	  sl->Cy[i] = k[i] * sl->By[i+1];
   }
}





/****************************************************************************
* ���ƣ�SPLINE_GetCurveCount()
* ���ܣ�ȡ�����������϶˵����ֵ��
* ��ڲ�����sl		Ҫ�������������߶���SPLINE�ṹ
* ���ڲ���������ֵ�����������߸����˵����
* ˵����slҪ��ʹ��SPLINE_SetSpline()���ø��������㡣
****************************************************************************/
int  SPLINE_GetCurveCount(SPLINE *sl)
{  CURVE  curve_bak;
   int    count = 0;
   int    i;
   
   for(i=0; i<(sl->Np - 1); i++)	// �������������� 
   {  /* ������������(����)curve_bak */
      CURVE_SetCurve(&curve_bak, 
                     sl->Ax[i], sl->Ay[i], 
                     sl->Bx[i], sl->By[i], 
                     sl->Cx[i], sl->Cy[i]);
      /* ȡ������curve_bak�϶˵�ĸ��� */
	  count += CURVE_GetCount(&curve_bak);
   }
   
   return(count);
}



/****************************************************************************
* ���ƣ�SPLINE_GetCurve()
* ���ܣ�ȡ�����������ϸ����˵㣬�Ա���ʹ��GUI_Line()�����������ߡ�
* ��ڲ�����sl			Ҫ�������������߶���SPLINE�ṹ
*          points		���ڽ������ߵ�Ļ�������PointXY����
*          PointCount	���ڽ������ߵ������ָ��
* ���ڲ�������
* ˵�����������ߵ���points���أ����ߵ������PointCount���أ�
*      slҪ��ʹ��SPLINE_SetSpline()���ø��������㡣
****************************************************************************/
void  SPLINE_GetCurve(SPLINE *sl, PointXY points[], int *PointCount)
{  CURVE  curve_bak;
   int    i;

   *PointCount = 0;						// ��ʼ���������Ϊ0
   for(i=0; i<(sl->Np-1); i++) 			// ��������������
   {  /* ������������(����)curve_bak */
   	  CURVE_SetCurve(&curve_bak, 
   					 sl->Ax[i], sl->Ay[i],
                     sl->Bx[i], sl->By[i], 
                     sl->Cx[i], sl->Cy[i]);
                     
      /* ȡ����������curve_bak�Ķ˵����ݼ����� */               
	  CURVE_GetCurve(&curve_bak, 
	                 sl->Px[i], sl->Py[i], 
	                 points, 
	                 PointCount);
   }
}


/****************************************************************************
* ���ƣ�GUI_Spline()
* ���ܣ��������������(3������)�������ߡ�
* ��ڲ����ppoints		����������
*          no			������ĸ���
* ���ڲ�������
****************************************************************************/
void  GUI_Spline(PointXY points[], int no, TCOLOR color)
{  SPLINE   sl;
   PointXY  sl_points[NPMAX * (int)DIV_FACTOR];
   
   if( (no<3)||(no>NPMAX) ) return;
   
   SPLINE_SetSpline(&sl, points, no);
   SPLINE_Generate(&sl);
   SPLINE_GetCurve(&sl, sl_points, &no);

   GUI_LineS((uint32 *)sl_points, no, color);
}



	









