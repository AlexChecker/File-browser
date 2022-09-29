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

//Check if directory exists. If yes, return true, otherwise false
bool directoryExists(std::string path)
{
    std::cout<<path<<std::endl;
       std::string parent = path.substr(0,path.find_last_of('/'));
       for(const auto& entry:fs::directory_iterator(parent))
       {
           if(entry.path()==path) return true;
       }
       return false;

}

void copyDirectory(std::string path_from,std::string path_to)
{
    std::cout<<path_from<<"|"<<path_from<<std::endl;
    if(path_from!=path_to) {
        std::string dirname = path_from.substr( path_from.find_last_of('/')+1,path_from.length());

        if(!directoryExists(path_to+'/'+dirname)) {
            fs::create_directory(path_to +'/'+dirname);
            for (const auto &entry: fs::directory_iterator(path_from)) {

                std::string temp = entry.path();
                temp = temp.substr(temp.find_last_of('/') + 1, temp.length());
                if (!fs::is_directory(entry)) {
                    if(temp!=".DS_Store") {
                        std::cout << "File " << entry.path() << std::endl;
                        fs::copy_file(entry.path(), path_to + '/'+dirname+'/' + temp);
                    }
                }
                else
                {
                    std::cout<<"Dir "<<entry.path()<<"|"<<path_to + '/' + dirname<<std::endl;
                    copyDirectory(entry.path(), path_to+'/'+dirname);
                }
            }
        }
        else
        {
            dirExists=true;
        }
    }
}



#endif //TESTIMGUI_FUNCTIONS_H
