#include "Membrane/EditorViewport.hpp"

#include "Membrane/MessageHandler.hpp"
#include "Membrane/Messages.hpp"

#include <Clove/Log/Log.hpp>

using namespace clove;

namespace membrane {
    namespace {
        Key convertKey(System::Windows::Input::Key key) {
            switch(key) {
                case System::Windows::Input::Key::None:
                    return Key::None;
                case System::Windows::Input::Key::Tab:
                    return Key::Tab;
                case System::Windows::Input::Key::Enter:
                    return Key::Enter;
                case System::Windows::Input::Key::Pause:
                    return Key::Pause;
                case System::Windows::Input::Key::CapsLock:
                    return Key::Caps_lock;
                case System::Windows::Input::Key::Escape:
                    return Key::Escape;
                case System::Windows::Input::Key::Space:
                    return Key::Space;
                case System::Windows::Input::Key::PageUp:
                    return Key::Page_up;
                case System::Windows::Input::Key::PageDown:
                    return Key::Page_down;
                case System::Windows::Input::Key::End:
                    return Key::End;
                case System::Windows::Input::Key::Home:
                    return Key::Home;
                case System::Windows::Input::Key::Left:
                    return Key::Left;
                case System::Windows::Input::Key::Up:
                    return Key::Up;
                case System::Windows::Input::Key::Right:
                    return Key::Right;
                case System::Windows::Input::Key::Down:
                    return Key::Down;
                case System::Windows::Input::Key::PrintScreen:
                    return Key::Print_screen;
                case System::Windows::Input::Key::Insert:
                    return Key::Insert;
                case System::Windows::Input::Key::Delete:
                    return Key::Delete;
                case System::Windows::Input::Key::D0:
                    return Key::_0;
                case System::Windows::Input::Key::D1:
                    return Key::_1;
                case System::Windows::Input::Key::D2:
                    return Key::_2;
                case System::Windows::Input::Key::D3:
                    return Key::_3;
                case System::Windows::Input::Key::D4:
                    return Key::_4;
                case System::Windows::Input::Key::D5:
                    return Key::_5;
                case System::Windows::Input::Key::D6:
                    return Key::_6;
                case System::Windows::Input::Key::D7:
                    return Key::_7;
                case System::Windows::Input::Key::D8:
                    return Key::_8;
                case System::Windows::Input::Key::D9:
                    return Key::_9;
                case System::Windows::Input::Key::A:
                    return Key::A;
                case System::Windows::Input::Key::B:
                    return Key::B;
                case System::Windows::Input::Key::C:
                    return Key::C;
                case System::Windows::Input::Key::D:
                    return Key::D;
                case System::Windows::Input::Key::E:
                    return Key::E;
                case System::Windows::Input::Key::F:
                    return Key::F;
                case System::Windows::Input::Key::G:
                    return Key::G;
                case System::Windows::Input::Key::H:
                    return Key::H;
                case System::Windows::Input::Key::I:
                    return Key::I;
                case System::Windows::Input::Key::J:
                    return Key::J;
                case System::Windows::Input::Key::K:
                    return Key::K;
                case System::Windows::Input::Key::L:
                    return Key::L;
                case System::Windows::Input::Key::M:
                    return Key::M;
                case System::Windows::Input::Key::N:
                    return Key::N;
                case System::Windows::Input::Key::O:
                    return Key::O;
                case System::Windows::Input::Key::P:
                    return Key::P;
                case System::Windows::Input::Key::Q:
                    return Key::Q;
                case System::Windows::Input::Key::R:
                    return Key::R;
                case System::Windows::Input::Key::S:
                    return Key::S;
                case System::Windows::Input::Key::T:
                    return Key::T;
                case System::Windows::Input::Key::U:
                    return Key::U;
                case System::Windows::Input::Key::V:
                    return Key::V;
                case System::Windows::Input::Key::W:
                    return Key::W;
                case System::Windows::Input::Key::X:
                    return Key::X;
                case System::Windows::Input::Key::Y:
                    return Key::Y;
                case System::Windows::Input::Key::Z:
                    return Key::Z;
                case System::Windows::Input::Key::LWin:
                    return Key::Super_Left;
                case System::Windows::Input::Key::RWin:
                    return Key::Super_Right;
                case System::Windows::Input::Key::NumPad0:
                    return Key::NumPad_0;
                case System::Windows::Input::Key::NumPad1:
                    return Key::NumPad_1;
                case System::Windows::Input::Key::NumPad2:
                    return Key::NumPad_2;
                case System::Windows::Input::Key::NumPad3:
                    return Key::NumPad_3;
                case System::Windows::Input::Key::NumPad4:
                    return Key::NumPad_4;
                case System::Windows::Input::Key::NumPad5:
                    return Key::NumPad_5;
                case System::Windows::Input::Key::NumPad6:
                    return Key::NumPad_6;
                case System::Windows::Input::Key::NumPad7:
                    return Key::NumPad_7;
                case System::Windows::Input::Key::NumPad8:
                    return Key::NumPad_8;
                case System::Windows::Input::Key::NumPad9:
                    return Key::NumPad_9;
                case System::Windows::Input::Key::Multiply:
                    return Key::NumPad_Multiply;
                case System::Windows::Input::Key::Add:
                    return Key::NumPad_Add;
                case System::Windows::Input::Key::Subtract:
                    return Key::Minus;
                case System::Windows::Input::Key::Decimal:
                    return Key::Period;
                case System::Windows::Input::Key::Divide:
                    return Key::NumPad_Divide;
                case System::Windows::Input::Key::F1:
                    return Key::F1;
                case System::Windows::Input::Key::F2:
                    return Key::F2;
                case System::Windows::Input::Key::F3:
                    return Key::F3;
                case System::Windows::Input::Key::F4:
                    return Key::F4;
                case System::Windows::Input::Key::F5:
                    return Key::F5;
                case System::Windows::Input::Key::F6:
                    return Key::F6;
                case System::Windows::Input::Key::F7:
                    return Key::F7;
                case System::Windows::Input::Key::F8:
                    return Key::F8;
                case System::Windows::Input::Key::F9:
                    return Key::F9;
                case System::Windows::Input::Key::F10:
                    return Key::F10;
                case System::Windows::Input::Key::F11:
                    return Key::F11;
                case System::Windows::Input::Key::F12:
                    return Key::F12;
                case System::Windows::Input::Key::F13:
                    return Key::F13;
                case System::Windows::Input::Key::F14:
                    return Key::F14;
                case System::Windows::Input::Key::F15:
                    return Key::F15;
                case System::Windows::Input::Key::F16:
                    return Key::F16;
                case System::Windows::Input::Key::F17:
                    return Key::F17;
                case System::Windows::Input::Key::F18:
                    return Key::F18;
                case System::Windows::Input::Key::F19:
                    return Key::F19;
                case System::Windows::Input::Key::F20:
                    return Key::F20;
                case System::Windows::Input::Key::F21:
                    return Key::F21;
                case System::Windows::Input::Key::F22:
                    return Key::F22;
                case System::Windows::Input::Key::F23:
                    return Key::F23;
                case System::Windows::Input::Key::F24:
                    return Key::F24;
                case System::Windows::Input::Key::NumLock:
                    return Key::Num_lock;
                case System::Windows::Input::Key::LeftShift:
                    return Key::Shift_Left;
                case System::Windows::Input::Key::RightShift:
                    return Key::Shift_Right;
                case System::Windows::Input::Key::LeftCtrl:
                    return Key::Control_Left;
                case System::Windows::Input::Key::RightCtrl:
                    return Key::Control_Right;
                case System::Windows::Input::Key::LeftAlt:
                    return Key::Alt_Left;
                case System::Windows::Input::Key::RightAlt:
                    return Key::Alt_Right;
                default:
                    return Key::Undefined;
            }
        }

