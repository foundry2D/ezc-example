const system = platform === Platform.Windows ? "win32" :
			   platform === Platform.Linux   ? "linux" :
			   platform === Platform.OSX     ? "macos" :
			   platform === Platform.HTML5   ? "html5" :
			   platform === Platform.Android ? "android" :
			   platform === Platform.iOS     ? "ios" :
			   								   "unknown";


let project = new Project('ezc-example');

if(system === "win32"){
	project.addDefine('IMGUI_USER_CONFIG="imguiconfig.h"');
}
project.addIncludeDir('include');
project.addIncludeDir('imgui');
project.addFiles(
	'imgui/imgui.cpp',
	'imgui/imgui.h',
	'imgui/imgui_draw.cpp',
	'imgui/imgui_tables.cpp',
	'imgui/imgui_widgets.cpp',
	'imgui/imgui_demo.cpp'
);

project.addFiles('Sources/**', 'Shaders/**');

project.setDebugDir('Deployment');
resolve(project);
