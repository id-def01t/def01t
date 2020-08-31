#include <stdio.h>
#include <malloc.h>
#include <locale.h>
#include <string.h>


///////////////////// ��������� ////////////////////////
int find(int, int*);
void d_add(void);
void d_del(void);
void d_show(void);
void quit(void);
char *get_str();
int get_int(int*);
void clean_stdin(void);
void find_d(void);
////////////////////////////////////////////////////////


///////////////////// �� ������ ////////////////////////
const int SZ = 10;   // ������������ ������ �������

typedef struct Item
{
   int key;       //���� ��������
   char *info;       // ��������� �� ����������
} ITEM;

const char *msgs[] = { "1. �������� ����� �������", "2. ������� �������", "3. ������� �������", "4. ����� �� ���������", "5. �����" };
// ��������� ����

// ���������� ������� ����
int NMsgs = sizeof( msgs ) / sizeof( msgs[0] );

// ������� ��������� ������� ����
void ( *fptr[] )() ={ d_add, d_del, d_show, find_d, quit };

int count = 0;      // ���������� ��������������� ��������� �������

ITEM table[10];   // �������
///////////////////////////////////////////////////////


/////////////// ������ + ���� ������///////////////////
int dialog(const char* mes[], int max)
{
   int rc, gi;
   do
   {
      for ( int i = 0; i < max; i++ )
      {
         printf(" %s \n", mes[i]);
      }
      printf("��� �����: ");
      gi = get_int(&rc);
      if ( rc > max )
      {
         printf("������� ����� �� 1 �� 5 \n");
      }
   }
   while ( rc < 1 || rc > max );   // ��������� �� ������������
   return rc;                  // ���������� ����� ������
}

void clean_stdin() 
{
   while(fgetc(stdin) != '\n');
}

char *get_str() {
	char *ptr = (char *)malloc(1);
	char buf[81];
	int n, len = 0;
	*ptr = '\0';
	do 
   {
		n = scanf("%80[^\n]", buf);
		if (n < 0) 
      {
			free(ptr);
			ptr = NULL;
			continue;
		}
		if (n == 0)
      {
			scanf("%*c");
      }
      else 
      {
			len += strlen(buf);
			ptr = (char *)realloc(ptr, len + 1);
			strcat(ptr, buf);
		}
	} 
   while (n > 0);
	return ptr;
}

int get_int(int *a) 
{
	int n;
	do 
   {
		n = scanf("%d", a, sizeof(int));
		if (n < 0)			/*????????? ????? ????? */
		{	
            return 0;
        }
        if (n == 0) 
        {			/*????????? ???????????? ?????? - ?????? */
			printf("%s\n", "������� ����� �� 1 �� 5: \n");
			clean_stdin();
		}
	} 
   while (n == 0);
	clean_stdin();
	return 1;
}
//////////////////////////////////////////////////////////////


///////////////////// ������� /////////////////////////
// ����� �� ������� �������� � �������� ������
// ����������� ������� ��������� ������
// ���������� 1 ���� ������� + ������ ���������� ��������
// 0 ���� �� ������� + ������ ���� �������� ����� �������
// �������� ����������� �� ����������� ������
int find(int key, int *idx)
{
   int first = 0;      // ������ ������� ������� 
    int last = count;   // ������� ������� ��������� �� ��������� 
                        // ���� ������� �������� first<last 
    int mid; 
 
   if ((count == 0) || (table[0].key > key))       // ���� � ������� ��� ��� ��������� ��� ��������� ���� ������ ������� �� � ������
   {
      *idx = 0;
      return 0;
   } 
   else if (table[count-1].key < key)        // ��������� ���� ������ ���������� � ������� �� � �����
   {
      *idx = count;
      return 0;
   }
 
   while (first < last)       // ���� �� ������ �� ������ ������� �� ������� ��� �� � �������� 
   {
      mid = (first+last) / 2;
      if (key <= table[mid].key)
      {
         last = mid;
      }
      else
      {
         first = mid + 1;
      }
   }
   if (table[last].key == key)         // ������� � ������ ������ ���������� ��� ������
   {
      *idx = last;
      return 1;
   } 
   else
   {
      *idx = last;         // ������� � ������ �� ������ ��� ����� last
      return 0;
   }
}

