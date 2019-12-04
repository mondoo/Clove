using System;
using System.Threading;
using Bulb.CloveWrapper;

namespace Bulb.Core {
	public class LayerTest : BulbLayer {
		protected override void onAttach() {
			Console.WriteLine("Attached");
		}

		protected override void onUpdate(float deltaTime) {
			//Console.WriteLine("Updated");
		}

		protected override void onDetach() {
			Console.WriteLine("Detached");
		}
	}

	public class EditorSession {
		private BulbApplication app;
		private Thread appThread;

		public void Begin(IntPtr hWnd, int width, int height) {
			app = new BulbApplication(hWnd, width, height);

			app.start();

			app.pushLayer(new LayerTest());

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