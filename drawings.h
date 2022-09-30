//
// Created by alex Cheker on 28.09.2022.
//

#ifndef TESTIMGUI_DRAWINGS_H
#define TESTIMGUI_DRAWINGS_H
#include "imgui-1.88/imgui.h"
#include "variables.h"
#include "functions.h"
#include "filesystem.hpp"
#include <iostream>

namespace fs = ghc::filesystem;

void showTextView(bool show)
{
        ImGui::Begin("Text view", &show, flags);
        ImGui::SetWindowSize(halfSize);
        ImGui::SetWindowPos(halfSize);
        char *txt = new char[contents.size() + 1];
        std::copy(contents.begin(), contents.end(), txt);
        txt[contents.size()] = '\0';
        std::cout << contents.length() << std::endl;
        ImGui::InputTextMultiline(" ", txt, contents.size() + 1, textViewSize, textFlag);

        ImGui::End();

}

void showDirectoriesList()
{

    ImGui::Begin("Directories", ope,fileFlags);
    ImGui::SetWindowSize(size);
    ImGui::SetWindowPos(pos);
    if(ImGui::BeginMenuBar())
    {
        if(ImGui::BeginMenu("Tools"))
        {
            ImGui::MenuItem("Copy", nullptr,&copyDir);
            ImGui::MenuItem("Paste",nullptr,&pasteDir,copyDir);
            ImGui::MenuItem("Delete", nullptr,&confirmDeleteDirectory);
            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }
    ImGui::Text("%s", path.c_str());
    ImGui::Checkbox("Show demo",&showdemo);

    static int sel =-1;
    for(int i=0;i<paths.size();i++)
    {
        char title[255];
        //sprintf(title,"Entry %d",i);
        strcpy(title,paths[i].c_str());

        if(ImGui::Selectable(title,sel==i)) {
            sel = i;
            if(!selectionMode) {
                if (sel == 0) {
                    path = path.substr(0, path.find_last_of('/'));
                } else
                    path = path + "/" + title;
            }
            else
            {
                if(sel>0) {
                    std::cout << copyDir << " " << dirCopied << std::endl;
                    fullDirPath = path + '/' + title;
                }

            }
            paths.clear();
            files.clear();
            fillVectorDirs(path,paths);
            fillVectorFiles(path,files);
        }



    }
    if(copyDir&&!dirCopied)
    {
        //copyingDirPath = path+'/'+title;
        std::cout<<"Copying dir "<<copyingDirPath<<std::endl;
        dirCopied= true;
        selectionMode=false;
    }


    if(pasteDir)
    {
        std::cout<<copyingDirPath<<"|"<<path<<std::endl;
        copyDirectory(fullDirPath,path);
        copyDir= false;
        dirCopied=false;
        pasteDir=false;
        fullDirPath="";
        paths.clear();
        files.clear();
        fillVectorDirs(path,paths);
        fillVectorFiles(path,files);
    }
    ImGui::End();
}

void fileExistsDialog()
{
    ImGui::Begin("File exists",ope,alertFlags);
    ImGui::Text("File %s exists, can't copy or move.",fullFileName.c_str());
    fileExists=!ImGui::Button("Close");
    beginCopy=fileExists;
    endCopy=fileExists;
    startMove=fileExists;
    ImGui::End();
}

void directoryExistsDialog()
{
    ImGui::Begin("Directory exists",ope,alertFlags);
    ImGui::Text("Directory %s exists, can't overwrite",fullDirPath.c_str());
    dirExists=!ImGui::Button("Close");
    copyDir=false;
    dirExists=false;
    pasteDir=false;
    dirCopied = false;
}

void confirmDeleteDirectoryDialog()
{
    if(directoryExists(fullDirPath))
    {
        ImGui::Begin("Delete?");
        ImGui::Text("Do you really want to delete this directory with it's contents?");
        if(ImGui::Button("Yes"))
        {
            deleteDirectory(fullDirPath);
            fullDirPath="";
            confirmDeleteDirectory=false;
        }
        if(ImGui::Button("Close"))
        {
            fullDirPath="";
            confirmDeleteDirectory=false;

        }
        paths.clear();
        fillVectorDirs(path,paths);
        selectionMode= false;
        ImGui::End();
    }
}

void confirmDeleteDialog()
{
    if(fs::exists(fullFileName)) {
        ImGui::Begin("Delete?");
        ImGui::Text("Do you really want to delete this file?");
        if (ImGui::Button("Yes")) {
            std::remove(fullFileName.c_str());
            confirmDelete = false;
            files.clear();
            fillVectorFiles(path, files);
            fullFileName = "";
        }
        if (ImGui::Button("Cancel")) {
            confirmDelete = false;
        }
        ImGui::End();
    }
}

void showFilesList()
{
    ImGui::Begin("Files",ope,fileFlags);
    ImGui::SetWindowPos(pos2);
    if(showTextPreview)
        ImGui::SetWindowSize(halfSize);
    else
        ImGui::SetWindowSize(size);
    if(ImGui::BeginMenuBar())
    {
        if(ImGui::BeginMenu("File"))
        {
            ImGui::MenuItem("Copy",nullptr,&beginCopy,(startMove)?false:true);
            ImGui::MenuItem("Paste",nullptr,&endCopy,(beginCopy||startMove)?true:false);
            ImGui::MenuItem("Move",nullptr,&startMove,(beginCopy)?false:true);
            ImGui::MenuItem("Delete", nullptr,&confirmDelete,(fullFileName!="")?true:false);
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }
    ImGui::Checkbox("Show file preview?",&showTextPreview);
    static int s =-1;

    for (int i=0;i<files.size();i++)
    {
        char name[255];
        strcpy(name,files[i].c_str());
        if(ImGui::Selectable(name,s==i))
        {
            s=i;
            fullFileName=path+'/'+name;
            std::cout<<"Info: "<<s<<' '<<beginCopy<<' '<<endCopy<<std::endl;
            if(showTextPreview)
            {
                //viewingFilePath = path + '/' + name;
                contents = "";
                std::cout << path + '/' + name << std::endl;
                readFile(path + '/' + name, contents);
            }
        }

        if(beginCopy&&!copied&&s==i)
        {
            copyFromFile = path+'/'+name;
            std::cout<<"Copying "<<copyFromFile<<std::endl;
            copyingFileName = name;
            copied=true;
        }
        if(startMove&&!endMove&&s==i)
        {
            copyFromFile=path+'/'+name;
            std::cout<<"Moving "<<copyFromFile<<std::endl;
            copyingFileName = name;
            endMove = true;
        }

    }
    if(endCopy)
    {
        if(copyFromFile != path+'/'+copyingFileName) {


            if (!fs::exists(path + '/' + copyingFileName)) {

                if (endMove) {
                    std::cout << "Moving file from " << copyFromFile << " to " << path + '/' + copyingFileName
                              << std::endl;
                    fs::copy_file(copyFromFile, path + '/' + copyingFileName);

                    std::remove(copyFromFile.c_str());
                    startMove=false;
                    endMove=false;
                    copyingFileName="";
                    endCopy=false;
                    copyFromFile="";
                    files.clear();
                    fillVectorFiles(path, files);

                }
                else {
                    fs::copy_file(copyFromFile, path + '/' + copyingFileName);
                    std::cout << "Pasting: " << copyFromFile << " " << path + '/' + copyingFileName
                              << std::endl;
                    beginCopy = false;
                    copied = false;
                    endCopy = false;
                    copyFromFile = "";
                    copyingFileName = "";
                    files.clear();
                    fillVectorFiles(path, files);
                }
            } else {
                fileExists = true;
                fullFileName = path + '/' + copyingFileName;
            }
        }
    }
    ImGui::End();
}

#endif //TESTIMGUI_DRAWINGS_H