        MouseButton convertMouseButton(System::Windows::Input::MouseButton button) {
            switch(button) {
                case System::Windows::Input::MouseButton::Left:
                    return MouseButton::Left;
                case System::Windows::Input::MouseButton::Middle:
                    return MouseButton::Middle;
                case System::Windows::Input::MouseButton::Right:
                    return MouseButton::Right;
                case System::Windows::Input::MouseButton::XButton1:
                    return MouseButton::_4;
                case System::Windows::Input::MouseButton::XButton2:
                    return MouseButton::_5;
                default:
                    return MouseButton::Undefined;
            }
        }
    }
}

namespace membrane {
    EditorViewport::EditorViewport() {
        MessageHandler::bindToMessage(gcnew MessageSentHandler<Editor_ViewportKeyEvent ^>(this, &EditorViewport::onKeyEvent));
        MessageHandler::bindToMessage(gcnew MessageSentHandler<Editor_ViewportMouseButtonEvent ^>(this, &EditorViewport::onMouseButtonEvent));
        MessageHandler::bindToMessage(gcnew MessageSentHandler<Editor_ViewportMouseMoveEvent ^>(this, &EditorViewport::onMouseMoveEvent));

        events = gcnew System::Collections::Generic::List<GenericEvent ^>;

        keyboardDispatcher = new clove::Keyboard::Dispatcher{};
        mouseDispatcher    = new clove::Mouse::Dispatcher{};

        keyboard = new clove::Keyboard{ *keyboardDispatcher };
        mouse    = new clove::Mouse{ *mouseDispatcher };
    }

