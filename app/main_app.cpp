#include "Validation.hpp"
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_opengl.h>
#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_opengl3.h"
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <string>

static bool vectorEdit(const char* label, Vec3& v) {
    ImGui::SeparatorText(label);
    ImGui::PushID(label);
    bool changed=ImGui::InputDouble("x",&v.x,0,0,"%.8g");
    changed|=ImGui::InputDouble("y",&v.y,0,0,"%.8g");
    changed|=ImGui::InputDouble("z",&v.z,0,0,"%.8g");
    ImGui::PopID();
    return changed;
}
static bool quatEdit(const char* label, Quat& q) {
    ImGui::SeparatorText(label);
    ImGui::PushID(label);
    bool changed=ImGui::InputDouble("s",&q.s,0,0,"%.8g");
    changed|=ImGui::InputDouble("x",&q.x,0,0,"%.8g");
    changed|=ImGui::InputDouble("y",&q.y,0,0,"%.8g");
    changed|=ImGui::InputDouble("z",&q.z,0,0,"%.8g");
    ImGui::PopID();
    return changed;
}
static bool matrixTable(const char* label, Matrix3x3& matrix, bool editable) {
    ImGui::SeparatorText(label);
    bool changed=false;
    if (ImGui::BeginTable(label,3,ImGuiTableFlags_Borders)) {
        for (int i=0;i<3;++i) {
            ImGui::TableNextRow();
            for (int j=0;j<3;++j) {
                ImGui::TableSetColumnIndex(j);
                if (editable) {
                    ImGui::PushID(i*3+j);
                    ImGui::SetNextItemWidth(-1);
                    changed|=ImGui::InputDouble("##valor",&matrix.m[i*3+j],0,0,"%.8g");
                    ImGui::PopID();
                } else ImGui::Text("%.9g",matrix.m[i*3+j]);
            }
        }
        ImGui::EndTable();
    }
    return changed;
}
static bool angleEdit(const char* label, double& angle) {
    double degrees=angle*180/Pi;
    if (!ImGui::InputDouble(label,&degrees,0,0,"%.8g")) return false;
    angle=degrees*Pi/180;
    return true;
}
static void vectorText(const char* label, const Vec3& v) {
    ImGui::Text("%s = (%.9g, %.9g, %.9g)",label,v.x,v.y,v.z);
}
static void projection(const Vec3& before, const Vec3& after) {
    if (!isFinite(before) || !isFinite(after)) return;
    ImGui::SeparatorText("Projecció dels vectors (vista obliqua)");
    const ImVec2 pos=ImGui::GetCursorScreenPos();
    const float width=std::max(100.0f,ImGui::GetContentRegionAvail().x),height=220;
    ImGui::InvisibleButton("vectors",ImVec2(width,height));
    ImDrawList* draw=ImGui::GetWindowDrawList();
    const ImVec2 center(pos.x+width/2,pos.y+height/2);
    const double extent=std::max({1.0,std::abs(before.x),std::abs(before.y),std::abs(before.z),
                                std::abs(after.x),std::abs(after.y),std::abs(after.z)});
    const float scale=static_cast<float>(std::min(width*0.25f,height*0.3f)/extent);
    auto endpoint=[&](Vec3 v) { return ImVec2(center.x+scale*static_cast<float>(v.x-0.55*v.z),
                                            center.y-scale*static_cast<float>(v.y-0.35*v.z)); };
    const ImU32 axisColor=IM_COL32(110,115,125,255);
    draw->AddLine(center,endpoint({extent,0,0}),axisColor);
    draw->AddLine(center,endpoint({0,extent,0}),axisColor);
    draw->AddLine(center,endpoint({0,0,extent}),axisColor);
    draw->AddText(endpoint({extent,0,0}),axisColor,"x");
    draw->AddText(endpoint({0,extent,0}),axisColor,"y");
    draw->AddText(endpoint({0,0,extent}),axisColor,"z");
    draw->AddLine(center,endpoint(before),IM_COL32(85,170,255,255),3);
    draw->AddCircleFilled(endpoint(before),4,IM_COL32(85,170,255,255));
    draw->AddLine(center,endpoint(after),IM_COL32(255,180,75,255),3);
    draw->AddCircleFilled(endpoint(after),4,IM_COL32(255,180,75,255));
    ImGui::TextUnformatted("Blau: vector original. Taronja: vector rotat.");
}

