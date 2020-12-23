#pragma once

namespace garlic::membrane {
    // clang-format off
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
        static int loopCount{0};

        static System::Collections::Generic::List<IMessageDelegateWrapper^> ^ delegates = gcnew System::Collections::Generic::List<IMessageDelegateWrapper^>;
        static System::Collections::Generic::List<IMessageDelegateWrapper^> ^ deferedDelegates = gcnew System::Collections::Generic::List<IMessageDelegateWrapper^>;

    public:
        generic<class MessageType> 
        static void bindToMessage(MessageSentHandler<MessageType> ^ handler);

        generic<class MessageType> 
        static void sendMessage(MessageType message);
    };
    // clang-format on
}