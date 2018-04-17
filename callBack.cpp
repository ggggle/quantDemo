#include "callBack.h"
#include <stdio.h>

int myCB(CodeDownLoadStat& downloadStat, void *pContext)
{
	printf("\r%u/%u", downloadStat.uFinCount, downloadStat.uTotalCount);
	if (downloadStat.uFinCount == downloadStat.uTotalCount)
		printf("\n");
	return 0;
}