#include "ui.h"


#include <kinc/graphics4/graphics.h>
#include <kinc/system.h>
#include <kinc/window.h>
#include <kinc/display.h>
#include <kinc/log.h>

#include <stdio.h>

#include "imgui.h"
#include "imgui_impl_g4.h"
#include "imgui_impl_kinc.h"

#define THEME_AMOUNT 6
typedef enum {
    Dark,
    Classic,
    Sith,
    Jedi,
    OhMyEyes,
    bindows98
} Themes;

char* theme_names[THEME_AMOUNT] = {
    "Dark",
    "Classic",
    "Sith",
    "Jedi",
    "Oh My Eyes !",
    "bindows98"
};

 typedef struct {
    int w;
    int h;
 } windowSettings;

 typedef struct {
    char* label;
    float width;
} ButtonHelper;


static windowSettings settings;

static void changeTheme(Themes t);

#define V2 ImVec2
static ImGuiStyle* style;

static double delta_time = 0.0;
static double lastTime = 0.0;

static void resize_window(int x, int y, void* data){
    settings.w = x;
    settings.h = y;
}


    static bool show_demo_window = false;

void ui_init(void){

    settings.w = 1024;
    settings.h = 768;
    

   
	kinc_init("uTAS", settings.w, settings.h, NULL, NULL);

    


    kinc_window_set_resize_callback(0, resize_window, &settings);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    io.Fonts->AddFontDefault();
    {
        ImFont* font = io.Fonts->AddFontFromFileTTF("./LiberationSans-Regular.ttf", 12.0f, NULL, io.Fonts->GetGlyphRangesDefault());
        io.Fonts->AddFontFromFileTTF("./LiberationSans-Bold.ttf", 12.0f, NULL, io.Fonts->GetGlyphRangesDefault());

        // Run-length encoding of some icons
        // In retrorespect I should have used an icon map like FONT_ATLAS_DEFAULT_TEX_DATA_PIXELS
        unsigned char minimize[] = { 86,6,6,6,0 };
        unsigned char close[] = { 14,2,4,2,5,2,2,2,7,4,9,2,9,4,7,2,2,2,5,2,4,2,0 };
        unsigned char* run_length[] = { close, minimize };

        int rect_ids[IM_ARRAYSIZE(run_length)];
        for (int i = 0; i < IM_ARRAYSIZE(run_length); i++) {
            // Starts with unicode multiplication sign and extends
            rect_ids[i] = io.Fonts->AddCustomRectFontGlyph(font, 215 + i, 12, 9, 13 + 1);
        }

        io.Fonts->Build();

        unsigned char* tex_pixels = NULL;
        int tex_width, tex_height;
        io.Fonts->GetTexDataAsRGBA32(&tex_pixels, &tex_width, &tex_height);

        for (int i = 0; i < IM_ARRAYSIZE(run_length); i++) {

            int rect_id = rect_ids[i];
            if (const ImFontAtlas::CustomRect* rect = io.Fonts->GetCustomRectByIndex(rect_id)) {
                bool black = false;
                unsigned char* run = run_length[i];
                int run_size = 0;

                for (int y = 0; y < rect->Height; y++)
                {
                    ImU32* p = (ImU32*)tex_pixels + (rect->Y + y) * tex_width + (rect->X);
                    for (int x = rect->Width; x > 0; x--) {
                        if (black) *p = IM_COL32(255, 0, 0, 255);
                        p++;
                        run_size++;
                        if (run_size == *run) {
                            run_size = 0;
                            run++;
                            black = !black;
                            if (*run == 0) goto done;
                        }
                    }
                }
            done:;
            }
        }
    }
    //Setup Dear ImGui style
    changeTheme(Dark);

    // Setup Platform/Renderer bindings
    ImGui_ImplKinc_InitForG4(0);
    ImGui_ImplG4_Init(0);

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error
    // and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which
    // ImGui_ImplXXXX_NewFrame below will call.
    // - Read 'misc/fonts/README.txt' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    // io.Fonts->AddFontDefault();
    // io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    // io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    // io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    // io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
    // ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
    // IM_ASSERT(font != NULL);
    
	lastTime = kinc_time();
}


