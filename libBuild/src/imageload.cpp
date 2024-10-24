#include "imageload.hpp"

#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>


namespace ewin
{
    std::optional<GLFWimage> LoadImage(const std::filesystem::path& path)
    {
        GLFWimage image;
		image.pixels = ::stbi_load(path.string().c_str(), &image.width, &image.height, 0, 4);
		if(image.pixels == nullptr)
		{
			//elog::Error("Failed to load image: {}", path.string());
			return std::nullopt;
		}
        return image;
    }

    void FreeImage(GLFWimage& image)
    {
        ::stbi_image_free(image.pixels);
    }
}