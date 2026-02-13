/*
 * Project: TITAN Spoofer
 * Codename: TSPF
 * Author: Damon
 * License: CC BY-NC-ND 4.0
 * Version: V2.0.0
 * Last Updated: 2025-08-18
 */

#include "Container/Services/Services.hpp"

#include "Container/Header/TraceCleaner.h"
#include "Container/Header/Installer.h"
#include "Container/Header/Mac.h"
#include "Container/Header/Registry.h"
#include "Container/Header/WMI.h"
#include "Container/Header/Watchdog.h"
#include "Container/Header/pMask.h"

#include "Container/System/Notify.h"

#include <iostream>
#include <thread>
#include <tlhelp32.h>
#include <wchar.h>   

bool IsRobloxRunning() {
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot == INVALID_HANDLE_VALUE) {
        return false;  // Assume not running on error
    }

    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32);

    if (!Process32First(snapshot, &pe32)) {
        CloseHandle(snapshot);
        return false;
    }

    bool found = false;
    do {
        if (_wcsicmp(pe32.szExeFile, L"RobloxPlayerBeta.exe") == 0) {
            found = true;
            break;
        }
    } while (Process32Next(snapshot, &pe32));

    CloseHandle(snapshot);
    return found;
}

int TspfLaunchRoutine(bool quiet) {
    TITAN::TsBlockHandle guard;

    if (TITAN::Notification::HandleProtocolIfPresentAndExitEarly())
        return 0;

    TsService::TITAN();

    TITAN::Notification notif;
    notif.Initialize();

    TITAN::Watchdog wd(L"RobloxPlayerBeta.exe");

    auto runSpoof = [&](const wchar_t* context) {
        bool success = true;
        try {
            TsService::__TerminateRoblox();
            TraceCleaner::run();
            MAC::MacSpoofer::run();
            Registry::RegSpoofer::run();
            WMI::WmiSpoofer::run();
            Installer::Install(wd);

            std::this_thread::sleep_for(std::chrono::seconds(3));
            TsService::__TerminateRoblox();
        }
        catch (...) {
            success = false;
        }

        notif.NotifyDesktop(
            success ? L"Spoof complete" : L"Spoof failed",
            success ? std::wstring(context) : L"One or more operations failed."
        );
    };

    wd.setOnAllExited([&]() {
        wd.pause();
        bool agreed = false;
        if (notif.PromptSpoofConsentAndWait(agreed) && agreed) {
           runSpoof(L"Spoof done.");
        }
        wd.resume();
    });

    if (!wd.start()) {
        if (!quiet)
            std::cerr << "[!] Failed to start watchdog\n";
        return 1;
    }

    if (!quiet)
        std::wcout << L"[*] TITAN Spoofer running... press Ctrl+C to exit.\n";

    if (!IsRobloxRunning()) {
        wd.pause();
        bool agreed = false;
        if (notif.PromptSpoofConsentAndWait(agreed,
            L"Initial spoof ready",
            L"Spoof?"
        ) && agreed) {
            runSpoof(L"Spoof done.");
        } else {
            notif.NotifyDesktop(L"Spoof declined", L"Waiting for Roblox to close.");
        }
        wd.resume();
    } else {
        notif.NotifyDesktop(L"Roblox detected", L"Close it to start the spoofing process.");
    }

    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(10));
    }

    wd.stop(); // unreachable

    return 0;
}

int SafeRun(bool quiet) {
    while (true) {
        try {
            int code = TspfLaunchRoutine(quiet);
            if (code == 0)
                return 0;

            std::cerr << "[!] TspfLaunchRoutine exited with code " << code << ", restarting...\n";

        }
        catch (const std::exception& ex) {
            std::cerr << "[!] Unhandled exception: " << ex.what() << "\n";
        }
        catch (...) {
            std::cerr << "[!] Unknown fatal exception, restarting...\n";
        }

        std::this_thread::sleep_for(std::chrono::seconds(3));
    }
}

// Release (no-console) entrypoint
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    TsService::TsAdjustAccess();
    return TspfLaunchRoutine(true);
}

// Debug (console) entrypoint
int main() {
    TsService::TsAdjustAccess();
    return SafeRun(false);
}