int main(int, char**) {
    if (!SDL_Init(SDL_INIT_VIDEO)) { std::fprintf(stderr, "%s\n", SDL_GetError()); return 1; }
#ifdef __APPLE__
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    const char* glsl = "#version 150";
#else
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    const char* glsl = "#version 330";
#endif
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_Window* window = SDL_CreateWindow("Lab 2 - Matemàtiques II - 2026", 1200, 850,
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY);
    SDL_GLContext context = window ? SDL_GL_CreateContext(window) : nullptr;
    if (!context || !SDL_GL_MakeCurrent(window, context)) {
        std::fprintf(stderr, "%s\n", SDL_GetError());
        if (context) SDL_GL_DestroyContext(context);
        if (window) SDL_DestroyWindow(window);
        SDL_Quit(); return 1;
    }
    SDL_GL_SetSwapInterval(1);
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    ImGui::GetIO().IniFilename = nullptr;

    ImGui::GetIO().Fonts->AddFontDefaultBitmap();
    ImGui::StyleColorsDark();
    const ImGuiStyle baseStyle = ImGui::GetStyle();
    float currentScale = 0;
    if (!ImGui_ImplSDL3_InitForOpenGL(window, context) || !ImGui_ImplOpenGL3_Init(glsl)) {
        std::fprintf(stderr, "No es poden inicialitzar els backends d'ImGui\n");
        ImGui::DestroyContext(); SDL_GL_DestroyContext(context);
        SDL_DestroyWindow(window); SDL_Quit(); return 1;
    }

    Input input=exampleInput(0);
    Report report;
    bool hasReport=false,running=true;
    int selected=0,example=0;
    std::string error;
    const char* tasks[]={"Comprovar IsRotation", "Eix i angle -> matriu", "Eix i angle -> quaternió",
        "Matriu -> eix i angle", "Matriu -> quaternió", "Matriu -> Euler ZYX",
        "Euler ZYX -> matriu", "Producte q1 * q2", "Quaternió -> eix i angle",
        "Quaternió -> matriu", "Rotar vector amb matriu", "Rotar vector amb quaternió"};
    const char* examples[]={"Gir de 45 graus sobre z", "Identitat (0 graus)",
        "180 graus, eix (0,1,-1)", "Prop de 180 graus", "Gimbal lock +90 graus",
        "Gimbal lock -90 graus", "Prop de gimbal lock", "Reflexió (matriu invàlida)",
        "Eix i quaternió nuls"};
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            ImGui_ImplSDL3_ProcessEvent(&event);
            if (event.type == SDL_EVENT_QUIT || (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED
                && event.window.windowID == SDL_GetWindowID(window))) running = false;
        }

        float scale = SDL_GetDisplayContentScale(SDL_GetDisplayForWindow(window));
        if (!std::isfinite(scale) || scale <= 0) scale = 1;

        scale = std::max(1.0f, std::round(scale));
        if (std::abs(scale - currentScale) > 0.001f) {
            ImGui::GetStyle() = baseStyle;
            ImGui::GetStyle().ScaleAllSizes(scale);
            ImGui::GetStyle().FontScaleDpi = scale;
            currentScale = scale;
        }
        ImGui_ImplOpenGL3_NewFrame(); ImGui_ImplSDL3_NewFrame(); ImGui::NewFrame();
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);

        ImGui::Begin("Laboratori 2",nullptr,ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize
            | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);
        ImGui::TextUnformatted("Laboratori 2 · Rotacions · Tardor 2026");
        ImGui::TextUnformatted("Angles a la UI en graus; funcions dels exercicis en radians.");
        bool changed=ImGui::Combo("Operació",&selected,tasks,IM_ARRAYSIZE(tasks));
        ImGui::Combo("Exemple",&example,examples,IM_ARRAYSIZE(examples));
        ImGui::SameLine();
        if (ImGui::Button("Carrega exemple")) { input=exampleInput(example); changed=true; }
        const Task task=static_cast<Task>(selected);
        const bool axisInput=task==Task::AxisMatrix || task==Task::AxisQuat;
        const bool matrixInput=task==Task::MatrixAxis || task==Task::MatrixQuat || task==Task::MatrixEuler
            || task==Task::MatrixVector || task==Task::IsRotation;
        const bool quatInput=task==Task::QuatProduct || task==Task::QuatAxis || task==Task::QuatMatrix || task==Task::QuatVector;
        const bool vectorInput=task==Task::MatrixVector || task==Task::QuatVector;
        if (ImGui::BeginTable("panells",2,ImGuiTableFlags_Resizable)) {
            ImGui::TableNextColumn();
            ImGui::BeginChild("entrades",ImVec2(0,0));
            ImGui::SeparatorText("Dades");
            if (axisInput) {
                changed|=vectorEdit("Eix u (es normalitza)",input.axis);
                changed|=angleEdit("Angle (graus)",input.angle);
            }
            if (matrixInput) changed|=matrixTable("Matriu R",input.matrix,true);
            if (quatInput) changed|=quatEdit("Quaternió q1 = (s,x,y,z)",input.first);
            if (task==Task::QuatProduct) changed|=quatEdit("Quaternió q2",input.second);
            if (vectorInput) changed|=vectorEdit("Vector v",input.vector);
            if (task==Task::EulerMatrix) {
                changed|=angleEdit("Yaw (psi, Z)",input.yaw);
                changed|=angleEdit("Pitch (theta, Y local)",input.pitch);
                changed|=angleEdit("Roll (phi, X local)",input.roll);
            }
            ImGui::TextWrapped("ZYX intrínseca: R = Rz(yaw) Ry(pitch) Rx(roll). Vectors columna; base local -> món.");
            ImGui::EndChild();
            ImGui::TableNextColumn();
            ImGui::BeginChild("resultats",ImVec2(0,0));
            if (changed) { hasReport=false; error.clear(); }
            if (ImGui::Button("Executa l'operació")) {
                try { report=runTask(task,input); hasReport=true; error.clear(); }
                catch (const std::exception& exception) { error=exception.what(); hasReport=false; }
            }
            if (!error.empty()) ImGui::TextWrapped("Error: %s",error.c_str());
            if (hasReport) {
                ImGui::TextColored(report.correct ? ImVec4(0.4f,0.9f,0.5f,1) : ImVec4(1,0.45f,0.35f,1),
                    "Resultat: %s",report.correct ? "CORRECTE" : "REVISAR");
                if (!report.detail.empty()) ImGui::TextWrapped("%s",report.detail.c_str());
                if (report.completed) {
                    ImGui::Text("Temps: %.6f ms (informatiu)",report.milliseconds);
                    ImGui::Text("Error: %.3e; tolerància: 5e-6",report.error);
                    if (report.countsApplicable) {
                        const auto& op=report.operations;
                        ImGui::Text("+ %llu | - %llu | * %llu | / %llu",
                            static_cast<unsigned long long>(op.add),static_cast<unsigned long long>(op.subtract),
                            static_cast<unsigned long long>(op.multiply),static_cast<unsigned long long>(op.divide));
                        ImGui::TextWrapped("Recompte habitual: %s. Altres algorismes poden tenir costos diferents: justifiqueu-los.",
                            report.countsExpected ? "COINCIDEIX" : "DIFERENT");
                    }
                    const bool matrixResult=task==Task::AxisMatrix || task==Task::EulerMatrix || task==Task::QuatMatrix;
                    const bool quatResult=task==Task::AxisQuat || task==Task::MatrixQuat || task==Task::QuatProduct;
                    if (matrixResult) {
                        matrixTable("R resultat",report.matrix,false);
                        if (ImGui::Button("Copia R a l'entrada")) input.matrix=report.matrix;
                    }
                    if (quatResult) {
                        ImGui::Text("q = (%.9g, %.9g, %.9g, %.9g)",report.quat.s,report.quat.x,report.quat.y,report.quat.z);
                        if (ImGui::Button("Copia q a q1")) { input.first=report.quat; hasReport=false; }
                    }
                    if (task==Task::MatrixAxis || task==Task::QuatAxis) {
                        vectorText("u",report.axis);
                        ImGui::Text("Angle: %.9g graus",report.angle*180/Pi);
                    }
                    if (task==Task::MatrixEuler)
                        ImGui::Text("Yaw: %.9g | Pitch: %.9g | Roll: %.9g graus",
                            report.yaw*180/Pi,report.pitch*180/Pi,report.roll*180/Pi);
                    if (task==Task::IsRotation) ImGui::Text("IsRotation: %s",report.rotation ? "true" : "false");
                    if (vectorInput) { vectorText("v'",report.vector); projection(input.vector,report.vector); }
                }
            }
            ImGui::EndChild();
            ImGui::EndTable();
        }
        ImGui::End(); ImGui::Render();
        int width, height; SDL_GetWindowSizeInPixels(window, &width, &height);
        glViewport(0, 0, width, height); glClearColor(0.10f, 0.11f, 0.14f, 1); glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData()); SDL_GL_SwapWindow(window);
    }
    ImGui_ImplOpenGL3_Shutdown(); ImGui_ImplSDL3_Shutdown(); ImGui::DestroyContext();
    SDL_GL_DestroyContext(context); SDL_DestroyWindow(window); SDL_Quit();
    return 0;
}
