#include <mysql.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

MYSQL *mysql;
//MYSQL *mysql1;
MYSQL_RES *res;
//int exists = 0;

void mydb_init()
{
  unsigned int timeout=5;
  int ret=0;

  mysql=mysql_init(NULL);
  if(mysql==NULL)
  {
    fprintf(stderr,"Mysql_init failed\n");
    return;
  }

  ret=mysql_options(mysql,MYSQL_OPT_CONNECT_TIMEOUT,(const char *)&timeout);
  if(ret!=0)
  {
    fprintf(stderr,"set timeout error\n");
    return;
  }
   if (mysql_real_connect(mysql, "localhost", "zjc", "998877", NULL, 0, NULL, 0) == NULL) {
        fprintf(stderr, "Error connecting to MySQL: %s\n", mysql_error(mysql));
        mysql_close(mysql);
        return;
    }

  if (mysql_query(mysql, "CREATE DATABASE IF NOT EXISTS qq")) {
        fprintf(stderr, "Error creating database: %s\n", mysql_error(mysql));
        mysql_close(mysql);
        return;
    }
    if (mysql_query(mysql, "USE qq")) {
        fprintf(stderr, "Error selecting database: %s\n", mysql_error(mysql));
        mysql_close(mysql);
        return;
    }
    if (mysql_query(mysql, "CREATE TABLE IF NOT EXISTS children (childno INT AUTO_INCREMENT,name VARCHAR(255), password VARCHAR(255),port VARCHAR(255), PRIMARY KEY (childno))")) {
        fprintf(stderr, "Error creating table: %s\n", mysql_error(mysql));
        mysql_close(mysql);
    }
    
/*
  mysql1=mysql_real_connect(mysql1,"localhost","zjc","998877",NULL,0,NULL,0);
  if(mysql1){
  	printf("Connected to MySQL server.\n");
  // 执行查询，检查数据库是否存在
        mysql_query(mysql1, "SHOW DATABASES LIKE 'qq'");
        res = mysql_store_result(mysql1);

        if (mysql_num_rows(res) > 0) {
            exists = 1;
            printf("Database already exists.\n");
        }

        mysql_free_result(res);
// 如果数据库不存在，则创建新的数据库
        if (!exists) {
            mysql_query(mysql1, "CREATE DATABASE qq");
            printf("Database created.\n");
            
        }
        mysql_close(mysql1);
        mysql=mysql_real_connect(mysql,"localhost","zjc","998877","qq",0,NULL,0);
        
        }
  
  if(mysql){
    printf("connection success\n");
  }else{ //have problem
    //mysql_query(mysql, "CREATE DATABASE qq");
    //fprintf(stderr,"Database created.\n");
    //
    fprintf(stderr,"connection failed\n");
    if(mysql_errno(mysql)){
      fprintf(stderr,"Connection error %d: %s\n",mysql_errno(mysql),mysql_error(mysql)); 
    }
    if (mysql_query(mysql, "CREATE TABLE IF NOT EXISTS children (name VARCHAR PRIMARY KEY,password VARCHAR)")) {
        fprintf(stderr, "Error creating table: %s\n", mysql_error(mysql));
    }
    return;
    
  }*/
  
}

int mydb_insert(MYSQL *mysql,const char *user,const char *password)
{
  int ret=0;
  /*
  int *a=(int)user;
  int res=*a;
  */
  char* user_data=(char *)user;
  char* pswd_data=(char *)password;
  char qs[100];

  if(mysql==NULL){
    fprintf(stderr,"Mysql error_mydb_insert\n");
    return 0;
  }

  sprintf(qs,"insert into children(name,password) values('%s','%s')",user_data,pswd_data);
  ret=mysql_query(mysql,qs);
  if(!ret){
    printf("inserted %lu rows\n",(unsigned long)mysql_affected_rows(mysql));
  }
  else
  {
    fprintf(stderr,"insert error %d: %s\n",mysql_errno(mysql),mysql_error(mysql));
  }

  return 1;
}

int mydb_update(MYSQL *mysql,const char *user,const char *password)
{
  int ret=0;

  char* user_data=(char *)user;
  char* pswd_data=(char *)password;
  char qs[100];

  if(mysql==NULL){
    fprintf(stderr,"Mysql error_mydb_update\n");
    return 0;
  }
  sprintf(qs,"update children set password='$pswd_data' where name='%s'",user_data);
  ret=mysql_query(mysql,qs);
  if(!ret){
    printf("update %lu rows\n",(unsigned long)mysql_affected_rows(mysql));
  }
  else
  {
    fprintf(stderr,"update error %d: %s\n",mysql_errno(mysql),mysql_error(mysql));
  }

  return 1;
}

int mydb_search(MYSQL *mysql,const char *data)
{
   MYSQL_RES *res_ptr;
   MYSQL_ROW sqlrow;
   int res;
   char *user_data=(char *)data;
   const char *tmp="NULL";
   char qs[100];

  if(mysql==NULL)
  {
    fprintf(stderr,"mysql error_mydb_search\n");
    return 0;
  }

  sprintf(qs,"select name,password from children where name='%s'",user_data);
  res=mysql_query(mysql,qs);
  if(res)
  {
    printf("select error mydb_search :%s\n",mysql_error(mysql));
    return 0;
  }
  else
  {
    res_ptr=mysql_store_result(mysql);
    if(res_ptr){
      printf("Search to %lu rows\n",(unsigned long)mysql_num_rows(res_ptr));

      while(sqlrow=mysql_fetch_row(res_ptr)){
    printf("Fetched data...\n");
    tmp=(const char *)&sqlrow[1];
    if(strstr(tmp,user_data)==NULL)
    {
      printf("not exist!\n");
      return 0;
    }
      }
      if(mysql_errno(mysql)){
    fprintf(stderr,"Retrive error: %s\n",mysql_error(mysql));
      }
      mysql_free_result(res_ptr);
    }
  }
  return 1;
}

int mydb_test(MYSQL *mysql,const void * username_text,const void *  password_text)
{
  MYSQL_RES *res_ptr;
  const char *username=(char *)username_text;
  const char *password=(char *)password_text;
  int res;
  unsigned long res_num;
  char qs[100];

  if(mysql==NULL)
  {
    fprintf(stderr,"mysql error_mydb_test\n");
    return 0;
  }

  sprintf(qs,"select childno from children where name='%s' and password='%s'",username,password);

  res=mysql_query(mysql,qs);
  if(res)
  {
    printf("select error mydb_test:%s\n",mysql_error(mysql));
    return 0;
  }
 else
  {
    res_ptr=mysql_store_result(mysql);
    if(res_ptr){
      res_num=(unsigned long)mysql_num_rows(res_ptr);
      printf("res_num： %ld\n",res_num);
      if(res_num==0)
      {
    return 0;
      }
      else
      {
    printf("Search to %lu rows\n",(unsigned long)mysql_num_rows(res_ptr));
      }

      if(mysql_errno(mysql)){
    fprintf(stderr,"Retrive error: %s\n",mysql_error(mysql));
      } 
    }
    mysql_free_result(res_ptr);
  }
  return 1;
}

void mydb_destory(MYSQL *mysql)
{
  if(mysql==NULL)
  {
    fprintf(stderr,"mysql error_mydb_destory\n");
    return ;
  }
  else
  {
    mysql_close(mysql);
  }
}