// ����� �� ���������
void find_d()
{
   if (count)        // ���� ������� �� �����
   { 
      int gi, l1, l2, k1, k2, idx, cnt = 0;
      check:
      printf("������� ����� ����� ���������: ");
      gi = get_int(&l1);       // ������ �������������� ���� ��������
      if (find(l1, &idx))        // ���� ���� ������� � ����� ������ �� ��������� �� ���� 
      {
         k1 = idx;
      }
      else        // ���� �������� � ����� ������ ���            
      {
         find(l1, &idx);         // �� ������ �� find �������� idx ��������������� �� ����� ��������     
         k1 = idx;
      }
      printf("������� ������ ����� ���������: ");
      gi = get_int(&l2);       // ������ �������������� ���� ��������
      if (find(l2, &idx))        // ���� ���� ������� � ����� ������ �� ��������� �� ���� 
      {
         k2 = idx;
      }  
      else        // ���� �������� � ����� ������ ���   
      {
         find(l2, &idx);         // �� ������ �� find �������� idx ���������� �� ����� �������� 
         k2 = idx - 1;        // �������� idx �� 1 ��������� �� ������ ���� ���������
      }
      
      ITEM rtab[10];       // ����� ������� 

      for( int i = k1; i <= k2; i++)         // ��������� � ����� ������� ������ ��������
      {
         rtab[i].key = table[i].key;
         rtab[i].info = malloc((strlen(table[i].info) + 1) * sizeof(char));
         strcpy(rtab[i].info, table[i].info);
         cnt++;
      }
      if (cnt == 0)     
      {
         printf("�������� ���� \n");
      }
      else        // ���� ����� ������� �� ����� �� �������
      {
         printf("-------- �������������� ������� --------\n");
         for( int i = k1; i <= k2; i++) 
         {
            printf(" %d : key = %d \t\t info = %s \n", i, rtab[i].key, rtab[i].info);   
         }
      }
}
else printf("������� ����� \n");
}

// ���������� �������� � �������
void d_add()
{
   int i, idx, num, gi;
   char *str = NULL;
   if (count < SZ)            // ���� � ������� ���� ����� 
   {
      printf("������� ���� ��������: ");
      gi = get_int(&num);            // ������ ���� ������ ��������
      if (find(num, &idx))      // ��������� ���� �� ����� � ������� � �������� ������ ��� �������
      {
         printf("������� � ����� ������ ��� ���� \n");
         return;
      }
      if (count)               // ���� ������� �� ����� 
      {   
         for (i=count-1; i>=idx; i--)   // ������� �� ���� ������� ������
         {
            table[i+1] = table[i];      // ��� ������������ ������� idx ��� ����� ������� 
         }
      }
      table[idx].key = num;     
      printf("������� ������: ");        
      str = get_str();     
      table[idx].info = malloc((strlen(str) + 1) * sizeof(char)); 
      strcpy(table[idx].info, str);
      count++;
      printf("������� ������� �������� \n");
   }
   else printf("������� ��������� ��������� \n");
}

// �������� �������� �� �����
void d_del()
{
   if ( count )        // ���� ������� �� ����� 
   {
      int idx, num, gi;
      printf("������� ���� ��������: ");
      gi = get_int(&num); 
      if (!find(num, &idx))         // ��������� ���� �� ����� ���� � ������� ���� ��� - ������ ���� �� - �������� ��� ������
      {
         printf("������� � ����� ������ �����������   \n");
         return;
      }
      free(table[idx].info);        // ��������� ������ 
      for (; idx<count-1; idx++)       // ������� �� ���� ������� � ������ ��� �������� ����� idx 
      {
         table[idx] = table[idx+1];       // ��� ������� �������
      }
      count--;       // ��������� ������� ���������
      printf("������� ������ \n");
   }
   else printf("������� ����� \n");
}

//����� ���� ������� �� �����
void d_show()
{
   if (count)        // ���� ������� �� ����� 
   {
      printf("--------------- ������� --------------- \n");
      for(int i = 0; i < count; i++)
      {
        printf(" %d: key = %d \t\t info = %s \n", i, table[i].key, table[i].info);
      }
   }
   else printf("������� ����� \n");
}


// ����� ������
void quit()
{ 
   for ( int i = 0; i < count; i++)         // ������ ������� �� ������
   {
      free(table[i].info);
   }
}
///////////////////////////////////////////////////////


int main()
{
   int rc;
   setlocale(LC_ALL,"Russian");         // ����� �������� ��-������ � �������
   do
   {
      rc = dialog(msgs, NMsgs);   // ������� ������ ����
      fptr[rc-1]();                  // ������������ ����� ����
      printf("--------------------------------------- \n");
      if (rc == NMsgs)  // ���� ������ quit
      {
         quit();        // ������� ������ 
         printf("�� ����� \n");
         break;
      }                 // �������
   } while (rc);
   return 0;
}