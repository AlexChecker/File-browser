//
// Created by alex Cheker on 25.09.2022.
//

#ifndef TESTIMGUI_VARIABLES_H
#define TESTIMGUI_VARIABLES_H
#include <string>
#include "imgui.h"

bool beginCopy=false;
bool copied = false;
bool endCopy=false;

std::string copyFromFile;
std::string copyingFileName;
std::string viewingFilePath;


ImVec2 pos;
ImVec2 pos2;
ImVec2 size;
ImVec2 halfSize;
ImVec2 textViewSize;



#endif //TESTIMGUI_VARIABLES_H
