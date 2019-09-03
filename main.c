#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <Windows.h>
#include <conio.h>


/*
while (1)
{
do {
scanf("%d", &k);
} while (k < 0);

if (k == 0)
break;

l = insord(k, l);
}
*/


typedef struct root {
	unsigned l;  // l e' la lunghezza del vettore data, cioe' il numero di mintermini coperti dall'implicante corrente
	int *data; //data contiene gli interi che corrispondono ai mintermini coperti dall'implicante corrente
} root;

typedef struct imp {
	int val;
	unsigned char taken;
} imp;

typedef struct el {
	root r;
	int *bin;
	unsigned ones;
	bool took;
} el;

void color(int color)                  // 0x0 nero, 1 blu, 2 verde, 3 azzurro, 4 rosso, 5 viola, 6 giallo, 7 normale, 8 grigio, 9 blu elettrico, da 10 in poi evidenziature
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);     // trovata su internet

	SetConsoleTextAttribute(hConsole, color);
}

void load() {
	int r, q;
	printf("\nInizio algoritmo ");
	for (r = 1; r <= 4; r++) {
		for (q = 0; q <= 150000000; q++);
		printf(".");
	}
	printf("\n\n");
}

bool isPresent(int v[], int n, int k) 
{
	for (int i = 0; i < n; ++i)
		if (v[i] == k)
			return true;
	return false;
}

size_t init(int **v, size_t *n_imp)
{
	size_t n = 0;
	int k = 0;
	char garbage;

	printf("\n\n\t\t\t    ------------------ALGORITMO DI QUINE-MCKLUSKEY-------------------\n\n");
	printf("Inserire valori numerici degli implicanti (si termina con -1) : ");
	/*while (scanf("%d", &k) > 0)
	{
		*v = realloc(*v, ++n * sizeof(int));
		(*v)[n - 1] = k;
	}
	*n_imp = n;
	printf("\nInserire valori numerici delle indifferenze : ");
	while (scanf("%d", &k) > 0)
	{
		*v = realloc(*v, ++n * sizeof(int));
		(*v)[n - 1] = k;
	}*/
	while (1)
	{
		
		scanf("%d", &k);

		if (k == -1)
			break;
		
		if (isPresent(*v, n, k)) 
		{
			printf("\nERRORE: Il numero %d e' stato inserito 2 volte\n", k);
			return 0;
		}

		*v = realloc(*v, ++n * sizeof(int));
		(*v)[n - 1] = k;
	}
	*n_imp = n;
	printf("\nInserire valori numerici delle indifferenze (si termina con -1) : ");
	while (1)
	{

		scanf("%d", &k);

		if (k == -1)
		{
			scanf("%c", &garbage);
			break;
		}

		if (isPresent(*v, n, k))
		{
			printf("\nERRORE: Il numero %d e' stato inserito 2 volte\n", k);
			return 0;
		}

		*v = realloc(*v, ++n * sizeof(int));
		(*v)[n - 1] = k;
	}
	load();
	
	return n;
}

int maximum(int v[], size_t n)
{
	int m = 0;
	for (unsigned i = 0; i < n; ++i)
	{
		if (v[i] > m)
			m = v[i];
	}
	return m;
}

unsigned hdist(int v1[], int v2[], int n)
{
	unsigned k = 0;
	for (int i = 0; i < n; ++i)
	{
		if (v1[i] != v2[i])
			++k;
	}
	return k;
}

bool equal(int v1[], int v2[], int n)
{
	if (v1 == NULL || v2 == NULL)
		return false;
	for (int i = 0; i < n; ++i)
	{
		if (v1[i] != v2[i])
			return false;
	}
	return true;
}

bool ispresent(int v1[], el *v2, int n1, int n2)
{
	if (v1 == NULL || v2 == NULL)
		return false;

	for (int i = 0; i < n2; ++i)
	{
		if (equal(v1, v2[i].bin, n1))
			return true;
	}
	return false;
}

unsigned length(int *v, size_t n)
{
	int m = maximum(v, n), num = 2;
	unsigned l = 1;
	while (num < m)
	{
		num *= 2;
		l++;
	}
	return l;
}

unsigned number_of_ones(int *v, unsigned l)
{
	unsigned k = 0;
	for (unsigned i = 0; i < l; ++i)
	{
		if (v[i] == 1)
			++k;
	}
	return k;
}

