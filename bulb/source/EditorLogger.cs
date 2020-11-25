using System;
using System.IO;
using System.Text;

namespace Garlic.Bulb
{
    public class TextEventArgs : EventArgs
    {
        public char Text { get; }

        public TextEventArgs(char text) => Text = text;
    }

    public class EditorLogger : TextWriter
    {
        public delegate void WriteTextEventHandler(object sender, TextEventArgs e);
        public event WriteTextEventHandler WriteTextEvent;

        public override Encoding Encoding
        {
            get { return Encoding.UTF8; }
        }

        public override void Write(char value)
        {
           WriteTextEvent?.Invoke(this, new TextEventArgs(value));
        }
    }
}