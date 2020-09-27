using System;
using System.Runtime.InteropServices;

namespace Scape.Engine
{
    public class Application
    {
        #region Membrane functions
        private delegate IntPtr application_create_delegate();
        private delegate void application_destroy_delegate(IntPtr app);
        private delegate void application_start_delegate(IntPtr app);
        private delegate void application_run_delegate(IntPtr app);

        private application_create_delegate application_create;
        private application_destroy_delegate application_destroy;
        private application_start_delegate application_start;
        private application_run_delegate application_run;
        #endregion

        private IntPtr app;
        private IntPtr dll; //TODO: We'd want to load this dll in a global sense. Not per object that requires it

        public Application()
        {
            dll = Native.LoadLibrary(getDllPath());

            application_create = Marshal.GetDelegateForFunctionPointer<application_create_delegate>(Native.GetProcAddress(dll, "application_create"));
            application_destroy = Marshal.GetDelegateForFunctionPointer<application_destroy_delegate>(Native.GetProcAddress(dll, "application_destroy"));

            application_start = Marshal.GetDelegateForFunctionPointer<application_start_delegate>(Native.GetProcAddress(dll, "application_start"));
            application_run = Marshal.GetDelegateForFunctionPointer<application_run_delegate>(Native.GetProcAddress(dll, "application_run"));

            app = application_create();
        }

        ~Application()
        {
            application_destroy(app);

            Native.FreeLibrary(dll);
        }

        public void start() => application_start(app);

        public void run() => application_run(app);

        //TODO: Doing this here but this should be placed into a global class
        private string getDllPath()
        {
            switch (OperatingSystem.GetOS())
            {
                case OS.Windows:
                    return "../../build/lib/Debug/Scape.Membrane.dll";
                    break;
                default:
                    throw new System.NotImplementedException("Missing required OS type");
            }
        }
    }
}