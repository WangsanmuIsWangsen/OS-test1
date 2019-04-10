#include "header.h"

struct share *buff;
int shmid1; // 共享内存标识符
int semid1; //信号灯标识符
FILE *fp;
int sizenext = 1;

void destroy_progress(GtkWidget *widget)
{
	fclose(fp);
	shmdt(buff);
	gtk_main_quit();  //退出
}

gint print_get(gpointer label)
{
	char outp[50];
	if (sizenext != 0)
	{
		P(semid1, 0);
		buff->len = fread(buff->data, sizeof(char), 10, fp);
		V(semid1, 1);
	}
	sizenext = buff->len;
	if (buff->len == 0)
		sprintf(outp, "get over!");
	else sprintf(outp, "%s from get to buffer1",buff->data);
	//if(label)
	gtk_label_set_text(label, outp);
        return TRUE;
}



int main(int argc, char const *argv[])
{

    // 创建共享内存
    shmid1 = shmget((key_t)1234, sizeof(struct share), 0666|IPC_CREAT);
    // 将共享内存连接到当前进程的地址空间
    buff = shmat(shmid1, 0, 0);
    semid1 = semget((key_t)7, 2, 0666 | IPC_CREAT);
	if (semid1 == -1)
	{
		printf("semget error\n");
		return 1;
	}
	if ((fp = fopen("getfile", "rb")) == NULL)
	{
		printf("open error\n");
		return -1;
	}
	//窗口创建以及设置
	GtkWidget *window;
	GtkWidget *vbox;
	GtkWidget *button;
	GtkWidget *label;
	GtkWidget *pbar;
	gpointer data;
	//初始化gtk库
	gtk_init(&argc, &argv);
	//新建一个顶层窗口
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	//使窗口获得伸缩功能
	gtk_window_set_resizable(GTK_WINDOW(window), TRUE);
	//窗口显示位置
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	//监听destroy事件
	g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(destroy_progress), NULL);
	//窗口标题
	gtk_window_set_title(GTK_WINDOW(window), "get");
	//容器，这里指窗口的边框宽度
	gtk_container_set_border_width(GTK_CONTAINER(window), 20);
	//新的纵向容器
	vbox = gtk_vbox_new(FALSE, 10);
	//纵向容器的边框宽度
	gtk_container_border_width(GTK_CONTAINER(vbox), 100);
	//将纵向容器控件加入到窗口控件中去
	gtk_container_add(GTK_CONTAINER(window), vbox);
	//使vbox能够被显示
	gtk_widget_show(vbox);
	//标签
	label = gtk_label_new("get");
	//在vbox中从前往后进行排列,不随窗口拉伸而改变大小
	gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 10);
	//显示标签
	gtk_widget_show(label);
	//每隔3000ms进行一次刷新，运行一次函数
	pbar = gtk_label_new("content");
	gtk_box_pack_start(GTK_BOX(vbox), pbar, FALSE, FALSE, 10);
	gtk_widget_show(pbar);
	g_timeout_add(3000, print_get, (gpointer)pbar);
	
	//增加close按钮
	button = gtk_button_new_with_label("close");
	g_signal_connect_swapped(G_OBJECT(button), "clicked", G_CALLBACK(destroy_progress), window);
	gtk_box_pack_start(GTK_BOX(vbox), button, FALSE, FALSE, 10);
	GTK_WIDGET_SET_FLAGS(button, GTK_CAN_DEFAULT);
	gtk_widget_grab_default(button);
	gtk_widget_show(button);
	//显示窗口
	gtk_widget_show(window);
	gtk_main();


	return 0;
}
