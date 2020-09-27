namespace Scape.Engine
{
    class Import
    {
        //TODO: Investigate a way to build and pull these files into the current working / build directory

#if SCAPE_PLATFORM_WINDOWS
        public const string MembraneDll = "../../build/lib/Debug/Scape.Membrane.dll";
#elif SCAPE_PLATFORM_MACOS
        public const string MembraneDll = ""; //TODO
#elif SCAPE_PLATFORM_linux
        public const string MembraneDll = ""; //TODO
#endif
    }
}