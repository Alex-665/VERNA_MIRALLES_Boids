#include "utils.hpp"

std::string file_content(std::filesystem::path const& path)
{
    auto ifs = std::ifstream{p6::make_absolute_path(path)};
    return std::string{std::istreambuf_iterator<char>{ifs}, {}};
}
