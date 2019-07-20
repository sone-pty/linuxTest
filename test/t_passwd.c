#include "../sysHeader.h"
#include <pwd.h>
#include <shadow.h>

int main(void)
{
	struct passwd* ppwd;
	ppwd = getpwnam("sone");

	if(ppwd == NULL)
		printf("not found\n");
	else
		printf("pw_passwd = %s\n",ppwd->pw_passwd);

	struct spwd* pspwd;
	pspwd = getspnam("sone");

	char* pwd = getpass("password:");

	ppwd->pw_passwd = pspwd->sp_pwdp;
	char* encrypted = crypt(pwd,ppwd->pw_passwd);

	if(strcmp(encrypted,pspwd->sp_pwdp) == 0)
		printf("验证通过\n");
	else
		printf("验证失败\n");

	return 0;
}
