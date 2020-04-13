/****************************************************************************************
* �ļ�����WINDOW.C
* ���ܣ����ڹ��������и��ִ��ڵĸ��Ƽ������ڲ����ơ�
* ���ߣ����ܱ�
* ���ڣ�2004.02.26
* ��ע��
****************************************************************************************/
#include "config.h"

#if  GUI_WINDOW_EN==1
/****************************************************************************
* ���ƣ�GUI_WindowsDraw()
* ���ܣ���ʾ���ڡ������ṩ�Ĵ��ڲ������л����ڡ�
* ��ڲ�����win		���ھ��
* ���ڲ���������0��ʾ����ʧ�ܣ�����1��ʾ�����ɹ�
****************************************************************************/
uint8  GUI_WindowsDraw(WINDOWS *win)
{
   char* str;   // ZXE�޸�
   int32  bak, bak1, bak2;
   
   /* �������ˣ������������Χ���򷵻�0 */
   if( ( (win->with)<20 ) || ( (win->hight)<20 ) ) return(0);		// ��ȡ��߶ȼ�飬������С����
   if( (win->x + win->with ) > GUI_LCM_XMAX ) return(0);			// ���ڿ���Ƿ����
   if( (win->y + win->hight ) > GUI_LCM_YMAX ) return(0);			// ���ڸ߶��Ƿ����
    
   /* ��ʼ������ */
   GUI_RectangleFill(win->x, win->y, win->x + win->with - 1, win->y + win->hight - 1, back_color);
   GUI_Rectangle(win->x, win->y, win->x + win->with - 1, win->y + win->hight - 1, disp_color);	// ������
   GUI_HLine(win->x, win->y + 12, win->x + win->with - 1, disp_color);							// ������Ŀ��
   GUI_RLine(win->x + 12, win->y, win->y + 12, disp_color);										// ���رմ��Ű�ť
   GUI_Line(win->x, win->y, win->x + 12, win->y + 12, disp_color);
   GUI_Line(win->x + 12, win->y, win->x, win->y + 12, disp_color);
   

   /* д���� */
   if( win->title != NULL ) 
   {  str = win->title;
      bak = win->x + 15;
      bak1 = win->y + 3;
      bak2 = win->x + win->with -1;
     
      while(1)						
      {  if( (bak+8) > bak2 ) break;								// �жϱ����Ƿ����
         if(*str=='\0') break;										// �ж��ַ����Ƿ����
         
         GUI_PutChar(bak, bak1, *str++);							// ��ʾ����
         bak += 6;
      }
   }


   /* д״̬�� */
   if( win->state != NULL )
   {  if( win->hight < 60) return(0);								// �ж��Ƿ���Ի�״̬��
      /* ��״̬�� */
      GUI_HLine(win->x, win->y + win->hight - 11, win->x + win->with - 1, disp_color);
      
      str = win->state;
      bak = win->x + 3;
      bak1 = win->y + win->hight - 9;
      bak2 = win->x + win->with -1;
      
      while(1)						
      {  if( (bak+8) > bak2 ) break;								// �жϱ����Ƿ����
         if(*str=='\0') break;										// �ж��ַ����Ƿ����
         
         GUI_PutChar(bak, bak1, *str++);							// ��ʾ����
         bak += 6;
      }      
   }
   
   return(1);

}


/****************************************************************************
* ���ƣ�GUI_WindowsHide()
* ���ܣ��������ڡ�
* ��ڲ�����win		���ھ��
* ���ڲ���������0��ʾ����ʧ�ܣ�����1��ʾ�����ɹ�
****************************************************************************/
uint8  GUI_WindowsHide(WINDOWS *win)
{  /* �������ˣ������������Χ���򷵻�0 */
   if( ( (win->with)<20 ) || ( (win->hight)<20 ) ) return(0);		// ��ȡ��߶ȼ�飬������С����
   if( (win->x + win->with ) > GUI_LCM_XMAX ) return(0);			// ���ڿ���Ƿ����
   if( (win->y + win->hight ) > GUI_LCM_YMAX ) return(0);			// ���ڸ߶��Ƿ����
    
   /* �������� */
   GUI_RectangleFill(win->x, win->y, win->x + win->with - 1, win->y + win->hight - 1, back_color);
   return(1);
}



/****************************************************************************
* ���ƣ�GUI_WindowsClr()
* ���ܣ��������ڡ�
* ��ڲ�����win		���ھ��
* ���ڲ�������
* ˵����ʹ�ô˺���ǰҪ��ʹ��GUI_WindowsDraw()�����ڻ�����
****************************************************************************/
void  GUI_WindowsClr(WINDOWS *win)
{  uint8  x0, y0;
   uint8  x1, y1;
 
   /* ����Ҫ���������� */
   x0 = win->x + 1;
   x1 = win->x + win->with - 2;
   y0 = win->y + 13;
   y1 = win->y + win->hight - 2;
   if( win->state != NULL )			// �ж��Ƿ���״̬��				
   {  if( win->hight >= 60)
      {  y1 = win->y + win->hight - 12;
      }
   }

   /* ʹ��������ʵ������ */
   GUI_RectangleFill(x0, y0, x1, y1, back_color);
}
#endif












