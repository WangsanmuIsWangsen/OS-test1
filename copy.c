#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>


void copy_file(const char *src, const char *dest);
void copy_directory(const char *src, const char *dest);

int main(int argc, char *argv[])

{
	//参数校验
	if (argc < 3)
	{
		printf("parament error!\n");
		return -1;
	}

	int i;
	char buffer[256] = { 0 };

	struct stat state;
		stat(argv[1], &state);
		if (S_ISREG(state.st_mode))  //判断参数是否为单独的文件
		{
			//拷贝文件
			copy_file(argv[1], argv[2]);
		}

		if (S_ISDIR(state.st_mode))  //判断参数是否为目录
		{
			//拷贝文件夹
			copy_directory(argv[1], argv[2]);
		}

	return 0;
}
//copy file
void copy_file(const char *src, const char *dest)
{

	int  src_fd;
	int  dest_fd;

	src_fd = open(src, O_RDONLY);

	if (src_fd == -1)
	{
		perror("mycy srcfile error!\n");
		return;
	}

	char ch;
	dest_fd = open(dest, O_WRONLY | O_CREAT | O_EXCL, 0777);

	if (dest_fd < 0)
	{
		//文件存在，判断是否覆盖
		printf("Over write ? Y/N\n");
		scanf("%c", &ch); getchar();
		if ((ch == 'Y') || (ch == 'y'))
		{
			close(dest_fd);
			dest_fd = open(dest, O_WRONLY | O_TRUNC);
		}
		else
		{
			return;
		}
	}

	//文件不存在，直接创建成功
	char buffer[10] = { 0 };
	int ret;

	while (1)
	{
		ret = read(src_fd, buffer, 10);
		write(dest_fd, buffer, ret);
		if (ret != 10)
			break;
	}

	close(dest_fd);
	close(src_fd);
}
//copy directory
void copy_directory(const char *src, const char *dest)
{
	char destpath[256] = { 0 };   //目的路径
	char srcpath[256] = { 0 };    //源路径
	strcpy(destpath, dest);
	int ret;
	struct stat  state;

	DIR *dir = NULL;
	struct dirent *entry = NULL;

	ret = stat(destpath, &state);
	if (ret == -1)
	{
		//目录不存在，copy
		ret = mkdir(destpath, 0755);
		dir = opendir(src);
		if (dir == NULL)
		{
			perror("open dir error");
			return;
		}

		while (1)
		{
			entry = readdir(dir);
			if (entry == NULL)
				break;
			if ((strcmp(entry->d_name, ".") == 0) || (strcmp(entry->d_name, "..") == 0))
				continue;

			sprintf(destpath, "%s/%s", dest, entry->d_name);  //des路径 
			sprintf(srcpath, "%s/%s", src, entry->d_name);  //src文件路径
			stat(srcpath, &state);

			if (S_ISDIR(state.st_mode))
			{
				copy_directory(srcpath, destpath);
			}
			else
			{
				copy_file(srcpath, destpath);
			}
		}
		closedir(dir);
		return;
	}
	else {   //如果目录存在
		ret = mkdir(destpath, 0755);
		dir = opendir(src);
		if (dir == NULL)
		{
			perror("open dir error");
			return;
		}

		while (1)
		{
			entry = readdir(dir);
			if (entry == NULL)
				break;
			if ((strcmp(entry->d_name, ".") == 0) || (strcmp(entry->d_name, "..") == 0))
				continue;

			sprintf(destpath, "%s/%s", dest, entry->d_name);  //des路径 
			sprintf(srcpath, "%s/%s", src, entry->d_name);  //src文件路径
			stat(srcpath, &state);

			if (S_ISDIR(state.st_mode))
			{
				copy_directory(srcpath, destpath);
			}
			else
			{
				copy_file(srcpath, destpath);
			}
		}
		closedir(dir);
		return;
	}
}