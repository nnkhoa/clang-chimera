/******************************************************************************
 * @file   Log.h
 * @author Federico Iannucci
 * @date   28 ott 2015
 * @brief  Chimera logging facility
 ******************************************************************************/
#ifndef SRC_INCLUDE_LOG_H_
#define SRC_INCLUDE_LOG_H_

#define ELPP_NO_DEFAULT_LOG_FILE            ///< Disable default logs folder.
#define ELPP_DISABLE_DEFAULT_CRASH_HANDLING ///< Disable crash handling
#include "lib/easylogging++.h"

namespace chimera {
namespace log {
  // FIXME Delete this intermediate class?
// Typedefs
using VerboseLevel = el::base::type::VerboseLevel;
/// @brief ChimeraLogger Class, provides all functions for the logging
/// @details Through this helper class, it's simpler to use easylogging++.
class ChimeraLogger {
public:
  /// @brief Initialize the logging class
  static void init();
  /// @brief Enable the verbose level
  static void initVerbose();
  /// @brief Enable the debug level
  static void initTrace();
  static void init(int argc, const char **argv);

  static bool isInitialize() {
    return el::Loggers::getLogger(loggerName, false);
  }

  static void setVerboseLevel(VerboseLevel vlevel) {
    el::Loggers::setVerboseLevel(vlevel);
  }

  static void setActualVLevel(VerboseLevel vlevel) {
    actualVLevel = vlevel >= 0 && vlevel <= 9 ? vlevel : 0;
  }
  static void resetActualVLevel() { actualVLevel = 0; }
  static void incrActualVLevel() {
    actualVLevel = actualVLevel < 9 ? actualVLevel + 1 : 9;
  }

  static void decrActualVLevel() {
    actualVLevel = actualVLevel > 0 ? actualVLevel - 1 : 0;
  }

  /// @brief Log a message for the Verbose level
  ///
  /// @details Verbose levels are from 0 to 9
  static void verbose(VerboseLevel, const std::string &msg);

  /// @brief Log a message in Verbose actual level

  static void verbose(const std::string &msg) { verbose(actualVLevel, msg); }

  static void verbose(const std::string &&msg) { verbose(actualVLevel, msg); }

  static void verboseAndIncr(const std::string &msg) {
    verbose(msg);
    incrActualVLevel();
  }

  static void verboseAndIncr(const std::string &&msg) {
    verbose(msg);
    incrActualVLevel();
  }

  static void verboseAndDecr(const std::string &msg) {
    verbose(msg);
    decrActualVLevel();
  }

  static void verboseAndDecr(const std::string &&msg) {
    verbose(msg);
    decrActualVLevel();
  }

  static void verbosePreDecr(const std::string &msg) {
    decrActualVLevel();
    verbose(msg);
  }

  static void verbosePreDecr(const std::string &&msg) {
    decrActualVLevel();
    verbose(msg);
  }

  static void debug(const std::string &msg) { CLOG(DEBUG, loggerName) << msg; }

  static void debug(const std::string &&msg) { CLOG(DEBUG, loggerName) << msg; }

  static void info(const std::string &msg) { CLOG(INFO, loggerName) << msg; }

  static void info(const std::string &&msg) { CLOG(INFO, loggerName) << msg; }

  static void trace(const std::string &msg) { CLOG(TRACE, loggerName) << msg; }

  static void trace(const std::string &&msg) { CLOG(TRACE, loggerName) << msg; }

  static void warning(const std::string &msg) {
    CLOG(WARNING, loggerName) << msg;
  }

  static void warning(const std::string &&msg) {
    CLOG(WARNING, loggerName) << msg;
  }

  static void error(const std::string &msg) { CLOG(ERROR, loggerName) << msg; }

  static void error(const std::string &&msg) { CLOG(ERROR, loggerName) << msg; }

  static void fatal(const std::string &msg) { CLOG(FATAL, loggerName) << msg; }

  static void fatal(const std::string &&msg) { CLOG(FATAL, loggerName) << msg; }

private:
  static const char *loggerName;
  static el::Configurations configurator;
  static VerboseLevel actualVLevel;
};
}
}

#endif /* SRC_INCLUDE_LOG_H_ */