void int_to_bin(el **arr, int *v, size_t n, unsigned l)
{
	int bin = 0;
	unsigned m = 0;

	color(0x06);
	printf("J0 :\n\n");
	color(0x07);
	for (size_t i = 0; i < n; ++i)
	{
		bin = v[i];
		arr[0][i].bin = malloc(l * sizeof(int));
		arr[0][i].r.l = 1;
		arr[0][i].r.data = malloc(sizeof(int));
		arr[0][i].r.data[0] = v[i];
		arr[0][i].took = 0;
		for (int j = l - 1; j >= 0; --j)
		{
			arr[0][i].bin[j] = bin % 2;
			bin /= 2;
		}
		arr[0][i].ones = number_of_ones(arr[0][i].bin, l);
	}
	for (size_t i = 0; i < n; ++i)
	{
		if (arr[0][i].ones > m)
			m = arr[0][i].ones;
	}
	for (unsigned i = 0; i <= m; ++i)
	{
		color(0x04);
		printf("\tS%d :\n", i);
		color(0x07);
		for (unsigned k = 0; k < n; ++k)
		{
			if (arr[0][k].ones == i)
			{
				printf("\t    ");
				for (unsigned j = 0; j < l; ++j)
					printf("%d ", arr[0][k].bin[j]);
				printf(" (%d)\n", arr[0][k].r.data[0]);
			}
		}
		
	}
	
}

void backtrack(el *v, int *cur, int *best, imp *check, unsigned l, int *cur_count, int *best_count, int s, int n)
{
	if (s == n)
	{
		for (unsigned i = 0; i < l; ++i)
		{
			if (check[i].taken == 0)
				return;
		}
		if (*cur_count < *best_count)
		{
			for (int i = 0; i < n; ++i)
				best[i] = cur[i];
			*best_count = *cur_count;
		}
		return;
	}
	if (*cur_count >= *best_count)
		return;

	cur[s] = 0;
	backtrack(v, cur, best, check, l, cur_count, best_count, s + 1, n);

	cur[s] = 1;
	(*cur_count)++;
	for (unsigned i = 0; i < v[s].r.l; ++i)
	{
		for (unsigned j = 0; j < l; ++j)
		{
			if (v[s].r.data[i] == check[j].val)
			{
				check[j].taken++;
				break;
			}
		}
	}
	backtrack(v, cur, best, check, l, cur_count, best_count, s + 1, n);
	(*cur_count)--; 
	for (unsigned i = 0; i < v[s].r.l; ++i)
	{
		for (unsigned j = 0; j < l; ++j)
		{
			if (v[s].r.data[i] == check[j].val)
			{
				check[j].taken--;
				break;
			}
		}
	}
}

