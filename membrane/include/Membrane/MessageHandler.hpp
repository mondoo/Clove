#pragma once

namespace garlic::membrane {
    //Editor messages. What the editor sends and the engine listens to.
    public ref class Editor_CreateEntity {};

    //Engine messages.  What the engine sends and the editor listens to.
    public ref class Engine_OnEntityCreated {
    public:
        System::UInt32 entity;
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
            MessageDelegateWrapper<MessageType> ^ wrapper = gcnew MessageDelegateWrapper<MessageType>;
            wrapper->handler                               = handler;
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
}