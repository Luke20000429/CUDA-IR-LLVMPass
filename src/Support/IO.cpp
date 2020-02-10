#include <kerma/Support/IO.h>
#include <boost/filesystem.hpp>
#include <boost/filesystem/directory.hpp>
#include <fstream>
#include <ios>
#include <stdexcept>
#include <system_error>


std::string kerma::IOErrToString(IOErr &err)
{
  switch(err) {
    case IO_SUCCESS:
      return "Success";
    case IO_NOT_FOUND:
      return "Not found";
    case IO_IS_DIR:
      return "Is a directory";
    case IO_INTERNAL_RD_ERR:
    case IO_INTERNAL_WR_ERR:
      return "Internal error";
    default:
      return "";
  }
}

std::string
kerma::readFile(const std::string &path)
{
  std::error_code err;
  std::string res = readFile(path, err);
  if ( err.value() == IOErr::IO_SUCCESS)
    return res;
  throw std::runtime_error("Error reading file: " + path);
}

std::string
kerma::readFile(const std::string &path, std::error_code& err)
{ 
  if ( !boost::filesystem::exists(path)) {
    err.assign(IO_NOT_FOUND, std::generic_category());
    return "";
  }
  
  if ( !boost::filesystem::is_directory(path)) {
    err.assign(IO_IS_DIR, std::generic_category());
    return "";
  }

  std::ifstream ifs(path, std::ios::binary);
  if ( ifs.is_open()) {
    try {
      ifs.seekg(0, ifs.end);
      size_t fSize = ifs.tellg();
      ifs.seekg(0, ifs.beg);
      std::string contents;
      contents.resize(fSize);
      ifs.read(const_cast<char*>(contents.data()), fSize);
      ifs.close();
      return contents;
    } catch ( ... ) {
      ifs.close();
      err.assign(IO_INTERNAL_RD_ERR, std::generic_category());
      return "";
    }
  }

  err.assign(IO_INTERNAL_RD_ERR, std::generic_category()));
  return "";
}
