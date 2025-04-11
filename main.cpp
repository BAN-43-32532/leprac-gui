#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

// clang-format off
#include <windows.h>
#include <tlhelp32.h>
// clang-format on

enum GameVersion {
  UNKNOWN = 0,

  Le01 = 1,
  Le02 = 2,
  Le03 = 3,
  Le04 = 4,

  v1_00a = 1 << 3,
  v1_10a = 2 << 3,
  v1_11a = 3 << 3,
  v1_20a = 4 << 3,
  v1_21a = 5 << 3,

  v1_20f = 6 << 3,
  v1_03a = 7 << 3,

  Le01_1_00a = Le01 | v1_00a,
  Le01_1_10a = Le01 | v1_10a,
  Le01_1_11a = Le01 | v1_11a,
  Le01_1_20a = Le01 | v1_20a,

  Le02_1_00a = Le02 | v1_00a,
  Le02_1_10a = Le02 | v1_10a,
  Le02_1_11a = Le02 | v1_11a,
  Le02_1_20a = Le02 | v1_20a,

  Le03_1_00a = Le03 | v1_00a,
  Le03_1_10a = Le03 | v1_10a,
  Le03_1_11a = Le03 | v1_11a,
  Le03_1_20a = Le03 | v1_20a,
  Le03_1_21a = Le03 | v1_21a,
};
std::unordered_map<std::string, GameVersion> versionStringToEnum{
  {"1.00a", v1_00a}, {"1.10a", v1_10a}, {"1.11a", v1_11a},
  {"1.20a", v1_20a}, {"1.21a", v1_21a},
};

GameVersion getGameVersionFromTitle(const std::string &title,
                                    GameVersion gameVersion) {
  size_t last_space = title.find_last_of(' ');
  if (last_space == std::string::npos) {
    return gameVersion;
  }
  std::string version = title.substr(last_space + 1);
  return static_cast<GameVersion>(gameVersion | versionStringToEnum[version]);
}

HANDLE GetProcessHandleByName(const std::string &processName) {
  HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
  if (hSnap == INVALID_HANDLE_VALUE) {
    return nullptr;
  }
  PROCESSENTRY32 pe;
  pe.dwSize = sizeof(PROCESSENTRY32);
  if (!Process32First(hSnap, &pe)) {
    CloseHandle(hSnap);
    return nullptr;
  }
  HANDLE hProcess = nullptr;
  do {
    if (_stricmp(pe.szExeFile, processName.c_str()) == 0) {
      hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE,
                             pe.th32ProcessID);
      break;
    }
  } while (Process32Next(hSnap, &pe));
  CloseHandle(hSnap);
  return hProcess;
}

// Structure to hold enumeration data
struct EnumData {
  DWORD processId; // Target process ID
  std::vector<HWND>
    windowList; // List to store window handles of the target process
};

// Callback function for enumerating all top-level windows
BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam) {
  EnumData *pData = reinterpret_cast<EnumData *>(lParam);
  DWORD windowProcessId = 0;
  GetWindowThreadProcessId(hwnd, &windowProcessId);
  // Check if the window belongs to the target process and is a valid window
  if (windowProcessId == pData->processId && IsWindow(hwnd)) {
    pData->windowList.push_back(hwnd);
  }
  return TRUE; // Continue enumerating
}

// Function to retrieve all top-level window handles for the given process ID
std::vector<HWND> GetProcessWindows(DWORD processId) {
  EnumData data;
  data.processId = processId;
  EnumWindows(EnumWindowsProc, reinterpret_cast<LPARAM>(&data));
  return data.windowList;
}

// Function to print window information
void PrintWindowInfo(HWND hwnd) {
  // Get window title
  char title[256];
  GetWindowTextA(hwnd, title, sizeof(title));

  // Get window class name
  char className[256];
  GetClassNameA(hwnd, className, sizeof(className));

  // Get window styles
  LONG_PTR style = GetWindowLongPtr(hwnd, GWL_STYLE);
  LONG_PTR exStyle = GetWindowLongPtr(hwnd, GWL_EXSTYLE);

  // Get window position and size
  RECT rect;
  GetWindowRect(hwnd, &rect);
  int width = rect.right - rect.left;
  int height = rect.bottom - rect.top;

  // Print window information
  std::cout << "Window Handle: " << hwnd << "\n";
  std::cout << "Title: " << title << "\n";
  std::cout << "Class Name: " << className << "\n";
  std::cout << "Style: 0x" << std::hex << style << "\n";
  std::cout << "Extended Style: 0x" << std::hex << exStyle << "\n";
  std::cout << "Position: (" << rect.left << ", " << rect.top << ")\n";
  std::cout << "Size: " << width << "x" << height << "\n";
  std::cout << "-----------------------------------\n";
}

int main() {
  // Specify the target process name
  std::string targetProcess = "Le01.exe";

  // Retrieve the process handle using the provided function
  HANDLE hProc = GetProcessHandleByName(targetProcess);
  if (!hProc) {
    std::cout << targetProcess
              << " is not running or there are insufficient privileges."
              << std::endl;
    return 1;
  }

  // Retrieve the Process ID from the process handle
  DWORD pid = GetProcessId(hProc);
  std::cout << "Process ID: " << pid << std::endl;

  // Close the process handle if not needed further
  CloseHandle(hProc);

  // Retrieve the window handles associated with the process ID
  std::vector<HWND> windows = GetProcessWindows(pid);
  if (windows.empty()) {
    std::cout << "No window handles found for " << targetProcess << std::endl;
  } else {
    std::cout << "Found " << windows.size()
              << " window handle(s):" << std::endl;
    for (HWND hwnd: windows) {
      PrintWindowInfo(hwnd);
    }
  }
  return 0;
}
