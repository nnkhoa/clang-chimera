//===- Log.cpp --------------------------------------------------*- C++ -*-===//
//
//  Copyright (C) 2015, 2016  Federico Iannucci (fed.iannucci@gmail.com)
// 
//  This file is part of Clang-Chimera.
//
//  Clang-Chimera is free software: you can redistribute it and/or modify
//  it under the terms of the GNU Affero General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  Clang-Chimera is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU Affero General Public License for more details.
//
//  You should have received a copy of the GNU Affero General Public License
//  along with Clang-Chimera. If not, see <http://www.gnu.org/licenses/>.
//
//===----------------------------------------------------------------------===//
/// \file Log.cpp
/// \author Federico Iannucci
/// \brief This file implements a logging facility
//===----------------------------------------------------------------------===//

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