void ui_window_begin(char* label,int x,int y, int w, int h){
    static bool p_open = true;
	ImGui::Begin(label,&p_open,ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove);
}
void ui_window_end(void){
    ImGui::End();
}

void ui_update(void){
    
    ezc_run();
    
    style = &ImGui::GetStyle();

    delta_time = kinc_time() - lastTime;

    lastTime = kinc_time();

	kinc_g4_begin(0);

	// Start the Dear ImGui frame
	ImGui_ImplG4_NewFrame();
	ImGui_ImplKinc_NewFrame(0);
	ImGui::NewFrame();

	// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	if (show_demo_window)
	    ImGui::ShowDemoWindow(&show_demo_window);

    ImGuiIO& io = ImGui::GetIO();
	{
        ImGui::SetNextWindowPos(V2(0,0));
		ImGui::SetNextWindowSize(V2((float)settings.w,(float)settings.h),ImGuiCond_None);
		

        if(ImGui::BeginMenuBar()){

    //        if (ImGui::BeginMenu("File"))
    //        {
    //            ShowExampleMenuFile();
    //            ImGui::EndMenu();
    //        }
    //        if (ImGui::BeginMenu("Edit"))
    //        {
    //            if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
    //            if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
    //            ImGui::Separator();
    //            if (ImGui::MenuItem("Cut", "CTRL+X")) {}
    //            if (ImGui::MenuItem("Copy", "CTRL+C")) {}
    //            if (ImGui::MenuItem("Paste", "CTRL+V")) {}
    //            ImGui::EndMenu();
    //        }
            if (ImGui::BeginMenu("Themes"))
            {
                for(size_t i =0; i < THEME_AMOUNT;++i){
                    if (ImGui::MenuItem(theme_names[i])) {
                        changeTheme((Themes)i);
                    }
                }
                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }
        
        ezc_draw();

        ImGui::Checkbox("Demo Window", &show_demo_window);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		
	}

	// Rendering
	ImGui::Render();
	/*g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, NULL);*/
	kinc_g4_clear(KINC_G4_CLEAR_COLOR, 0xff0000ff, 0.0f, 0);
	ImGui_ImplG4_RenderDrawData(ImGui::GetDrawData());

	kinc_g4_end(0);
	kinc_g4_swap_buffers();
}

void ui_deinit(bool showed_ui){
    g_object_unref(parser);
    parser = NULL;
    free(script_h->name);
    free(script_h->path);
    free(script_h->buf);
    if (showed_ui) {
        ImGui_ImplG4_Shutdown();
        ImGui_ImplKinc_Shutdown();
        ImGui::DestroyContext();
    }
}




//Drawing funcs


void Circle(ImDrawList* d, V2 a, V2 b, V2 sz, ImVec4 mouse, float t,bool doAnimation)
{
    ImVec4* colors = style->Colors;
    ImVec4 col = colors[ImGuiCol_Button];
    if(ImGui::IsItemHovered()){
        col = colors[ImGuiCol_ButtonHovered];
    }

    d->AddRectFilled(a,b,ImGui::GetColorU32(col),0.0f,ImDrawFlags_None);

    float radius = sz.x * 0.25f;
    if(radius > sz.y){
        radius = sz.y *0.25f;
    }
    d->AddCircleFilled(V2(a.x+sz.x * 0.5f,a.y+sz.y *0.5f),radius,IM_COL32(255, 0, 0,255),0);
    int alpha = 255;
    if(doAnimation){
        alpha = (int)(255 * t);
        if(alpha < 125){
            alpha = 125;
        }
    }
    d->AddCircleFilled(V2(a.x+sz.x * 0.5f,a.y+sz.y *0.5f),radius * 0.90f* t,IM_COL32(141, 2, 31,alpha),0);
}




static bool RecordButton(bool isRecording){
    ImGuiIO& io = ImGui::GetIO();
    ImVec2 size(100.0f, 60.0f);
    bool out = ImGui::InvisibleButton("c_canvas", size);
    if(ImGui::IsItemVisible()){
        ImVec2 p0 = ImGui::GetItemRectMin();
        ImVec2 p1 = ImGui::GetItemRectMax();
        ImDrawList* draw_list = ImGui::GetWindowDrawList();
        draw_list->PushClipRect(p0, p1);

        ImVec4 mouse_data;
        mouse_data.x = (io.MousePos.x - p0.x) / size.x;
        mouse_data.y = (io.MousePos.y - p0.y) / size.y;
        mouse_data.z = io.MouseDownDuration[0];
        mouse_data.w = io.MouseDownDuration[1];


        if(isRecording && c_t >= 1.0f){
            c_t = 0.0f;
        }
        else if(isRecording){
            c_t+= delta_time;
        }

        Circle(draw_list, p0, p1, size, mouse_data,c_t,isRecording);
        draw_list->PopClipRect();
    }

    return out;
}

void Triangle(ImDrawList* d, V2 a, V2 b, V2 sz, ImVec4 mouse, float t,bool doAnimation)
{
    ImVec4* colors = style->Colors;
    ImVec4 col = colors[ImGuiCol_Button];
    if(ImGui::IsItemHovered() || doAnimation){
        col = colors[ImGuiCol_ButtonHovered];
    }

    int alpha = 255;
    if(doAnimation){
        alpha = (int)(255 * t);
        if(alpha < 125){
            alpha = 125;
        }
        col.w *= t_t;
        if(col.w < 0.2f){
            col.w = 0.2f;
        }
    }

    d->AddRectFilled(a,b,ImGui::GetColorU32(col),0.0f,ImDrawFlags_None);

    d->AddTriangleFilled(V2(a.x + sz.x * 0.35f,a.y + style->FramePadding.x),V2(a.x + sz.x * 0.35f,b.y - style->FramePadding.x),V2(b.x - sz.x * 0.2f,a.y + sz.y *0.5f),IM_COL32(0,128,0,255));

    d->AddTriangleFilled(V2(a.x + sz.x * 0.35f+ style->FramePadding.x *0.5f,a.y + style->FramePadding.x * 2.0f),V2(a.x + sz.x * 0.35f + style->FramePadding.x * 0.5f,b.y - style->FramePadding.x * 2.0f ),V2(b.x - sz.x * 0.2f - style->FramePadding.x,a.y + sz.y *0.5f),IM_COL32(0,100,0,alpha));
}

static bool goDown = false;
static bool PlayButton(bool isPlaying){
    ImGuiIO& io = ImGui::GetIO();
    ImVec2 size(100.0f, 60.0f);
    bool out = ImGui::InvisibleButton("t_canvas", size);
    if(ImGui::IsItemVisible()){
        ImVec2 p0 = ImGui::GetItemRectMin();
        ImVec2 p1 = ImGui::GetItemRectMax();
        ImDrawList* draw_list = ImGui::GetWindowDrawList();
        draw_list->PushClipRect(p0, p1);

        ImVec4 mouse_data;
        mouse_data.x = (io.MousePos.x - p0.x) / size.x;
        mouse_data.y = (io.MousePos.y - p0.y) / size.y;
        mouse_data.z = io.MouseDownDuration[0];
        mouse_data.w = io.MouseDownDuration[1];


        if(isPlaying && t_t >= 1.0f){
            goDown = true;
        }
        else if(isPlaying && t_t < 0.0f){
            goDown = false;
        }

        if(isPlaying && !goDown){
            t_t+= delta_time;
        }
        else if(isPlaying && goDown){
            t_t-= delta_time;
        }

        Triangle(draw_list, p0, p1, size, mouse_data,t_t,isPlaying);
        draw_list->PopClipRect();
    }

    return out;
}


static void changeTheme(Themes t){
    ImGuiIO& io = ImGui::GetIO();
    ImVec4* colors = ImGui::GetStyle().Colors;
    switch(t){
        case Classic:
            io.FontDefault = io.Fonts->Fonts[0];
            ImGui::StyleColorsDark();
            break;
        case OhMyEyes:
            for (int n = 0; n < io.Fonts->Fonts.Size; n++){
                if (strcmp(io.Fonts->ConfigData[n].Name, "LiberationSans-Bold")) {
                    io.FontDefault = io.Fonts->Fonts[n];
                }
            }
            ImGui::StyleColorsLight();
            break;
        case Dark:
            for (int n = 0; n < io.Fonts->Fonts.Size; n++){
                if (strcmp(io.Fonts->ConfigData[n].Name, "LiberationSans-Bold")) {
                    io.FontDefault = io.Fonts->Fonts[n];
                }
            }
            colors[ImGuiCol_Text]                  = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
            colors[ImGuiCol_TextDisabled]          = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
            colors[ImGuiCol_WindowBg]              = ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
            colors[ImGuiCol_ChildBg]               = ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
            colors[ImGuiCol_PopupBg]               = ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
            colors[ImGuiCol_Border]                = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
            colors[ImGuiCol_BorderShadow]          = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
            colors[ImGuiCol_FrameBg]               = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
            colors[ImGuiCol_FrameBgHovered]        = ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
            colors[ImGuiCol_FrameBgActive]         = ImVec4(0.67f, 0.67f, 0.67f, 0.39f);
            colors[ImGuiCol_TitleBg]               = ImVec4(0.08f, 0.08f, 0.09f, 1.00f);
            colors[ImGuiCol_TitleBgActive]         = ImVec4(0.08f, 0.08f, 0.09f, 1.00f);
            colors[ImGuiCol_TitleBgCollapsed]      = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
            colors[ImGuiCol_MenuBarBg]             = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
            colors[ImGuiCol_ScrollbarBg]           = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
            colors[ImGuiCol_ScrollbarGrab]         = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
            colors[ImGuiCol_ScrollbarGrabHovered]  = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
            colors[ImGuiCol_ScrollbarGrabActive]   = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
            colors[ImGuiCol_CheckMark]             = ImVec4(0.11f, 0.64f, 0.92f, 1.00f);
            colors[ImGuiCol_SliderGrab]            = ImVec4(0.11f, 0.64f, 0.92f, 1.00f);
            colors[ImGuiCol_SliderGrabActive]      = ImVec4(0.08f, 0.50f, 0.72f, 1.00f);
            colors[ImGuiCol_Button]                = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
            colors[ImGuiCol_ButtonHovered]         = ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
            colors[ImGuiCol_ButtonActive]          = ImVec4(0.67f, 0.67f, 0.67f, 0.39f);
            colors[ImGuiCol_Header]                = ImVec4(0.22f, 0.22f, 0.22f, 1.00f);
            colors[ImGuiCol_HeaderHovered]         = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
            colors[ImGuiCol_HeaderActive]          = ImVec4(0.67f, 0.67f, 0.67f, 0.39f);
            colors[ImGuiCol_Separator]             = colors[ImGuiCol_Border];
            colors[ImGuiCol_SeparatorHovered]      = ImVec4(0.41f, 0.42f, 0.44f, 1.00f);
            colors[ImGuiCol_SeparatorActive]       = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
            colors[ImGuiCol_ResizeGrip]            = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
            colors[ImGuiCol_ResizeGripHovered]     = ImVec4(0.29f, 0.30f, 0.31f, 0.67f);
            colors[ImGuiCol_ResizeGripActive]      = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
            colors[ImGuiCol_Tab]                   = ImVec4(0.08f, 0.08f, 0.09f, 0.83f);
            colors[ImGuiCol_TabHovered]            = ImVec4(0.33f, 0.34f, 0.36f, 0.83f);
            colors[ImGuiCol_TabActive]             = ImVec4(0.23f, 0.23f, 0.24f, 1.00f);
            colors[ImGuiCol_TabUnfocused]          = ImVec4(0.08f, 0.08f, 0.09f, 1.00f);
            colors[ImGuiCol_TabUnfocusedActive]    = ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
            colors[ImGuiCol_PlotLines]             = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
            colors[ImGuiCol_PlotLinesHovered]      = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
            colors[ImGuiCol_PlotHistogram]         = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
            colors[ImGuiCol_PlotHistogramHovered]  = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
            colors[ImGuiCol_TextSelectedBg]        = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
            colors[ImGuiCol_DragDropTarget]        = ImVec4(0.11f, 0.64f, 0.92f, 1.00f);
            colors[ImGuiCol_NavHighlight]          = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
            colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
            colors[ImGuiCol_NavWindowingDimBg]     = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
            colors[ImGuiCol_ModalWindowDimBg]      = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
            ImGui::GetStyle().GrabRounding         = ImGui::GetStyle().FrameRounding = 2.3f;
//            colors[ImGuiCol_DockingPreview]        = ImVec4(0.26f, 0.59f, 0.98f, 0.70f);
//            colors[ImGuiCol_DockingEmptyBg]        = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
            break;
        case Sith:
            for (int n = 0; n < io.Fonts->Fonts.Size; n++){
                if (strcmp(io.Fonts->ConfigData[n].Name, "LiberationSans-Bold")) {
                    io.FontDefault = io.Fonts->Fonts[n];
                }
            }
            colors[ImGuiCol_Text]                   = ImVec4(0.75f, 0.75f, 0.75f, 1.00f);
            colors[ImGuiCol_TextDisabled]           = ImVec4(0.35f, 0.35f, 0.35f, 1.00f);
            colors[ImGuiCol_WindowBg]               = ImVec4(0.00f, 0.00f, 0.00f, 0.94f);
            colors[ImGuiCol_ChildBg]                = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
            colors[ImGuiCol_PopupBg]                = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
            colors[ImGuiCol_Border]                 = ImVec4(0.00f, 0.00f, 0.00f, 0.50f);
            colors[ImGuiCol_BorderShadow]           = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
            colors[ImGuiCol_FrameBg]                = ImVec4(0.00f, 0.00f, 0.00f, 0.54f);
            colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.37f, 0.14f, 0.14f, 0.67f);
            colors[ImGuiCol_FrameBgActive]          = ImVec4(0.39f, 0.20f, 0.20f, 0.67f);
            colors[ImGuiCol_TitleBg]                = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
            colors[ImGuiCol_TitleBgActive]          = ImVec4(0.48f, 0.16f, 0.16f, 1.00f);
            colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(0.48f, 0.16f, 0.16f, 1.00f);
            colors[ImGuiCol_MenuBarBg]              = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
            colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
            colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
            colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
            colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
            colors[ImGuiCol_CheckMark]              = ImVec4(0.56f, 0.10f, 0.10f, 1.00f);
            colors[ImGuiCol_SliderGrab]             = ImVec4(1.00f, 0.19f, 0.19f, 0.40f);
            colors[ImGuiCol_SliderGrabActive]       = ImVec4(0.89f, 0.00f, 0.19f, 1.00f);
            colors[ImGuiCol_Button]                 = ImVec4(1.00f, 0.19f, 0.19f, 0.40f);
            colors[ImGuiCol_ButtonHovered]          = ImVec4(0.80f, 0.17f, 0.00f, 1.00f);
            colors[ImGuiCol_ButtonActive]           = ImVec4(0.89f, 0.00f, 0.19f, 1.00f);
            colors[ImGuiCol_Header]                 = ImVec4(0.33f, 0.35f, 0.36f, 0.53f);
            colors[ImGuiCol_HeaderHovered]          = ImVec4(0.76f, 0.28f, 0.44f, 0.67f);
            colors[ImGuiCol_HeaderActive]           = ImVec4(0.47f, 0.47f, 0.47f, 0.67f);
            colors[ImGuiCol_Separator]              = ImVec4(0.32f, 0.32f, 0.32f, 1.00f);
            colors[ImGuiCol_SeparatorHovered]       = ImVec4(0.32f, 0.32f, 0.32f, 1.00f);
            colors[ImGuiCol_SeparatorActive]        = ImVec4(0.32f, 0.32f, 0.32f, 1.00f);
            colors[ImGuiCol_ResizeGrip]             = ImVec4(1.00f, 1.00f, 1.00f, 0.85f);
            colors[ImGuiCol_ResizeGripHovered]      = ImVec4(1.00f, 1.00f, 1.00f, 0.60f);
            colors[ImGuiCol_ResizeGripActive]       = ImVec4(1.00f, 1.00f, 1.00f, 0.90f);
            colors[ImGuiCol_Tab]                    = ImVec4(0.07f, 0.07f, 0.07f, 0.51f);
            colors[ImGuiCol_TabHovered]             = ImVec4(0.86f, 0.23f, 0.43f, 0.67f);
            colors[ImGuiCol_TabActive]              = ImVec4(0.19f, 0.19f, 0.19f, 0.57f);
            colors[ImGuiCol_TabUnfocused]           = ImVec4(0.05f, 0.05f, 0.05f, 0.90f);
            colors[ImGuiCol_TabUnfocusedActive]     = ImVec4(0.13f, 0.13f, 0.13f, 0.74f);
//            colors[ImGuiCol_DockingPreview]         = ImVec4(0.47f, 0.47f, 0.47f, 0.47f);
//            colors[ImGuiCol_DockingEmptyBg]         = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
            colors[ImGuiCol_PlotLines]              = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
            colors[ImGuiCol_PlotLinesHovered]       = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
            colors[ImGuiCol_PlotHistogram]          = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
            colors[ImGuiCol_PlotHistogramHovered]   = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
            colors[ImGuiCol_TableHeaderBg]          = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
            colors[ImGuiCol_TableBorderStrong]      = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);
            colors[ImGuiCol_TableBorderLight]       = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);
            colors[ImGuiCol_TableRowBg]             = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
            colors[ImGuiCol_TableRowBgAlt]          = ImVec4(1.00f, 1.00f, 1.00f, 0.07f);
            colors[ImGuiCol_TextSelectedBg]         = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
            colors[ImGuiCol_DragDropTarget]         = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
            colors[ImGuiCol_NavHighlight]           = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
            colors[ImGuiCol_NavWindowingHighlight]  = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
            colors[ImGuiCol_NavWindowingDimBg]      = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
            colors[ImGuiCol_ModalWindowDimBg]       = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
            break;
        case Jedi:
            for (int n = 0; n < io.Fonts->Fonts.Size; n++){
                if (strcmp(io.Fonts->ConfigData[n].Name, "LiberationSans-Bold")) {
                    io.FontDefault = io.Fonts->Fonts[n];
                }
            }
            colors[ImGuiCol_Text]                   = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
            colors[ImGuiCol_TextDisabled]           = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
            colors[ImGuiCol_WindowBg]               = ImVec4(0.06f, 0.06f, 0.06f, 0.94f);
            colors[ImGuiCol_ChildBg]                = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
            colors[ImGuiCol_PopupBg]                = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
            colors[ImGuiCol_Border]                 = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
            colors[ImGuiCol_BorderShadow]           = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
            colors[ImGuiCol_FrameBg]                = ImVec4(0.44f, 0.44f, 0.44f, 0.60f);
            colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.57f, 0.57f, 0.57f, 0.70f);
            colors[ImGuiCol_FrameBgActive]          = ImVec4(0.76f, 0.76f, 0.76f, 0.80f);
            colors[ImGuiCol_TitleBg]                = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
            colors[ImGuiCol_TitleBgActive]          = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
            colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(0.00f, 0.00f, 0.00f, 0.60f);
            colors[ImGuiCol_MenuBarBg]              = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
            colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
            colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
            colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
            colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
            colors[ImGuiCol_CheckMark]              = ImVec4(0.13f, 0.75f, 0.55f, 0.80f);
            colors[ImGuiCol_SliderGrab]             = ImVec4(0.13f, 0.75f, 0.75f, 0.80f);
            colors[ImGuiCol_SliderGrabActive]       = ImVec4(0.13f, 0.75f, 1.00f, 0.80f);
            colors[ImGuiCol_Button]                 = ImVec4(0.13f, 0.75f, 0.55f, 0.40f);
            colors[ImGuiCol_ButtonHovered]          = ImVec4(0.13f, 0.75f, 0.75f, 0.60f);
            colors[ImGuiCol_ButtonActive]           = ImVec4(0.13f, 0.75f, 1.00f, 0.80f);
            colors[ImGuiCol_Header]                 = ImVec4(0.13f, 0.75f, 0.55f, 0.40f);
            colors[ImGuiCol_HeaderHovered]          = ImVec4(0.13f, 0.75f, 0.75f, 0.60f);
            colors[ImGuiCol_HeaderActive]           = ImVec4(0.13f, 0.75f, 1.00f, 0.80f);
            colors[ImGuiCol_Separator]              = ImVec4(0.13f, 0.75f, 0.55f, 0.40f);
            colors[ImGuiCol_SeparatorHovered]       = ImVec4(0.13f, 0.75f, 0.75f, 0.60f);
            colors[ImGuiCol_SeparatorActive]        = ImVec4(0.13f, 0.75f, 1.00f, 0.80f);
            colors[ImGuiCol_ResizeGrip]             = ImVec4(0.13f, 0.75f, 0.55f, 0.40f);
            colors[ImGuiCol_ResizeGripHovered]      = ImVec4(0.13f, 0.75f, 0.75f, 0.60f);
            colors[ImGuiCol_ResizeGripActive]       = ImVec4(0.13f, 0.75f, 1.00f, 0.80f);
            colors[ImGuiCol_Tab]                    = ImVec4(0.13f, 0.75f, 0.55f, 0.80f);
            colors[ImGuiCol_TabHovered]             = ImVec4(0.13f, 0.75f, 0.75f, 0.80f);
            colors[ImGuiCol_TabActive]              = ImVec4(0.13f, 0.75f, 1.00f, 0.80f);
            colors[ImGuiCol_TabUnfocused]           = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);
            colors[ImGuiCol_TabUnfocusedActive]     = ImVec4(0.36f, 0.36f, 0.36f, 0.54f);
