#include "imageload.hpp"

#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <log.hpp>

namespace ewin
{
    EWIN_API std::optional<GLFWimage> LoadImage(const std::filesystem::path& path)
    {
        GLFWimage image;
		image.pixels = ::stbi_load(path.string().c_str(), &image.width, &image.height, 0, 4);
		if(image.pixels == nullptr)
		{
            lc::Log<"EWIN">("ERROR", "Failed to load image: {}", path.string());
			return std::nullopt;
		}
        return image;
    }

    EWIN_API void FreeImage(GLFWimage& image)
    {
        ::stbi_image_free(image.pixels);
    }
}