    EditorViewport::~EditorViewport() {
        this->!EditorViewport();
    }

    EditorViewport::!EditorViewport() {
        delete keyboard;
        delete mouse;

        delete keyboardDispatcher;
        delete mouseDispatcher;
    }

    void EditorViewport::processInput() {
        for each(auto event in events) {
            switch(event->type) {
                case GenericEvent::Type::Keyboard:
                    if(event->state == GenericEvent::State::Pressed) {
                        if(!keyboard->isKeyPressed(*event->key) || keyboard->isAutoRepeatEnabled()) {
                            keyboardDispatcher->onKeyPressed(*event->key);
                        }
                    } else if(event->state == GenericEvent::State::Released) {
                        keyboardDispatcher->onKeyReleased(*event->key);
                    }
                    break;
                case GenericEvent::Type::Mouse:
                    if(event->button != nullptr) {
                        if(event->state == GenericEvent::State::Pressed) {
                            mouseDispatcher->onButtonPressed(*event->button, *event->pos);
                        } else if(event->state == GenericEvent::State::Released) {
                            mouseDispatcher->onButtonReleased(*event->button, *event->pos);
                        }
                    } else {
                        mouseDispatcher->onMouseMove(*event->pos);
                    }
                    break;
                default:
                    break;
            }
        }

        events->Clear();
    };

    Keyboard *EditorViewport::getKeyboard() {
        return keyboard;
    }

    Mouse *EditorViewport::getMouse() {
        return mouse;
    }

    void EditorViewport::onKeyEvent(Editor_ViewportKeyEvent ^ event) {
        GenericEvent ^ genericEvent { gcnew GenericEvent };
        genericEvent->type  = GenericEvent::Type::Keyboard;
        genericEvent->state = event->type == Editor_ViewportKeyEvent::Type::Pressed ? EditorViewport::GenericEvent::State::Pressed : EditorViewport::GenericEvent::State::Released;
        genericEvent->key   = new Key{ convertKey(event->key) };

        events->Add(genericEvent);
    }

    void EditorViewport::onMouseButtonEvent(Editor_ViewportMouseButtonEvent ^ event) {
        GenericEvent ^ genericEvent { gcnew GenericEvent };
        genericEvent->type   = GenericEvent::Type::Mouse;
        genericEvent->state  = event->state == System::Windows::Input::MouseButtonState::Pressed ? EditorViewport::GenericEvent::State::Pressed : EditorViewport::GenericEvent::State::Released;
        genericEvent->button = new MouseButton{ convertMouseButton(event->button) };
        genericEvent->pos    = new vec2i{ event->position.X, event->position.Y };

        events->Add(genericEvent);
    }

    void EditorViewport::onMouseMoveEvent(Editor_ViewportMouseMoveEvent ^ event) {
        GenericEvent ^ genericEvent { gcnew GenericEvent };
        genericEvent->type = GenericEvent::Type::Mouse;
        genericEvent->pos  = new vec2i{ event->position.X, event->position.Y };

        events->Add(genericEvent);
    }
}