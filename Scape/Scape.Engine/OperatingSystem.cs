using System.Runtime.InteropServices;

namespace Scape.Engine
{
    enum OS
    {
        Windows,
        MacOS,
        Linux
    }

    class OperatingSystem
    {
        public static bool IsWindows() => RuntimeInformation.IsOSPlatform(OSPlatform.Windows);
        public static bool IsMacOS() => RuntimeInformation.IsOSPlatform(OSPlatform.OSX);
        public static bool IsLinux() => RuntimeInformation.IsOSPlatform(OSPlatform.Linux);

        public static OS GetOS()
        {
            if (IsWindows())
            {
                return OS.Windows;
            }
            else if (IsMacOS())
            {
                return OS.MacOS;
            }
            else if (IsLinux())
            {
                return OS.Linux;
            }
            else
            {
                throw new System.NotImplementedException("Missing required OS type");
            }
        }
    }
}