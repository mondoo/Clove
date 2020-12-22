#pragma once

namespace garlic::membrane {
    // clang-format off

    //Editor_ are messages that the editor sends and the engine listens to.
    //Engine_ are messages that the engine sends and the editor listens to.

    public enum class ComponentType {
        Transform,
        Mesh
    };

    //Messages
    public ref class Editor_CreateEntity {};
    public ref class Engine_OnEntityCreated {
    public:
        System::UInt32 entity;
    };

    public ref class Editor_CreateComponent {
    public:
        System::UInt32 entity;
        ComponentType componentType;
    };
    public ref class Engine_OnComponentCreated {
    public:
        System::UInt32 entity;
        ComponentType componentType;
    };

    //Delegates
    generic<class MessageType> 
    public delegate void MessageSentHandler(MessageType message);

    //Helper types
    private interface class IMessageDelegateWrapper{};

    generic<class MessageType> 
    private ref class MessageDelegateWrapper : public IMessageDelegateWrapper {
    public:
        MessageSentHandler<MessageType> ^ handler;
    };

    public ref class MessageHandler {
    private:
        static System::Collections::Generic::List<IMessageDelegateWrapper^> ^ delegates = gcnew System::Collections::Generic::List<IMessageDelegateWrapper^>;

    public:
        generic<class MessageType> 
        static void bindToMessage(MessageSentHandler<MessageType> ^ handler) {
            MessageDelegateWrapper<MessageType> ^ wrapper { gcnew MessageDelegateWrapper<MessageType> };
            wrapper->handler = handler;
            delegates->Add(wrapper);
        }

        generic<class MessageType> 
        static void sendMessage(MessageType message){
            for each(IMessageDelegateWrapper^ wrapper in delegates){
                if(wrapper->GetType() == MessageDelegateWrapper<MessageType>::typeid) {
                    ((MessageDelegateWrapper<MessageType>^)wrapper)->handler->Invoke(message);
                }
            }
        }
    };

    // clang-format on
}