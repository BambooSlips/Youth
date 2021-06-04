#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>

#define MAXNUMBER 1000

struct stu_info {                                //学生信息
	char stu_num[12];
	char name[10];
	short int sex;
	char mobile_phone[12];
};

struct stu_info stu[MAXNUMBER];			 //保存全部学生信息
int length;                                      //stu长度
sem_t bin_sem;                                   //信号量

void readInfo();                                 //读取文件数据并存入stu
void writeInfo(FILE* wp, struct stu_info stu[]); //将指定学生数据以指定方式写入指定文件
void* menu(void* option);			 //菜单
void printSpace(char s[], int L);		 //计算表格中所需的空格并输出
void space(int l);				 //输出一定数目的空格
int chplace(char s[]);				 //计算字符所占空间（优化关于汉字的计算）
void showStudents();				 //以表格形式显示学生信息
void search(char number[]);			 //根据学号查询学生信息（支持模糊查询）
void delete(char number[]);			 //根据学号删除指定学生信息

int main()
{
	while(1)
	{
		int res, option;                 
		pthread_t a_thread;
		void* thread_result;
		struct stu_info stu[MAXNUMBER];

		res=sem_init(&bin_sem,0,0);
		if(res != 0)
		{
			perror("Semaphore initialition failed!\n");
			exit(EXIT_FAILURE);
		}

		printf("\n\t\t\t\tAll My Students\n");
		printf("\t\t1: get all records\n");
		printf("\t\t2: search one record by the student's number\n");
		printf("\t\t3: insert one or more records\n");
		printf("\t\t4: delete one record by the student's number\n");
		printf("\t\t5: exit\n");
		printf("\n> please input your choice[1,2,3,4,5]: ");
		scanf("%d",&option);
		sem_post(&bin_sem);

		//创建线程并用参数提交选项
		res = pthread_create(&a_thread,NULL, menu, &option);
		if(res != 0)
		{
			perror("Thread creation failed.\n");
		}

		res=pthread_join(a_thread, &thread_result);
		if(res != 0)
		{
			perror("Thread join failed!\n");
		}
		sem_destroy(&bin_sem);
	}
	return 0;
}

void readInfo()
{
	FILE *rp;
	rp = fopen("stu.info","r");
	int i = 0;

	if(rp == 0)
	{
		printf("error occured when reading stu.info");
		exit(1);
	}
	fread(stu, sizeof(struct stu_info), MAXNUMBER, rp);

	//get the length
	length = 0;
	while(strncmp(stu[i].stu_num, "", 1) != 0)
	{
		length++;
		i++;
	}

	//showStudents();
	fclose(rp);
}

void writeInfo(FILE* wp, struct stu_info stu[])
{
	int i = 0;
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
			break;	
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

void* menu(void* option)
{
	sem_wait(&bin_sem);
	char number[12]="";
	FILE *wp;
	struct stu_info stu[MAXNUMBER];
	int c = *(int *)option;
	switch(c)
	{
		case 1:
			printf("Here follow(s) all the record(s):\n");
			readInfo();
			showStudents();
			break;
		case 2:
			printf("Please input the student's number: ");
			scanf("%s",number);
			search(number);
			break;
		case 3:
			wp = fopen("stu.info", "a+");   //在文件后附加
			printf("please input the student's information:\n");
			writeInfo(wp, stu);
			break;
		case 4:
			printf("please input the student's number that is about to delete: ");
			scanf("%s", number);
			delete(number);
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
	readInfo();	
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
	int i = 0, j = 0, k = 0;
	struct stu_info stucpy[length-1];
	FILE* wp = fopen("stu.info", "w");
	for(; i < length; i++)
	{
		if(strncmp(number, stu[i].stu_num, strlen(stu[i].stu_num)) != 0)
		{
			stucpy[j] = stu[i];
			j++;	
		}
	}
	//更新全局变量
	memset(stu, 0, sizeof(struct stu_info)*MAXNUMBER);
	for(; k < j; k++)
	{
		stu[k] = stucpy[k];
	}	
	fwrite(stucpy, sizeof(struct stu_info), j, wp);
	fclose(wp);
	printf("1 record has been successfully deleted!\n");
}
