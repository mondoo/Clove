# escape=`

FROM mcr.microsoft.com/windows/servercore:1809

SHELL ["cmd", "/S", "/C"]

#Get chocolatey
RUN powershell -Command iex ((new-object net.webclient).DownloadString(‘https://chocolatey.org/install.ps1‘));

#Install environment
RUN choco install -y git
RUN choco install -y cmake.install --installargs '"ADD_CMAKE_TO_PATH=System"'
RUN choco install -y python

#Install compiler
ADD https://aka.ms/vs/16/release/vs_buildtools.exe C:\TEMP\vs_buildtools.exe
ADD https://aka.ms/vs/16/release/channel C:\Temp\VisualStudio.chman
RUN C:\TEMP\vs_buildtools.exe --quiet --wait --norestart --nocache `
    --channelUri C:\TEMP\VisualStudio.chman `
    --installChannelUri C:\TEMP\VisualStudio.chman `
    --add Microsoft.VisualStudio.Workload.VCTools --includeRecommended`
    --installPath C:\BuildTools

RUN rd /s /q "C:\TEMP"

CMD ["powershell.exe"]