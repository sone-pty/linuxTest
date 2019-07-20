#include "../sysHeader.h"

typedef struct _STU
{
	char name[20];
	int age;
	int no;
}stu,*pstu;

int main(void)
{
	printf("now program break = %p\n",sbrk(0));
	pstu p = (pstu)calloc(5,sizeof(stu));
	
	int i;
	pstu ptr;
	
	for(i = 0;i < 5;i++)
	{
		ptr = p + i;
		memcpy(ptr->name,"pty",3);
		ptr->age = i + 20;
		ptr->no = i + 1;
	}

	printf("NO:%d,name:%s,age:%d\n",ptr->no,ptr->name,ptr->age);
	free(p);

	printf("now program break = %p\n",sbrk(0));
	return 0;
}
