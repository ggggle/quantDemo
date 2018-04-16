#include "export.h"
#include <iostream>
#include <chrono>
#include <fstream>
#include "cfg.h"
#include "callBack.h"
#ifdef _WIN32
#include "minidump.h"
class DumpClass {
public:
	DumpClass() {
		InitMinDump();
	}
};
DumpClass g_dump;
#endif // WIN32

#if _MSC_VER >= 1900
#pragma comment(lib, "legacy_stdio_definitions.lib")
#endif

#define GET(arg) \
cout<<#arg##":";\
cin>>arg;

#define OUTPUT(arg) \
cout<<"###"<<#arg##":"<<arg<<endl;

#define COUNT_TIME(RET, FUNC) \
do { \
auto start = chrono::system_clock::now(); \
RET = FUNC; \
auto duration = chrono::duration_cast<microseconds>(chrono::system_clock::now() - start); \
cout <<"**cost[" << double(duration.count()) / microseconds::period::den << "s]" << endl; \
} while (0);

using namespace std;
using namespace chrono;

void formatDatas(double* klineDatas, unsigned int* dfIndex, int num)
{
	cout << dfIndex[num] << endl;
	cout << "---------" << endl;
	cout << klineDatas[13 * num + KLINE_OPEN] << endl;
	cout << klineDatas[13 * num + KLINE_HIGH] << endl;
	cout << klineDatas[13 * num + KLINE_LOW] << endl;
	cout << klineDatas[13 * num + KLINE_CLOSE] << endl;
	cout << klineDatas[13 * num + KLINE_PRECLOSE] << endl;
	cout << klineDatas[13 * num + KLINE_AMOUNT] << endl;
	cout << klineDatas[13 * num + KLINE_BALANCE] << endl;
	cout << endl;
}

void testgetHistoryByDateSize(HsQuantDataSDKInterface* sdk)
{
	string mkt, code;
	int startDate, endDate, frequency, fillFlag, recordNum;
	GET(mkt);
	GET(code);
	GET(startDate);
	GET(endDate);
	GET(frequency);
	GET(fillFlag);
	int ret;
	COUNT_TIME(ret, sdk->getHistoryByDateSize(mkt.c_str(), code.c_str(), startDate, endDate, 
		frequency, fillFlag, recordNum));
	OUTPUT(ret);
	OUTPUT(recordNum);
}

void testgetHistoryByDate(HsQuantDataSDKInterface* sdk)
{
	string mkt, code;
	int startDate, endDate, frequency, fillFlag, right, recordNum, tmp;
	GET(mkt);
	GET(code);
	GET(startDate);
	GET(endDate);
	GET(frequency);
	GET(fillFlag);
	GET(right);
	sdk->getHistoryByDateSize(mkt.c_str(), code.c_str(), startDate, endDate, frequency, fillFlag, tmp);
	if (tmp <= 0)
		return;
	double* klineDatas = new double[13 * tmp];
	unsigned int* dfIndex = new unsigned int[tmp];
	int ret;
	COUNT_TIME(ret, sdk->getHistoryByDate(mkt.c_str(), code.c_str(), startDate, endDate, frequency, 
		right, fillFlag, recordNum, klineDatas, dfIndex, nullptr, nullptr));
	OUTPUT(ret);
	OUTPUT(recordNum);
	if (0 == ret)
	{
		int showResult;
		GET(showResult);
		if (1 == showResult)
		{
			for (int i = 0; i < recordNum; ++i)
			{
				formatDatas(klineDatas, dfIndex, i);
			}
		}
	}
	delete[] klineDatas;
	delete[] dfIndex;
}

void testgetLocalHistoryByDateSize(HsQuantDataSDKInterface* sdk)
{
	string mkt, code;
	int startDate, endDate, frequency, recordNum;
	GET(mkt);
	GET(code);
	GET(startDate);
	GET(endDate);
	GET(frequency);
	int ret;
	COUNT_TIME(ret, 
		sdk->getLocalHistoryByDateSize(mkt.c_str(), code.c_str(), startDate, endDate, frequency, recordNum));
	OUTPUT(ret);
	OUTPUT(recordNum);
}

