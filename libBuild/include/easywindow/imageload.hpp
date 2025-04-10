#pragma once

#include <filesystem>
#include <optional>
#include "defines.hpp"

struct GLFWimage;

namespace ewin
{
    EWIN_API std::optional<GLFWimage> LoadImage(const std::filesystem::path& path);
    EWIN_API void FreeImage(GLFWimage& image);
}