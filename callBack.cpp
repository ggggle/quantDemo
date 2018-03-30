#include "callBack.h"
#include <iostream>

using namespace std;

void myCB(CodeDownLoadStat& downloadStat, void *pContext)
{
	cout << downloadStat.uFinCount << endl;
	cout << downloadStat.uTotalCount << endl;
}