void quine_mccluskey(int v[], size_t n, size_t n_imp)
{
	if (v == NULL)
	{
		printf("\nInput not valid!\n");
		return;
	}
	/*l e' il numero minimo di cifre binarie che servono per rappresentare il maggiore tra i numeri di v*/
	unsigned l = length(v, n), m = 0;
	size_t size = 1, number_of_implicants = 0, counter = n;
	el **arr = malloc(size * sizeof(el *));
	arr[0] = malloc(n * sizeof(el));
	int *data = malloc(l * sizeof(int)), *check_if_all = calloc((5 + n), sizeof(int));
	bool stayin = true;

	int_to_bin(arr, v, n, l);

	while (1)
	{
		number_of_implicants = counter;
		counter = 0;
		stayin = false;
		arr = realloc(arr, ++size * sizeof(el **));
		arr[size - 1] = NULL;

		for (unsigned i = 0; i < number_of_implicants; ++i)
			check_if_all[i] = 0;

		for (unsigned i = 0; i < number_of_implicants - 1; ++i) //per ogni implicante
		{
			for (unsigned j = i + 1; j < number_of_implicants; ++j) //scorro tutti gli implicanti successivi
			{
				if (abs(arr[size - 2][i].ones - arr[size - 2][j].ones) == 1 && hdist(arr[size - 2][i].bin, arr[size - 2][j].bin, l) == 1) //controllo se ce n'e' uno compatibile
				{
					for (unsigned k = 0; k < l; ++k)  //copio l'implicante in data
					{
						if (arr[size - 2][i].bin[k] == arr[size - 2][j].bin[k])
							data[k] = arr[size - 2][i].bin[k];
						else
							data[k] = -1;
					}
					if (!ispresent(data, arr[size - 1], l, counter)) //controllo se data e' gia' presente in arr[size - 1]
					{
						arr[size - 1] = realloc(arr[size - 1], ++counter * sizeof(el));
						arr[size - 1][counter - 1].bin = malloc(l * sizeof(int));
						arr[size - 1][counter - 1].r.l = arr[size - 2][i].r.l + arr[size - 2][j].r.l;
						arr[size - 1][counter - 1].r.data = malloc(arr[size - 1][counter - 1].r.l * sizeof(int));
						for (unsigned h = 0; h < l; ++h)  //copio data come implicante di liello successivo
							arr[size - 1][counter - 1].bin[h] = data[h];
						for (unsigned h = 0; h < arr[size - 1][counter - 1].r.l; ++h)  //riempio il vettore data di r (root) con i valori dei vettori dei due implicanti
						{
							if (h < arr[size - 2][i].r.l)
								arr[size - 1][counter - 1].r.data[h] = arr[size - 2][i].r.data[h];
							else
								arr[size - 1][counter - 1].r.data[h] = arr[size - 2][j].r.data[h - arr[size - 2][i].r.l];
						}
						arr[size - 1][counter - 1].ones = number_of_ones(arr[size - 1][counter - 1].bin, l);
						stayin = true;
					}
					check_if_all[i] = 1;
					check_if_all[j] = 1;
				}
			}
		}
		if (!stayin)
		{
			arr = realloc(arr, --size * sizeof(el **));
			break;
		}
		for (unsigned i = 0; i < number_of_implicants; ++i)
		{
			if (check_if_all[i] == 0)
			{
				arr[size - 1] = realloc(arr[size - 1], ++counter * sizeof(el));
				arr[size - 1][counter - 1].bin = malloc(l * sizeof(int));
				for (unsigned h = 0; h < l; ++h)
					arr[size - 1][counter - 1].bin[h] = arr[size - 2][i].bin[h];
				arr[size - 1][counter - 1].ones = number_of_ones(arr[size - 1][counter - 1].bin, l);
				arr[size - 1][counter - 1].r.l = arr[size - 2][i].r.l;
				arr[size - 1][counter - 1].r.data = malloc(arr[size - 1][counter - 1].r.l * sizeof(int));
				for (unsigned j = 0; j < arr[size - 1][counter - 1].r.l; ++j)
					arr[size - 1][counter - 1].r.data[j] = arr[size - 2][i].r.data[j];
			}
		}

		color(0x06);
		printf("\nJ%d :\n\n", size - 1);
		color(0x07);
		m = 0;
		for (unsigned i = 0; i < counter; ++i)
		{
			if (arr[size - 1][i].ones > m)
				m = arr[size - 1][i].ones;
		}
		for (unsigned i = 0; i <= m; ++i)
		{
			color(0x04);
			printf("\tS%d :\n", i);
			color(0x07);
			for (unsigned k = 0; k < counter; ++k)
			{
				if (arr[size - 1][k].ones == i)
				{
					printf("\t    ");
					for (unsigned j = 0; j < l; ++j)
					{
						if (arr[size - 1][k].bin[j] == -1)
							printf("- ");
						else
							printf("%d ", arr[size - 1][k].bin[j]);
					}
					printf(" (");
					for (unsigned j = 0; j < arr[size - 1][k].r.l; ++j)
					{
						printf("%d", arr[size - 1][k].r.data[j]);
						if (j != arr[size - 1][k].r.l - 1)
							printf(", ");
					}
					printf(")\n");
				}
			}
		}
	}

	int cur_count = 0, cur_best = INT_MAX;
	int *cur = malloc(number_of_implicants * sizeof(int)), *best = malloc(number_of_implicants * sizeof(int));
	imp *check = malloc(n * sizeof(imp));
	for (unsigned i = 0; i < number_of_implicants; ++i)
	{
		cur[i] = 0;
		best[i] = 0;
	}
	for (unsigned i = 0; i < n_imp; ++i)
	{
		check[i].val = v[i];
		check[i].taken = 0;
	}

	backtrack(arr[size - 1], cur, best, check, n_imp, &cur_count, &cur_best, 0, number_of_implicants);

	printf("\n\nSOLUZIONE :\n\n     ");
	for (unsigned i = 1; i <= l; ++i)
	{
		printf("X%d ", l - i);
	}
	printf("\n");
	unsigned Pi = 0;
	for (unsigned i = 0; i < number_of_implicants; ++i)
	{
		if (best[i] == 1)
		{
			printf("P%d)  ", Pi);
			for (unsigned j = 0; j < l; ++j)
			{
				if (arr[size - 1][i].bin[j] == -1)
					printf("-  ");
				else
					printf("%d  ", arr[size - 1][i].bin[j]);
			}
			printf("\n");
			++Pi;
		}
	}

}



int main(void)
{
	char risposta = 0, svuota_buffer = 0;
	int *v = NULL;
	size_t n_imp = 0;
	size_t n;

	while (1)
	{
		n = init(&v, &n_imp);
		if(n > 0)
			quine_mccluskey(v, n, n_imp);
		else
		{
			_getch();
			return 0;
		}
		printf("\nVuoi eseguire di nuovo l'algoritmo(s/n)? ");
		scanf("%c", &risposta);
		svuota_buffer = risposta;
		while (svuota_buffer != '\n')
			scanf("%c", &svuota_buffer);
		if (risposta != 's' && risposta != 'S')
			break;
		system("CLS");
	}
	return 0;
}