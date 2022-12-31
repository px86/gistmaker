#include "argparser.hpp"
#include "cpr/cpr.h"
#include "gistmaker.hpp"
#include <filesystem>
#include <iostream>

int main(int argc, char **argv)
{
  const char *fpath = nullptr;
  const char *description = "";
  bool public_gist = false;

  auto argparser = pr::ArgParser("gistmaker");
  argparser.add_option(fpath, "file to be uploaded", "file", 'f');
  argparser.add_option(description, "gist description", "desc", 'd');
  argparser.add_option(public_gist, "make gist public", "public", 'p');
  argparser.parse(argc, argv);

  if (fpath == nullptr) {
    std::cerr << "Error: No file provided!\n";
    std::exit(EXIT_FAILURE);
  }

  auto filepath = std::filesystem::path{ fpath };

  auto gist_maker = pr::GistMaker{ pr::getenv_or_die("GISTMAKER_TOKEN") };
  gist_maker.add_file(filepath);
  auto res = gist_maker.create_gist(description, public_gist);
  (void)res;

  return EXIT_SUCCESS;
}
