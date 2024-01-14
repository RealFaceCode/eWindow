#pragma once

#include <filesystem>
#include <optional>

struct GLFWimage;

namespace ewin::internal
{
    std::optional<GLFWimage> LoadImage(std::filesystem::path path);
    void FreeImage(GLFWimage& image);
}