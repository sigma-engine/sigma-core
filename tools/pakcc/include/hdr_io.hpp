#ifndef SIGMA_HDR_IO_HPP
#define SIGMA_HDR_IO_HPP

#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <boost/gil/image.hpp>
#include <boost/gil/typedefs.hpp>

#include <array>
#include <fstream>
#include <vector>

const constexpr unsigned int HDR_MIN_ENCODE_LEN = 8;
const constexpr unsigned int HDR_MAX_ENCODE_LEN = 0x7FFF;

using rgbe8_pixel_t = std::array<unsigned char, 4>;

boost::gil::rgb32f_pixel_t hrd_rgbe_to_rgb(const rgbe8_pixel_t& pixel)
{
    int exp = pixel[3] - 128;
    boost::gil::rgb32f_pixel_t out;
    for (int i = 0; i < 3; ++i)
        out[i] = (pixel[i] / 256.0f) * float(pow(2, exp));
    return out;
}

bool hdr_old_rle_decode(std::vector<rgbe8_pixel_t>& scanline, int start, std::ifstream& file)
{
    int rshift = 0;
    for (std::size_t i = start; i < scanline.size();) {
        file.read((char*)&scanline[i], sizeof(rgbe8_pixel_t));
        if (file.eof())
            return false;

        if (scanline[i][0] == 1 && scanline[i][1] == 1 && scanline[i][2] == 1) {
            int count = scanline[i][3] << rshift;
            for (int j = 0; j < count; ++j, ++i)
                scanline[i] = scanline[i - 1];
            rshift += 8;
        } else {
            rshift = 0;
            i++;
        }
    }
    return true;
}

bool hdr_rle_decode(std::vector<rgbe8_pixel_t>& scanline, std::ifstream& file)
{
    if (scanline.size() < HDR_MIN_ENCODE_LEN || scanline.size() > HDR_MAX_ENCODE_LEN)
        return hdr_old_rle_decode(scanline, 0, file);

    file.read((char*)&scanline[0], sizeof(rgbe8_pixel_t));
    if (scanline[0][0] != 2 || scanline[0][1] != 2 || scanline[0][2] & 128)
        return hdr_old_rle_decode(scanline, 1, file);

    for (int i = 0; i < 4; i++) {
        for (std::size_t j = 0; j < scanline.size();) {
            unsigned char count = file.get();
            if (count > 128) { // RLE run
                count &= 127;
                unsigned char val = file.get();
                while (count--)
                    scanline[j++][i] = val;
            } else {
                while (count--)
                    scanline[j++][i] = file.get();
            }
        }
    }

    return !file.eof();
}

template <class Image>
void hdr_read_and_convert_image(const std::string& filename, Image& res)
{
    std::ifstream file(filename, std::ios::binary);

    std::string line;
    std::getline(file, line);
    if (!file || line != "#?RADIANCE")
        throw std::runtime_error("The file '" + std::string(filename) + "' is not a radiance file!");

    while (std::getline(file, line) && line != "") {
        if (line[0] == '#')
            continue;
        if (boost::starts_with(line, "FORMAT=")) {
            boost::replace_all(line, "FORMAT=", "");
            if (line != "32-bit_rle_rgbe" && line != "32-bit_rle_xyze")
                throw std::runtime_error("The format '" + line + "' is not supported!");
        }
    }

    // Read the resolution string
    std::string vert_axis;
    std::string hor_axis;
    int hor_size, vert_size;
    file >> vert_axis >> vert_size >> hor_axis >> hor_size;
    file.get(); // Read the new line
    if (!file)
        throw std::runtime_error("Invalid resolution!");

    res = Image(hor_size, vert_size);
    auto out_view = boost::gil::flipped_left_right_view(boost::gil::view(res));

    std::vector<rgbe8_pixel_t> scanline(hor_size);
    for (int y = 0; y < hor_size; y++) {
        if (!hdr_rle_decode(scanline, file))
            break;
        for (int x = 0; x < hor_size; ++x)
            boost::gil::color_convert(hrd_rgbe_to_rgb(scanline[x]), out_view(x, y));
    }
}

#endif // SIGMA_HDR_IO_HPP
