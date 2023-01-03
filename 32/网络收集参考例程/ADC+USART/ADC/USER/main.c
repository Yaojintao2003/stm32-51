//需要引入的文件
#include<stdio.h>
#include<math.h>
#define N 4
//如解答错误，注意检查错误信息提示，对比预期输出和实际输出之间的差异
int main() 
{
  int a[N][N]={{3,6,4,17},{8,5,9,10},{9,19,7,20},{4,14,21,23}};
  int i,r,j,x,s=0,t=0;
  for(i=0;i<N;i++)
  for(r=0;r<=i;r++)
  {
    for(j=2;j<=sqrt(a[i][r]);j++)
      if(a[i][r]%j==0)
      a[i][r]=0;
    for(j=2;j<=sqrt(a[i][r]);j++)
    {
      if(a[i][r]%j!=0)
       {
         while(a[i][r]+=2)
         {
            for(j=2;j<=sqrt(a[i][r]);j++)           
             if(a[i][r]%j!=0)
             break;
             if(j>sqrt(a[i][r]))
             break;
         }
       }
       if(j>sqrt(a[i][r]))
             break;
    }
     if(a[i][r]==3)
      a[i][r]+=2;   
  }
  for(i=0;i<N;i++)
  for(r=0;r<N;r++)
  {
  printf("%d ",a[i][r]);
    s++;
    if(s%4==0)
    printf("\n");
  }
  for(i=0;i<N;i++)
  for(r=0;r<=i;r++)
  {  
    for(j=2;j<=sqrt(a[i][r]);j++)
    if(a[i][r]%j!=0)
    t++;
  }
  printf("count=%d",t);
  return 0;
// your code
}