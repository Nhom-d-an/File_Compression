#pragma once
#include "huffman.h"
#include "LZ77.h"
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <iostream>
#include <iostream>
#include <string>
#include <fstream>
#include <unordered_map>
#include "dirent.h"

void encodingFile();
void encodingFile(string inPath);
void decodingFile();
void decodingFile(string inPath);
void encodingFolder();
void decodingFolder();