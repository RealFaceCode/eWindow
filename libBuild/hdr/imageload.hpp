#pragma once

#include <filesystem>
#include <optional>

struct GLFWimage;

namespace ewin
{
    std::optional<GLFWimage> LoadImage(const std::filesystem::path& path);
    void FreeImage(GLFWimage& image);
}