#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

#define FILE_NAME "myfifo"

int main()
{
	umask(0); //将文件默认掩码设置为0
	if (mkfifo(FILE_NAME, 0666) < 0){ //使用mkfifo创建命名管道文件
		perror("mkfifo");
		return 1;
	}

	//create success...

	return 0;
}
