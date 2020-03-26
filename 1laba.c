#include <stdio.h>
#include <malloc.h>

int* cout(int *a, int k)
{
  int d[k];
  for (int i=0; i<k; i++)
  {
     d[i]=a[i];
  }
  int *rc = (int* ) malloc(sizeof(int)*k);
  for (int i=0; i<k; i++)
  {
     int count=0;
     while (d[i]!=0)
    {        
      d[i]/=10;
      ++count;
    }
   rc[i]=count;
   }
   return &rc[0];
}

int* minmax(int *a,int k)
{
  int *c;
  int d[k];
  int *re = (int* ) malloc(sizeof(int)*k);
  for (int i=0; i<k; i++)
    {
      d[i]=a[i];
    }
    c = cout(d,k);
    for (int i=0; i<k; i++)
    {
      int min=9, max=0;
      while (c[i]>0)
      {
        if((d[i]%10)<min) min=d[i]%10;
        if((d[i]%10)>max) max=d[i]%10;
        d[i]/=10;
        --c[i];
      }
      re[i]=min+max;
    }
  return &re[0];
}

int* check(int *a, int k)
{
  int *por = (int* ) malloc(sizeof(int)*k);
  int *q,*w,*e,x,y,c,qm[k],wm[k],count=0;
  for(int i=0; i<k; i++)
  {
    qm[i]=a[i];
  }
  for(int i=0; i<k; i++)
  {
    wm[i]=a[i];
  }
  q=cout(qm,k);
  w=minmax(wm,k);
  for (int i=0; i<k; i++)
  {
    if (w[i] == w[k-1]) por[i]=a[i];
    else por[i]=0;
  }
  e=cout(por,k);
  for (int i=0; i<k; i++)
  {
    for (int i=0; i<k; i++)
    { 
      if (e[i]>e[i+1])
      {
        x=e[i];y=por[i];e[i]=e[i+1];por[i]=por[i+1];e[i+1]=x;por[i+1]=y;
      }
    }
  } 
  free(q);
  free(w);
  return &por[0];
}




int main()
{    
  int i,n,l;
  int *c,*f,*p;
  printf("Vvedite kol-vo chisel\n" );
  scanf("%i",&n);
  printf("Vvedite chisla tekushei posledovatel'nosti\n" );
  int *a = (int* ) malloc(sizeof(int)*n);
  for (i = 0; i<n; i++)
    {
      printf("a[%d] = ", i);
      scanf("%d", &a[i]);
    }
  p = check(a,n);
  for (int i=0; i<n; i++)
  {
    if (p[i]==0) ++l;
  }
  printf("\n");
  printf("Chisla novoi posledovatel'nosti\n");
  for (int i=l; i<n; i++)
  {
    printf("b[%i] = %i\n",i-l+1,p[i] );
  }
  free(p);
}
//проверить всякие бяки связанные с выводом и так далее//