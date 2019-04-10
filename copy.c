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
	//����У��
	if (argc < 3)
	{
		printf("parament error!\n");
		return -1;
	}

	int i;
	char buffer[256] = { 0 };

	struct stat state;
		stat(argv[1], &state);
		if (S_ISREG(state.st_mode))  //�жϲ����Ƿ�Ϊ�������ļ�
		{
			//�����ļ�
			copy_file(argv[1], argv[2]);
		}

		if (S_ISDIR(state.st_mode))  //�жϲ����Ƿ�ΪĿ¼
		{
			//�����ļ���
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
		//�ļ����ڣ��ж��Ƿ񸲸�
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

	//�ļ������ڣ�ֱ�Ӵ����ɹ�
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
	char destpath[256] = { 0 };   //Ŀ��·��
	char srcpath[256] = { 0 };    //Դ·��
	strcpy(destpath, dest);
	int ret;
	struct stat  state;

	DIR *dir = NULL;
	struct dirent *entry = NULL;

	ret = stat(destpath, &state);
	if (ret == -1)
	{
		//Ŀ¼�����ڣ�copy
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

			sprintf(destpath, "%s/%s", dest, entry->d_name);  //des·�� 
			sprintf(srcpath, "%s/%s", src, entry->d_name);  //src�ļ�·��
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
	else {   //���Ŀ¼����
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

			sprintf(destpath, "%s/%s", dest, entry->d_name);  //des·�� 
			sprintf(srcpath, "%s/%s", src, entry->d_name);  //src�ļ�·��
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