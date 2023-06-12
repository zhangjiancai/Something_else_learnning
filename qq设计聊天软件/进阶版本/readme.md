gcc your_source_code.c -o output_file -pthread `mysql_config --cflags --libs` `pkg-config --cflags --libs gtk+-3.0`
