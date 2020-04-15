#include <stdio.h>
#include <stdlib.h>
#include <string.h> 


// Считывание строки через построчный буферизированный ввод
char *getstr() 
{
  	char *ptr=(char *)malloc(1);                    
  	*ptr='\0';                                     
  	char buf[100];                                    
  	int n, len=0; 
  	
  	do {                                            
    		n=scanf("%99[^\n]", buf);                  
    		
    		if (n<0) {                                 
      		free(ptr);
      		ptr=NULL;
    		continue;
		}
    	
    	if (n==0) scanf("%*c");                                  
		
		else {                                       
      		len+=strlen(buf);                      
      		ptr=(char *)realloc(ptr, len+1);      
      		strcat(ptr, buf);                        
		}	
	} 
	while (n>0); 
    
    return ptr;	
}


// Функция для подсчёта едениц в слове
int count( char* str, int n)
{
	int con=0;
	for (int i=0; i<n; ++i)
	{
		if (str[i]=='1') ++con;
	}
	return con;
}


// Пропуск лидирующих пробелов
char* skip(char *s)
{
  	int k=strspn(s, " \t");     
  	return s+k;              
}

	
// Формирование итоговой строки
char *ref(char *s) 
{
  	char *res=(char *)calloc(strlen(s), sizeof(char)); 
	char *buf=s;
	
	int o=strcspn(buf, " \t\0");
	int g=count(buf,o);
	int j=0,k=0,e=0;
	
	while (*buf!='\0')
	{
		k=0;
		e=0;
		buf=skip(buf);
		k=strcspn(buf, " \t\0");
		e=count(buf,k);	
		
		if(e==g){
			strncat(res,buf,k);
			j+=k;
			buf+=k;
			*(res+j)=' ';
			++j;
		}
		
		else buf+=k;
	}
	
	if (*(res+j-1)==' ') *(res+j-1) ='\0';
	
	else *(res+j)='\0';
	
	res=(char*)realloc(res,strlen(res)+1);
	
	return res;
}


int main() {
  	char *p = NULL, *s = NULL;
	while (puts("Input"), s = getstr()) 
	{
    	printf("Input string: \"%s\"\n", s);
    		
    	p = ref(s);
    		
    	printf("Res string: \"%s\"\n", p);
    	printf("\n");
    		
    	free(s);
    	free(p);
  	}
  	
  	return 0;
}