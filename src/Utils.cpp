/******************************************************************************
 * @file   Utils.cpp
 * @author Federico Iannucci
 * @date   24 ott 2015
 * @brief  Utils implementation.
 ******************************************************************************/

#include "Utils.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/Path.h"

#include <sstream>
#include <fstream>

using namespace std;
using namespace llvm;

using StringVector = std::vector<std::string>;

bool chimera::conf::readFunctionsOperatorsConfFile(
    const std::string& filename, std::map< ::std::string, StringVector> &fileMap) {
  // Key
  ::std::string function;
  ::std::vector<std::string> operators;
  // Open the file
  fstream confFile(filename);
  if (!confFile.good()) {
    return false;
  }
  // Read line by line
  std::string line;
  std::getline(confFile, line);  // Read the first line
  while (confFile.good()) {
    // Clear vector
    operators.clear();
    // Read tokens delimited by ','
    stringstream lineTokens(line);
    std::string op;
    // Read first token -> it's the key of the entry
    std::getline(lineTokens, function, ',');
    // Check if it's a comment
    if (function.substr(0, 2) != "//") {
      while (lineTokens.good()) {
        std::getline(lineTokens, op, ',');
        // Populate the vector
        operators.push_back(op);
      }
      // Create map entry
      fileMap.insert(std::pair<std::string, StringVector>(function, operators));
    }
    std::getline(confFile, line);  // Read a new line
  }
  return true;
}

// Filesystem
const char chimera::fs::pathSep = getPathSeparator();

char chimera::fs::getPathSeparator() {
  return *(sys::path::get_separator().data());
}

std::string chimera::fs::getParentPath(const std::string& path) {
  // Use this trick because llvm::sys::path::parent_path doesn't work
  std::size_t found = path.find_last_of(pathSep);
  return path.substr(0, found);
}

bool chimera::fs::createDirectories(const llvm::Twine& path,
                                    bool ignoreExisting) {
  // Create directories
  sys::fs::create_directories(path, ignoreExisting);
  return sys::fs::is_directory(path);
}
