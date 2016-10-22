#include <printf.h>
#include <stdlib.h>
#include "DocumentUtil.h"
#include "MyString.h"

int main()
{
	char * date = aupetitADitTroisLignes(1, 9, 2010);
	printf("%s\n", date);
	free(date);
	return 0;
}
