#include "ImageCompressor.h"

int main() {
    // test.jpg -> encoded.jpg -> decoded.jpg
    ImageCompressor::compress("test.jpg", "encoded.jpg");
    ImageCompressor::decompress("encoded.jpg", "decoded.jpg");

    // test.png -> encoded.png -> decoded.png
    ImageCompressor::compress("test.png", "encoded.png");
    ImageCompressor::decompress("encoded.png", "decoded.png");

    // test.bmp -> encoded.bmp -> decoded.bmp
    ImageCompressor::compress("test.bmp", "encoded.bmp");
    ImageCompressor::decompress("encoded.bmp", "decoded.bmp");

    return 0;
}
