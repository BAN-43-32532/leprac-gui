#include <windows.h>
#include <tlhelp32.h>
#include <iostream>
#include <vector>
#include <string>

// Function to get the process handle by process name
HANDLE GetProcessHandleByName(const std::string& processName) {
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
            hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pe.th32ProcessID);
            break;
        }
    } while (Process32Next(hSnap, &pe));
    CloseHandle(hSnap);
    return hProcess;
}

// Structure to hold enumeration data
struct EnumData {
    DWORD processId;                // Target process ID
    std::vector<HWND> windowList;   // List to store window handles of the target process
};

// Callback function for enumerating all top-level windows
BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam) {
    EnumData* pData = reinterpret_cast<EnumData*>(lParam);
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
        std::cout << targetProcess << " is not running or there are insufficient privileges." << std::endl;
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
        std::cout << "Found " << windows.size() << " window handle(s):" << std::endl;
        for (HWND hwnd : windows) {
            PrintWindowInfo(hwnd);
        }
    }
    return 0;
}