void testgetLocalHistoryByDate(HsQuantDataSDKInterface* sdk)
{
	string mkt, code;
	int startDate, endDate, frequency, recordNum, right;
	GET(mkt);
	GET(code);
	GET(startDate);
	GET(endDate);
	GET(frequency);
	GET(right);
	sdk->getLocalHistoryByDateSize(mkt.c_str(), code.c_str(), startDate, endDate, frequency, recordNum);
	if (recordNum <= 0)
		return;
	double* klineDatas = new double[13 * recordNum];
	unsigned int* dfIndex = new unsigned int[recordNum];
	int ret;
	COUNT_TIME(ret, sdk->getLocalHistoryByDate(mkt.c_str(), code.c_str(), startDate, endDate, 
		frequency, right, klineDatas, dfIndex));
	OUTPUT(ret);
	OUTPUT(recordNum);
	if (0 == ret)
	{
		int showResult;
		GET(showResult);
		if (1 == showResult)
		{
			for (int i = 0; i < recordNum; ++i)
			{
				formatDatas(klineDatas, dfIndex, i);
			}
		}
	}
	delete[] klineDatas;
	delete[] dfIndex;
}

void testgetLocalHistoryByOffset(HsQuantDataSDKInterface* sdk)
{
	string mkt, code;
	int queryDate, barCount, iFrequency, iRight, fillFlag, recordNum;
	GET(mkt);
	GET(code);
	GET(queryDate);
	GET(barCount);
	GET(iFrequency);
	GET(iRight);
	GET(fillFlag);
	if (barCount == 0)
		return;
	int count = barCount;
	if (count < 0)
		count *= -1;
	double* klineDatas = new double[13 * count];
	unsigned int* dfIndex = new unsigned int[count];
	int ret;
	COUNT_TIME(ret, sdk->getLocalHistoryByOffset(mkt.c_str(), code.c_str(), queryDate, 
		barCount, iFrequency, iRight, fillFlag, recordNum, klineDatas, dfIndex));
	OUTPUT(ret);
	OUTPUT(recordNum);
	if (0 == ret)
	{
		int showResult;
		GET(showResult);
		if (1 == showResult)
		{
			for (int i = 0; i < recordNum; ++i)
			{
				formatDatas(klineDatas, dfIndex, i);
			}
		}
	}
	delete[] klineDatas;
	delete[] dfIndex;
}

void testdownLoadHistoryData(HsQuantDataSDKInterface* sdk)
{
	string mkt;
	int startDate, endDate, frequency;
	GET(mkt);
	GET(startDate);
	GET(endDate);
	GET(frequency);
	int ret;
	COUNT_TIME(ret, sdk->downLoadHistoryData(mkt.c_str(), frequency, startDate, endDate, myCB, nullptr));
	OUTPUT(ret);
}

void testdeleteLocalStockData(HsQuantDataSDKInterface* sdk)
{
	int codeCount;
	GET(codeCount);
	if (codeCount <= 0)
		return;
	CodeInfo* _codeInfo = new CodeInfo[codeCount];
	string mkt,code;
	int frequency, date, direction;
	for (int i = 0; i < codeCount; ++i)
	{
		GET(mkt);
		GET(code);
		GET(frequency);
		GET(date);
		GET(direction);
		memcpy(_codeInfo[i].code, code.c_str(), code.length());
		memcpy(_codeInfo[i].market, mkt.c_str(), mkt.length());
		_codeInfo[i].frequency = frequency;
		_codeInfo[i].startDate = date;
		_codeInfo[i].direction = direction;
	}
	int ret;
	COUNT_TIME(ret, sdk->deleteLocalStockData(_codeInfo, codeCount));
	OUTPUT(ret);
	delete[] _codeInfo;
}

void testgetStorageConfig(HsQuantDataSDKInterface* sdk)
{
	StorageConfig _cfg;
	int ret = sdk->getStorageConfig(_cfg);
	OUTPUT(ret);
	OUTPUT(_cfg.storagePath);
}

void testsetStorageConfig(HsQuantDataSDKInterface* sdk)
{
	string storagePath;
	GET(storagePath);
	if (storagePath.length() < 1)
		return;
	StorageConfig _cfg;
	memcpy(_cfg.storagePath, storagePath.data(), storagePath.length());
	int ret = sdk->setStorageConfig(_cfg);
	OUTPUT(ret);
}

