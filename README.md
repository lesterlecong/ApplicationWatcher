## Project Description

This project is used for monitoring applications events and automatically launching if it is closed manually by the user or the system.
Currently only working in Windows OS but having a plan to expand it in Ubuntu OS.

## Compiler used

Windows: MingGW-w64 g++
Please see the Installation process[https://www.msys2.org/]

## Included Library

nlohmann json

### Setting up

```
    git clone https://github.com/lesterlecong/ApplicationWatcher.git
    cd ApplicationWatcher
    git submodule update --init --recursive
    compile.bat
```

### Register provider in Registry

Add "MeldCX" as a registry key under the following key:
**_HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\services\eventlog\Application_**
Make sure you run the **Registry Editor** as admin.
Set the following values:

| Value name           | Type      | Value data                                                             |
| -------------------- | --------- | ---------------------------------------------------------------------- |
| CategoryCount        | REG_DWORD | 0x00000003                                                             |
| CategoryMessageFile  | REG_SZ    | **ApplicationWatcher path**/LoggingHandler/Windows/MeldCX_provider.dll |
| EventMessageFile     | REG_SZ    | **ApplicationWatcher path**/LoggingHandler/Windows/MeldCX_provider.dll |
| ParameterMessageFile | REG_SZ    | **ApplicationWatcher path**/LoggingHandler/Windows/MeldCX_provider.dll |
| TypeSupported        | REG_DWORD | 0x00000007                                                             |
