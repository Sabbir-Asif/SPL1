#include <iostream>
#include <fstream>

#pragma pack(2)
struct TIFFHeader {
    uint16_t byteOrder;
    uint16_t magicNumber;
    uint32_t offset;
};

#pragma pack(2)
struct TIFFTag {
    uint16_t tagID;
    uint16_t dataType;
    uint32_t dataCount;
    uint32_t dataOffset;
};

int main() {
    // Open the .tif image file
    std::ifstream file("B5.tif", std::ios::binary);
    if (!file.is_open()) {
        std::cout << "Failed to open the image file" << std::endl;
        return 1;
    }

    // Read the TIFF header
    TIFFHeader header;
    file.read(reinterpret_cast<char*>(&header), sizeof(TIFFHeader));

    // Check the byte order of the TIFF file
    bool isLittleEndian = header.byteOrder == 0x4949; // "II" in ASCII
    if (!isLittleEndian) {
        std::cout << "Unsupported byte order in the TIFF file" << std::endl;
        file.close();
        return 1;
    }

    // Read the TIFF tags
    TIFFTag tag;
    for (uint32_t i = 0; i < header.offset - sizeof(TIFFHeader);) {
        file.read(reinterpret_cast<char*>(&tag), sizeof(TIFFTag));
        i += sizeof(TIFFTag);

        // Check if the tag contains the image data
        if (tag.tagID == 0x8769 || tag.tagID == 0x8825) {
            file.seekg(tag.dataOffset, std::ios::beg); // Move to the data offset

            // Read the image data
            uint32_t imageSize = tag.dataCount * (tag.dataType == 3 ? 2 : 4); // 2 bytes for USHORT, 4 bytes for ULONG
            char* imageData = new char[imageSize];
            file.read(imageData, imageSize);

            // Process the image data here
            // ...

            delete[] imageData;
        }

        i += tag.dataCount * (tag.dataType == 3 ? 2 : 4); // Move to the next tag
    }

    // Close the file
    file.close();

    return 0;
}
