#include <stdio.h>
#include <malloc.h>


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


    printf("Ishodnaya matrica:\n");
    for(i=0; i<n; i++,printf("\n"))
    {
        m=*a[i];
        for(j=1; j<=m; j++) printf("%3d",a[i][j]);
    } 
    
    for(i=0; i<n; i++,printf("\n"))
    {
    	min=1000,max=0;
        m=*a[i];
        for(j=1; j<=m; j++) 
        {    
            if (a[i][j]<=min) min=a[i][j]; 
            if (a[i][j]>=max) max=a[i][j];  
        }
        for(j=1; j<=m; j++) 
        {
            if (a[i][j]==min) a[i][j]=max;
            else {if (a[i][j]==max) a[i][j]=min;}
        }		    	
    } 

    printf("Resultat(poezd sdelal bum):\n");
    for(i=0; i<n; i++,printf("\n"))
    {
        m=*a[i];
        for(j=1; j<=m; j++) printf("%3d",a[i][j]);
    } 
 
    for(i=0; i<n; i++) free(a[i]);
    free(a);
    getchar();
    return 0;
}