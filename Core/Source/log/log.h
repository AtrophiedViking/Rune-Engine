#pragma once
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include <stdio.h>

#define RUNE_LOGGER "Rune"

#ifndef RELEASE
#define RUNE_TRACE(...) if(spdlog::get(RUNE_LOGGER)) {spdlog::get(RUNE_LOGGER)->trace(__VA_ARGS__);}
#define RUNE_INFO(...)  if(spdlog::get(RUNE_LOGGER)) {spdlog::get(RUNE_LOGGER)->info(__VA_ARGS__);}
#define RUNE_DEBUG(...) if(spdlog::get(RUNE_LOGGER)) {spdlog::get(RUNE_LOGGER)->debug(__VA_ARGS__);}
#define RUNE_WARN(...) if(spdlog::get(RUNE_LOGGER)) {spdlog::get(RUNE_LOGGER)->warn(__VA_ARGS__);}
#define RUNE_ERROR(...) if(spdlog::get(RUNE_LOGGER)) {spdlog::get(RUNE_LOGGER)->error(__VA_ARGS__);}
#define RUNE_FATAL(...) if(spdlog::get(RUNE_LOGGER)) {spdlog::get(RUNE_LOGGER)->critical(__VA_ARGS__);}
#define RUNE_ASSERT(x, msg) if((x)){} else { RUNE_FATAL("Assertion Failed: {}\n\t{}\n\tin file: {}\n\ton line: {}", #x, msg,__FILE__,__LINE__);}
#else
#define RUNE_TRACE(...) (void)0
#define RUNE_INFO(...) (void)0
#define RUNE_DEBUG(...) (void)0
#define RUNE_WARN(...) (void)0
#define RUNE_ERROR(...) (void)0
#define RUNE_FATAL(...) (void)0
#define RUNE_ASSERT(...) (void)0
#endif

namespace Rune
{
	class Logger
	{
	public:
		Logger(std::string name = "Rune");
		~Logger();
	private:
		std::shared_ptr<spdlog::logger> m_Logger;

	};
}