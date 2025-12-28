/*
Автор: студент ВГТУ ФИТКБ группы ИЦ-251 Кравченко В. В.
*/

#include <string.h> 
#include <stdio.h>
#include <stdbool.h>
#include <locale.h>
#include <stdlib.h>
#include <time.h>
#include "Check_rule.h"

#define N 9       // строка
#define MAX 1000  // максимальное кол-во попыток создания массива
#define CHECK 10  // число, которое должен ввести пользователь для проверки "прохождения игры"
#define CLEAR 15  // число, которое должен ввести пользователь для сброса игрового поля к "пустому" виду

/*
Отчищает массив пользователя (все ячейки незаполнены)
   @param arr_u - массив, который будет заполнятся символами;
   @param size - размер массива arr;
   @return 1 (побочно), если массив успешно заполнен 
*/
int clear_arr_user(char* arr_user, int size);
/*
Выводит двумерный массив и поля в консоль
   @param arr_u - двумерный массив, выводимы в центре консоли;
   @param size - размер массива arr;
   @param field_С_R - двумерный массив, формирующий поля; 
   @return 1 (побочно), если вывод успешен

*/
int output(const char* arr_user, int size, int* field_С_R);
/*
Записывает поля головоломки в файл и закрывает файл.
   @param file_txt - файл, куда будет вестить запись двумерного массива-поля;
   @param field_С_R - двумерный массив, формирующий поля; 
	  @param size - размер первого измерения массива field_C_R, второе измерение массива - 2;
   @return 1 (побочно), если запись успешна.
*/
int file_out(FILE* file_txt, const int* field_C_R, int size);


int main()
{
	setlocale(LC_ALL, "RUS");

	int task = 0;        
	int size = N;        
	int field_С_R[2][N]; 
	char arr[N][N];      
	char arr_user[N][N]; 
	int amount_r[N];     
	int amount_c[N];     
	bool check = true;   
	int row_user = 0, col_user = 0; 
	char mode = T, mode_check; 
	char pusto;          

	FILE* file_txt; 
	char save = 'y';      

	puts("     ===== Gappy ====="); // приветствие пользователя
	puts("Дорогой пользователь, Вам предлагается решить");
	puts("логическоу головоломку Промежутки (Gappy).");
	puts("Сгенерировать поле и начать играть?  (1)");
	puts("Правила игры и объяснение интерфейса (2)");

	while (true)    // ввод пользователем цифры-навигации
	{
		if (scanf("%d", &task) == 1 && (task == 1 || task == 2))
		{
			system("cls"); 
			break;
		}
		else
		{
			puts("Ошибка ввода");
		}
	} 

	if (task == 2) // вывод правил игры
	{ 
		puts("Правила игры:");
		puts("1. Каждая строка и каждый столбец содержат две чёрные ячейки.");
		puts("2. Каждая заполненная ячейка не должна иметь \"соседей\", то есть других заполненных клеток в радиусе одной клетки");
		puts("3. Числа вне сетки показывают количество белых клеток между чёрными клетками в соответствующем ряду или столбце.\n");
		puts("Интерфейс:");
		printf("1. Строки цифр сверху и слева - это то количество пустых (%c) ячеек между заполненными (%c).\n", T, Not_T);
		puts("2. Строки цифр снизу и справа - это индекс строки/столбца (для обращения к конкретной ячейки).");
		printf("3. Символы внутри квадрата - игровое поле, состоящее из заполненных (%c) и незаполненных (%c) ячеек.", Not_T, T);
		puts("4. Формат ввода: X Y, где X (номер строки), а Y (номер столбца)");
		puts("Сгенерировать поле и начать играть? (1)");
		puts("Закрыть программу? (2)");
		while (true)
		{
			task = -1; 
			if (scanf("%d", &task) == 1 && (task == 1 || task == 2)) {
				system("cls");
				if (task == 2) return 0;
			}
			else {
				puts("Ошибка ввода");
			}
		}
	}

	while (!rand_generation(arr, size, amount_r, amount_c)); // много попыток сгенерировать массив с точками по правилами "игры"
	
	if (completion_filed(arr, size, field_С_R)); // заполнение полей с промежутками в соответствии с заполненным массивом
	else 
	{
		puts("Произошла ошибка :(");
		return -1;
	}

	if(clear_arr_user(arr_user, size)); 
	else
	{
		puts("Произошла ошибка :(");
		return -2;
	}

	while (check) {    // вывод, пока пользователь не закроет программу

		system("cls"); 

		row_user = -1;
		col_user = -1; 

		if(output(arr_user, size, field_С_R)); 
		else
		{
			puts("Произошла ошибка :(");
			return -3;
		}

		puts(" Введите координаты ячейки (строки и столбца) (x y)"); 
		printf(" Если желаете изменить вводимый символ (\"%d\" - %c) или (\"%d\" - %c) // сейчас %c\n", Not_T, Not_T, T, T, mode);
		printf(" Если желаете отчистить игровое поле от точек, то введите %d \n", CLEAR);
		printf(" Если желаете проверить прошли ли Вы игру, то введите %d \n", CHECK);

		while (true) // ввод от пользователя
		{
			if (scanf("%d", &row_user) == 1) // считка первого числа/цифры
			{
				if (row_user == T || row_user == Not_T || row_user == CHECK || row_user == CLEAR) { // если смена мода (т е символов, которые хочет вводит user)

					if (row_user == Not_T) // пустая ячейка
					{
						mode = Not_T;
						puts(" Вводимый символ + (пустая ячейка)");
					}
					if (row_user == T)     // заполненная ячейка
					{
						mode = T; 
						puts(" Вводимый символ # (заполненная ячейка)");
					}
					if (row_user == CLEAR)  // отчистить игровое поле
					{
						if (clear_arr_user(arr_user, size))
						{
							puts("Поле успешно отчищено.");
						}
						else
						{
							puts("Произошла ошибка :(");
						}
					}
					if (row_user == CHECK)  // проверить игровое поле
					{
						if (check_arr_user_(arr_user, size, field_С_R))
						{
							pusto = getchar(); // для отчистки входного потока от лишних символов, если введены символы не по заданию
							puts("Игра-головоломка успешно ПРОЙДЕНА!");
							puts("Сохранить поля головоломки в файле .txt? // y - да, n - нет (закрыть программу)");
							if (scanf("%c", &save) == 1) // считка первого числа/цифры
							{
								if (save == 'y')
								{	
									file_txt = fopen("Puzzle_fields.txt", "w");
									file_out(file_txt, field_С_R, size);
									puts("Файл успешно сохранён");
								}
								if (save == 'n')
								{
									return 0;
								}
							}
						}
						else
						{
							puts("Есть ошибки, подумайте ещё...");
						}
					}
					system("pause");
					break;
				}
				else if (row_user < 1 && row_user > N + 1) break; // если другое число не = 43 или 35 ( т е числа от 1 до 9)
			}
			else
			{
				pusto = getchar(); // для отчистки входного потока от лишних символов, если введены символы не по заданию
				puts(" Ошибка ввода (введён символ)");
				break;
			}

			if (scanf("%d", &col_user) == 1) break; // считака второго числа/цифры
			else
			{
				puts(" Ошибка ввода (введён символ)");
				system("pause");
				break;
			}
		}

		if (row_user > 0 && row_user < size + 1 && col_user > 0 && col_user < size + 1) { // записать в массив значение
			arr_user[row_user - 1][col_user - 1] = mode;
		}
		else puts(" Ошибка ввода");
	}
	
	return 0;
}

