using System;
using System.Runtime.InteropServices;

namespace Scape.Engine
{
    public class Application
    {
        private IntPtr app;

        public Application() => app = application_create();

        ~Application() => application_destroy(app);

        public void start() => application_start(app);

        public void run() => application_run(app);

        #region Membrane functions
        [DllImport(Import.MembraneDll)]
        private static extern IntPtr application_create();
        [DllImport(Import.MembraneDll)]
        private static extern void application_destroy(IntPtr app);
        [DllImport(Import.MembraneDll)]
        private static extern void application_start(IntPtr app);
        [DllImport(Import.MembraneDll)]
        private static extern void application_run(IntPtr app);
        #endregion
    }
}