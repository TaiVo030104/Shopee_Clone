#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING 1
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <locale>
#include <fcntl.h> 
#include <filesystem>
#include <vector>
#include <io.h>
#include <codecvt>
#include <Windows.h>


using namespace std;
namespace fs = std::filesystem;

struct Student {
    wchar_t* id;
    wchar_t* name;
    wchar_t* department;
    wchar_t* batch;
    wchar_t* dob;
    wchar_t* image;
    wchar_t* description;
    vector<wstring> hobby;
};
wchar_t* removeElement(const wchar_t* str);
Student* readFileAndAllocateMemory(const wchar_t* filename, int& numStudents);
void createWebsiteDirectory(const wchar_t* basePath, const wchar_t* websiteName);
void createAndPrintCSSFile(const wchar_t* directoryPath, const wchar_t* filename);
void moveImagesFolder(const wchar_t* sourcePath, const wchar_t* destinationPath);
int* inputSelectedInfo(int& numSelectedInfo);
void createHTMLFiles(Student* students, int numStudents, const wchar_t* outputDirectory, const wchar_t* imageDirectory, int* selectedInfo);