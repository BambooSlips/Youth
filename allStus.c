#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAXNUMBER 1000

struct stu_info {
	char stu_num[12];
	char name[10];
	short int sex;
	char mobile_phone[12];
};

struct stu_info stu[MAXNUMBER];
int length = 0;

void read();
void write();
int menu();
void printSpace(char s[], int L);
void space(int l);
void printLine(char s[], int L);
void line(int l);
int chplace(char s[]);
void showStudents();
void search(char number[]);
void delete(char number[]);

int main()
{
	//menu();
	while(menu())   //显示菜单直至退出
	{
		printf("\t\t\t\t\t*\n");
	}
	return 0;
}

void read()
{
	FILE *rp;
	//struct stu_info stu[MAXNUMBER];
	rp = fopen("stu.info","r");
	int i = 0;

	if(rp == 0)
	{
		printf("error occured when reading stu.info");
		exit(1);
	}
	fread(stu, sizeof(struct stu_info), MAXNUMBER, rp);

	//get the length
	while(strncmp(stu[i].stu_num, "", 1) != 0)
	{
		length++;
		i++;
	}

	//showStudents();
	fclose(rp);
}

void write()
{
	FILE *wp;
	int i = 0;
	struct stu_info stu[MAXNUMBER];
	wp = fopen("stu.info", "a+");   //在文件后附加
	if(wp == 0)
	{
		printf("fopen error\n");
		exit(1);
	}
	while(1)
	{
		printf("> number: ");
		scanf("%s",stu[i].stu_num);
		if(strncmp(stu[i].stu_num, "end", 3) == 0)
		{
			//strcpy(stu[i].stu_num, "end");
			break;	
		}
		printf("> name: ");
		scanf("%s",stu[i].name);
		printf("> sex: ");
		scanf("%hd",&stu[i].sex);
		printf("> mobile phone: ");
		scanf("%s",stu[i].mobile_phone);
		i++;
	}

	fwrite(stu, sizeof(struct stu_info), i, wp);
	fclose(wp);
	printf("%d record(s) is(are) successfully written!\n", i);
}

int menu()
{
	int option = -1;
	char number[12]="";
	printf("\n\t\t\t\tAll My Students\n");
	printf("\t\t1: get all records\n");
	printf("\t\t2: search one record by the student's number\n");
	printf("\t\t3: insert one or more records\n");
	printf("\t\t4: delete one record by the student's number\n");
	printf("\t\t5: exit\n");
	printf("\n> please input your choice[1,2,3,4,5]: ");
	scanf("%d",&option);
	switch(option)
	{
		case 1:
			printf("Here follow(s) all the record(s):\n");
			read();
			showStudents();
			break;
		case 2:
			printf("Please input the student's number: ");
			scanf("%s",number);
			search(number);
			break;
		case 3:
			printf("please input the student's information:\n");
			write();
			break;
		case 4:
			printf("please input the student's number that is about to delete:\n");
			break;
		case 5:
			printf("bye~\n");
			exit(0);
			break;
		default:
			printf("please check your input.\n");
			break;
	}
	return option;	
}

void printSpace(char s[], int L)
{
	int l = 0, left;
	if(chplace(s) > 1)
		left = L - chplace(s);
	else
		left = L - chplace(s);

	for(; l < left; l++)
	{
		printf(" ");
	}	
}

void space(int l)
{
	while(l)
	{
		printf(" ");
		l--;
	}
}

void printLine(char s[], int L)
{
	int l = 0;
	for(; l < L-strlen(s); l++)
	{
		printf("-");
	}	
}

void line(int l)
{
	while(l)
	{
		printf("-");
		l--;
	}
}

int chplace(char s[])
{
	int i, la = 0, han=0;

	for(i = 0; s[i] != '\0'; i++)
	{
		if(s[i] > 0)
			la++;	
		else
		{
			han++;
		}
	}
	//printf("count=%d\n",han/2+la);
	//printf("%d\n",han);
	//printf("han=%d\n",han);
	if(han>3)
		han-=3;	
	return han+la;
}

void showStudents()
{
	int i = 0;
	printf("+-----------------------------------------------------------------------+\n");
	//printf("|\tnumber\t|\tname\t|\tgender\t|\tmobile phone\t|\n");
	printf("|\tnumber"); space(6);
	printf("|\tname"); space(6);
	printf("|\tgender\t");
	printf("|\tmobile phone"); space(4);
	printf("|\n");
	printf("+-----------------------------------------------------------------------+\n");
	while(i < length)
	{
		printf("|\t%s",stu[i].stu_num);
		printSpace(stu[i].stu_num, 12);
		printf("|\t%s",stu[i].name);
		printSpace(stu[i].name, 10);
		if(stu[i].sex)
			printf("|\tmale\t");
		else
			printf("|\tfemale\t");
		printf("|\t%s",stu[i].mobile_phone);
		printSpace(stu[i].mobile_phone,16);
		printf("|\n");
		printf("+-----------------------------------------------------------------------+\n");
		i++;
	}
	printf("%d record(s) in total.\n", length);
}

void search(char number[])
{
	int i = 0, r = 0;
	read();	
	printf("Here follows the search result:\n");
	printf("+-----------------------------------------------------------------------+\n");
	printf("|\tnumber"); space(6);
	printf("|\tname"); space(6);
	printf("|\tgender\t");
	printf("|\tmobile phone"); space(4);
	printf("|\n");
	printf("+-----------------------------------------------------------------------+\n");
	for(; i < length; i++)
	{
		if(strncmp(number, stu[i].stu_num, strlen(number)) == 0) //模糊查询
		{
			printf("|\t%s",stu[i].stu_num);
			printSpace(stu[i].stu_num, 12);
			printf("|\t%s",stu[i].name);
			printSpace(stu[i].name, 10);
			if(stu[i].sex)
				printf("|\tmale\t");
			else
				printf("|\tfemale\t");
			printf("|\t%s",stu[i].mobile_phone);
			printSpace(stu[i].mobile_phone,16);
			printf("|\n");
			printf("+-----------------------------------------------------------------------+\n");
			r++;
		}
	}	
	printf("%d record(s) in total\n", r);
}

void delete(char number[])
{
}
