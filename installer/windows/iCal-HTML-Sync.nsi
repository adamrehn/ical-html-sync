; Include the Modern UI 2 Header File
!include "MUI2.nsh"

; The name of the installer
Name "iCalendar HTML Sync"

; The filename of the installer
OutFile "Install-iCalHtmlSync.exe"

; The default installation directory
InstallDir $PROGRAMFILES\iCalHtmlSync

; Request application privileges for Windows Vista and above
RequestExecutionLevel admin

; Default Settings
ShowInstDetails show
ShowUninstDetails show

;--------------------------------

; Installer Pages
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES

; Uninstaller Pages
!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES
!insertmacro MUI_UNPAGE_FINISH

; Languages
!insertmacro MUI_LANGUAGE "English"

; Version Settings
VIAddVersionKey /LANG=${LANG_ENGLISH} "ProductName" "iCalendar HTML Sync Installer"
VIAddVersionKey /LANG=${LANG_ENGLISH} "Comments" "iCalendar HTML Sync Installer"
VIAddVersionKey /LANG=${LANG_ENGLISH} "CompanyName" "Adam Rehn"
VIAddVersionKey /LANG=${LANG_ENGLISH} "LegalCopyright" "Copyright © 2011-2014, Adam Rehn"
VIAddVersionKey /LANG=${LANG_ENGLISH} "FileDescription" "iCalendar HTML Sync Installer"
VIAddVersionKey /LANG=${LANG_ENGLISH} "FileVersion" "1.0.0.0"
VIProductVersion "1.0.0.0"

;--------------------------------

; Uninstaller instructions
Section "Uninstall"

  ; No reboot required
  SetRebootFlag false
  
  ; Kill the icalsync process if it is currently running
  Processes::KillProcess "icalsync.exe"
  
  ; Delete the registry key
  DeleteRegValue HKCU "Software\Microsoft\Windows\CurrentVersion\Run" "icalsync"
  
  ; Delete the executables
  Delete $INSTDIR\icalsync.exe
  Delete $INSTDIR\ics2html.exe
  
  ; Delete the uninstaller itself
  Delete $INSTDIR\uninstall.exe
  
  ; Delete the config files and directory
  Delete $APPDATA\ical-html-sync\icalsync.conf
  Delete $APPDATA\ical-html-sync\events_template.html
  Delete $APPDATA\ical-html-sync\tasks_template.html
  RMDir  $APPDATA\ical-html-sync
  
  ; Make completely sure the icalsync executable is deleted
  Delete $INSTDIR\icalsync.exe
  
  ; Delete the installation directory
  RMDir $INSTDIR

SectionEnd

; Installer instructions
Section ""

  ; Set output path to the installation directory.
  SetOutPath $INSTDIR
  
  ; Set icalsync to run at Windows startup
  WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Run" "icalsync" "$INSTDIR\icalsync.exe"
  
  ; Install the executables
  File icalsync.exe
  File ics2html.exe
  
  ; Run icalsync
  Exec $INSTDIR\icalsync.exe
  
  ; Write the uninstaller
  WriteUninstaller $INSTDIR\uninstall.exe
  
SectionEnd
