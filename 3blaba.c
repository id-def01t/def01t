#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <locale.h>

typedef struct Elem
{                    //Элемент списка
  char ch;           //символ строки
  struct Elem *next; //Указатель на следующий элемент
} Elem;

//Создать список из строки
Elem *GetLine()
{
  char s[81];            //Временная строка 80 + 1 размера (' \0 ')
  Elem *Elem0 = NULL;    //Указатель на первый элемент
  Elem *ElemCurr = NULL; //Указатель на текущий элемент
  Elem *ElemPrev = NULL; //Указатель на предыдущий элемент
  scanf("%80[^\n]", s);  //Считаем строку
  for (int i = 0; i < strlen(s); i++)
  {                                          //Пробежимся по каждому символу строки
    ElemCurr = (Elem *)malloc(sizeof(Elem)); //Выделим память для элемента списка
    ElemCurr->ch = s[i];                     //Сохранить символ в списке
    if (ElemPrev)
    {                            //Если определён предыдущий элемент
      ElemPrev->next = ElemCurr; //Сослаться на текущий элемент
    }
    else
    {
      Elem0 = ElemCurr; //Или запомнить как первый элемент
    }
    ElemPrev = ElemCurr;
  }
  if (Elem0)
    ElemCurr->next = NULL; //Если список не пустой, то завершить как NULL
  return Elem0;
}

//Удалить число или разделители, начиная с текущего элемента
//где r = 0 - число, 1 - разделитель
Elem *DelElems(Elem *Elem1, int r)
{
  int r1;
  Elem *Elem2;
  while (Elem1)
  {                                                       //Пока не конец списка
    r1 = (Elem1->ch == ' ' || Elem1->ch == '\t') ? 1 : 0; //цифра или разделитель?
    if (r1 != r)
      break;             //Досрочный выход
    Elem2 = Elem1;       //Запомним элемент для удаления
    Elem1 = Elem1->next; //Перейдём на следующий элемент
    free(Elem2);         //Удалить элемент
  }
  return Elem1; //Указатель на первый неудалённый элемент
}

//Обработка строк
Elem *ProcessLine(Elem *Elem0)
{
  if (!Elem0)
    return 0;                 //Если список пустой, то выйти
  int flag = 0;               //Флаг на число не прошедшее критерий
  Elem *ElemCurr = Elem0;     //Запомним начало списка
  Elem *ElemPrev = Elem0;     //Указатель на предыдущий элемент
  Elem *ElemBeforeNum = NULL; //Указатель на элемент перед текущим числом
  while (1)
  { //Переберём все элементы
    if ((!ElemCurr) || (ElemCurr->ch == '\t') || (ElemCurr->ch == ' '))
    { //Если встретился разделитель или конец списка
      if (ElemCurr == Elem0)
      {                             //Если элемент первый, то
        Elem0 = DelElems(Elem0, 1); //Удалить разделители в начале списка
        if (!Elem0)
          return 0;       //Если список опустел, то выйти
        ElemCurr = Elem0; //Текущий элемент - начало списка
        ElemPrev = Elem0; //Предыдущий элемент - начало списка
        continue;         //Опять проверить новый первый элемент
      }
      else
      { //Число закончилось
        if (ElemCurr)
        {                                               //Если не конец списка,
          ElemCurr->next = DelElems(ElemCurr->next, 1); //то удалить лишние разделители,
          if (!ElemCurr->next)
          {                 //Если список закончился,
            free(ElemCurr); //то удалить последний разделитель
            ElemPrev->next = NULL;
            ElemCurr = NULL;
          }
          else
          {
            ElemCurr->ch = ' '; //иначе, заменить оставшийся разделитель на пробел
          }
        }
        if (flag)
        { //Если число не удовлетворяет критерию, то удалить его (изначально флаг 0 => ничего не потеряем)
          if (!ElemBeforeNum)
          {                             //Если число в начале строки,
            Elem0 = DelElems(Elem0, 0); //То удалить всё число, и переместить начало списка
            ElemPrev = Elem0;
            ElemCurr = Elem0;
          }
          else
          {                                                         //Если число не в начале строки
            ElemBeforeNum->next = DelElems(ElemBeforeNum->next, 0); //то удалить всё число от разделителя
            ElemCurr = ElemBeforeNum;                               //Текущий элемент - пробел стоявший перед числом
            ElemPrev = Elem0;                                       //С начала списка
            while (ElemPrev->next != ElemCurr)
              ElemPrev = ElemPrev->next; //найдём элемент перед текущим
          }
          flag = 0; //Сбросить флаг критерия отбора
          continue; //Опять проверим текущий элемент
        }
      }
    }
    else
    { //если встретилось число то проверяем его
      if ((ElemCurr->ch < '0') || (ElemCurr->ch > '9'))
        flag = 1; //Любой другой символ кроме цифры нарушает критерий отбора.
      if (ElemPrev->ch == ' ')
      {                           //Если цифра первая в числе, а число не первое
        ElemBeforeNum = ElemPrev; //то запомнить указатель на разделитель перед числом т.к сравниваются ПРЕДЫДУЩИЙ с ТЕКУЩИМ => необходимо чтобы как минимум 2 цифры шли подряд
      }
      else
      { //цифра не первая в числе
        if (ElemCurr->ch < ElemPrev->ch)
          flag = 1; //Если цифры в числе убывают, то поднять флаг нарушения критерия отбора
      }
    }
    if (!ElemCurr)
      break;                   //Выйти, если список закончился
    ElemPrev = ElemCurr;       //Запомним текущий элемент как предыдущий
    ElemCurr = ElemCurr->next; //Следующий элемент
  }
  return Elem0; //Вернуть указатель на начало списка
}

//Вывод результата
void PrintLine(Elem *ElemCurr)
{
  while (ElemCurr)
  {
    printf("%c", ElemCurr->ch);
    ElemCurr = ElemCurr->next;
  }
  printf("\n");
}

//Очистить память
void FreeElems(Elem *ElemCurr)
{
  Elem *Elem0;
  while (ElemCurr)
  {
    Elem0 = ElemCurr;
    ElemCurr = ElemCurr->next;
    free(Elem0);
  }
}

int main()
{
  while (1)
  {
    //Создать список
    printf("Input string \n");
    Elem *ElemFirst = GetLine();
    if (ElemFirst)
    { //Если список не пустой
      //Обработка строк
      ElemFirst = ProcessLine(ElemFirst);
      //Вывод результата
      printf("Output string \n");
      PrintLine(ElemFirst);
      //Очистить память
      FreeElems(ElemFirst);
    }
  }
  return 0;
}