#include <locale.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>


///////////////////// Прототипы ////////////////////////
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


///////////////////// По мелочи ////////////////////////
const int   SZ = 10;   //максимальный размер таблицы
typedef struct Item
{
   int      key;      //ключ элемента
   int      seek;      //смещение в файле поля info
   int      len;      //длина info
}ITEM;
const char *msgs[] = { "1. Добавить новый элемент", "2. Удалить элемент", "3. Вывести таблицу", "4. Поиск по диапазону", "5. Выход" };
// сообщения меню
// количество пунктов меню
int NMsgs = sizeof( msgs ) / sizeof( msgs[0] );
// функции отработки пунктов меню
void ( *fptr[] )() ={ d_add, d_del, d_show, find_d, quit };
int count = 0;      // количество задействованных элементов таблицы
ITEM table[10];   // таблица
FILE *file = NULL;   // указатель на открытый файл
char *fName = NULL;      // имя файла
////////////////////////////////////////////////////////


///////////////////// диалог + ввод /////////////////
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

// вывод меню и ввод пункта меню
int dialog(const char* mes[], int max)
{
   int rc, gi;
   do
   {
      for ( int i = 0; i < max; i++ )
      {
         printf(" %s \n", mes[i]);
      }
      printf("Ваш выбор: ");
      gi = get_int(&rc);
      if ( rc > max )
      {
         printf("Введите число от 1 до 5 \n");
      }
   }
   while ( rc < 1 || rc > max );   // проверяем на допустимость
   return rc;                  // возвращаем номер пункта
}
///////////////////////////////////////////////////////


///////////////////// Функции /////////////////////////
// чтение таблицы из файла
// формата
// int   count       - количество задействованных элементов таблицы
// ITEM   table[SZ]         - таблица (вся)
// ...         - строки info
void r_table()
{
   printf("Введите имя файла: ");
   fName = get_str();         // вводим имя файла
   file = fopen(fName, "r+b");         // открываем
   if (NULL == file)       // если такого нет
   {
      file = fopen(fName, "w+b");         // то создаем новый 
      if (file)         // если создался запишем файл с пустой таблицей
      {                 
         count = 0;            // обнулим количество (на всякий случай)
         fwrite(&count, sizeof(int), 1, file);        // впишем количество
         fwrite(table, sizeof(ITEM), SZ, file);        // и таблицу
      }
      else printf("Ошибка создания файла \n");
   }
   else        // файл есть
   {                       
      fread(&count, sizeof(int), 1, file);         // читаем количество
      fread(table, sizeof(ITEM), SZ, file);         // и таблицу
   }
   fclose(file);        //файл закрываем
}


//запись таблицы в файл (в конце работы)
void w_table()
{
   if (fName[0])        // если задано имя файла
   {
      file = fopen(fName, "r+b");         // открываем 
      if (file)         // если открылся 
      {
         fwrite(&count, sizeof(int), 1, file);        // впишем количество
         fwrite(table, sizeof(ITEM), SZ, file);        // и таблицу
         fclose(file);        // файл закрываем
      }
      else printf("Ошибка открытия \n");
   }
   else printf("Имя файла не задано \n");
}


// поиск по таблице элемента с заданным ключем
// использован агоритм двоичного поиска
// возвращает 1 если найдено + индекс найденного элемента
// 0 если не найдено + индекс куда вставить новый элемент
// элементы сортируются по возрастанию ключей
int find(int key, int *idx)
{
   int first = 0;      // Первый элемент таблицы 
    int last = count;   // Элемент таблицы следующий за последним 
                        // Если участок непустой first<last 
    int mid; 
 
   if ((count == 0) || (table[0].key > key))       // есди в таблице еще нет элементов или введенный ключ меньше первого то в начало
   {
      *idx = 0;
      return 0;
   } 
   else if (table[count-1].key < key)        // введенный ключ больше последнего в таблице то в конец
    {
      *idx = count;
      return 0;
   }
 
   while (first < last)       // если ни первое ни второе условие то элемент где то в середине 
   {
      /* ВНИМАНИЕ! В отличие от более простого (first+last)/2, этот код стоек к переполнениям.
         Если first и last знаковые, возможен код (unsigned)(first+last) >> 1.    */
      mid = (first+last) / 2;
      if (key <= table[mid].key)
         last = mid;
      else
         first = mid + 1;
   }
   if (table[last].key == key)         // элемент с ключом найден запоминаем его индекс
   {
      *idx = last;
      return 1;
   } 
   else
   {
      *idx = last;         // элемент с ключом не найден его место last
      return 0;
   }
}


// очистка потока


