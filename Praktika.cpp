#include "pch.h"
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <malloc.h>
#include <clocale>
#include <windows.h> 

using namespace std;
using namespace System;
using namespace System::IO;

#define ENTER 13 //обозначение кнопок 
#define ESC 27
#define UP 72
#define DOWN 80
#define HOME 71
#define END 79

char dan[8][60] = {
"Самый старший студент                                   ",
"Самый молодой четверокурсник                            ",
"Количество студентов, которые учатся бесплатно и очно   ",
"Прямой и обратный алфавитный списки студентов           ",
"Список студентов из определённой группы                 ",
"Диаграмма                                               ",
"Выход                                                   ",
};
char BlankLine[] = "                                                          ";

struct z {
	char fam[20];
	char form[20];
	char obuch[20];
	char grup[20];
	int kurs;
	int let;
};

struct sp {
	char fam[20];
	char grup[20];
	int let;
	int kurs;
	struct sp* sled;
	struct sp* pred;
};

struct sp* spisok = NULL;

int menu(int);
void maxstd(struct z*, int);
void mol4(struct z*, int);
void ochbes(struct z*, int);
void alfalist(struct z*, int);
void vstavka(struct z*, int, char*);
void stdlist(struct z*, int);
void diagram(struct z*, int);

int main(array<System::String^>^ args)
{
	int i, n;
	FILE* in;
	struct z* students;
	int NC;

	setlocale(LC_CTYPE, "Russian");
	Console::CursorVisible::set(false);
	Console::BufferHeight = Console::WindowHeight;
	Console::BufferWidth = Console::WindowWidth;

	if ((in = fopen("student.txt", "r")) == NULL) {
		printf("\nФайл student.txt не открыт!");
		getch(); exit(1);
	}

	fscanf(in, "%d", &NC);
	students = (struct z*)malloc(NC * sizeof(struct z));

	for (i = 0; i < NC; i++) {
		fscanf(in, "%s%s%s%s%d%d", students[i].fam,
			students[i].form, students[i].obuch,
			students[i].grup, &students[i].kurs, &students[i].let);
	}

	printf("Фамилия              Тип              Форма обучения           Группа              Курс     Возраст\n");
	for (i = 0; i < NC; i++) {
		printf("\n%-20s %-20s %-20s %-20s %d %10d",
			students[i].fam, students[i].form, students[i].obuch, students[i].grup, students[i].kurs, students[i].let);
	}
	getch();

	while (1) {
		Console::ForegroundColor = ConsoleColor::Red;
		Console::BackgroundColor = ConsoleColor::Gray;
		Console::Clear();
		Console::ForegroundColor = ConsoleColor::Red;
		Console::BackgroundColor = ConsoleColor::Gray;
		Console::CursorLeft = 10;
		Console::CursorTop = 4;
		printf(BlankLine);
		for (i = 0; i < 7; i++) {
			Console::CursorLeft = 10;
			Console::CursorTop = i + 5;
			printf(" %s ", dan[i]);
		}
		Console::CursorLeft = 10;
		Console::CursorTop = 12;
		printf(BlankLine);

		n = menu(7);
		switch (n) {
		case 1: maxstd(students, NC); break;
		case 2: mol4(students, NC); break;
		case 3: ochbes(students, NC); break;
		case 4: alfalist(students, NC); break;
		case 5: stdlist(students, NC); break;
		case 6: diagram(students, NC); break;
		case 7: exit(0);
		}
	}

	free(students); // освобождаем выделенную память
	return 0;
}

int menu(int n)
{
	int y1 = 0, y2 = n - 1;
	char c = 1;
	while (c != ESC) {
		switch (c) {
		case DOWN: y2 = y1; y1++; break;
		case UP: y2 = y1; y1--; break;
		case ENTER: return y1 + 1;
		case HOME: y2 = y1; y1 = 0; break;
		case END: y2 = y1; y1 = n - 1; break;
		}
		if (y1 > n - 1) { y2 = n - 1; y1 = 0; }
		if (y1 < 0) { y2 = 0; y1 = n - 1; }
		Console::ForegroundColor = ConsoleColor::Black;
		Console::BackgroundColor = ConsoleColor::Blue;
		Console::CursorLeft = 11;
		Console::CursorTop = y1 + 5;
		printf("%s", dan[y1]);
		Console::ForegroundColor = ConsoleColor::Red;
		Console::BackgroundColor = ConsoleColor::Gray;
		Console::CursorLeft = 11;
		Console::CursorTop = y2 + 5;
		printf("%s", dan[y2]);
		c = getch();
	}
	exit(0);
}

void maxstd(struct z* student, int NC) {
	int i = 0;
	struct z best;
	strcpy(best.fam, student[0].fam);
	best.let = student[0].let;
	for (i = 1; i < NC; i++) {
		if (student[i].let > best.let) {
			strcpy(best.fam, student[i].fam);
			best.let = student[i].let;
		}
	}
	Console::ForegroundColor = ConsoleColor::Yellow;
	Console::BackgroundColor = ConsoleColor::Black;
	Console::CursorLeft = 10;
	Console::CursorTop = 15;
	printf("Самый большой возраст %ld лет имеет студент %s", best.let, best.fam);
	getch();
}

