# CowByte Engine Component Model and Messaging Overview

### Basic Data Structures:
- Each Component has a MessageQueue and a ComponentList that contains all children.
- A message bus has a list of subscribers, that is, a CBVector of pointers to some components.
- A message is a simple struct that by default only contains a MessageType member. Additional messages inherit from Message struct.

### Where:
 * Where are message data stored?
    - A single message is always allocated and constructed on the heap, within one of the proper sized memory blocks. To prevent accidental construction, Messages can only be created by Message::Create() method, and all constructors will be declared protected (using macro DECLARE_MESSAGE_CONSTRUCTORS(Msg_SomeMessageClass)).
 * Where are message references stored?
    - Each Component only has a vector of pointers to all messages queued.

### When:
 * When does a component post a message?
    - In most cases, during Update(), but can be whenever - so long as the messages will be polled later. Post a message using CBMessaging::PostMessage(msg, bus).
 * When do the messages get processed and in what order?
    - While most messages are posted during Update(), Update() shouldn't process any messages.
    - This is because Update() gets called in a tree structure. When parent has almost finished updating, it calls Update() on all its children. Therefore it becomes unclear when each message gets processed. 
    - So this is what the procedure will look like:
    - ```C++
        // In an engine update cycle:
        Engine::Runloop()
        {
            UpdateTree(); // Normal update stuff, calls children update in the end.
                          // When this returns we will have all normal updates finished,
                          // As well as all message posted.

            MessageBus::Broadcast(); // Message bus broadcasts to subscribers. 
                                    // By the end of this all subscribers and 
                                    // Offsprings of subscribers should have 
                                    // references of the messages in the queue.

            MessageBus::SubsHandleMessagesQueueTree(); // Works similiarly to Update(), first, parents process 
                                                       // their messages queued, then children do it too.

            // TODO
            PostUpdate(); // If there's anything else the needs to be done within this frame, 
                          // For example message processing results need to be dealt with.

        }

      ```
 * What about if I want a message to be processed right away?
    - You can call CBMessaging::PostImediateMessage(msg, bus). Do notice that this one doesn't put the message into queue and it doesn't respect the procedure mentioned above so use it carefully.
 * When are messages released?
    - Since multiple components will end up sharing reference to the same message data, and there's no way to track whether it's not needed anymore than using a smart pointer. CBRefCountPtr which is a reference counting pointer that release memory when no one is referencing it anymore, was created for this purpose.


### Who:
 * Who do components send messages to?
    - They all send directly to specified message bus. While there can be multiple message buses, the only one I'm currently implementing is a engine-wise global message bus. Pointer of which kept as a static under MessageBus scope, tho the actual bus is dynamically allocated on the heap. Obviously this is to make things less global and to make object initialization more controlled.
 * Who do message bus broadcast to?
   - Like mentioned above, messages gets sent to direct subscribers from bus, and then propogate downwards through component hierarchy.