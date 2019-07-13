#include <sigma/Texture.hpp>

#include <sigma/Log.hpp>

FlagSet<ImageUsage> operator|(const ImageUsage &inA, const ImageUsage &inB)
{
	return FlagSet<ImageUsage>(inA) | inB;
}


uint64_t imageFormatPixelSize(ImageFormat inFormat)
{
    switch (inFormat) {
    case ImageFormat::UnormB8G8R8A8:
    case ImageFormat::UnormR8G8B8A8:
        return 4;
    default:
        break;
    }
    SIGMA_ASSERT(false, "Unknown ImageFormat");
    return 4;
}