#include "gistmaker.hpp"
#include "cpr/cprtypes.h"
#include "cpr/response.h"
#include "cpr/status_codes.h"
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>

namespace pr {

auto getenv_or_die(const char *key) -> std::string
{
  const char *val = std::getenv(key);
  if (val == nullptr) {
    std::cerr << "[Error]: '" << key << "' environment variable is not set!\n";
    std::exit(EXIT_FAILURE);
  }
  return std::string{ val };
}

void GistMaker::add_file(const std::filesystem::path &filepath) { m_files.emplace_back(read_file(filepath)); }

auto GistMaker::read_file(const std::filesystem::path &filepath) -> FileNameContentPair
{
  if (!std::filesystem::is_regular_file(filepath)) {
    std::cerr << "Error: " << filepath << " is not a regular file\n";
    std::exit(EXIT_FAILURE);
  }
  auto inf = std::ifstream{ filepath };
  if (!inf) {
    std::cerr << "Error: " << filepath << " can not be opened\n";
    std::exit(EXIT_FAILURE);
  }
  auto strm = std::ostringstream{};
  while (inf) {
    auto c = inf.get();
    if (c == EOF) { break; }
    switch (c) {
    case '\n':
      strm << '\\' << 'n';
      break;
    case '\r':
      strm << '\\' << 'r';
      break;
    case '"':
      strm << '\\' << '"';
      break;
    case '\t':
      strm << '\\' << 't';
      break;
    case '\f':
      strm << '\\' << 'f';
      break;
    case '\\':
      strm << '\\' << '\\';
      break;
    default:
      strm << (char)c;
    }
  }

  return { filepath.filename(), strm.str() };
}

auto GistMaker::auth_header() const -> cpr::Header
{
  return cpr::Header{ { "Authorization", "Bearer " + m_authtoken } };
}

auto GistMaker::create_gist(std::string &&gist_desc, bool pub) const -> cpr::Response
{
  auto strm = std::ostringstream{};
  strm << R"({"description": ")" << gist_desc << R"(", "public": )" << (pub ? "true" : "false") << ", ";

  strm << "\"files\": { ";
  for (auto i = 0UL; i < m_files.size(); ++i) {
    const auto &[filename, filecontent] = m_files.at(i);
    strm << "\"" << filename << R"(": { "content": ")" << filecontent << "\" }";
    if (i != m_files.size() - 1) { strm << ", "; }
  }
  strm << "}}";

  auto res = cpr::Post(cpr::Url{ "https://api.github.com/gists" },
    cpr::Header{ { "Accept", "application/vnd.github+json" } },
    cpr::Header{ { "X-GitHub-Api-Version", "2022-11-28" } },
    auth_header(),
    cpr::Body{ strm.str() });

  if (res.status_code == cpr::status::HTTP_CREATED) {
    std::cout << "[OK] gist created\n";
  } else {
    std::cerr << "[ERROR] gist not created\n" << res.text << std::endl;
  }
  return res;
}

}// namespace pr
