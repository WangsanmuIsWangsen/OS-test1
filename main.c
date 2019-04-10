#include "header.h"


int main(void)

{

	/*共享内存的设置*/
	int shmid1, shmid2;
	//分配共享内存块1
	shmid1 = shmget((key_t)1234, sizeof(struct share), 0666 | IPC_CREAT);
	//分配共享内存块2
	shmid2 = shmget((key_t)1111, sizeof(struct share), 0666 | IPC_CREAT);



	/*创建4个信号灯，*/
	int semid1, semid2;
	union semun arg;

	semid1 = semget((key_t)7, 2, IPC_CREAT | 0666);
	//信号量赋初值
	arg.val = 1;
	semctl(semid1, 0, SETVAL, arg);  
	arg.val = 0;
	semctl(semid1, 1, SETVAL, arg);    

	semid2 = semget((key_t)10, 2, IPC_CREAT | 0666);
	arg.val = 1;
	semctl(semid2, 0, SETVAL, arg);
	arg.val = 0;
	semctl(semid2, 1, SETVAL, arg);   

	int pid1, pid2, pid3;
	if ((pid1 = fork()) == 0)
	{
		execv("./get", NULL);
	}
	else if ((pid2 = fork()) == 0)
	{
		execv("./copytobuf", NULL);
	}
	else if (pid3 = fork() == 0)
	{
		execv("./put", NULL);
	}
	else
	{
		/*等待退出*/
		waitpid(pid1, NULL, 0);
		waitpid(pid2, NULL, 0);
		waitpid(pid3, NULL, 0);


		/*删除信号灯&共享内存*/
		semctl(semid1, 1, IPC_RMID, 0);
		semctl(semid2, 1, IPC_RMID, 0);
		shmctl(shmid1, IPC_RMID, 0);
		shmctl(shmid2, IPC_RMID, 0);
	}

}