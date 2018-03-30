#include "cfg.h"
#include <stdio.h>
#include <Include/rapidjson/document.h>

#define GET_STRING_VALUE(doc, key) \
if (doc.HasMember(#key)) key = doc[#key].GetString();
#define GET_INT_VALUE(doc, key) \
if (doc.HasMember(#key)) key = doc[#key].GetInt();

Cfg::Cfg()
{
	hq_port = 9601;
	ok = false;
	FILE *cfgFile = fopen("cfg.json", "rb");
	if (nullptr != cfgFile)
	{
		fseek(cfgFile, 0, SEEK_END);
		long fileSize = ftell(cfgFile);
		fseek(cfgFile, 0, SEEK_SET);
		char buf[10240] = { 0 };
		fread(buf, fileSize, 1, cfgFile);
		rapidjson::Document doc;
		doc.Parse<rapidjson::kParseDefaultFlags>(buf);
		if (!doc.HasParseError())
		{
			GET_STRING_VALUE(doc, hq_ip);
			GET_STRING_VALUE(doc, hq_http);
			GET_STRING_VALUE(doc, info_net);
			GET_INT_VALUE(doc, hq_port);
			ok = true;
		}
		fclose(cfgFile);
	}
}