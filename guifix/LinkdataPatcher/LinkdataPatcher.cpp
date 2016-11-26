#include "stdafx.h"
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>

struct Header
{
	unsigned int magic;
	unsigned int numFiles;
	unsigned int fileAlignment;
	unsigned int pad;
};

struct TableEntry
{
	unsigned int fileOffset;//multiplied by fileAlignment
	unsigned int pad;
	unsigned int compressedSize;
	unsigned int uncompressedSize;
};

int main(int argc, const char* argv[])
{
	if (argc < 4)
	{
		printf("Too few arguments.\nUsage: LinkdataPatcher archiveFile newFile tableIndex\n");
		return 0;
	}

	std::fstream archiveFile(argv[1], std::ios::binary | std::ios::ate | std::ios::in | std::ios::out);
	if (!archiveFile.is_open())
	{
		printf("Error: could not open %s.\n", argv[1]);
		return 0;
	}

	std::ifstream inputFile(argv[2], std::ofstream::binary | std::ios::ate);
	if (!inputFile.is_open())
	{
		printf("Error: could not open %s.\n", argv[2]);
		return 0;
	}

	unsigned int fileIdx = atoi(argv[3]);

	unsigned int archiveSize = (unsigned int)archiveFile.tellg();
	unsigned int fileSize = (unsigned int)inputFile.tellg();
	archiveFile.seekg(0);
	inputFile.seekg(0);

	char buffer[1024];

	Header header;
	archiveFile.read((char*)&header, sizeof(Header));

	if (header.magic != 0x102A49)
	{
		printf("Error: %s is not a LinkData archive file.\n", argv[1]);
		return 0;
	}

	if (fileIdx >= header.numFiles)
	{
		printf("Error: index %d out of bounds.\n", fileIdx);
		return 0;
	}

	archiveFile.seekg(archiveSize);

	while (archiveFile.tellp() % header.fileAlignment != 0)
	{
		char zero = 0;
		archiveFile.write(&zero, 1);
	}

	unsigned int bytesLeft = fileSize;

	while (bytesLeft > 0)
	{
		unsigned int bytesToCopy = std::min(1024U, bytesLeft);
		inputFile.read(buffer, bytesToCopy);
		archiveFile.write(buffer, bytesToCopy);
		bytesLeft -= bytesToCopy;
	}

	while (archiveFile.tellp() % header.fileAlignment != 0)
	{
		char zero = 0;
		archiveFile.write(&zero, 1);
	}

	archiveFile.seekp((fileIdx + 1) * 16);

	inputFile.seekg(0);
	unsigned int uncompressedSize;
	inputFile.read((char*)&uncompressedSize, sizeof(uncompressedSize));

	TableEntry entry;
	entry.fileOffset = archiveSize / header.fileAlignment;
	entry.uncompressedSize = uncompressedSize;
	entry.compressedSize = fileSize;
	entry.pad = 0;
	archiveFile.write((char*)&entry, sizeof(TableEntry));

	printf("Succeeded\n");

	return 0;
}

