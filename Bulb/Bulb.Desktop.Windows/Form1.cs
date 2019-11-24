using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;
using Bulb.Engine;

namespace WinFormBasic
{
    public partial class Form1 : Form
    {
        private CancellationTokenSource cancelTokenSource;
        private CancellationToken token;

        private readonly Wrapper wp;

        Thread cloveThread;

        public Form1() {
            InitializeComponent();

            cancelTokenSource = new CancellationTokenSource();
            token = cancelTokenSource.Token;

            wp = new Wrapper();
        }

        private void button1_Click(object sender, EventArgs e) {
            RunClove(RenderArea.Handle, RenderArea.Width, RenderArea.Height);
        }

        private void RunClove(IntPtr hWnd, int width, int height) {
            //This is an extremely jank solution to check for the problem
            //Clove will either need to run on another thread 
            //or (more appropriately) not have the while loop in run
            cloveThread = new Thread(() => {
                wp.OpenClove(hWnd, width, height);
            });

            cloveThread.Start();
        }

        private void button2_Click(object sender, EventArgs e) {
            wp.StopClove();
            cloveThread.Join();
        }
    }
}
