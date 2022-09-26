// This file was taken from the official ImGui repository to simplify the script

// Dear ImGui: standalone example application for GLFW + OpenGL 3, using programmable pipeline
// (GLFW is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan/Metal graphics context creation, etc.)
// If you are new to Dear ImGui, read documentation from the docs/ folder + read the top of imgui.cpp.
// Read online: https://github.com/ocornut/imgui/tree/master/docs

#include "imgui-1.88/imgui.h"
#include <iostream>
#include <vector>
#include "filesystem.hpp"
#include "imgui-1.88/backends/imgui_impl_glfw.h"
#include "imgui-1.88/backends/imgui_impl_opengl3.h"
#include "imgui-1.88/misc/cpp/imgui_stdlib.h"
#include <stdio.h>
#include <fstream>
#include "variables.h"
#include <spawn.h>
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include </usr/local/Cellar/glfw/3.3.8/include/GLFW/glfw3.h> // Will drag system OpenGL headers

namespace fs = ghc::filesystem;
// [Win32] Our example includes a copy of glfw3.lib pre-compiled with VS2010 to maximize ease of testing and compatibility with old VS compilers.
// To link with VS2010-era libraries, VS2015+ requires linking with legacy_stdio_definitions.lib, which we do using this pragma.
// Your own project should not be affected, as you are likely to link with a newer binary of GLFW that is adequate for your version of Visual Studio.
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif



static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

static void glfw_window_resize_callback(GLFWwindow* window,int w,int h)
{
    size=ImVec2(w/2,h);
    pos2=ImVec2(w/2,0);
    halfSize=ImVec2(w/2,h/2);
    
}

void readFile(std::string path,std::string& content)
{
    std::ifstream file (path);
    std::string temp;
    if(file.is_open())
    {
        while(getline(file,temp))
        {
            content = content+temp+'\n';
        }
        file.close();
    }
}


void fillVectorDirs(std::string path, std::vector<std::string>& paths)
{
    paths.push_back("..");
    for (const auto & entry : fs::directory_iterator(path))
        if(fs::is_directory(entry)) {
            //paths.push_back(entry.path());

            std::string temp = entry.path();
            temp = temp.substr(temp.find_last_of('/')+1,temp.length());
            paths.push_back(temp);
        }
}

void fillVectorFiles(std::string path,std::vector<std::string>& files)
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


int main(int, char**)
{
    //fs::path("/users/alex");
    std::string path ="/users/alex";
    std::vector<std::string> paths;
    std::vector<std::string> files;
    fillVectorDirs(path,paths);
    fillVectorFiles(path,files);
    // Setup window
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

    // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100
    const char* glsl_version = "#version 100";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
    // GL 3.2 + GLSL 150
    const char* glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

    // Create window with graphics context
    GLFWwindow* window = glfwCreateWindow(1280, 720, "File browser", NULL, NULL);
    if (window == NULL)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    //ImGui::StyleColorsClassic();
    ImGui::StyleColorsClassic();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != NULL);

    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // Main loop
    //ImVec2 pos(0,0);
    int width;
    int height;
    glfwGetWindowSize(window,&width,&height);
    size=ImVec2(width/2,height);
    pos2=ImVec2(width/2,0);
    halfSize=ImVec2(width/2,height/2);
    textViewSize=ImVec2(width/2-10,height/2-35);
    bool showdemo = false;
    ImGuiWindowFlags flags =0;
    flags |= ImGuiWindowFlags_NoMove;
    flags |= ImGuiWindowFlags_NoResize;
    flags |= ImGuiWindowFlags_NoCollapse;
    ImGuiWindowFlags fileFlags =0;
    fileFlags |= ImGuiWindowFlags_NoMove;
    fileFlags |= ImGuiWindowFlags_NoResize;
    fileFlags |= ImGuiWindowFlags_NoCollapse;
    fileFlags |= ImGuiWindowFlags_MenuBar;
    ImGuiInputTextFlags textFlag =0;
    textFlag |= ImGuiInputTextFlags_ReadOnly;
    bool *open=NULL;
    glfwSetWindowSizeCallback(window,glfw_window_resize_callback);
    std::string contents="";


    while (!glfwWindowShouldClose(window))
    {

        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if(showTextPreview) {
            ImGui::Begin("Text view", open, flags);
            ImGui::SetWindowSize(halfSize);
            ImGui::SetWindowPos(halfSize);
            char* txt= new char[contents.size()+1];
            std::copy(contents.begin(),contents.end(),txt);
            txt[contents.size()]='\0';
            //strcpy(txt,contents);
            std::cout<<contents.length()<<std::endl;
            ImGui::InputTextMultiline(" ",txt,contents.size()+1,textViewSize);

            ImGui::End();
        }

	//Put your ImGui stuff here
        ImGui::Begin("Directories", open,flags);
        ImGui::SetWindowSize(size);
        ImGui::SetWindowPos(pos);

        ImGui::Text(path.c_str());
        ImGui::Checkbox("Show demo",&showdemo);

        static int sel =-1;
        for(int i=0;i<paths.size();i++)
        {
            char title[255];
            //sprintf(title,"Entry %d",i);
            strcpy(title,paths[i].c_str());
            if(ImGui::Selectable(title,sel==i)) {
                sel = i;
                if(sel ==0)
                {
                    path = path.substr(0,path.find_last_of("/"));
                }
                else
                path = path+"/"+title;
                paths.clear();
                files.clear();
                fillVectorDirs(path,paths);
                fillVectorFiles(path,files);
            }
            //std::cout<<sel<<std::endl;
        }
        ImGui::End();
        
        if(fileExists)
        {
            ImGui::Begin("File exists");
            ImGui::Text("File %s exists, can't copy or move.",fullFileName.c_str());
            fileExists=!ImGui::Button("Close");
            beginCopy=fileExists;
            endCopy=fileExists;
            startMove=fileExists;
            ImGui::End();
        }

        ImGui::Begin("Files",open,fileFlags);
        ImGui::SetWindowPos(pos2);
        if(showTextPreview)
            ImGui::SetWindowSize(halfSize);
        else
            ImGui::SetWindowSize(size);
        if(ImGui::BeginMenuBar())
        {
            if(ImGui::BeginMenu("File"))
            {
                ImGui::MenuItem("Copy",NULL,&beginCopy,(startMove)?false:true);
                ImGui::MenuItem("Paste",NULL,&endCopy,(beginCopy||startMove)?true:false);
                ImGui::MenuItem("Move",NULL,&startMove,(beginCopy)?false:true);
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
                if(startMove&&!copied&&s==i)
                {
                    
                }
                if(endCopy)
                {
                    if(!fs::exists(path+'/'+copyingFileName))
                    {
                        
                    
                    fs::copy_file(copyFromFile,path+'/'+copyingFileName);
                    std::cout<<"Pasting: "<<copyFromFile<<" "<<path+'/'+copyingFileName<<std::endl;
                    beginCopy=false;
                    copied=false;
                    endCopy=false;
                    copyFromFile="";
                    copyingFileName="";
                    files.clear();
                    fillVectorFiles(path,files);
                    }
                    else
                    {
                        fileExists=true;
                        fullFileName=path+'/'+copyingFileName;
                    }
                }
            }
        ImGui::End();

        if(showdemo) ImGui::ShowDemoWindow(&showdemo);

        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
