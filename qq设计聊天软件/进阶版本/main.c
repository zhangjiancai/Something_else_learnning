#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "view.h"
#include <spawn.h>

static GtkWidget* entry1;
static GtkWidget* entry2;

GtkWidget* window;

void on_button_clicked(GtkWidget* button, gpointer data)
{
    if ((intptr_t)data == 1)
    {
        gtk_entry_set_text(GTK_ENTRY(entry1), "");
        gtk_entry_set_text(GTK_ENTRY(entry2), "");
    }
    else if ((intptr_t)data == 2)
    {
        const gchar* username = gtk_entry_get_text(GTK_ENTRY(entry1));
        const gchar* password = gtk_entry_get_text(GTK_ENTRY(entry2));

        mydb_init();
        if (mydb_test(mysql, username, password) == 1)
        {
            if (write_to_file("login success!") == 0)
            {
                exit(-1);
            }
            mydb_destory(mysql);
            // 登陆到客户端窗口程序
            execlp("./client", username, password, NULL);
        }
        else
        {
            if (write_to_file("login failed!") == 0)
            {
                exit(-1);
            }
            exit(-1);
        }
    }
    else if ((intptr_t)data == 3)
    {
        /*改变文本空的可编辑状态*/
        gtk_editable_set_editable(GTK_EDITABLE(entry1), TRUE);
        gtk_editable_set_editable(GTK_EDITABLE(entry2), TRUE);
    }
    else if ((intptr_t)data == 4)
    {
        const char* programPath = "./register";  // 替换为你要打开的程序路径
        int status = system(programPath);

        if (status == -1)
        {
            // 执行系统命令失败
            perror("Failed to execute system command");
            exit(-1);
        }
    }
}

int main(int argc, char* argv[])
{
    GtkWidget* box;
    GtkWidget* box1;
    GtkWidget* box2;
    GtkWidget* box3;
    GtkWidget* label1;
    GtkWidget* label2;
    GtkWidget* button;
    GtkWidget* sep;

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(closeApp), NULL);
    gtk_window_set_title(GTK_WINDOW(window), "register windows");
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_container_set_border_width(GTK_CONTAINER(window), 20);

    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(window), box);

    box1 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_box_pack_start(GTK_BOX(box), box1, FALSE, FALSE, 5);

    box2 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_box_pack_start(GTK_BOX(box), box2, FALSE, FALSE, 5);

    sep = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_box_pack_start(GTK_BOX(box), sep, FALSE, FALSE, 5);

    box3 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_box_pack_start(GTK_BOX(box), box3, TRUE, TRUE, 5);

    label1 = gtk_label_new("用户名：");
    entry1 = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(box1), label1, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(box1), entry1, FALSE, FALSE, 5);

    label2 = gtk_label_new("密    码：");
    entry2 = gtk_entry_new();
    gtk_entry_set_visibility(GTK_ENTRY(entry2), FALSE);
    gtk_box_pack_start(GTK_BOX(box2), label2, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(box2), entry2, FALSE, FALSE, 5);

    button = gtk_check_button_new_with_label("Editable");
    g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(on_button_clicked), (gpointer)3);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(button), TRUE);
    gtk_box_pack_start(GTK_BOX(box3), button, TRUE, TRUE, 10);
    gtk_widget_show(button);

    button = gtk_button_new_with_label("清空");
    g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(on_button_clicked), (gpointer)1);
    gtk_box_pack_start(GTK_BOX(box3), button, TRUE, TRUE, 10);
    gtk_widget_show(button);

    button = gtk_button_new_with_label("确认");
    g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(on_button_clicked), (gpointer)2);
    g_signal_connect_swapped(G_OBJECT(button), "clicked", G_CALLBACK(gtk_widget_destroy), window);
    gtk_box_pack_start(GTK_BOX(box3), button, TRUE, TRUE, 5);
    gtk_widget_show(button);

    button = gtk_button_new_with_label("register");
    g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(on_button_clicked), (gpointer)4);
    gtk_box_pack_start(GTK_BOX(box3), button, TRUE, TRUE, 5);
    gtk_widget_show(button);

    gtk_widget_show_all(window);
    gtk_main();
    return 0;
}
