#pragma once

namespace garlic::membrane {
    ref class EditorMessage;
    ref class EngineMessage;
}

namespace garlic::membrane {
    // clang-format off
    //Delegates
    generic<class MessageType> 
    public delegate void MessageSentHandler(MessageType message);

    //Helper types
    private interface class IMessageDelegateWrapper {
    public:
        virtual void tryInvoke(EditorMessage ^message) = 0;
        virtual void tryInvoke(EngineMessage ^message) = 0;
    };

    generic<class MessageType> 
    private ref class MessageDelegateWrapper : public IMessageDelegateWrapper {
        //VARIABLES
    public:
        MessageSentHandler<MessageType> ^ handler;

        //FUNCTIONS
    public:
        virtual void tryInvoke(EditorMessage ^message) {
            tryInvoke_interal(message);
        }

        virtual void tryInvoke(EngineMessage ^message) {
            tryInvoke_interal(message);
        }

    private:
        generic<class InvokeMessageType>
        void tryInvoke_interal(InvokeMessageType message) {
            if (message->GetType() == MessageType::typeid) {
                handler->Invoke((MessageType)message);
            }
        }
    };

    public ref class MessageHandler {
    private:
        static System::Object^ editorLock{ gcnew System::Object() };

        static System::Collections::Generic::List<IMessageDelegateWrapper ^> ^delegates{ gcnew System::Collections::Generic::List<IMessageDelegateWrapper^> };

        static System::Collections::Generic::List<EditorMessage ^> ^editorMessages{ gcnew System::Collections::Generic::List<EditorMessage ^> };
        static System::Collections::Generic::List<EngineMessage ^> ^engineMessages{ gcnew System::Collections::Generic::List<EngineMessage ^> };
    public:
        generic<class MessageType> 
        static void bindToMessage(MessageSentHandler<MessageType> ^handler);

        generic<class MessageType>
        static void sendMessage(MessageType message);

        static void flushEditor();
        static void flushEngine();
    };
    // clang-format on
}