//            colors[ImGuiCol_DockingPreview]         = ImVec4(0.13f, 0.75f, 0.55f, 0.80f);
//            colors[ImGuiCol_DockingEmptyBg]         = ImVec4(0.13f, 0.13f, 0.13f, 0.80f);
            colors[ImGuiCol_PlotLines]              = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
            colors[ImGuiCol_PlotLinesHovered]       = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
            colors[ImGuiCol_PlotHistogram]          = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
            colors[ImGuiCol_PlotHistogramHovered]   = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
            colors[ImGuiCol_TableHeaderBg]          = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
            colors[ImGuiCol_TableBorderStrong]      = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);
            colors[ImGuiCol_TableBorderLight]       = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);
            colors[ImGuiCol_TableRowBg]             = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
            colors[ImGuiCol_TableRowBgAlt]          = ImVec4(1.00f, 1.00f, 1.00f, 0.07f);
            colors[ImGuiCol_TextSelectedBg]         = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
            colors[ImGuiCol_DragDropTarget]         = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
            colors[ImGuiCol_NavHighlight]           = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
            colors[ImGuiCol_NavWindowingHighlight]  = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
            colors[ImGuiCol_NavWindowingDimBg]      = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
            colors[ImGuiCol_ModalWindowDimBg]       = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
            break;
        case bindows98:
        {
            ImGui::GetStyle().FrameBorderSize = 1.0f;
            ImGui::GetStyle().FramePadding = ImVec2(4.0f, 4.0f);
//            ImGui::GetStyle().WindowMenuButtonPosition = ImGuiDir_Right;
            ImGui::GetStyle().ScrollbarSize = 16.0f;

            colors[ImGuiCol_Text]                   = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
            colors[ImGuiCol_TextDisabled]           = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
            colors[ImGuiCol_WindowBg]               = ImVec4(0.75f, 0.75f, 0.75f, 1.00f);
            colors[ImGuiCol_ChildBg]                = ImVec4(0.75f, 0.75f, 0.75f, 1.00f);
            colors[ImGuiCol_PopupBg]                = ImVec4(0.75f, 0.75f, 0.75f, 1.00f);
            colors[ImGuiCol_Border]                 = ImVec4(0.00f, 0.00f, 0.00f, 0.30f);
            colors[ImGuiCol_BorderShadow]           = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
            colors[ImGuiCol_FrameBg]                = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
            colors[ImGuiCol_FrameBgHovered]         = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
            colors[ImGuiCol_FrameBgActive]          = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
            colors[ImGuiCol_TitleBg]                = ImVec4(0.96f, 0.96f, 0.96f, 1.00f);
            colors[ImGuiCol_TitleBgActive]          = ImVec4(0.00f, 0.00f, 0.50f, 1.00f);
            colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(1.00f, 1.00f, 1.00f, 0.51f);
            colors[ImGuiCol_MenuBarBg]              = ImVec4(0.86f, 0.86f, 0.86f, 1.00f);
            colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.98f, 0.98f, 0.98f, 0.53f);
            colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.69f, 0.69f, 0.69f, 0.80f);
            colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(0.49f, 0.49f, 0.49f, 0.80f);
            colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(0.49f, 0.49f, 0.49f, 1.00f);
            colors[ImGuiCol_CheckMark]              = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
            colors[ImGuiCol_SliderGrab]             = ImVec4(0.26f, 0.59f, 0.98f, 0.78f);
            colors[ImGuiCol_SliderGrabActive]       = ImVec4(0.46f, 0.54f, 0.80f, 0.60f);
            colors[ImGuiCol_Button]                 = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
            colors[ImGuiCol_ButtonHovered]          = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
            colors[ImGuiCol_ButtonActive]           = ImVec4(0.06f, 0.53f, 0.98f, 1.00f);
            colors[ImGuiCol_Header]                 = ImVec4(0.26f, 0.59f, 0.98f, 0.31f);
            colors[ImGuiCol_HeaderHovered]          = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
            colors[ImGuiCol_HeaderActive]           = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
            colors[ImGuiCol_Separator]              = ImVec4(0.39f, 0.39f, 0.39f, 0.62f);
            colors[ImGuiCol_SeparatorHovered]       = ImVec4(0.14f, 0.44f, 0.80f, 0.78f);
            colors[ImGuiCol_SeparatorActive]        = ImVec4(0.14f, 0.44f, 0.80f, 1.00f);
            colors[ImGuiCol_ResizeGrip]             = ImVec4(0.80f, 0.80f, 0.80f, 0.56f);
            colors[ImGuiCol_ResizeGripHovered]      = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
            colors[ImGuiCol_ResizeGripActive]       = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
            colors[ImGuiCol_Tab]                    = ImVec4(0.76f, 0.80f, 0.84f, 0.95f);
            colors[ImGuiCol_TabHovered]             = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
            colors[ImGuiCol_TabActive]              = ImVec4(0.60f, 0.73f, 0.88f, 0.95f);
            colors[ImGuiCol_TabUnfocused]           = ImVec4(0.92f, 0.92f, 0.94f, 0.95f);
            colors[ImGuiCol_TabUnfocusedActive]     = ImVec4(0.74f, 0.82f, 0.91f, 1.00f);
            colors[ImGuiCol_PlotLines]              = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
            colors[ImGuiCol_PlotLinesHovered]       = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
            colors[ImGuiCol_PlotHistogram]          = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
            colors[ImGuiCol_PlotHistogramHovered]   = ImVec4(1.00f, 0.45f, 0.00f, 1.00f);
            colors[ImGuiCol_TextSelectedBg]         = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
            colors[ImGuiCol_DragDropTarget]         = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
            colors[ImGuiCol_NavHighlight]           = colors[ImGuiCol_HeaderHovered];
            colors[ImGuiCol_NavWindowingHighlight]  = ImVec4(0.70f, 0.70f, 0.70f, 0.70f);
            colors[ImGuiCol_NavWindowingDimBg]      = ImVec4(0.20f, 0.20f, 0.20f, 0.20f);
            colors[ImGuiCol_ModalWindowDimBg]       = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);

            for (int n = 0; n < io.Fonts->Fonts.Size; n++){
                if (strcmp(io.Fonts->ConfigData[n].Name, "LiberationSans-Regular")) {
                    io.FontDefault = io.Fonts->Fonts[n];
                }
            }
            break;
        }
        default:
            break;
    }
    style = &ImGui::GetStyle();
}
