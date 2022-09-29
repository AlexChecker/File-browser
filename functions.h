//
// Created by alex Cheker on 28.09.2022.
//

#ifndef TESTIMGUI_FUNCTIONS_H
#define TESTIMGUI_FUNCTIONS_H

namespace fs=ghc::filesystem;

void readFile(const std::string& path,std::string& content)
{
    if(!fs::exists(path))
    {
        content="";
        return;
    }
    std::ifstream file (path);
    std::string temp;
    if(file.is_open())
    {
        while(getline(file,temp))
        {
            content += temp+'\n';
        }
        file.close();
    }
}


void fillVectorDirs(const std::string& path, std::vector<std::string>& paths)
{
    paths.emplace_back("..");
    for (const auto & entry : fs::directory_iterator(path))
        if(fs::is_directory(entry)) {
            //paths.push_back(entry.path());

            std::string temp = entry.path();
            temp = temp.substr(temp.find_last_of('/')+1,temp.length());
            paths.push_back(temp);
        }
}

void fillVectorFiles(const std::string& path,std::vector<std::string>& files)
{
    for(const auto& entry : fs::directory_iterator(path))
    {
        if (!fs::is_directory(entry)) {
            std::string temp = entry.path();
            temp = temp.substr(temp.find_last_of('/')+1,temp.length());
            files.push_back(temp);

        }
    }
}

#endif //TESTIMGUI_FUNCTIONS_H