//добавление элемента в таблицу
void d_add()
{
   int i, idx, num, gi;
   char *str = NULL;
   if (count < SZ)       // если есть место 
   {
      printf("Введите ключ элемента: ");
      gi = get_int(&num);        // вводим ключ нового элемента
      if (find(num, &idx))       // проверка на наличие эл-ма с таким ключом
      {
         printf("Элемент с таким ключем уже есть \n");
         return;
      }
      if (count)        // если таблица не пуста 
         for (i=count-1; i>=idx; i--)        // смещаем на одну позицию вперед
         {
            table[i+1] = table[i];        // освобождая нужный idx под новый элемент 
         }
      table[idx].key = num;         // запоминаем ключ
      printf("Введите строку: ");
      str = get_str();   
      if (fName[0])        /* запишем данные в элемент таблицы  и в файл */ /* если задано имя файла */ 
      {
         file = fopen(fName, "r+b");         // открываем
         if (file)         // если открылся
         {
            fseek(file, 0, SEEK_END);        // идем конец файла
            table[idx].seek = ftell(file);         // запоминаем позицию конца файла как смещение в файле поля info
            table[idx].len = strlen(str)+1;        // запоминаем длину строки
            fwrite(str, table[idx].len, 1, file);        // пишем строку в конец файла
            fclose(file);        // закрываем файл
            count++;                        
         }
         else printf("Ошибка открытия \n");
      }
      else printf("Имя файла не задано \n");
   }
   else printf("Таблица полностью заполнена \n");
}


// удаление элемента таблицы по ключу
void d_del()
{
   if ( count )         // если таблица не пуста 
   {
      int idx, num, gi;
      printf("Введите ключ элемента: ");
      gi = get_int(&num);        // вводим ключ   
      if (!find(num, &idx))         // есть ли такой + узнаем индекс
      {
         printf("Элемент с таким ключем отсутствует \n");
         return;
      }
      for (; idx<count-1; idx++)       // смещаем на одну позицию к началу
      {
         table[idx] = table[idx+1];       // тем самым, удаляем элемент
      }
      count--;          // уменьшаем счетчик элементов
      printf("Элемент удален \n");
   }
   else printf("Таблица пуста \n");
}


// вывод таблицы
void d_show()
{
   char str[80];
   if (count)        // если таблица не пуста
   {
      if (fName[0])        // если задано имя файла
      {
         file = fopen(fName, "r+b");         // открываем файл
         if (file)         // если открылся
         {
            printf("--------------- Таблица --------------- \n");
            for(int i=0; i<count; i++)       // по всем элементам читаем строки
            {
               fseek(file, table[i].seek, SEEK_SET);        // в начало строки
               fread(str, table[i].len, 1, file);        // читаем строку
               printf(" %d : key = %d \t\t info = %s \n", i, table[i].key, str);         // выводим элемент
            }
         }
         else printf("Ошибка открытия \n");
      }
      else printf("Не задано имя файла \n");
   }
   else printf("Таблица пуста \n");
}


void find_d()
{
   if (count)       // если таблица не пуста 
   {
      if (fName[0])         // если задано имя файла 
      {
         file = fopen(fName, "r+b");   // открываем файл
         if (file)               // если открылся
         {
            int gi, l1, l2, k1, k2, idx, cnt = 0;
            char str[80];
            printf("Введите левую метку диапазона: ");
            gi = get_int(&l1);       // вводим предполагаемый ключ элемента
            if (find(l1, &idx))        // если есть элемент с таким ключом то цепляемся за него 
            {
               k1 = idx;
            }
            else        // если элемента с таким ключом нет            
            {
               find(l1, &idx);         // на выходе из find получаем idx предшествующего по ключу элемента     
               k1 = idx;
            }
            printf("Введите правую метку диапазона: ");
            gi = get_int(&l2);       // вводим предполагаемый ключ элемента
            if (find(l2, &idx))        // если есть элемент с таким ключом то цепляемся за него 
            {
               k2 = idx;
            }  
            else        // если элемента с таким ключом нет   
            {
               find(l2, &idx);         // на выходе из find получаем idx следующего по ключу элемента 
               k2 = idx - 1;        // уменьшая idx на 1 цепляемся за правый край диапазона
            }
            
            ITEM rtab[10];
            
            printf("-------- Результирующая таблица --------\n");
            for( int i = k1; i <= k2; i++)
            {
               rtab[i].key = table[i].key;
               fseek(file, table[i].seek, SEEK_SET);        // в начало строки
               fread(str, table[i].len, 1, file);        // читаем строку
               printf(" %d : key = %d \t\t info = %s \n", i, rtab[i].key, str); 
               cnt++;
            }
            if (cnt == 0)
            {
               printf("Диапазон пуст \n");
            }
         }
         else printf("Ошибка открытия \n");
      }
      else printf("Не задано имя файла \n");
   }
   else printf("Таблица пуста  \n");
}


// конец работы
void quit()
{
   w_table();      // сохраним таблицу в файл
}

int main()
{
   int rc;
   setlocale(LC_ALL,"Russian");         // чтобы писалось по-русски в консоли
   r_table();
   do
   {
      rc = dialog(msgs, NMsgs);   // выводим пункты меню
      fptr[rc-1]();                  // отрабатываем пункт меню
      printf("--------------------------------------- \n");
      if (rc == NMsgs)  // если выбран quit
      {
         quit();        // очищаем память 
         printf("До связи \n");
         break;
      }                 // выходим
   } while (rc);
   return 0;
}