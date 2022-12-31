#pragma once

#include "cpr/cpr.h"
#include <filesystem>
#include <string>
#include <utility>

namespace pr {

using FileNameContentPair = std::pair<std::string, std::string>;

auto getenv_or_die(const char *key) -> std::string;

class GistMaker {
private:
  std::string m_authtoken{};
  std::vector<FileNameContentPair> m_files{};

  [[nodiscard]] auto auth_header() const -> cpr::Header;
  [[nodiscard]] static auto read_file(const std::filesystem::path &filepath) -> FileNameContentPair;

public:
  GistMaker() = delete;
  explicit GistMaker(std::string &&token) : m_authtoken(std::move(token)){};
  GistMaker(const GistMaker &other) : m_authtoken(other.m_authtoken){};
  GistMaker(GistMaker &&other) noexcept : m_authtoken(std::move(other.m_authtoken)){};
  GistMaker &operator=(const GistMaker &other) = default;
  GistMaker &operator=(GistMaker &&other) = default;

  ~GistMaker() = default;

  void add_file(const std::filesystem::path &filepath);
  auto create_gist(std::string &&gist_desc, bool pub = false) const -> cpr::Response;
};

}// namespace pr
