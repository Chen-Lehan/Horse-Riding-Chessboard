#include <stdio.h>
#include <stdlib.h>
#define SqStack_Size SIZE*SIZE
#define SqSElemType Plot
#define OK 1
#define ERROR -1
#define SIZE 8

int recurse = 0;

int HTry1[8] = { -2, -1,  1,  2,  2,  1, -1, -2 };
int HTry2[8] = {  1,  2,  2,  1, -1, -2, -2, -1 };
int ChessBoard[8][8] = {
	{0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0} };
int ChoiceNum[8][8] = {
	{2, 3, 4, 4, 4, 4, 3, 2},
	{3, 4, 6, 6, 6, 6, 4, 3},
	{4, 6, 8, 8, 8, 8, 6, 4},
	{4, 6, 8, 8, 8, 8, 6, 4},
	{4, 6, 8, 8, 8, 8, 6, 4},
	{4, 6, 8, 8, 8, 8, 6, 4},
	{3, 4, 6, 6, 6, 6, 4, 3},
	{2, 3, 4, 4, 4, 4, 3, 2} };

typedef enum { True, False } bool;

typedef struct {
	int x;
	int y;
	int n;
}Plot;

typedef struct {
	Plot p;
	Plot* next;
	int length;
	int n;
}StackElem;

typedef struct {
	StackElem* base;
	int top;
}SqStack;

SqStack* Init_SqStack() {
	SqStack* S;
	if (!(S = (SqStack*)malloc(sizeof(SqStack))))
		exit(ERROR);
	if (!(S->base = (StackElem*)malloc(SqStack_Size * sizeof(StackElem))))
		exit(ERROR);
	S->top = 0;
	return S;
}

void swap(Plot v[], int i, int j)
{
	Plot temp;
	temp = v[i];
	v[i] = v[j];
	v[j] = temp;
}

void quicksort(Plot v[], int left, int right)
{
	int i, last;

	if (left >= right)
		return;

	swap(v, left, (left + right) / 2);
	last = left;

	for (i = left + 1; i <= right; i++)
	{
		if (v[i].n < v[left].n)
			swap(v, ++last, i);
	}
	swap(v, left, last);
	quicksort(v, left, last - 1);
	quicksort(v, last + 1, right);
}

bool InChessBoard(int x, int y) {
	if (x >= 0 && y >= 0 && x < 8 && y < 8)
		return True;
	else
		return False;
}

int NextStep(Plot p, Plot* next, bool sort) {
	int i, length;
	for (i = 0, length = 0; i < 8; i++) {
		if (InChessBoard(p.x + HTry1[i], p.y + HTry2[i]) == True) {
			if (ChessBoard[p.x + HTry1[i]][p.y + HTry2[i]] == 0) {
				next[length].x = p.x + HTry1[i];
				next[length].y = p.y + HTry2[i];
				next[length].n = ChoiceNum[next[length].x][next[length].y];
				length++;
			}
		}
	}
	if(sort == True)
		quicksort(next, 0, length - 1);
	return length;
}

StackElem Pop_SqStack(SqStack* S) {
	if (S->top <= 0)
		exit(ERROR);
	StackElem data = S->base[--S->top];
	int i;

	ChessBoard[data.p.x][data.p.y] = 0;
	for (i = 0; i < 8; i++) {
		if (InChessBoard(data.p.x + HTry1[i], data.p.y + HTry2[i]) == True) {
			if (ChessBoard[data.p.x + HTry1[i]][data.p.y + HTry2[i]] == 0) {
				ChoiceNum[data.p.x + HTry1[i]][data.p.y + HTry2[i]]++;
			}
		}
	}
	recurse++;

	return data;
}

int Push_SqStack(SqStack* S, Plot p, int n) {
	int i;
	StackElem data;
	data.n = n;
	data.p = p;
	data.p.n = S->base[S->top - 1].p.n + 1;

	if (!(data.next = (Plot*)malloc(8 * sizeof(Plot))))
		exit(ERROR);

	ChessBoard[p.x][p.y] = data.p.n;
	for (i = 0; i < 8; i++) {
		if (InChessBoard(p.x + HTry1[i], p.y + HTry2[i]) == True) {
			if (ChessBoard[p.x + HTry1[i]][p.y + HTry2[i]] == 0) {
				ChoiceNum[p.x + HTry1[i]][p.y + HTry2[i]]--;
			}
		}
	}

	data.length = NextStep(p, data.next, True);

	if (S->top >= SqStack_Size)
		return ERROR;
	S->base[S->top++] = data;
	return OK;
}

void ClearLine() {
	char c;
	while (c = getchar()) {
		if (c == '\n')
			break;
	}
}

main() {
	int x, y, num;
	int count = 0;
	char print;
	Plot step1;
	SqStack* S = Init_SqStack();

	printf("坐标的有效值为0至7\n");
	printf("请输入x坐标：");
	while (scanf("%d", &x) != 1 || x < 0 || x > 7) {
		ClearLine();
		printf("请重新输入x坐标：");
	}
	printf("请输入y坐标：");
	while (scanf("%d", &y) != 1 || y < 0 || y > 7) {
		ClearLine();
		printf("请重新输入y坐标：");
	}
	printf("请输入解的个数：");
	while (1) {
		if (1 != scanf("%d", &num)) {
			ClearLine();
			printf("请重新输入解的个数：");
		}
		else {
			ClearLine();
			if (num > 0)
				break;
			printf("请重新输入解的个数：");
		}
	}
	do {
		printf("是否打印全部(y/n)：");
		print = getchar();
		ClearLine();
	} while (print != 'y' && print != 'n');

	step1.n = 1;
	step1.x = x;
	step1.y = y;
	Push_SqStack(S, step1, 0);
	while (S->top) {
		if (S->base[S->top - 1].length == 0) {
			StackElem data;
			//data = Pop_SqStack(S);		With this line, the program can run faster by less traceback, but may miss some potential solutions.
			while (S->base[S->top - 1].n == S->base[S->top - 2].length)
				data = Pop_SqStack(S);
			data = Pop_SqStack(S);
			Push_SqStack(S, S->base[S->top - 1].next[data.n], data.n + 1);
		}
		else {
			Push_SqStack(S, S->base[S->top - 1].next[0], 1);
		}
		if (S->top == 64) {
			count++;
			printf("FOUND %d\n", count);
			int i, j;
			if (print == 'y') {
				for (i = 0; i < SIZE; i++) {
					for (j = 0; j < SIZE; j++) {
						printf("%3d", ChessBoard[i][j]);
					}
					printf("\n");
				}
				printf("\n");
			}
		}
		if (count == num) {
			printf("共回溯%d次\n", recurse);
			printf("输入任意字符后回车关闭此窗口\n");
			exit(0);
		}
	}
}