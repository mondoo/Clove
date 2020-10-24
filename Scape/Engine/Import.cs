namespace Scape.Engine
{
    class Import
    {
#if SCAPE_PLATFORM_WINDOWS
        public const string MembraneDll = "Scape.Membrane.dll";
#elif SCAPE_PLATFORM_MACOS
        public const string MembraneDll = ""; //TODO
#elif SCAPE_PLATFORM_LINUX
        public const string MembraneDll = "libScape.Membrane.so";
#endif
    }
}