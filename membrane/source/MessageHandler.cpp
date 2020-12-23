#include "Membrane/MessageHandler.hpp"

namespace garlic::membrane {
    // clang-format off
    generic<class MessageType> 
    void MessageHandler::bindToMessage(MessageSentHandler<MessageType> ^ handler) {
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
    void MessageHandler::sendMessage(MessageType message){
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
    // clang-format on
}