using System;
using System.Threading;

namespace Bulb.Core {
	public class LayerTest : Clove.Layer {
		private Clove.Entity cubeEntity;
		private Clove.Entity camEntity;

		private int width;
		private int height;

		public LayerTest(int vpWidth, int vpHeight) {
			width = vpWidth;
			height = vpHeight;
		}

		public override void onAttach() {
			cubeEntity = Clove.Application.createEntity();
			cubeEntity.makeCubeEnt();

			camEntity = Clove.Application.createEntity();
			camEntity.makeCamEnt(width, height);
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

			app.pushLayer(new LayerTest(width, height));

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