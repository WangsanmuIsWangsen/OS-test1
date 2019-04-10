#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <pthread.h>
#include <sys/shm.h>
#include <unistd.h>
#include <stdio.h>
#include<gtk/gtk.h>                            //必须引用gtk/gtk.h这个头文件



union semun {
	int val; /* value for SETVAL */
	struct semid_ds *buf; /* buffer for IPC_STAT, IPC_SET */
	unsigned short *array; /* array for GETALL, SETALL */
	struct seminfo *__buf; /* buffer for IPC_INFO */
};
//P操作
void P(int semid, int index)
{
	struct sembuf {
		ushort sem_num;  //将要处理的信号量的下标。
		short sem_op;  //要执行的操作。
		short sem_flag;  //操作标志
	}sem = { index,-1,0 };
	semop(semid, &sem, 1);
}

void V(int semid, int index)   //P  V  
{
	struct sembuf {
		ushort sem_num;
		short sem_op;
		short sem_flag;
	}sem = { index,+1,0 };
	semop(semid, &sem, 1);
}

struct share {
	char data[10];
	int len;  //记录每个缓冲区的实际大小
};
