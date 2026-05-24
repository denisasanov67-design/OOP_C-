#pragma once
#include <fstream>
#include <vector>

using namespace std;

// Упаковка структур без выравнивания
#pragma pack(push,1)


// Заголовок BMP файла (14 байт)
// Содержит основную информацию о файле 
struct BMPFileHeader
{
	uint16_t bfType;
	uint32_t bfSize;
	uint16_t bfReserved1;
	uint16_t bfReserved2;
	uint32_t bfOffBits;
};

// Заголовок информации о изображении (40 байт для BITMAPINFOHEADER)
struct BMPInfoHeader {
    uint32_t biSize;
    int32_t width;
    int32_t height;
    uint16_t planes;
    uint16_t bitCount;
    uint32_t compression;
    uint32_t sizeImage;
    int32_t xPixelsPerMeter;
    int32_t yPixelsPerMeter;
    uint32_t colorsUsed;
    uint32_t colorsImportant;
};

// Восстановление стандартного выравнивания
#pragma pack(pop)

// Структура пикселя BGR
struct Pixel {
    unsigned char b;
    unsigned char g;
    unsigned char r;
};

// Оператор сравнения для приоритетной очереди
struct Node {
    int dist, y, x;
    bool operator > (const Node& other)const;
};