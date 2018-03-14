# CowByte Engine Component Model and Messaging Overview

### Basic Architecture:
- Each Component has a MessageQueue and a ComponentList that contains all children.
- There is a message bus that has a list of subscribers.
- A message is a simple struct that by default only contains a type member. Additional messages inherit from Message struct.

### Where:
 * Where are message data stored?
    - A single message is always allocated and constructed on the heap, within one of the proper sized memory blocks. To prevent accidental construction, Messages can only be created by Message::Construct() factory method, and all constructors will be declared private.
 * Where are message references stored?
    - Each Component only has a vector of pointers to all messages queued.

### When:
 * When does a component post a message?
    - In most cases, when during Update(), but can be whenever so long as the messages will be polled later.
 * When do the messages get processed and in what order?
    - While most messages are posted during Update(), Update() shouldn't process any messages.
    - This is because Update() gets called in a tree structure. When parent has almost finished updating, it calls Update() on all its children. Therefore it becomes unclear when each message gets processed. 
    - So this is what the structure will look like:
    - ```C++
        // In an engine update cycle:

        Update(); // Normal update stuff, calls children update in the end.
                  // When this returns we will have all normal updates finished,
                  // As well as all message posted.

        MessageBus.Broadcast(); // Message bus broadcasts to subscribers. 
                                // By the end of this all subscribers and 
                                // Offsprings of subscribers should have 
                                // references of the messages in the queue.

        MessageProcessing(); // Works similiarly to Update(), first parents process 
                             // their messages in the queue, then children do it too.

        PostUpdate(); // If there's anything else the needs to be done within this frame, 
                      // For example message processing results need to be dealt with.
      ```

 * When are messages released?
    - Since multiple components will end up sharing reference to the same message data, there needs to be a smart management like a shared pointer, that releases the message memory once no one needs it anymore. 


### Who:
 * Who do components send messages to?
    - They all send directly to respective message bus. While there can be multiple message buses, the only one I'm currently implementing is a engine-wise global message bus. Pointer of which kept as a static, tho the actual bus is dynamically allocated on the heap.
 * Who do message bus broadcast to?
   - Like mentioned above, messages get propagated from parents to children.