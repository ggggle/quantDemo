#include "export.h"
#include <iostream>
#include <chrono>
#include "cfg.h"
#include "callBack.h"


#pragma comment(lib, "legacy_stdio_definitions.lib")

#define GET(arg) \
cout<<#arg##":";\
cin>>arg;

#define OUTPUT(arg) \
cout<<"###"<<#arg##":"<<arg<<endl;

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
	int ret = sdk->getHistoryByDateSize(mkt.c_str(), code.c_str(), startDate, endDate, frequency, fillFlag, recordNum);
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
	int ret = sdk->getHistoryByDate(mkt.c_str(), code.c_str(), startDate, endDate, frequency, right, fillFlag, recordNum, klineDatas, dfIndex, nullptr, nullptr);
	OUTPUT(ret);
	OUTPUT(recordNum);
	int showResult;
	GET(showResult);
	if (1 == showResult)
	{
		for (int i = 0; i < recordNum; ++i)
		{
			formatDatas(klineDatas, dfIndex, i);
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
	int ret = sdk->getLocalHistoryByDateSize(mkt.c_str(), code.c_str(), startDate, endDate, frequency, recordNum);
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
	int ret = sdk->getLocalHistoryByDate(mkt.c_str(), code.c_str(), startDate, endDate, frequency, right, klineDatas, dfIndex);
	OUTPUT(ret);
	OUTPUT(recordNum);
	int showResult;
	GET(showResult);
	if (1 == showResult)
	{
		for (int i = 0; i < recordNum; ++i)
		{
			formatDatas(klineDatas, dfIndex, i);
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
	if (barCount <= 0)
		return;
	double* klineDatas = new double[13 * barCount];
	unsigned int* dfIndex = new unsigned int[barCount];
	int ret = sdk->getLocalHistoryByOffset(mkt.c_str(), code.c_str(), queryDate, barCount, iFrequency, iRight, fillFlag, recordNum, klineDatas, dfIndex);
	OUTPUT(ret);
	OUTPUT(recordNum);
	int showResult;
	GET(showResult);
	if (1 == showResult)
	{
		for (int i = 0; i < recordNum; ++i)
		{
			formatDatas(klineDatas, dfIndex, i);
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
	int ret = sdk->downLoadHistoryData(mkt.c_str(), frequency, startDate, endDate, myCB, nullptr);
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
	int ret = sdk->deleteLocalStockData(_codeInfo, codeCount);
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
	GET(nStartDate);
	GET(nEndDate);
	int size = sdk->getLocalStorageStockInfo(pMarket.c_str(), nFrequency, nStartDate, nEndDate);
	OUTPUT(size);
}

void testgetStorageStockInfo(HsQuantDataSDKInterface* sdk)
{
	string pMarket;
	int nFrequency, nStartDate, nEndDate;
	GET(pMarket);
	GET(nFrequency);
	GET(nStartDate);
	GET(nEndDate);
	int size = sdk->getStorageStockInfo(pMarket.c_str(), nFrequency, nStartDate, nEndDate);
	OUTPUT(size);
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
		cout << "--------------------------------" << endl;
		cin >> flag;
		switch (flag)
		{
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
		default:
			break;
		}
	}
	return 0;
}

/*
int main11()
{
	SdkCB* m_sdkCB = new SdkCB("any", "any");
	SDKOption* opt = CreateSDKOption();
	opt->setHQConnectInfo("192.168.46.217", 9601);
	opt->setHQhttpDomain("http://10.20.135.140:8081");
	opt->setInfoNetDomain("http://10.20.135.125:8080");
	opt->setSDKCallback(m_sdkCB);
	HsQuantDataSDKInterface* sdk = CreateQuantSDK(opt);
	
// 	Session* m_session = sdk->getSdkSession();
// 	IHsCommMessage* msg = m_session->CreateMessage(BIZ_H5PROTO, H5SDK_SERVER_INFO, REQUEST);
// 	IHsCommMessage* rsp = NULL;
// 	int ret = m_session->SyncCall(msg, &rsp, 1000);
// 	//int ret = m_session->AsyncSend(msg);
// 	msg->Release();
// 	const char* svr_name = rsp->GetItem(H5SDK_TAG_SERVER_NAME)->GetString();
	//sdk->downLoadHistoryData("XSHG", 6, 0, 99991231, NULL, NULL);
	CodeInfo* _codeInfo;
	int count = 0; 
	auto start = chrono::system_clock::now();
	sdk->getMarketCodeList("XSHG", 20180101, 20180102, &_codeInfo, count);
	auto end = chrono::system_clock::now();
	auto duration = duration_cast<microseconds>(end - start);
	cout << double(duration.count()) / microseconds::period::den << endl;
	int recordNum = 0;
	double klineDatas[256] = { 0 };
	unsigned int dfIndex[8] = { 0 };
	int r = sdk->getHistoryByDate("XSHG", "600570", 20180101, 20180106, 6, 2, 1, recordNum, klineDatas, dfIndex, 0, 0);
// 	CodeInfo* tmp;
// 	for (int i = 0; i < count; ++i)
// 	{
// 		tmp = &_codeInfo[i];
// 	}
	ShareGroup* share;
	int ret = sdk->getIndexData("399001", "XSHE", 20180313, 20180315, &share, count);
	sdk->releaseShareGroup(share);
	return 0;
}*/