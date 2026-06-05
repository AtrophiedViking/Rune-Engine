#include "log.h"

namespace Rune
{
	Logger::Logger(std::string name)
		: m_Logger(spdlog::stdout_color_mt(name))
	{
		m_Logger->set_pattern("\033[90m[\033[32m%T.%e\033[90m] [%^%l%$] \033[33m%v\033[90m\n");
		m_Logger->set_level(spdlog::level::trace);
	}
	Logger::~Logger()
	{
		spdlog::drop(RUNE_LOGGER);
	}
}