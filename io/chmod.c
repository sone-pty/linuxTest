#include "IOHeader.h"

int main(int argc,char* arg[])
{
	mode_t mode=0;
	if(argc < 3)
	{
		printf("argc < 3\n");
		exit(1);
	}
	char* mode_str=arg[1];
	char ch=*mode_str;
	
	int i=1;
	while(i<=3)
	{
		switch(ch)
		{
			case '7':
			{
				switch(i)
				{
					case 1:
					{
						mode=mode | S_IRUSR | S_IWUSR | S_IXUSR;
						break;
					}
					case 2:
					{
						mode=mode | S_IRGRP | S_IWGRP | S_IXGRP;
						break;
					}
					case 3:
					{
						mode=mode | S_IROTH | S_IWOTH | S_IXOTH;
						break;
					}
				}
				break;
			}
			case '6':
			{
				switch(i)
				{
					case 1:
					{
						mode|=S_IRUSR | S_IWUSR;
						break;
					}
					case 2:
					{
						mode|=S_IRGRP | S_IWGRP;
						break;
					}
					case 3:
					{
						mode|=S_IROTH | S_IWOTH;
						break;
					}
				}
				break;
			}
			case '5':
			{
				switch(i)
				{
					case 1:
					{
						mode|=S_IRUSR | S_IXUSR;
						break;
					}
					case 2:
					{
						mode|=S_IRGRP | S_IXGRP;
						break;
					}
					case 3:
					{
						mode|=S_IROTH | S_IXOTH;
						break;
					}
				}
				break;
			}
			case '4':
			{
				switch(i)
				{
					case 1:
					{
						mode|=S_IRUSR;
						break;
					}
					case 2:
					{
						mode|=S_IRGRP;
						break;
					}
					case 3:
					{
						mode|=S_IROTH;
						break;
					}
				}
				break;
			}
			case '3':
			{
				switch(i)
				{
					case 1:
					{
						mode|=S_IWUSR | S_IXUSR;
						break;
					}
					case 2:
					{
						mode|=S_IWGRP | S_IXGRP;
						break;
					}
					case 3:
					{
						mode|=S_IWOTH | S_IXOTH;
						break;
					}
				}
				break;
			}
			case '2':
			{
				switch(i)
				{
					case 1:
					{
						mode|=S_IWUSR;
						break;
					}
					case 2:
					{
						mode|=S_IWGRP;
						break;
					}
					case 3:
					{
						mode|=S_IWOTH;
						break;
					}
				}
				break;
			}
			case '1':
			{
				switch(i)
				{
					case 1:
					{
						mode|=S_IXUSR;
						break;
					}
					case 2:
					{
						mode|=S_IXGRP;
						break;
					}
					case 3:
					{
						mode|=S_IXOTH;
						break;
					}
				}
				break;
			}
		}
		ch=*mode_str++;
		i++;
	}
	chmod(arg[2],mode);
	return 0;
}
