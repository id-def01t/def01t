#include <stdio.h>
#include <malloc.h>
#include <string.h>


int* cout(int *a, int k)
{
  int *d = (int*) malloc(sizeof(int)*k);
  int *rc = (int*) malloc(sizeof(int)*k);
  
  memcpy(d,a,(sizeof(int)*k));
  
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

  free(d);

  return rc;
}


int* minmax(int *a,int k)
{
  int *c = (int*) malloc(sizeof(int)*k);
  int *d = (int*)  malloc(sizeof(int)*k);
  int *re = (int*) malloc(sizeof(int)*k);
  
  memcpy(d,a,(sizeof(int)*k));
    
  c = cout(d,k);

  for (int i=0; i<k; i++)
  {
    int min=9, max=0;
    while (c[i]>0)
    {
     if((d[i]%10)<=min) min=d[i]%10;
     if((d[i]%10)>=max) max=d[i]%10;
     d[i]/=10;
     --c[i]; 
    }  
    re[i]=min+max;
  }

  free(d);
  free(c);

  return re;
}


int check(int *a, int k)
{
  int x,y,c,count=0; 
  int *q = (int*) malloc(sizeof(int)*k);
  int *w = (int*)malloc(sizeof(int)*k);
  int *e = (int*)malloc(sizeof(int)*k);
  
  q=cout(a,k);
  w=minmax(a,k);
  
  for (int i=0; i<k; i++)
  {
    if (w[i] == w[k-1])  {++count; a = (int*) realloc (a, (k-count) * sizeof(int)); a[count-1]=a[i];}
  }

  e=cout(a,k);
  
  for (int j=0; j<count-1; j++)
  {
    for (int i=0; i<count-1-j; i++)
    { 
      if (e[i]>e[i+1])
      {
        x=e[i];y=a[i];e[i]=e[i+1];a[i]=a[i+1];e[i+1]=x;a[i+1]=y;
      }
    }
  } 

  free(q);
  free(w);
  free(e);

  return count;
}


int main()
{    
  int i,n,l;
  int *c,*f;
  printf("Vvedite kol-vo chisel\n" );
  scanf("%i",&n);
  printf("Vvedite chisla tekushei posledovatel'nosti\n" );
  int *a = (int* ) malloc(sizeof(int)*n);
  
  for (i = 0; i<n; i++)
  {
    printf("a[%d] = ", i);
    scanf("%d", &a[i]);
  }
  
  int p = check(a,n);
 
  printf("\n");
  printf("Chisla novoi posledovatel'nosti\n");
  
  for (i=0; i<p; i++)
  {
    printf("%i\n",a[i] );
  }
  
  free(a);
}
