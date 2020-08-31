#include <locale.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>


///////////////////// ��������� ////////////////////////
void find_d();
void clean_stdin(void);
void r_table(void);
void w_table(void);
int find(int, int*);
void d_add(void);
void d_del(void);
void d_show(void);
void quit(void);
int get_int(int*);
char *get_str();
////////////////////////////////////////////////////////


///////////////////// �� ������ ////////////////////////
const int   SZ = 10;   //������������ ������ �������
typedef struct Item
{
   int      key;      //���� ��������
   int      seek;      //�������� � ����� ���� info
   int      len;      //����� info
}ITEM;
const char *msgs[] = { "1. �������� ����� �������", "2. ������� �������", "3. ������� �������", "4. ����� �� ���������", "5. �����" };
// ��������� ����
// ���������� ������� ����
int NMsgs = sizeof( msgs ) / sizeof( msgs[0] );
// ������� ��������� ������� ����
void ( *fptr[] )() ={ d_add, d_del, d_show, find_d, quit };
int count = 0;      // ���������� ��������������� ��������� �������
ITEM table[10];   // �������
FILE *file = NULL;   // ��������� �� �������� ����
char *fName = NULL;      // ��� �����
////////////////////////////////////////////////////////


///////////////////// ������ + ���� /////////////////
void clean_stdin() 
{
    while(fgetc(stdin) != '\n');
}

char *get_str() 
{
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
			printf("%s\n", "Enter a number: \n");
			clean_stdin();
		}
	} 
    while (n == 0);
	clean_stdin();
	return 1;
}

// ����� ���� � ���� ������ ����
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
///////////////////////////////////////////////////////


///////////////////// ������� /////////////////////////
// ������ ������� �� �����
// �������
// int   count       - ���������� ��������������� ��������� �������
// ITEM   table[SZ]         - ������� (���)
// ...         - ������ info
void r_table()
{
   printf("������� ��� �����: ");
   fName = get_str();         // ������ ��� �����
   file = fopen(fName, "r+b");         // ���������
   if (NULL == file)       // ���� ������ ���
   {
      file = fopen(fName, "w+b");         // �� ������� ����� 
      if (file)         // ���� �������� ������� ���� � ������ ��������
      {                 
         count = 0;            // ������� ���������� (�� ������ ������)
         fwrite(&count, sizeof(int), 1, file);        // ������ ����������
         fwrite(table, sizeof(ITEM), SZ, file);        // � �������
      }
      else printf("������ �������� ����� \n");
   }
   else        // ���� ����
   {                       
      fread(&count, sizeof(int), 1, file);         // ������ ����������
      fread(table, sizeof(ITEM), SZ, file);         // � �������
   }
   fclose(file);        //���� ���������
}


//������ ������� � ���� (� ����� ������)
void w_table()
{
   if (fName[0])        // ���� ������ ��� �����
   {
      file = fopen(fName, "r+b");         // ��������� 
      if (file)         // ���� �������� 
      {
         fwrite(&count, sizeof(int), 1, file);        // ������ ����������
         fwrite(table, sizeof(ITEM), SZ, file);        // � �������
         fclose(file);        // ���� ���������
      }
      else printf("������ �������� \n");
   }
   else printf("��� ����� �� ������ \n");
}


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
      /* ��������! � ������� �� ����� �������� (first+last)/2, ���� ��� ����� � �������������.
         ���� first � last ��������, �������� ��� (unsigned)(first+last) >> 1.    */
      mid = (first+last) / 2;
      if (key <= table[mid].key)
         last = mid;
      else
         first = mid + 1;
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


// ������� ������


//���������� �������� � �������
void d_add()
{
   int i, idx, num, gi;
   char *str = NULL;
   if (count < SZ)       // ���� ���� ����� 
   {
      printf("������� ���� ��������: ");
      gi = get_int(&num);        // ������ ���� ������ ��������
      if (find(num, &idx))       // �������� �� ������� ��-�� � ����� ������
      {
         printf("������� � ����� ������ ��� ���� \n");
         return;
      }
      if (count)        // ���� ������� �� ����� 
         for (i=count-1; i>=idx; i--)        // ������� �� ���� ������� ������
         {
            table[i+1] = table[i];        // ���������� ������ idx ��� ����� ������� 
         }
      table[idx].key = num;         // ���������� ����
      printf("������� ������: ");
      str = get_str();   
      if (fName[0])        /* ������� ������ � ������� �������  � � ���� */ /* ���� ������ ��� ����� */ 
      {
         file = fopen(fName, "r+b");         // ���������
         if (file)         // ���� ��������
         {
            fseek(file, 0, SEEK_END);        // ���� ����� �����
            table[idx].seek = ftell(file);         // ���������� ������� ����� ����� ��� �������� � ����� ���� info
            table[idx].len = strlen(str)+1;        // ���������� ����� ������
            fwrite(str, table[idx].len, 1, file);        // ����� ������ � ����� �����
            fclose(file);        // ��������� ����
            count++;                        
         }
         else printf("������ �������� \n");
      }
      else printf("��� ����� �� ������ \n");
   }
   else printf("������� ��������� ��������� \n");
}


// �������� �������� ������� �� �����
void d_del()
{
   if ( count )         // ���� ������� �� ����� 
   {
      int idx, num, gi;
      printf("������� ���� ��������: ");
      gi = get_int(&num);        // ������ ����   
      if (!find(num, &idx))         // ���� �� ����� + ������ ������
      {
         printf("������� � ����� ������ ����������� \n");
         return;
      }
      for (; idx<count-1; idx++)       // ������� �� ���� ������� � ������
      {
         table[idx] = table[idx+1];       // ��� �����, ������� �������
      }
      count--;          // ��������� ������� ���������
      printf("������� ������ \n");
   }
   else printf("������� ����� \n");
}


// ����� �������
void d_show()
{
   char str[80];
   if (count)        // ���� ������� �� �����
   {
      if (fName[0])        // ���� ������ ��� �����
      {
         file = fopen(fName, "r+b");         // ��������� ����
         if (file)         // ���� ��������
         {
            printf("--------------- ������� --------------- \n");
            for(int i=0; i<count; i++)       // �� ���� ��������� ������ ������
            {
               fseek(file, table[i].seek, SEEK_SET);        // � ������ ������
               fread(str, table[i].len, 1, file);        // ������ ������
               printf(" %d : key = %d \t\t info = %s \n", i, table[i].key, str);         // ������� �������
            }
         }
         else printf("������ �������� \n");
      }
      else printf("�� ������ ��� ����� \n");
   }
   else printf("������� ����� \n");
}


void find_d()
{
   if (count)       // ���� ������� �� ����� 
   {
      if (fName[0])         // ���� ������ ��� ����� 
      {
         file = fopen(fName, "r+b");   // ��������� ����
         if (file)               // ���� ��������
         {
            int gi, l1, l2, k1, k2, idx, cnt = 0;
            char str[80];
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
            
            ITEM rtab[10];
            
            printf("-------- �������������� ������� --------\n");
            for( int i = k1; i <= k2; i++)
            {
               rtab[i].key = table[i].key;
               fseek(file, table[i].seek, SEEK_SET);        // � ������ ������
               fread(str, table[i].len, 1, file);        // ������ ������
               printf(" %d : key = %d \t\t info = %s \n", i, rtab[i].key, str); 
               cnt++;
            }
            if (cnt == 0)
            {
               printf("�������� ���� \n");
            }
         }
         else printf("������ �������� \n");
      }
      else printf("�� ������ ��� ����� \n");
   }
   else printf("������� �����  \n");
}


// ����� ������
void quit()
{
   w_table();      // �������� ������� � ����
}

int main()
{
   int rc;
   setlocale(LC_ALL,"Russian");         // ����� �������� ��-������ � �������
   r_table();
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