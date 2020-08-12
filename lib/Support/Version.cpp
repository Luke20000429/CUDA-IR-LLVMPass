//===- Version.cpp ----------------------------------------------*- C++ -*-===//
//
// Part of the Kerma Project, under the Apache License v2.0
//
//===----------------------------------------------------------------------===//
//
// This file defines version-related functions for Kerma.
//
//===----------------------------------------------------------------------===//

#include "kerma/Support/Version.h"

namespace kerma {

std::string getVersionMajor() {
#ifdef KERMA_VERSION_MAJOR
  return KERMA_VERSION_MAJOR;
#else
  return ""
#endif
}

std::string getVersionMinor() {
#ifdef KERMA_VERSION_MINOR
  return KERMA_VERSION_MINOR;
#else
  return ""
#endif
}

std::string getVersionPatch() {
#ifdef KERMA_VERSION_PATCH
  return KERMA_VERSION_PATCH;
#else
  return ""
#endif 
}

std::string getVersion() {
  auto minor = getVersionMinor();
  auto patch = getVersionPatch();
  return getVersionMajor() 
    + (minor.size() > 0 ? "." + minor : "")
    + (patch.size() > 0 ? "." + patch : "");
}

} // end namespace kerma