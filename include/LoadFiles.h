#pragma once
#include<iostream>
#include<string>
#include<vector>
#include<io.h>

class LoadFiles
{
public:
    LoadFiles(std::string basicpath);
    void search_files(std::string path);  // search files in specific folder
    std::vector<std::string> get_files();

public:
    std::string root_path;

private:
	std::vector<std::string> filenames;
    
};


LoadFiles::LoadFiles(std::string basicpath)
{
    root_path = basicpath;
}


void LoadFiles::search_files(std::string path)
{
    intptr_t hFile = 0;   //file handle (for win10)
    struct _finddata_t fileinfo; // file inforamtion
    std::string p;

    if ((hFile = _findfirst(p.assign(root_path).append("\\*").c_str(), &fileinfo)) != -1)
    {
        do
        { 
            if ((fileinfo.attrib & _A_SUBDIR))  // read subfolder
            {
                if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
                    search_files(p.assign(path).append("\\").append(fileinfo.name));
            }
            else  // read .mp4 and .avi videos
            {
                std::string filename = fileinfo.name;
                std::string suffixStr = filename.substr(filename.find_last_of('.') + 1);
                if ((suffixStr == "avi") || (suffixStr == "mp4"))
                    filenames.push_back(path + "\\" + fileinfo.name);
            }
        } while (_findnext(hFile, &fileinfo) == 0);
        _findclose(hFile);
    }
}


std::vector<std::string> LoadFiles::get_files()
{
    return filenames;
}



