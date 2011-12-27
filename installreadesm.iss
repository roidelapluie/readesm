#define EXE "{app}\bin\readesm.exe"

[Setup]
AppCopyright=Andreas Gölzer
AppName=Readesm
AppVersion=2011.12
ChangesAssociations=true
DefaultDirName=readesm
DefaultGroupName=Readesm
SetupIconFile=ressources\icons\readesm.ico
WizardImageFile=ressources\windows-integration\installer.bmp
AppId=readesm_isl
OutputBaseFilename=readesm-setup


[Files]
Source: "build/readesm.exe"; DestDir: "{app}\bin";
Source: "build/*.qm"; DestDir: "{app}\translations";
Source: "dlls/*.dll"; DestDir: "{app}\bin";

[Icons]
Name: "{group}\Readesm"; Filename: {#EXE}; WorkingDir: "{app}"
Name: "{userdesktop}\Readesm"; Filename: {#EXE}; WorkingDir: "{app}"

[Registry]
Root: HKCR; Subkey: ".ddd"; ValueType: string; ValueName: ""; ValueData: "x-esm.File"; Flags: uninsdeletevalue
Root: HKCR; Subkey: ".esm"; ValueType: string; ValueName: ""; ValueData: "x-esm.File"; Flags: uninsdeletevalue
Root: HKCR; Subkey: ".tgd"; ValueType: string; ValueName: ""; ValueData: "x-esm.File"; Flags: uninsdeletevalue
Root: HKCR; Subkey: ".add"; ValueType: string; ValueName: ""; ValueData: "x-esm.File"; Flags: uninsdeletevalue
Root: HKCR; Subkey: "x-esm.File"; ValueType: string; ValueName: ""; ValueData: "Digital Tachograph File"; Flags: uninsdeletekey
Root: HKCR; Subkey: "x-esm.File\DefaultIcon"; ValueType: string; ValueName: ""; ValueData: "{app}\bin\readesm.exe,0"
Root: HKCR; Subkey: "x-esm.File\shell\open\command"; ValueType: string; ValueName: ""; ValueData: """{#EXE}"" ""%1"""

[Languages]
Name: "en"; MessagesFile: "compiler:Default.isl"
Name: "de"; MessagesFile: "compiler:languages\German.isl"
Name: "hu"; MessagesFile: "compiler:languages\Hungarian.isl"
Name: "ru"; MessagesFile: "compiler:languages\Russian.isl"
Name: "es"; MessagesFile: "compiler:languages\Spanish.isl"
