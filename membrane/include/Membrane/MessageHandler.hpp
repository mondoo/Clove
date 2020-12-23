#pragma once

namespace garlic::membrane {
    // clang-format off

    //Editor_ are messages that the editor sends and the engine listens to.
    //Engine_ are messages that the engine sends and the editor listens to.

    public enum class ComponentType {
        Transform,
        Mesh
    };

    public value struct Vector3{
        float x;
        float y;
        float z;

        Vector3(float x, float y, float z)
            : x{ x }, y{ y }, z{ z }{
        }
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

    public ref class Editor_UpdateTransform{
    public:
        System::UInt32 entity;

        Vector3 position;
        Vector3 rotation;
        Vector3 scale;
    };
    public ref class Engine_OnTransformChanged{
    public:
        System::UInt32 entity;
        
        Vector3 position;
        Vector3 rotation;
        Vector3 scale;
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
        static int loopCount{0};

        static System::Collections::Generic::List<IMessageDelegateWrapper^> ^ delegates = gcnew System::Collections::Generic::List<IMessageDelegateWrapper^>;
        static System::Collections::Generic::List<IMessageDelegateWrapper^> ^ deferedDelegates = gcnew System::Collections::Generic::List<IMessageDelegateWrapper^>;

    public:
        generic<class MessageType> 
        static void bindToMessage(MessageSentHandler<MessageType> ^ handler) {
            MessageDelegateWrapper<MessageType> ^ wrapper { gcnew MessageDelegateWrapper<MessageType> };
            wrapper->handler = handler;

            //Make sure we don't add to the current list while looping
            if(loopCount == 0){
                delegates->Add(wrapper);
            }else{
                deferedDelegates->Add(wrapper);
            }
        }

        generic<class MessageType> 
        static void sendMessage(MessageType message){
            ++loopCount;
            for each(IMessageDelegateWrapper^ wrapper in delegates){
                if(wrapper->GetType() == MessageDelegateWrapper<MessageType>::typeid) {
                    ((MessageDelegateWrapper<MessageType>^)wrapper)->handler->Invoke(message);
                }
            }
            --loopCount;

            if(loopCount == 0){
                for each(IMessageDelegateWrapper^ wrapper in deferedDelegates){
                    delegates->Add(wrapper);
                }
                deferedDelegates->Clear();
            }
        }
    };

    // clang-format on
}