void testgetLocalStorageStockInfo(HsQuantDataSDKInterface* sdk)
{
	string pMarket;
	int nFrequency, nStartDate, nEndDate;
	GET(pMarket);
	GET(nFrequency);
	long long size;
	COUNT_TIME(size, sdk->getLocalStorageStockInfo(pMarket.c_str(), nFrequency, nStartDate, nEndDate));
	OUTPUT(size);
	OUTPUT(nStartDate);
	OUTPUT(nEndDate);
}

void testgetStorageStockInfo(HsQuantDataSDKInterface* sdk)
{
	string pMarket;
	int nFrequency, nStartDate, nEndDate;
	GET(pMarket);
	GET(nFrequency);
	long long size;
	COUNT_TIME(size, sdk->getStorageStockInfo(pMarket.c_str(), nFrequency, nStartDate, nEndDate));
	OUTPUT(size);
	OUTPUT(nStartDate);
	OUTPUT(nEndDate);
}

void testgetLocalStorageStockInfoOneCode(HsQuantDataSDKInterface* sdk)
{
	string code, mkt;
	int nFrequency;
	GET(code);
	GET(mkt);
	GET(nFrequency);
	CodeInfo _info;
	memcpy(_info.code, code.c_str(), code.length());
	memcpy(_info.market, mkt.c_str(), mkt.length());
	_info.frequency = nFrequency;
	long long size;
	COUNT_TIME(size, sdk->getLocalStorageStockInfo(&_info, 1));
	OUTPUT(size);
	OUTPUT(_info.startDate);
	OUTPUT(_info.endDate);
}

void testgetMarketCodeList(HsQuantDataSDKInterface* sdk)
{
	string hqType;
	int beginDate, endDate;
	GET(hqType);
	GET(beginDate);
	GET(endDate);
	CodeInfo* codes = NULL;
	int count;
	int ret;
	COUNT_TIME(ret,
		sdk->getMarketCodeList(hqType.c_str(), beginDate, endDate, &codes, count));
	OUTPUT(ret);
	OUTPUT(count);
	fstream file("codeList.txt", ios::out);
	for (int i = 0; i < count; ++i)
	{
		file << codes[i].code << "." << codes[i].market << "," << codes[i].startDate << endl;
	}
	file.close();
	sdk->releaseCodeInfo(codes);
	cout << "结果已保存至codeList.txt" << endl;
}

void testgetIndexData(HsQuantDataSDKInterface* sdk)
{
	string indexCode, market;
	int beginDate, endDate, shareCount;
	GET(indexCode);
	GET(market);
	GET(beginDate);
	GET(endDate);
	ShareGroup* _shareGrp;
	int ret;
	COUNT_TIME(ret,
		sdk->getIndexData(indexCode.c_str(), market.c_str(), beginDate, endDate, &_shareGrp, shareCount));
	OUTPUT(ret);
	OUTPUT(shareCount);
	fstream file("indexData.txt", ios::out);
	for (int i = 0; i < shareCount; ++i)
	{
		file << _shareGrp[i].code << "." << _shareGrp[i].market << "," << _shareGrp[i].date << endl;
	}
	file.close();
	sdk->releaseShareGroup(_shareGrp);
	cout << "结果已保存至indexData.txt" << endl;
}

void testgetHistoryByOffset(HsQuantDataSDKInterface* sdk)
{
	string mkt, code;
	int queryDate, barCount, iFrequency, iRight, fillFlag, recordNum;
	GET(mkt);
	GET(code);
	GET(queryDate);
	GET(barCount);
	GET(iFrequency);
	GET(iRight);
	GET(fillFlag);
	if (0 == barCount)
		return;
	int count = barCount;
	if (count < 0)
		count *= -1;
	double* klineDatas = new double[13 * count];
	unsigned int* dfIndex = new unsigned int[count];
	int ret;
	COUNT_TIME(ret, sdk->getHistoryByOffset(mkt.c_str(), code.c_str(), queryDate, barCount, iFrequency,
		iRight, recordNum, klineDatas, dfIndex, nullptr, nullptr));
	OUTPUT(ret);
	OUTPUT(recordNum);
	if (0 == ret)
	{
		int showResult;
		GET(showResult);
		if (1 == showResult)
		{
			for (int i = 0; i < recordNum; ++i)
			{
				formatDatas(klineDatas, dfIndex, i);
			}
		}
	}
	delete[] klineDatas;
	delete[] dfIndex;
}

