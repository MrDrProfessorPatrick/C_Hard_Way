#include <stdio.h>

struct string {
	int length;
	char *data;
};

int main()
{
	struct string s;
	s.length = 4;
	s.data = "This string is much longer then expected";
	
	puts(s.data);
	
	return 0;
}
