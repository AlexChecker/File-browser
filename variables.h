//
// Created by alex Cheker on 25.09.2022.
//

#ifndef TESTIMGUI_VARIABLES_H
#define TESTIMGUI_VARIABLES_H
#include <string>
#include "imgui-1.88/imgui.h"

bool beginCopy=false;
bool copied = false;
bool endCopy=false;
bool startMove=false;
bool endMove = false;
bool showTextPreview = false;
bool fileExists=false;
bool confirmDelete=false;
bool showdemo = false;
bool *ope = nullptr;
bool copyDir = false;
bool pasteDir = false;
bool dirExists = false;
bool dirCopied = false;
bool selectionMode =false;

std::vector<std::string> paths;
std::vector<std::string> files;


std::string copyingDirPath;
std::string path;
std::string copyFromFile;
std::string copyingFileName;
std::string fullFileName;
std::string context;
std::string contents;


ImVec2 pos;
ImVec2 pos2;
ImVec2 size;
ImVec2 halfSize;
ImVec2 textViewSize;
ImVec2 consize(400,400);


ImGuiWindowFlags flags =0;

ImGuiWindowFlags fileFlags =0;
ImGuiInputTextFlags textFlag =0;
ImGuiWindowFlags alertFlags =0;


#endif //TESTIMGUI_VARIABLES_H
