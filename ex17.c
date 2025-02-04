#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define MAX_DATA 512
#define MAX_ROWS 100

struct Address {
	int id;
	int set;
	char name[10];
	char email[10];
};

struct Database {
	struct Address rows[10];
	int max_rows;
	int max_data;
};

struct Connection {
	FILE *file;
	struct Database *db;
};

void die(const char *message, struct Connection *conn) 
{
	if(errno) {
		perror(message);
	} else {
		printf("ERROR %s", message);
	}
	
	if(conn) {
		if(conn->file) fclose(conn->file);
		if(conn->db) free(conn->db);
		free(conn);
	}
	
	exit(1);
}

void Address_print(struct Address *addr)
{
printf("id = %d name = %s email = %s\n",
	addr->id, addr->name, addr->email);
}

void Database_load(struct Connection *conn) {
	int rc = fread(conn->db, sizeof(struct Database), 1, conn->file);
	if(rc != 1) die("Fail to load database", conn);
}

struct Connection *Database_open(const char *filename, char mode, int max_rows, int max_data) 
{
	struct Connection *conn = malloc(sizeof(struct Connection));
	
	conn->db = malloc(sizeof(struct Database));
	if(!conn->db) die("Memory error", conn);

	if(mode == 'c') {
		conn->file = fopen(filename, "w");
		conn->db->max_rows = max_rows;
		conn->db->max_data = max_data;
	} else {
		conn->file = fopen(filename, "r+");
		
		if(conn->file) {
			Database_load(conn);
		}
	}	
	
	if(!conn->file) die("Failed to open the file", conn);
	
	return conn;
}

void Database_close (struct Connection *conn) 
{
	if(conn) {
		if(conn->file) fclose(conn->file);
		if(conn->db) free(conn->db);
		free(conn);
	}
}

void Database_write (struct Connection *conn)
{
	rewind(conn->file);
	
	int rc = fwrite(conn->db, sizeof(struct Database), 1, conn->file);
	if(rc != 1) die("Failed to write DB", conn);
	
	rc = fflush(conn->file);
	if(rc != 0) die("Cannot flush DB", conn);
}	

void Database_create(struct Connection *conn) 
{
	int i = 0;
	
	for(i = 0; i < conn->db->max_rows; i++) {
		struct Address addr = {.id = i, .set = 0};
		conn->db->rows[i] = addr;
	}
}

void Database_set(struct Connection *conn, int id, const char *name, const char *email)
{
	struct Address *addr = &conn->db->rows[id];
	if(addr->set) die("Alredy set, remove it first", conn);
	printf("Name before setting %p", addr->name);
	addr->set = 1;
	// BUG HERE
	char *res = strncpy(addr->name, name, conn->db->max_data);
	if(conn->db->max_data > 0) addr->name[conn->db->max_data - 1] = '\0';
	if(!res) die("Name copy failed", conn);
	res = strncpy(addr->email, email, conn->db->max_data);
	if(conn->db->max_data > 0) addr->email[conn->db->max_data - 1] = '\0';
	if(!res) die("Email copy failed", conn);
}

void Database_get(struct Connection *conn, int id)
{
	struct Address *addr = &conn->db->rows[id];
	
	if(addr->set) {
		Address_print(addr);
	} else {
		die("ID is not set", conn);
	}
}

void Database_delete(struct Connection *conn, int id)
{
	struct Address addr = {.id = id, .set = 0};
	conn->db->rows[id] = addr;
}

void Database_list(struct Connection *conn)
{
	int i = 0;
	struct Database *db = conn->db;
	
	for(i = 0; i < conn->db->max_rows; i++) {
		struct Address *cur = &db->rows[i];
		
		if(cur->set) {
			Address_print(cur);
		}
	}
}

int main(int argc, char *argv[])
{
	if(argc < 3) die("USAGE: ex17 <dbfile> <action> [action params]", NULL);
	char *filename = argv[1];
	char action = argv[2][0];

	int max_rows = atoi(argv[3]);
	int max_data = atoi(argv[4]);
	printf("max_rows = %d, max_data = %d", max_rows, max_data);
	printf("%s - %c - %d\n", filename, action, argc);
	struct Connection *conn = Database_open(filename, action, max_rows, max_data);
	int id = 0;
	
	if(argc > 5) id = atoi(argv[5]);
	if(id >= max_rows) die("There's not that many records.", conn);

	switch(action) {
		case 'c':
			if(argc < 5) die("Max rows and data length should be set", NULL);
			Database_create(conn);
			Database_write(conn);
			break;
		case 'g':
			if(argc != 6) die("Need an id to get", conn);
			Database_get(conn, id);
			break;
		case 's':
			if(argc != 8) die("Need id, name, email to set", conn);
			id = atoi(argv[5]);
			Database_set(conn, id, argv[6], argv[7]);
			Database_write(conn);
			break;
		case 'd':
			if(argc != 6) die("Need id to delete", conn);
			Database_delete(conn, id);
			Database_write(conn);
			break;
		case 'l':
			Database_list(conn);
			break;
			default:
				die("Invalid action, only: c=create, g=get, s=set, d=del, l=list", conn);
	}
	
	Database_close(conn);
	return 0;
}



