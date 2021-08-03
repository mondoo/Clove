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
        private Visibility sidePanelVisibility = Visibility.Visible;
        private Visibility contentPanelVisibility = Visibility.Visible;

        public MainWindow() {
            InitializeComponent();

            ContentBrowser.Visibility = contentPanelVisibility;

            SceneView.Visibility = sidePanelVisibility;
            EntityView.Visibility = sidePanelVisibility;
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
            //Ignore key events to toggle editor elements
            if(IsToggleElementsPressed(e.Key) || IsToggleContentPressed(e.Key)) {
                e.Handled = false;
                return;
            }

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

        private void Window_MouseMove(object sender, MouseEventArgs e) {
            //Capture mouse move on the window incase the user moves their mouse out of the viewport while moving the camera
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

        private void Window_KeyUp(object sender, KeyEventArgs e) {
            //Handle keys to show / hide editor elements
            if (IsToggleElementsPressed(e.Key)) {
                sidePanelVisibility = sidePanelVisibility == Visibility.Collapsed ? Visibility.Visible : Visibility.Collapsed;
                SceneView.Visibility = sidePanelVisibility;
                EntityView.Visibility = sidePanelVisibility;

                e.Handled = true;
            }

            if (IsToggleContentPressed(e.Key)) {
                contentPanelVisibility = contentPanelVisibility == Visibility.Collapsed ? Visibility.Visible : Visibility.Collapsed;
                ContentBrowser.Visibility = contentPanelVisibility;

                e.Handled = true;
            }
        }

        private static bool IsToggleElementsPressed(Key key) => Keyboard.Modifiers == ModifierKeys.Control && key == Key.Tab;

        private static bool IsToggleContentPressed(Key key) => Keyboard.Modifiers == ModifierKeys.Control && key == Key.Space;
    }
}