int main()
{
	Cfg _cfg;
	if (!_cfg.ok)
	{
		return -1;
	}
	SDKOption* opt = CreateSDKOption();
	opt->setHQConnectInfo(_cfg.hq_ip.c_str(), _cfg.hq_port);
	opt->setHQhttpDomain(_cfg.hq_http.c_str());
	opt->setInfoNetDomain(_cfg.info_net.c_str());
	HsQuantDataSDKInterface* sdk = CreateQuantSDK(opt);
	unsigned char flag(0);
	while (true)
	{
		cout << "--------------------------------" << endl;
		cout << "0.getHistoryByOffset" << endl;
		cout << "1.getHistoryByDateSize" << endl;
		cout << "2.getHistoryByDate" << endl;
		cout << "3.getLocalHistoryByDate" << endl;
		cout << "4.getLocalHistoryByDateSize" << endl;
		cout << "5.getLocalHistoryByOffset" << endl;
		cout << "6.downLoadHistoryData" << endl;
		cout << "7.deleteLocalStockData" << endl;
		cout << "8.getLocalStorageStockInfo" << endl;
		cout << "9.getStorageStockInfo" << endl;
		cout << "a.setStorageConfig" << endl;
		cout << "b.getStorageConfig" << endl;
		cout << "c.getMarketCodeList" << endl;
		cout << "d.getIndexData" << endl;
		cout << "p.getLocalStorageStockInfoOneCode" << endl;
		cout << "--------------------------------" << endl;
		cin >> flag;
		switch (flag)
		{
		case '0':
			testgetHistoryByOffset(sdk);
			break;
		case '1':
			testgetHistoryByDateSize(sdk);
			break;
		case '2':
			testgetHistoryByDate(sdk);
			break;
		case '3':
			testgetLocalHistoryByDate(sdk);
			break;
		case '4':
			testgetLocalHistoryByDateSize(sdk);
			break;
		case '5':
			testgetLocalHistoryByOffset(sdk);
			break;
		case '6':
			testdownLoadHistoryData(sdk);
			break;
		case '7':
			testdeleteLocalStockData(sdk);
			break;
		case '8':
			testgetLocalStorageStockInfo(sdk);
			break;
		case '9':
			testgetStorageStockInfo(sdk);
			break;
		case 'a':
			testsetStorageConfig(sdk);
			break;
		case 'b':
			testgetStorageConfig(sdk);
			break;
		case 'c':
			testgetMarketCodeList(sdk);
			break;
		case 'd':
			testgetIndexData(sdk);
			break;
		case 'p':
			testgetLocalStorageStockInfoOneCode(sdk);
			break;
		default:
			break;
		}
	}
	return 0;
}

int main11()
{
// 	int a = 0;
// 	int b = 3;
// 	int c = b / a;
	SDKOption* opt = CreateSDKOption();
	opt->setHQConnectInfo("192.168.46.217", 9602);
	opt->setHQhttpDomain("http://10.20.135.140:8081");
	opt->setInfoNetDomain("http://10.20.135.125:8080");
	HsQuantDataSDKInterface* sdk = CreateQuantSDK(opt);
/*
	CodeInfo* _codeInfo;
	int count = 0; 
	auto start = chrono::system_clock::now();
	sdk->getMarketCodeList("XSHG", 20180101, 20180102, &_codeInfo, count);
	auto end = chrono::system_clock::now();
	auto duration = duration_cast<microseconds>(end - start);
	cout << double(duration.count()) / microseconds::period::den << endl;
	CodeInfo* tmp;
	for (int i = 0; i < count; ++i)
	{
		tmp = &_codeInfo[i];
	}
	ShareGroup* share;
	int ret = sdk->getIndexData("399001", "XSHE", 20180313, 20180315, &share, count);
	sdk->releaseShareGroup(share);*/
	//Sleep(5000);
	int startDate, endDate;
	long long r = sdk->getStorageStockInfo("XSHG", 6, startDate, endDate);
	r = sdk->getLocalStorageStockInfo("XSHG", 7, startDate, endDate);
	return 0;
}