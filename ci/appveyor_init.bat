7z x "-p%PROXER_DATA%" -o. ".\ci\proxer.data" > nul
echo CONFIG += create_installer >> install.pri
