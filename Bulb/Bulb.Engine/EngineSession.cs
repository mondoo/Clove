using System;
using System.Threading;
using Bulb.Engine.Clove;

namespace Bulb.Engine {
	public class EngineSession {
		private static Wrapper wp;
		private static Thread engineThread;

		public static void begin(IntPtr hWnd, int width, int height) {
			wp = new Wrapper();

			engineThread = new Thread(() => wp.OpenClove(hWnd, width, height));
			engineThread.Start();
		}
		public static void end() {
			wp.StopClove();
			engineThread.Join();
		}
	}
}