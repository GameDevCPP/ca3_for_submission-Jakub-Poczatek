#include "bmp.h"

BMP::BMP(const char *fname){
    read(fname);
    setColouredPixels();
}

void BMP::read(const char *fname) {
    std::ifstream inp{fname, std::ios_base::binary};
    if (inp) {
        inp.read((char*)&_fileHeader, sizeof(_fileHeader));
        if(_fileHeader.fileType != 0x4D42) {
            throw std::runtime_error("Error! Unrecognized file format.");
        }
        inp.read((char*)&_infoHeader, sizeof(_infoHeader));

        // Jump to the pixel data location
        inp.seekg(_fileHeader.offsetData, inp.beg);

        // Adjust the header fields for output.
        // Some editors will put extra info in the image file, only save the headers and data.
        _infoHeader.size = sizeof(BMPInfoHeader);
        _fileHeader.offsetData = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader);

        _fileHeader.fileSize = _fileHeader.offsetData;

        if(_infoHeader.height < 0){
            throw std::runtime_error("The program can only treat BMP images "
                                     "with the origin in the bottom left corner!");
        }

        _data.resize(_infoHeader.width * _infoHeader.height * _infoHeader.bitCount / 8);

        // Check for row padding
        if(_infoHeader.width % 4 == 0) {
            inp.read((char*)_data.data(), _data.size());
            _fileHeader.fileSize += static_cast<uint32_t>(_data.size());
        } else {
            _rowStride = _infoHeader.width * _infoHeader.bitCount / 8;
            uint32_t newStride = makeStrideAligned(4);
            std::vector<uint8_t> paddingRow(newStride - _rowStride);

            for (int y = 0; y < _infoHeader.height; ++y){
                inp.read((char*)(_data.data() + _rowStride * y), _rowStride);
                inp.read((char*)paddingRow.data(), paddingRow.size());
            }
            _fileHeader.fileSize += static_cast<uint32_t>(_data.size()) +
                                    _infoHeader.height * static_cast<uint32_t>(paddingRow.size());
        }
    } else {
        throw std::runtime_error("Unable to open the input image file.");
    }
}

uint32_t BMP::makeStrideAligned(uint32_t alignStride){
    uint32_t newStride = _rowStride;
    while(newStride % alignStride != 0){
        newStride++;
    }
    return newStride;
}

void BMP::setColouredPixels(){
    for(int i = 0; i < _data.size(); i+=3){
        _colouredPixels.emplace_back(sf::Color(
                    _data[i+2], _data[i+1], _data[i]
                ));
    }
}

std::vector<uint8_t> BMP::getData() {
    return _data;
}

std::vector<sf::Color> BMP::getColouredPixels(){
    return _colouredPixels;
}

sf::Color BMP::getColourAt(int pixelIndex){
    return _colouredPixels[pixelIndex];
}

int32_t BMP::getWidth(){
    return _width;
}

int32_t BMP::getHeight(){
    return _height;
}

/*void BMP::write(const char *fname) {
    std::ofstream of{fname, std::ios_base::binary};
    if(of) {
        if(_infoHeader.bitCount == 24) {
            if(_infoHeader.width % 4 == 0){
                writeHeadersAndData(of);
            } else {
                uint32_t newStride = makeStrideAligned(4);
                std::vector<uint8_t> paddingRow(newStride - _rowStride);
                writeHeaders(of);

                for (int y = 0; y < _infoHeader.height; ++y){
                    of.write((const char*)(_data.data() + _rowStride * y), _rowStride);
                    of.write((const char*)paddingRow.data(), paddingRow.size());
                }
            }
        } else {
            throw std::runtime_error("This program can treat only 24bpp BMP files");
        }
    } else {
        throw std::runtime_error("Unable to open the output image file");
    }
}

void BMP::writeHeadersAndData(std::ofstream &of) {
    writeHeaders(of);
    of.write((const char*)_data.data(), _data.size());
}

void BMP::writeHeaders(std::ofstream &of) {
    of.write((const char*) &_fileHeader, sizeof(_fileHeader));
    of.write((const char*) &_infoHeader, sizeof(_infoHeader));
}*/