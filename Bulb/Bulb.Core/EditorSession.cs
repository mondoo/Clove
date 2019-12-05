using System;
using System.Threading;

namespace Bulb.Core {
	public class LayerTest : Clove.Layer {
		public override void onAttach() {
			Console.WriteLine("Attached");
		}

		public override void onUpdate(float deltaTime) {
			//Console.WriteLine("Updated");
		}

		public override void onDetach() {
			Console.WriteLine("Detached");
		}
	}

	public class EditorSession {
		private Clove.Application app;
		private Thread appThread;

		public void Begin(IntPtr hWnd, int width, int height) {
			app = new Clove.Application(hWnd, width, height);

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