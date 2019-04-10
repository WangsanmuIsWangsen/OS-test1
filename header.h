#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <pthread.h>
#include <sys/shm.h>
#include <unistd.h>
#include <stdio.h>
#include<gtk/gtk.h>                            //��������gtk/gtk.h���ͷ�ļ�



union semun {
	int val; /* value for SETVAL */
	struct semid_ds *buf; /* buffer for IPC_STAT, IPC_SET */
	unsigned short *array; /* array for GETALL, SETALL */
	struct seminfo *__buf; /* buffer for IPC_INFO */
};
//P����
void P(int semid, int index)
{
	struct sembuf {
		ushort sem_num;  //��Ҫ������ź������±ꡣ
		short sem_op;  //Ҫִ�еĲ�����
		short sem_flag;  //������־
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
	int len;  //��¼ÿ����������ʵ�ʴ�С
};
