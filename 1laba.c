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
   return rc;
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
        if((d[i]%10)<=min) min=d[i]%10;
        if((d[i]%10)>=max) max=d[i]%10;
        d[i]/=10;
        --c[i];
       
      }
      
      re[i]=min+max;
    }

  return re;
}

int check(int *a, int k)
{
 
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
