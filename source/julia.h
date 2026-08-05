#ifndef JULIA_H
#define JULIA_H

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>
#include <tuple>

std::tuple<int, int, int> colormap(int c)
{
    if (!c)
      return {0, 0, 0};

    int r = 0, g = 0, b = 0;
    const int val = (c%64) * 4;

    if (c < 64) {
      r = 0; g = 0; b = val;
    } else if (c >= 64 && c < 128) {
      r = 0; g = val; b = 255 - val;
    } else if (c >= 128 && c < 192) {
      r = val; g = 255; b = 0;
    } else {
      r = 255; g = 255-val; b = 0;
    }

    return {r, g, b};
}

void writePNG(unsigned char const * pixels, size_t nx, size_t ny, const char * filename) {
  std::vector<std::byte> stb_buffer(nx*ny*3);

  for (size_t j = 0; j < ny; ++j) {
    for (size_t i = 0; i < nx; ++i) {
      unsigned char col = pixels[i + (ny-j-1)*nx];
      auto [r, g, b] = colormap(col);

      stb_buffer[3*(i + j*nx)] = static_cast<std::byte>(static_cast<unsigned char>(r));
      stb_buffer[3*(i + j*nx)+1] = static_cast<std::byte>(static_cast<unsigned char>(g));
      stb_buffer[3*(i + j*nx)+2] = static_cast<std::byte>(static_cast<unsigned char>(b));
    }
  }

  stbi_write_png(filename, nx, ny, 3, stb_buffer.data(), 3*nx);

  std::cout << "Wrote " << filename << "\n";
}

template<typename Dims>
void juliaCPU(const Dims dim,
           size_t maxIter, float maxMagnitude, unsigned char *image,
           float cReal, float cImag)
{
  const float dx = (dim.xmax - dim.xmin) / dim.nx;
  const float dy = (dim.ymax - dim.ymin) / dim.ny;

  for (unsigned int j = 0; j < dim.ny; ++j) {
    for (unsigned int i = 0; i < dim.nx; ++i) {
      float zReal = dim.xmin + i * dx;
      float zImag = dim.ymin + j * dy;
      size_t k = 0;

      do {
        // Compute z^2 + c for complex numbers:
        auto const tmpzReal = zReal*zReal - zImag*zImag + cReal;
        zImag = 2. * zReal*zImag + cImag;
        zReal = tmpzReal;
      } while (++k < maxIter && (zReal*zReal + zImag*zImag) < maxMagnitude*maxMagnitude);

      image[i + dim.nx*j] = k < maxIter ? 1 + (255 * k)/maxIter : 0;
    }
  }
}

#endif
