#include "../sysHeader.h"
#include "../std.h"
#include <bitset>

static char table[65]="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

struct s{
	int i;
	char c;
};

char Convert(unsigned char c)
{
    unsigned int index = c;
    return table[index];
}

string encode(const char* buf,size_t buflen)
{
	string ret;
    unsigned char split3[3];
    unsigned char split4[4];
    int i = 0;

    while(buflen--)
    {
        split3[i++] = *(buf++);

        if(i == 3)
        {
            split4[0] = (split3[0]&0xfc) >> 2;
            split4[1] = ((split3[0]&0x03) << 4) + ((split3[1]&0xf0) >> 4);
            split4[2] = ((split3[1]&0x0f) << 2) + ((split3[2]&0xc0) >> 6);
            split4[3] = (split3[2]&0x3f);

            for(int j = 0;j < 4;j++)
            {
                ret += Convert(split4[j]);
            }

            i = 0;
        }
    }

    if(i == 1)
    {
        split4[0] = (split3[0]&0xfc) >> 2;
        split4[1] = (split3[0]&0x03) << 4;

        for(int j = 0;j < 2;j++)
        {
            ret += Convert(split4[j]);
        }

        ret += '=';
        ret += '=';
    }
    else if(i == 2)
    {
        split4[0] = (split3[0]&0xfc) >> 2;
        split4[1] = ((split3[0]&0x03)<<4) + ((split3[1]&0xf0)>>4);
        split4[2] = ((split3[1]&0x0f)<<2);

        for(int j = 0;j < 3;j++)
        {
            ret += Convert(split4[j]);
        }

        ret += '=';
    }

    return ret;
}
int main(void)
{
	/*
	enum model
	{
		M,N
	};

	model i = M;
	model dest;

	memcpy(&dest,&i,sizeof(dest));
	
	cout << "size == " << sizeof(dest) << endl;
	cout << "i == " << i << endl;
	cout << "dest == " << dest << endl;
	
	
	int flag = fcntl(STDIN_FILENO,F_GETFL);
	flag |= O_NONBLOCK;
	fcntl(STDIN_FILENO,F_SETFL);
	
	char c = getchar();
	char buf[1024];
	cout << c << endl;

	read(STDIN_FILENO,buf,1024);

	cout << buf << endl;
	

	int fd = open("include",O_RDWR);
	if(fd == -1)
	{
		perror("open");
		exit(1);
	}
	*/
	int i = 8;
	char* p = (char*)&i;

	for(char* k = p;k != p + 4;++k)
	{
		int m = 1 << 7;
		for(int n = 0;n < 8;++n)
		{
			if(*k & m)
				cout << "1";
			else
				cout << "0";
			m = m >> 1;
		}
	}
	cout << endl;

	return 0;
}
