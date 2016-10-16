#include <printf.h>
#include <stdlib.h>
#include "DocumentUtil.h"
#include "MyString.h"

int main()
{
	int ori = 233;
	char * conv = computeDocumentNumber(ori);
	printf("%d --> %s\n", ori, conv);
	free(conv);
	return 0;
}