int clear_arr_user(char* arr_user, int size)
{
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			arr_user[i * size + j] = Not_T; 
		}
	}
	return 1;
}

int output(const char* arr_user, int size, int* field_С_R)
{
	puts("   === Игра Gappy ===\n");

	printf("   "); // вывод индексов ячеек по столбцам
	for (int i = 0; i < size; i++)
	{
		printf("%d ", i + 1);
	}
	puts("");

	printf("   "); // вывод разделителя ------
	for (int i = 0; i < size; i++)
	{
		printf("--");
	}
	puts("");

	for (int i = 0; i < size; i++)
	{
		printf(" %d|", i + 1); // вывод индексов ячеек по строкам

		for (int j = 0; j < size; j++)
		{
			printf("%c ", arr_user[i * size + j]);
		}

		printf("|%d", field_С_R[i]); // вывод поля с промежутками для строк
		puts("");
	}


	printf("   "); // вывод разделителя ------
	for (int i = 0; i < size; i++)
	{
		printf("--");
	}
	puts("");

	printf("   ");
	for (int i = 0; i < size; i++) // вывод поля с промежутками для столбцов
	{
		printf("%d ", field_С_R[size + i]);
	}
	puts("\n");

	return 1;
}

int file_out(FILE* file_txt, const int* field_C_R, int size)
{
	for (int i = 0; i < 2; i++)
	{
		if (i == 0) fprintf(file_txt, "Столбцы: ");
		else fprintf(file_txt, "Строки:  ");

		for (int j = 0; j < size; j++)
			fprintf(file_txt, "%d ", field_C_R[i * size + j]);
		fprintf(file_txt, "\n");
	}
	fclose(file_txt);
	return 1;
}