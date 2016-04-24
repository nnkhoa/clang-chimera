/******************************************************************************
 * @file   Log.cpp
 * @author Federico Iannucci
 * @date   28 ott 2015
 * @brief  Easyloggingcpp wrapper, ChimeraLogger implementation.
 ******************************************************************************/

#include "Log.h"

using namespace chimera;

INITIALIZE_EASYLOGGINGPP

const char* chimera::log::ChimeraLogger::loggerName = "chimeraLogger";  ///< Member initialization
el::Configurations chimera::log::ChimeraLogger::configurator =
    el::Configurations();
log::VerboseLevel chimera::log::ChimeraLogger::actualVLevel = 0;

void chimera::log::ChimeraLogger::init() {
  /// Configure el++ : chimeraLogger
  configurator.setGlobally(el::ConfigurationType::Enabled, "false");
  configurator.setGlobally(el::ConfigurationType::ToFile, "false");
  configurator.set(el::Level::Error, el::ConfigurationType::ToStandardOutput,
                   "true");
  // Debug level
  configurator.set(el::Level::Debug, el::ConfigurationType::Enabled, "true");
  configurator.set(el::Level::Debug, el::ConfigurationType::Format,
                   "[CHIMERA DEBUG] %msg");

  // Warning Level
  configurator.set(el::Level::Info, el::ConfigurationType::Enabled, "true");
  configurator.set(el::Level::Info, el::ConfigurationType::Format,
                   "[CHIMERA INFO] %msg");

  // Warning Level
  configurator.set(el::Level::Warning, el::ConfigurationType::Enabled, "true");
  configurator.set(el::Level::Warning, el::ConfigurationType::Format,
                   "[CHIMERA WARNING] %msg");

  // Error Level
  configurator.set(el::Level::Error, el::ConfigurationType::Enabled, "true");
  configurator.set(el::Level::Error, el::ConfigurationType::Format,
                   "[CHIMERA ERROR] %msg");
  // Fatal Level
  configurator.set(el::Level::Fatal, el::ConfigurationType::Enabled, "true");
  ;
  configurator.set(el::Level::Fatal, el::ConfigurationType::Format,
                   "[CHIMERA FATAL] %msg");
  el::Loggers::reconfigureLogger(loggerName, configurator);
}

void chimera::log::ChimeraLogger::initVerbose() {
  // Verbose Level
  configurator.set(el::Level::Verbose, el::ConfigurationType::Enabled, "true");
  configurator.set(el::Level::Verbose, el::ConfigurationType::ToFile, "false");
  configurator.set(el::Level::Verbose, el::ConfigurationType::ToStandardOutput,
                   "true");
  configurator.set(el::Level::Verbose, el::ConfigurationType::Format,
                   "[CHIMERA VERBOSE-%vlevel] %msg");
  el::Loggers::reconfigureLogger(loggerName, configurator);
}

void chimera::log::ChimeraLogger::initTrace() {
  // Init the verbose level
  initVerbose();
  // Debug Level
  configurator.set(el::Level::Trace, el::ConfigurationType::Enabled, "true");
  configurator.set(el::Level::Trace, el::ConfigurationType::ToFile, "false");
  configurator.set(el::Level::Trace, el::ConfigurationType::ToStandardOutput,
                   "true");
  configurator.set(el::Level::Trace, el::ConfigurationType::Format,
                   "[CHIMERA TRACE] %msg");
  el::Loggers::reconfigureLogger(loggerName, configurator);
}

void chimera::log::ChimeraLogger::init(int argc, const char** argv) {
  START_EASYLOGGINGPP(argc, argv);
  init();
}

void chimera::log::ChimeraLogger::verbose(VerboseLevel vlevel,
                                          const std::string& msg) {
  // Check if the logger has been registered
  if (!isInitialize())
    return;
  // Check the verbose level
  if (!VLOG_IS_ON(vlevel))
    return;
  std::string message = "";
  for (unsigned char i = 0; i < vlevel; i++) {
    message += "=";
  }
  message += "> " + msg;
  CVLOG(vlevel,loggerName) << message;
}
