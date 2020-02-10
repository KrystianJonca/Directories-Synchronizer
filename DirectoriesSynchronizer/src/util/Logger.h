#pragma once
#include <memory>
#include "spdlog/spdlog.h"

class Logger
{
public:
	static void Init();

	inline static std::shared_ptr<spdlog::logger>& GetLogger() { return s_Logger; }
private:
	static std::shared_ptr<spdlog::logger> s_Logger;
};

#ifdef _DEBUG 
	#define LOG_DEBUG(...)    Logger::GetLogger()->debug(__VA_ARGS__)
	#define LOG_INFO(...)     Logger::GetLogger()->info(__VA_ARGS__)
	#define LOG_WARN(...)     Logger::GetLogger()->warn(__VA_ARGS__)
	#define LOG_ERROR(...)    Logger::GetLogger()->error(__VA_ARGS__)
	#define LOG_CRITICAL(...) Logger::GetLogger()->critical(__VA_ARGS__)
#else
	#define LOG_DEBUG(...)   
	#define LOG_INFO(...)    
	#define LOG_WARN(...)    
	#define LOG_ERROR(...)  
	#define LOG_CRITICAL(...)
#endif 