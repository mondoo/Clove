#pragma once

#include <Clove/Platform/Input/Keyboard.hpp>
#include <Clove/Platform/Input/Mouse.hpp>
#include <msclr/gcroot.h>
#include <vector>

namespace membrane {
    ref class Editor_ViewportKeyEvent;
    ref class Editor_ViewportMouseButtonEvent;
    ref class Editor_ViewportMouseMoveEvent;
}

namespace membrane {
    /**
     * @brief A viewport manages input events and render regions inside the editor.
     */
    public ref class EditorViewport {
        //TYPES
    private:
        /**
         * @brief Holds any input event recieved.
         */
        ref struct GenericEvent{
            enum class Type{
                Keyboard,
                Mouse
            };
            enum class State{
                None,
                Pressed,
                Released,
            };

            Type type;
            State state;

            clove::Key *key{ nullptr };

            clove::MouseButton *button{ nullptr };
            clove::vec2i *pos{ nullptr };

            ~GenericEvent(){
                this->!GenericEvent();
            }
            !GenericEvent(){
                if(key != nullptr) {
                    delete key;
                }

                if(button != nullptr) {
                    delete button;
                }
                if(pos != nullptr) {
                    delete pos;
                }
            }
        };

        //VARIABLES
    private:
        System::Collections::Generic::List<GenericEvent ^> ^events;

        clove::Keyboard *keyboard{ nullptr };
        clove::Mouse *mouse{ nullptr };

        clove::Keyboard::Dispatcher *keyboardDispatcher{ nullptr };
        clove::Mouse::Dispatcher *mouseDispatcher{ nullptr };

        //FUNCTIONS
    public:
        //TODO: Ctors
        EditorViewport();
        ~EditorViewport();
        !EditorViewport();

        void processInput();

        clove::Keyboard *getKeyboard();
        clove::Mouse *getMouse();

    private:
        void onKeyEvent(Editor_ViewportKeyEvent ^ event);
        void onMouseButtonEvent(Editor_ViewportMouseButtonEvent ^ event);
        void onMouseMoveEvent(Editor_ViewportMouseMoveEvent ^ event);
    };
}