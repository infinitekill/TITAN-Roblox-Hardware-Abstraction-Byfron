# **TITAN** (Roblox)

[![Discord](https://img.shields.io/discord/1240608336005828668?label=TITAN%20Softworks&logo=discord&color=5865F2&style=flat)](https://discord.gg/yUWyvT9JyP)

# OVERVIEW

[TITAN's](https://titansoftwork.net) is designed to protect your Main/Alt accounts from Byfron's account detection system's & Roblox's BanAsync component. To use this effectively, a VPN is heavily recomended.

For a more detailed guide, join the Discord above and read the guides provided.

High-Level Overview of Roblox's System's in motion;

![Diagram](./Images/TDiagram.png)

### HOW IT WORKS

The hardware abstraction tool modifies components to disrupt Roblox's components, for more detail the source code is available and the PDB for the executable is also available within the Discord.

<br>

# INSTALL & SETUP
### DOWNLOAD
For prebuilt binaries (.exe's), download the latest version from the Discord ``#spoofer`` channel **[TITAN Discord](https://hub.titansoftwork.com)**. 

### COMPILING REQUIREMENTS:
- **Visual Studio** (Latest Version)  
- **C++ Build Tools** (Install via Visual Studio Installer)  

### BUILDING FROM SOURCE:
1. **Clone the Repository**  
    ```sh
    git clone https://github.com/infinitekill/TITAN-Roblox-Hardware-Abstraction-Byfron.git
    ```

2. **Open the Solution File (`.sln`)**  
    - Navigate to the cloned repository.  
    - Open `TITAN Spoofer.sln` using **Visual Studio**.
    - If it ask you to resolve missing components issues, change them from **Ignore** to **Retarget to Platform Toolset** 

3. **Build the Project**  
    - Choose your build configuration. Using **Debug** instead of **Release** is recommended.
    - Click **Build Solution**.  
    - The compiled executable (`tspf.exe`) will be located in the `/Release` or `/Debug` directory depending on your build configuration.
<br>

# API

### TITAN DLL
TITAN.dll provides these exports:
```C++
extern "C" __declspec(dllexport) void RunSpoofer()
extern "C" __declspec(dllexport) void KillRoblox()
extern "C" __declspec(dllexport) void SpoofMAC()
extern "C" __declspec(dllexport) void CleanFS()
extern "C" __declspec(dllexport) void SpoofRegistry()
extern "C" __declspec(dllexport) void SpoofWMI()
```
See ``DLL/DLLmain.cpp`` for in-depth

### TITAN.h

#### **Example Usage**

```cpp
#include "TITAN.h"

std::thread TitanThread = TitanSpoofer::run(true);

TitanThread.join();
```

### API REF
#### FUNCTION `TitanSpoofer::run(bool logs)`
- **Params:**
  - `logs` (`true`/`false`): Enables or disables logging. If `false`, suppresses all `std::cout` output except critical errors.  
- **Return Value:** A `std::thread` object executing the spoofing process asynchronously.  


### IMPORTANT NOTES
- The Spoofer does NOT unban you from specific games OR on-site bans (Eg; Roblox website bans)

## SUPPORT
For problems, open a support ticket via the **[TITAN Discord](https://hub.titansoftwork.com)**.  

## License

TITAN Spoofer is licensed under Apache 2.0 with the Commons Clause.

- You may use, modify, and redistribute the software with attribution.
- You may **not Sell** the software or any service whose value derives substantially from it.
- Commercial use is prohibited unless you obtain explicit written permission from TITAN Softwork Solutions.

## LEGAL
This software is provided for **educational and research purposes only**. The use of this tool to **circumvent security protections** or violate the terms of service of **Roblox or any other platform** is strictly prohibited. The developers **do not endorse or condone** any illegal activities and assume no liability for misuse.
