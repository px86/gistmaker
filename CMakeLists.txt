cmake_minimum_required(VERSION 3.16)

# Not ideal to use this global variable, but necessary to make sure
# that tooling and projects use the same version
set(CMAKE_CXX_STANDARD 17)

# strongly encouraged to enable this globally to avoid conflicts between
# -Wpedantic being enabled and -std=c++20 and -std=gnu++20 for example
# when compiling with PCH enabled
set(CMAKE_CXX_EXTENSIONS OFF)

# Export comile_commands.json file, necessary for clangd language server to work.
# Either copy the generated file to the root of source tree, or create a symlink.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Note: by default ENABLE_DEVELOPER_MODE is True
# This means that all analysis (sanitizers, static analysis)
# is enabled and all warnings are treated as errors
# if you want to switch this behavior, change TRUE to FALSE
set(ENABLE_DEVELOPER_MODE
    TRUE
    CACHE BOOL "Enable 'developer mode'")

# Change this to false if you want to disable warnings_as_errors in developer mode
set(OPT_WARNINGS_AS_ERRORS_DEVELOPER_DEFAULT TRUE)

# Set the project name and language
project(
  GistMaker
  VERSION 0.0.1
  DESCRIPTION "Upload given file as a github gist"
  HOMEPAGE_URL "https://github.com/px86/gistmaker"
  LANGUAGES CXX C)

# Add include directories
include_directories(include/)

# Include Conan build files
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

# Add libraries
add_library(gistmaker src/gistmaker.cpp)

# Add executable
add_executable(gm src/main.cpp)

# Link libraries
target_link_libraries(gm gistmaker ${CONAN_LIBS})
