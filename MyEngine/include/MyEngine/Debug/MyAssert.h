#pragma once
#include <windows.h>
#include <cassert>
#include <string>
#include <filesystem>
#include <shellapi.h>
#include "MyEngine/Debug/GameNotification.h"
#include "MyEngine/Log/LogManager.h"
#include "MyEngine/Utils/ConvertString.h"

#define MY_ASSERT(condition)                                                                                                                                                                           \
	if (!(condition)) {                                                                                                                                                                                \
		std::string myAssert_logPath = LogManager::GetLogFilePath();                                                                                                                                   \
		std::wstring myAssert_absoluteLogPath = std::filesystem::absolute(myAssert_logPath).wstring();                                                                                                 \
		std::wstring myAssert_command = L"/c code \"" + myAssert_absoluteLogPath + L"\"";                                                                                                              \
		ShellExecuteW(nullptr, L"open", L"cmd.exe", myAssert_command.c_str(), nullptr, SW_HIDE);                                                                                                       \
		GameNotification::Send("Assert失敗", "条件: " + std::string(#condition));                                                                                                                      \
		assert(condition);                                                                                                                                                                                 \
	}