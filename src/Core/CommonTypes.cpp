#include "Core/CommonTypes.hpp"

ColorRGB::ColorRGB(double red, double green, double blue) : r(red), g(green), b(blue) {}
ColorRGB::ColorRGB(double grayscale) : r(grayscale), g(grayscale), b(grayscale) {}
ColorRGB::ColorRGB(const ColorRGBA& color) : r(color.r), g(color.g), b(color.b) {}

ColorRGB ColorRGB::operator+(const ColorRGB& other) const { return {r + other.r, g + other.g, b + other.b}; }

ColorRGB& ColorRGB::operator+=(const ColorRGB& other) {
  r += other.r;
  g += other.g;
  b += other.b;
  return *this;
}

ColorRGB ColorRGB::operator*(double scalar) const { return {r * scalar, g * scalar, b * scalar}; }

bool ColorRGB::operator==(const ColorRGB& other) const { return (r == other.r) && (g == other.g) && (b == other.b); }

ColorRGB operator*(double scalar, const ColorRGB& color) { return color * scalar; }

ColorRGBA::ColorRGBA(double red, double green, double blue, double alpha) : r(red), g(green), b(blue), a(alpha) {}
ColorRGBA::ColorRGBA(double grayscale) : r(grayscale), g(grayscale), b(grayscale) {}
ColorRGBA::ColorRGBA(double grayscale, double alpha) : r(grayscale), g(grayscale), b(grayscale), a(alpha) {}
ColorRGBA::ColorRGBA(const ColorRGB& color) : r(color.r), g(color.g), b(color.b) {}
ColorRGBA::ColorRGBA(const ColorRGB& color, double alpha) : r(color.r), g(color.g), b(color.b), a(alpha) {}

ColorRGBA ColorRGBA::operator+(const ColorRGBA& other) const {
  return {r + other.r, g + other.g, b + other.b, a + other.a};
}

ColorRGBA ColorRGBA::operator*(double scalar) const { return {r * scalar, g * scalar, b * scalar, a * scalar}; }

bool ColorRGBA::operator==(const ColorRGBA& other) const {
  return (r == other.r) && (g == other.g) && (b == other.b) && (a == other.a);
}

ColorRGBA operator*(double scalar, const ColorRGBA& color) { return color * scalar; }
