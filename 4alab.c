#include <stdio.h>
#include <malloc.h>
#include <locale.h>
#include <string.h>


///////////////////// Прототипы ////////////////////////
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


///////////////////// По мелочи ////////////////////////
const int SZ = 10;   // максимальный размер таблицы

typedef struct Item
{
   int key;       //ключ элемента
   char *info;       // указатель на информацию
} ITEM;

const char *msgs[] = { "1. Добавить новый элемент", "2. Удалить элемент", "3. Вывести таблицу", "4. Поиск по диапазону", "5. Выход" };
// сообщения меню

// количество пунктов меню
int NMsgs = sizeof( msgs ) / sizeof( msgs[0] );

// функции отработки пунктов меню
void ( *fptr[] )() ={ d_add, d_del, d_show, find_d, quit };

int count = 0;      // количество задействованных элементов таблицы

ITEM table[10];   // таблица
///////////////////////////////////////////////////////


/////////////// Диалог + ввод данных///////////////////
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
			printf("%s\n", "Введите число от 1 до 5: \n");
			clean_stdin();
		}
	} 
   while (n == 0);
	clean_stdin();
	return 1;
}
//////////////////////////////////////////////////////////////


///////////////////// Функции /////////////////////////
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

// поиск по диапахону
void find_d()
{
   if (count)        // если таблица не пуста
   { 
      int gi, l1, l2, k1, k2, idx, cnt = 0;
      check:
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
      
      ITEM rtab[10];       // новая таблица 

      for( int i = k1; i <= k2; i++)         // сохраняем в новую таблицу нужный диапазон
      {
         rtab[i].key = table[i].key;
         rtab[i].info = malloc((strlen(table[i].info) + 1) * sizeof(char));
         strcpy(rtab[i].info, table[i].info);
         cnt++;
      }
      if (cnt == 0)     
      {
         printf("Диапазон пуст \n");
      }
      else        // если новая таблица не пуста то выводим
      {
         printf("-------- Результирующая таблица --------\n");
         for( int i = k1; i <= k2; i++) 
         {
            printf(" %d : key = %d \t\t info = %s \n", i, rtab[i].key, rtab[i].info);   
         }
      }
}
else printf("Таблица пуста \n");
}

// добавление элемента в таблицу
void d_add()
{
   int i, idx, num, gi;
   char *str = NULL;
   if (count < SZ)            // если в таблице есть место 
   {
      printf("Введите ключ элемента: ");
      gi = get_int(&num);            // вводим ключ нового элемента
      if (find(num, &idx))      // проверяем есть ли такой в таблице и получаем индекс для вставки
      {
         printf("Элемент с таким ключем уже есть \n");
         return;
      }
      if (count)               // если таблица не пуста 
      {   
         for (i=count-1; i>=idx; i--)   // смещаем на одну позицию вперед
         {
            table[i+1] = table[i];      // для освобождения нужного idx под новый элемент 
         }
      }
      table[idx].key = num;     
      printf("Введите строку: ");        
      str = get_str();     
      table[idx].info = malloc((strlen(str) + 1) * sizeof(char)); 
      strcpy(table[idx].info, str);
      count++;
      printf("Элемент успешно вставлен \n");
   }
   else printf("Таблица полностью заполнена \n");
}

// удаление элемента по ключу
void d_del()
{
   if ( count )        // если таблица не пуста 
   {
      int idx, num, gi;
      printf("Введите ключ элемента: ");
      gi = get_int(&num); 
      if (!find(num, &idx))         // проверяем есть ли такой ключ в таблице если нет - ошибка если да - запомним его индекс
      {
         printf("Элемент с таким ключем отсутствует   \n");
         return;
      }
      free(table[idx].info);        // освободим память 
      for (; idx<count-1; idx++)       // смещаем на одну позицию к началу все элементы после idx 
      {
         table[idx] = table[idx+1];       // так удаляем элемент
      }
      count--;       // уменьшаем счетчик элементов
      printf("Элемент удален \n");
   }
   else printf("Таблица пуста \n");
}

//вывод всей таблицы на экран
void d_show()
{
   if (count)        // если таблица не пуста 
   {
      printf("--------------- Таблица --------------- \n");
      for(int i = 0; i < count; i++)
      {
        printf(" %d: key = %d \t\t info = %s \n", i, table[i].key, table[i].info);
      }
   }
   else printf("Таблица пуста \n");
}


// конец работы
void quit()
{ 
   for ( int i = 0; i < count; i++)         // удалим таблицу из памяти
   {
      free(table[i].info);
   }
}
///////////////////////////////////////////////////////


int main()
{
   int rc;
   setlocale(LC_ALL,"Russian");         // чтобы писалось по-русски в консоли
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