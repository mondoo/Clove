using System;
using System.Threading;
using Bulb.CloveWrapper;

namespace Bulb.Core {
	public class EditorSession {
		private BulbApplication app;
		private Thread appThread;

		public void Begin(IntPtr hWnd, int width, int height) {
			app = new BulbApplication(hWnd, width, height);

			app.start();

			//app->pushLayer(std::make_shared<TestLayer>());

			appThread = new Thread(() => Update());
			appThread.Start();
		}

		public void End() {
			app.stop();
			appThread.Join();
		}

		private void Update() {
			while(app.shouldRun()) {
				app.update();
			}
		}
	}
}