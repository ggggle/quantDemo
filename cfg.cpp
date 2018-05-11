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
		fread(buf, fileSize > sizeof buf ? sizeof buf : fileSize, 1, cfgFile);
		rapidjson::Document doc;
		doc.Parse<rapidjson::kParseDefaultFlags>(buf);
		if (!doc.HasParseError())
		{
			GET_STRING_VALUE(doc, hq_ip);
			GET_STRING_VALUE(doc, hq_http);
			GET_STRING_VALUE(doc, info_net);
			GET_STRING_VALUE(doc, app_key);
			GET_STRING_VALUE(doc, app_secret);
			GET_STRING_VALUE(doc, user_name);
			GET_STRING_VALUE(doc, user_pswd);
			GET_INT_VALUE(doc, hq_port);
			ok = true;
		}
		else
		{
			printf("≈‰÷√Œƒº˛¥ÌŒÛ\n");
		}
		fclose(cfgFile);
	}
}