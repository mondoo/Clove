using System;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Shapes;
using Membrane = garlic.membrane;

namespace Garlic.Bulb {
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window {
        public MainWindow() {
            InitializeComponent();
        }

        public override void OnApplyTemplate() {
            //Set up the functionality of the window control buttons
            if (GetTemplateChild("MinimiseButton") is Button minimizeButton) {
                minimizeButton.Click += (sender, e) => WindowState = WindowState.Minimized;
            }
            if (GetTemplateChild("RestoreButton") is Button restoreButton) {
                restoreButton.Click += (sender, e) => WindowState = (WindowState == WindowState.Normal) ? WindowState.Maximized : WindowState.Normal;
            }
            if (GetTemplateChild("CloseButton") is Button closeButton) {
                closeButton.Click += (sender, e) => Close();
            }

            //Set up the top grid to allow it to be draggable
            if (GetTemplateChild("MoveRectangle") is Rectangle moveRectangle) {
                moveRectangle.MouseDown += MoveRectangle_MouseDown;
            }

            base.OnApplyTemplate();
        }

        private void EditorViewport_Key(object sender, KeyEventArgs e) {
            var message = new Membrane.Editor_ViewportKeyEvent {
                key = e.Key,
                type = e.IsDown ? Membrane.Editor_ViewportKeyEvent.Type.Pressed : Membrane.Editor_ViewportKeyEvent.Type.Released
            };

            Membrane.MessageHandler.sendMessage(message);

            e.Handled = true;
        }

        private void EditorViewport_MouseButton(object sender, MouseButtonEventArgs e) {
            if (!EditorViewport.IsFocused) {
                EditorViewport.Focus();
            }

            var message = new Membrane.Editor_ViewportMouseButtonEvent {
                button = e.ChangedButton,
                state = e.ButtonState,
                position = e.GetPosition(EditorViewport)
            };

            Membrane.MessageHandler.sendMessage(message);

            e.Handled = true;
        }

        //Capture mouse move on the window if the user moves their mouse out of the viewport will moving the camera
        private void Window_MouseMove(object sender, MouseEventArgs e) {
            if (EditorViewport.IsFocused) {
                var message = new Membrane.Editor_ViewportMouseMoveEvent {
                    position = e.GetPosition(EditorViewport)
                };

                Membrane.MessageHandler.sendMessage(message);

                e.Handled = true;
            }
        }

        private void MoveRectangle_MouseDown(object sender, MouseButtonEventArgs e) {
            if(e.LeftButton == MouseButtonState.Pressed) {
                DragMove();
            }
        }
    }
}
