#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct stu_info {
	char stu_num[12];
	char name[10];
	short int sex;
	char mobile_phone[12];
};

void read();
void write();

int main()
{
	FILE *rp;
	struct stu_info stu;
	write();
	read();
	return 0;
}

void read()
{
	FILE *rp;
	struct stu_info stu;
	char sex[3];
	rp = fopen("stu.info","r");
	if(rp == 0)
	{
		printf("error occured when reading stu.info");
		exit(1);
	}

	fread(&stu, sizeof(struct stu_info), 1, rp);

	//printf("student:\n number:%s\nname:%s\ngender:%d\nmobile phone:%s\n",stu.stu_num,stu.name,stu.sex,stu.mobile_phone);
    printf("\nHere follows all the imformation about <%s>:\n",stu.stu_num);
    printf("\tName: %s\n",stu.name);
    if(stu.sex)
        printf("\tGender: male\n");
    else
        printf("\tGender: female\n");
    printf("\tMobile phone: %s\n",stu.mobile_phone);
	fclose(rp);
}

void write()
{
	FILE *wp;
	struct stu_info stu;
	wp = fopen("stu.info", "w");
	if(wp == 0)
	{
		printf("fopen error\n");
		exit(1);
	}
	printf("> number: ");
	scanf("%s",stu.stu_num);
	printf("> name: ");
	scanf("%s",stu.name);
	printf("> sex: ");
	scanf("%d",&stu.sex);
	printf("> mobile phone: ");
	scanf("%s",stu.mobile_phone);

	fwrite(&stu, sizeof(struct stu_info), 1, wp);
	fclose(wp);
	//printf("Write compeleted!");
}

