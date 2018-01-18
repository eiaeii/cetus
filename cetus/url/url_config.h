#pragma once

#include "cetus_define.h"

namespace terra
{
	struct FUrlConfig
	{
		std::string DefaultProtocol;
		std::string DefaultName;
		std::string DefaultHost;
		std::string DefaultPortal;
		std::string DefaultSaveExt;
		int32_t DefaultPort;

		/**
		* Initialize with defaults from ini
		*/
		void Init()
		{
			/*
			DefaultProtocol = GConfig->GetStr(TEXT("URL"), TEXT("Protocol"), GEngineIni);
			DefaultName = GConfig->GetStr(TEXT("URL"), TEXT("Name"), GEngineIni);
			// strip off any file extensions from map names
			DefaultHost = GConfig->GetStr(TEXT("URL"), TEXT("Host"), GEngineIni);
			DefaultPortal = GConfig->GetStr(TEXT("URL"), TEXT("Portal"), GEngineIni);
			DefaultSaveExt = GConfig->GetStr(TEXT("URL"), TEXT("SaveExt"), GEngineIni);

			FString Port;
			// Allow the command line to override the default port
			if (FParse::Value(FCommandLine::Get(), TEXT("Port="), Port) == false)
			{
				Port = GConfig->GetStr(TEXT("URL"), TEXT("Port"), GEngineIni);
			}
			DefaultPort = FCString::Atoi(*Port);
			*/
		}

		/**
		* Reset state
		*/
		void Reset()
		{
			DefaultProtocol = "";
			DefaultName = "";
			DefaultHost = "";
			DefaultPortal = "";
			DefaultSaveExt = "";
		}
	};
}