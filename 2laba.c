#include <stdio.h>
#include <malloc.h>

void** ish(int n, int m, int **k)
{
    printf("Ishodnaya matrica:\n");
    int i,j;
    for(i=0; i<n; i++,printf("\n"))
    {
        m=*k[i];
        for(j=1; j<=m; j++) printf("%3d",k[i][j]);
    }   
}

void** res(int n, int m, int **k)
{
    printf("Resultat(poezd sdelal bum):\n");
    int i,j;
    for(i=0; i<n; i++,printf("\n"))
    {
        m=*k[i];
        for(j=1; j<=m; j++) printf("%3d",k[i][j]);
    } 
}

int** reform(int n, int m, int **k)
{
    int i,j,min,max;
    for(i=0; i<n; i++,printf("\n"))
    {
        min=214748647,max=0;  
        m=*k[i];
        for(j=1; j<=m; j++) 
        {    
            if (k[i][j]<=min) min=k[i][j]; 
            if (k[i][j]>=max) max=k[i][j];  
        }
        for(j=1; j<=m; j++) 
        {
            if (k[i][j]==min) k[i][j]=max;
            else {if (k[i][j]==max) k[i][j]=min;}
        }               
    } 
    return k;
}

int main()
{
	int i,j,n,m,**a,k,l,z,x,min,max;
    printf("Vvedite kol-vo strok:");
    scanf("%d",&n);
    a=(int**)malloc(n*sizeof(int*));
    for(i=0; i<n; i++)
    {
        printf("Vvedite kol-vo elementov v %d stroke:",i+1);
        scanf("%d",&m);
        a[i]=(int*)malloc((m+1)*sizeof(int));
        *a[i]=m;
        for(j=1; j<=m; j++) 
        {
            printf("a[%d][%d] = ",i,j);
        	scanf("%d",&a[i][j]);
        }
    } 

    ish(n,m,a);
    
    a=reform(n,m,a);

    res(n,m,a);
 
    for(i=0; i<n; i++) free(a[i]);
    free(a);
    getchar();
    return 0;
}