void mol4(struct z* student, int NC) {
	int i;
	struct z mol;
	strcpy(mol.fam, student[0].fam);
	mol.let = student[0].let;
	for (i = 1; i < NC; i++) {
		if (student[i].let < mol.let && student[i].kurs == 4) {
			strcpy(mol.fam, student[i].fam);
			mol.let = student[i].let;
		}
	}
	Console::ForegroundColor = ConsoleColor::Yellow;
	Console::BackgroundColor = ConsoleColor::Black;
	Console::CursorLeft = 10;
	Console::CursorTop = 15;
	printf("Самый молодой четверокурсник - %s", mol.fam);
	getch();
}

void ochbes(struct z* student, int NC) {
	int i, k = 0;
	for (i = 0; i < NC; i++) {
		if (strcmp(student[i].form, "Бесплатная") == 0 && strcmp(student[i].obuch, "Очная") == 0) {
			k++;
		}
	}
	Console::ForegroundColor = ConsoleColor::Yellow;
	Console::BackgroundColor = ConsoleColor::Black;
	Console::CursorLeft = 10;
	Console::CursorTop = 15;
	printf("Количество студентов, которые учатся бесплатно и очно: %d", k);
	getch();
}

void alfalist(struct z* student, int NC) {
	int i, n = 0;
	struct sp* nt;
	struct sp* z = NULL;
	Console::ForegroundColor = ConsoleColor::Black;
	Console::BackgroundColor = ConsoleColor::Gray;
	Console::Clear();
	if (!spisok) {
		for (i = 0; i < NC; i++) {
			vstavka(student, NC, student[i].fam);
		}
	}

	Console::Clear();
	printf("\n Алфавитный список студентов");
	printf("\n ===========================                    ====================================\n");
	printf("\n Фамилия              Группа                     Фамилия              Группа");
	printf("\n                ");
	for (nt = spisok; nt != 0; nt = nt->sled) {
		printf("\n %-20s %-20s", nt->fam, nt->grup);
	}
	Console::CursorLeft = 48;
	Console::CursorTop = 1;
	printf("Обратный алфавитный список студентов");
	for (nt = spisok; nt != 0; nt = nt->sled) {
		z = nt; // Устанавливаем z на последний элемент
	}
	for (nt = z; nt != NULL; nt = nt->pred) {
		Console::CursorLeft = 48;
		Console::CursorTop = 6 + n;
		printf(" %-20s %-20s", nt->fam, nt->grup);
		n += 1;
	}
	getch();
}

void vstavka(struct z* student, int NC, char* fam) {
	int i;
	struct sp* nov, * nt, * z = 0;
	for (nt = spisok; nt != 0 && strcmp(nt->fam, fam) < 0; z = nt, nt = nt->sled);
	if (nt && strcmp(nt->fam, fam) == 0) return;
	nov = (struct sp*)malloc(sizeof(struct sp));
	strcpy(nov->fam, fam);
	nov->sled = nt;
	nov->pred = z;
	strcpy(nov->grup, "");
	for (i = 0; i < NC; i++) {
		if (strcmp(student[i].fam, fam) == 0) {
			strcat(nov->grup, student[i].grup);
			nov->let = student[i].let;
		}
	}
	if (!z) spisok = nov;
	else z->sled = nov;
	if (nt) {
		nt->pred = nov;
	}
	return;
}

void diagram(struct z* student, int NC) {
	struct sp* nt;
	int len, i, NColor;
	long sum = 0;
	char str1[40];
	char str2[20];
	System::ConsoleColor Color;

	Console::ForegroundColor = ConsoleColor::White;
	Console::BackgroundColor = ConsoleColor::Black;
	Console::Clear();

	for (i = 0; i < NC; i++) {
		sum += student[i].let;
	}

	if (!spisok) {
		for (i = 0; i < NC; i++) {
			vstavka(student, NC, student[i].fam);
		}
	}

	Color = ConsoleColor::Black;
	NColor = 0;

	for (nt = spisok, i = 0; nt != NULL; nt = nt->sled, i++) {
		sprintf(str1, "%s", nt->fam);
		float percentage = (nt->let * 100.0 / sum);
		sprintf(str2, "%3.1f%%", percentage);

		Console::ForegroundColor = ConsoleColor::White;
		Console::BackgroundColor = ConsoleColor::Black;

		Console::CursorLeft = 5;
		Console::CursorTop = i + 1;
		printf("%s", str1);

		Console::CursorLeft = 50;
		printf("%s", str2);

		Color = static_cast<System::ConsoleColor>((i % 14) + 1);
		Console::BackgroundColor = Color;

		Console::CursorLeft = 55;
		int barLength = static_cast<int>(percentage);
		for (len = 0; len < barLength; len++) {
			printf(" ");
		}

		NColor++;
	}

	getch();
	return;
}

void stdlist(struct z* student, int NC) {
	int i;
	char ngrup[20];
	setlocale(LC_ALL, "Russian");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	Console::CursorLeft = 10;
	Console::CursorTop = 15;
	printf("Введите группу: ");
	scanf("%s", ngrup);
	printf(ngrup);
	Console::ForegroundColor = ConsoleColor::Yellow;
	Console::BackgroundColor = ConsoleColor::Black;
	Console::Clear();

	Console::CursorLeft = 10;
	Console::CursorTop = 5;

	printf("Студенты из группы %s:\n", ngrup);
	int cursorTop = 6;

	for (i = 0; i < NC; i++) {
		if (strcmp(student[i].grup, ngrup) == 0) {
			Console::CursorLeft = 10;
			Console::CursorTop = cursorTop++;
			printf("%s\n", student[i].fam);
		}
	}
	getch();
}