/*
Автор: студент ВГТУ ФИТКБ группы ИЦ-251 Кравченко В. В.
*/

#include <stdbool.h>
#include <time.h>

#define T '#' // ячейка заполнена
#define Not_T '+' //  ячейка  не заполнена

struct Pos_Line //для хранения координаты точек в строке / столбцеs
{
	int pos1;
	int pos2;
};

bool check_o(const char* arr, int size, int r, int c, int method)
{
	for (int i = r - 1; i <= r + 1; i++)     // проверка по строкам, в сроке 3 ячейки (проверяем вокруг переданной точки)
	{
		for (int j = c - 1; j <= c + 1; j++) // по столбцам, в столбце 3 ячейки 
		{
			if ((i >= 0 && i < size) && (j >= 0 && j < size)) // можем выйти за массив, проверяем входит ли i, j в массив (если не входит - не проверяем, иначе ошибка)
			{
				if (method == 1 && i == r && j == c)          // проверяет область без учёта точки в центре
					continue;

				if (arr[i * size + j] == T)  // нашли точку в круге - проверка не пройдена
				{
					return false;
				}
			}
		}
	}
	return true;
}

bool rand_generation(char* arr, int size, int* amount_r, int* amount_c)
{
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			arr[i * size + j] = Not_T; // заполняем всё плюсами
			amount_c[j] = 0;           // сколько точек в столбце
		}
		amount_r[i] = 0; // сколько точек в строке
	}

	srand(time(NULL));

	for (int i = 0; i < size; i++)
	{
		for (int j = 0, max; j < size; j++)  // для ограничения кол-ва попыток на установку 1 точки в строке
		{
			max = 0;
			while (amount_r[i] < 2) {

				if (max >= 50) return false; // превышено кол-во попыток

				j = rand() % 9;              // выбрали случайную точку для вставки
				if (check_o(arr, size, i, j, 0) && amount_r[i] < 2 && amount_c[j] < 2) // проверяем по кругу && точек в сроке/столбце < 2
				{
					arr[i * size + j] = T;
					amount_r[i]++; // + точка в строке 
					amount_c[j]++; // + точка в столбце 
				}
				else max++;
			}
		}
	}

	return true;
}

bool cheak_line(const char* arr_u, int size, const int* field_С_R, int i, int j)
{

	bool b_up = false, b_down = false, b_left = false, b_right = false;
	int up = 0, down = 0, left = 0, right = 0;

	//   field_С_R[1][j]  - Это для проверки вверх и вниз (т е столбец)
	//   field_С_R[0][i]) -       -/-         влево и вправо (т е строка)

	for (int i_i = 0, a = 0, b = 0; i_i < size; i_i++) // для проверки точек в строке и столбце
	{
		if (arr_u[i * size + i_i] == T) a++; // проверка по столбцам
		if (arr_u[i_i * size + j] == T) b++; // проверка по строкам
		if (a > 2 || b > 2)
			return false;
	}

	for (int i_i = 1; i_i < size; i_i++) // проверка на промежутки между точками
	{
		if (i - i_i >= 0) {
			if (arr_u[(i - i_i) * size + j] == Not_T && !b_up && !b_down)   up++; // проверка вверх
			else if (up == field_С_R[ size + j])      b_up = true;
		}

		if (i + i_i < size) {
			if (arr_u[(i + i_i) * size + j] == Not_T && !b_up && !b_down)   down++; // проверка вниз
			else if (down == field_С_R[size + j])      b_down = true;
		}

		if (j - i_i >= 0) {
			if (arr_u[i * size + (j - i_i)] == Not_T && !b_left && !b_right)   left++; // проверка влево
			else if (left == field_С_R[i])      b_left = true;
		}

		if (j + i_i < size) {
			if (arr_u[i * size + (j + i_i)] == Not_T && !b_left && !b_right)   right++; // проверка вправо
			else if (right == field_С_R[i])      b_right = true;
		}

	}

	if ((b_up == true || b_down == true) && (b_left == true || b_right == true)) // или вверх или вниз совпадает с полем И или влево или вправо  совпадает с полем 
		return true;
	else return false;
}

bool check_arr_user_(const char* arr_u, int size, const int* field_С_R)
{
	bool point_found = false;
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			if (arr_u[i * size + j] == T) // нашли точку в массиве
			{
				point_found = true;
				if (!check_o(arr_u, size, i, j, 1)) // проверили её по кругу
				{
					return false;         // если в круге найдена точка
				}
				else if (!cheak_line(arr_u, size, field_С_R, i, j)) // проверили, что промежутки по линии совпадают с цифрой на поле и точек в линии < 2
				{
					return false;
				}
			}
		}
	}

	return point_found;
}

int completion_filed(const char* arr, int size, int* field_С_R)
{
	// заполняем поле строк (справа)
	for (int i = 0; i < size; i++)
	{
		struct Pos_Line p = { -1,-1 };  // для хранения 2х точек в строке

		for (int j = 0; j < size; j++)
		{
			if (arr[i * size + j] == T) // нашли точку в строке
			{
				if (p.pos1 == -1) p.pos1 = j; // если в структуре первая позиция не занята, значит точка первая в строке
				else
				{
					p.pos2 = j;         // если занята, то вторая точка в строке
					break;
				}
			}
		}
		field_С_R[i] = p.pos2 - p.pos1 - 1; // для вычисления промежутка и записи их в массив полей
	}

	// заполняем поле столбцов (снизу)
	for (int j = 0; j < size; j++)
	{
		struct Pos_Line p = { -1,-1 };  // для хранения 2х точек в столбце

		for (int i = 0; i < size; i++)
		{
			if (arr[i * size + j] == T) // нашли точку в столбце
			{
				if (p.pos1 == -1) p.pos1 = i; // если в структуре первая позиция не занята, значит точка первая в столбце
				else
				{
					p.pos2 = i;         // если занята, то вторая точка в столбце
					break;
				}
			}
		}
		field_С_R[size + j] = p.pos2 - p.pos1 - 1; // для вычисления промежутка и записи их в массив полей
	}
	return 1;
}