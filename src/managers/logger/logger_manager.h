

#pragma once

#include <chrono>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <sstream>
#include <string>

enum class LogLevel { INFO, WARNING, ERROR, FATAL };

class LoggerManager {
public:
  /**
   * Get the singleton instance of LoggerManager.
   */
  static LoggerManager *get() {
    static std::unique_ptr<LoggerManager> instance =
        std::make_unique<LoggerManager>();
    return instance.get();
  }

  /**
   * Initialize the logger and create a new log file for the current session.
   */
  static void init() {
    auto *logger = get();

    logger->_log_path = "../cache/log_" + logger->get_current_time() + ".txt";

    std::stringstream ss;
    ss << "Session started." << std::endl;

    logger->log(LogLevel::INFO, ss.str());
  }

  /**
   * Clean up and close the logger.
   */
  void clean() {
    _log_stream.close();
    _log_path.clear();
  }

  /**
   * Log a message with the specified log level.
   * @param level The log level.
   * @param object The object to log.
   */
  template <typename T> static void log(LogLevel level, const T &object) {
    auto *logger = get();

    std::lock_guard<std::mutex> lock(logger->_log_mutex);

    std::stringstream ss;
    ss << "[" << logger->get_current_time() << "] ";
    ss << "[" << logger->get_log_level_string(level) << "] ";
    ss << object << std::endl;

    std::cout << logger->get_log_level_color(level) << ss.str() << "\033[0m";

    logger->_log(level, ss);
  }

  /**
   * Log an error message.
   * @param object The object to log as an error.
   */
  template <typename T> static void log_error(const T &object) {
    log(LogLevel::ERROR, object);
  }

  /**
   * Log a fatal error message.
   * @param object The object to log as a fatal error.
   */
  template <typename T> static void log_fatal(const T &object) {
    log(LogLevel::FATAL, object);
  }

  /**
   * Log an informational message.
   * @param object The object to log as information.
   */
  template <typename T> static void log_info(const T &object) {
    log(LogLevel::INFO, object);
  }

  /**
   * Log a warning message.
   * @param object The object to log as a warning.
   */
  template <typename T> static void log_warning(const T &object) {
    log(LogLevel::WARNING, object);
  }

private:
  /**
   * Get the current time as a formatted string.
   * @return The current time as a string.
   */
  std::string get_current_time() {
    auto now = std::chrono::system_clock::now();
    auto now_time_t = std::chrono::system_clock::to_time_t(now);
    auto now_tm = std::localtime(&now_time_t);

    std::stringstream ss;
    ss << std::put_time(now_tm, "%Y-%m-%d %H:%M:%S");
    return ss.str();
  }

  /**
   * Get the string representation of a log level.
   * @param level The log level.
   * @return The string representation of the log level.
   */
  std::string get_log_level_string(LogLevel level) {
    switch (level) {
    case LogLevel::INFO:
      return "INFO";
    case LogLevel::WARNING:
      return "WARNING";
    case LogLevel::ERROR:
      return "ERROR";
    case LogLevel::FATAL:
      return "FATAL";
    }
    return "UNKNOWN";
  }

  std::string get_log_level_color(LogLevel level) {
    switch (level) {
    case LogLevel::INFO:
      return "\033[0;34m";
    case LogLevel::WARNING:
      return "\033[0;33m";
    case LogLevel::ERROR:
      return "\033[0;31m";
    case LogLevel::FATAL:
      return "\033[0;31m";
    }
    return "\033[0m";
  }

  /**
   * Internal log function to handle writing logs to file and flushing the
   * stream.
   * @param level The log level.
   * @param final_stream The final log message stream.
   */
  void _log(LogLevel level, const std::stringstream &final_stream) {
    auto *logger = get();

    // Write to file
    if (!logger->_log_path.empty()) {
      if (std::filesystem::exists(logger->_log_path)) {
        logger->_log_stream.open(logger->_log_path, std::ios_base::app);
      } else {
        logger->_log_stream.open(logger->_log_path, std::ios_base::out);
      }

      logger->_log_stream << final_stream.str();

      if (level == LogLevel::FATAL) {
        logger->_log_stream << "=== Session ended abnormally ===" << std::endl;
        std::cout << logger->get_log_level_color(level)
                  << "=== Session ended abnormally ===" << std::endl
                  << "\033[0m";
      }

      logger->_log_stream.close();
    }

    logger->_log_stream.flush();

    if (level == LogLevel::FATAL) {
      // add a message to the log file that the session ended
      logger->clean();
      exit(EXIT_FAILURE);
    }
  }

  std::mutex _log_mutex;
  std::ofstream _log_stream;
  std::filesystem::path _log_path = "../cache/log.txt";
};
