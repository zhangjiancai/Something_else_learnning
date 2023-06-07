
#include <gtk/gtk.h>
#include <stdio.h>
#include "mydb.h"

GtkWidget *username_entry,*password_entry;

GtkWidget *vbox;

int write_to_file(char *name)
{   
    FILE *fp;
    char buffer[50];
    time_t timep;
    char time_buffer[100];

    fp=fopen("client.log","a+");
    if(fp==NULL)
    {
        exit(-1);
    }   

    sprintf(buffer,"%s\n",name);
    if(fwrite(buffer,sizeof(char),strlen(buffer),fp)<=0)
    {
        return 0;
    }

    time (&timep);
    sprintf(time_buffer,"%s\n",ctime(&timep));
    if(fwrite(time_buffer,sizeof(char),strlen(time_buffer),fp)<=0)
    {
        return 0;
    }       

    fclose(fp);
    return 1;
}

void closeApp(GtkWidget *window,gpointer data)
{
  mydb_destory(mysql);
  gtk_main_quit();
} 
