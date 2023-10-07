#include "frame/logger.h"

#include <spdlog/sinks/basic_file_sink.h>

namespace frame
{

Logger::Logger()
{
    logger_ptr_ = spdlog::basic_logger_mt("log", "./log.txt", true);
    logger_ptr_->info("start logging!");
}

Logger::~Logger() = default;

Logger& Logger::GetInstance()
{
    static Logger logger_;
    return logger_;
}

const std::shared_ptr<spdlog::logger> Logger::operator->() const
{
    return logger_ptr_;
}

} // End namespace frame.
