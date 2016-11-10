#include <stdio.h>
#include <stdlib.h>

int main()
{
	char * test = "123";
	double d = strtod(test, NULL);
	printf("%1.2lf\n", d);
	return 0;
}
