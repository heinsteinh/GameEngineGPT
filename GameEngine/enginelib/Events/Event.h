
#ifndef COREEVENT_H
#define COREEVENT_H

#include <Utils/HashedString.h>
#include <functional>
#include <list>
#include <map>
#include <memory>
#include <mutex>
#include <queue>
#include <typeinfo>

// Complement
// https://github.com/erbuka/blue-spheres-forever/blob/8d75ae69f609a51477ddb53e7bec8c5550e7f8fa/src/EventEmitter.h
// https://github.com/TB989/Fierce-Engine/blob/c03c77e137808e3b1d2963d7318147c2acb9f8f1/Engine/02_system/02_event/Event.h

/*
Usage
https://github.com/LaG1924/AltCraft/blob/4354b1b85b92e378ac0ea7c542edc033c6abdaff/src/Render.cpp
https://github.com/Hengar1711/Ordinal-Oasis/blob/c12189b3e6143fd27c496d4945ab67c142f1cf9f/Project1/Tutorial.h


https://github.com/Alexei-117/X-Kating/blob/3e81b62d7c95cd67434c8458f95fca876abf1fa9/Game/src/GameManager/InputManager.cpp
*/

namespace fts
{

    // Enumeration of engine event
    enum EventType
    {
        // unused
        FTS_EVENT_NONE = 0,

        // application events
        FTS_EVENT_QUIT = 0x100,

        // window events
        FTS_EVENT_WINDOW_MOVED = 0x200,
        FTS_EVENT_WINDOW_RESIZE,
        FTS_EVENT_WINDOW_CLOSE,
        FTS_EVENT_WINDOW_FOCUS_GAINED,
        FTS_EVENT_WINDOW_FOCUS_LOST,

        FTS_EVENT_WINDOW_RESTORE,
        FTS_EVENT_WINDOW_MINIMIZED,
        FTS_EVENT_WINDOW_MAXIMIZED,

        /* Keyboard events */
        FTS_EVENT_KEY_PRESSED = 0x300,
        FTS_EVENT_KEY_REPEAT,
        FTS_EVENT_KEY_RELEASED,
        FTS_EVENT_TEXT_INPUT,
        FTS_EVENT_KEY,

        /* Mouse events */
        FTS_EVENT_MOUSE_MOVED = 0x400,
        FTS_EVENT_MOUSE_PRESSED,
        FTS_EVENT_MOUSE_RELEASED,
        FTS_EVENT_MOUSE_SCROLLED,

        /* GamePlay events */
        FTS_EVENT_ENTITY_CREATED = 0x500,

    };

    typedef size_t EventTypeId;

    class Event
    {
        struct EventDataBase
        {
            virtual ~EventDataBase()
            {
            }
            virtual const std::type_info& Type() const = 0;
        };

        template <typename T>
        struct EventData : EventDataBase
        {
            EventData(const T& val)
                : data(val)
            {
            }

            const std::type_info& Type() const override
            {
                return typeid(data);
            }

            const T* Data() const
            {
                return &data;
            }

            T data;
        };

        std::shared_ptr<EventDataBase> data;

    public:
        const EventTypeId id;

        template <typename T>
        Event(EventTypeId eventId, const T& value)
            : id(eventId)
            , data(std::make_shared<EventData<T>>(value))
        {
        }

        ~Event() = default;

        Event(const Event& other)      = default;
        Event& operator=(const Event&) = default;

        Event(Event&&)            = delete;
        Event& operator=(Event&&) = delete;

        template <typename T>
        const T& get() const
        {
            if(typeid(T) != data->Type())
                throw std::runtime_error(std::string("Type ") + typeid(T).name() + " encountered but " + data->Type().name() + " expected");
            return static_cast<EventData<T>*>(data.get())->data;
        }

        template <typename T>
        const T* getPtr() const
        {
            if(typeid(T) != data->Type())
                return nullptr;
            return static_cast<EventData<T>*>(data.get())->Data();
        }
    };

    class EventListener
    {
        friend class EventSystem;
        using HandlerType = std::function<void(const Event&)>;

        using HandlerFnPtr = void (*)(const Event&);

        template <typename T>
        using MemberHandlerFnPtr = void (T::*)(const Event&);

        std::map<size_t, HandlerType> handlers;
        std::recursive_mutex handlersMutex;

        std::queue<Event> events;
        std::recursive_mutex eventsMutex;

        std::queue<Event> rawEvents;
        std::recursive_mutex rawEventsMutex;

    public:
        EventListener();
        ~EventListener();

        void HandleEvent();
        void HandleAllEvents();
        bool Empty();

        void RegisterHandler(size_t eventId, HandlerType&& data);
        void RegisterHandler(size_t eventId, const HandlerType& data);

        void RegisterHandler(const char* eventId, const HandlerType& data)
        {
            RegisterHandler(hashed_string(eventId).value(), data);
        }

        template <typename T>
        void RegisterHandler(size_t eventId, T* instance, MemberHandlerFnPtr<T> fnPtr)
        {
            return RegisterHandler(eventId, [instance, fnPtr](const Event& evt)
                                   {
                                       (instance->*fnPtr)(evt);
                                   });
        }

        template <typename T>
        void RegisterHandler(const char* eventId, T* instance, MemberHandlerFnPtr<T> fnPtr)
        {
            return RegisterHandler(hashed_string(eventId).value(), [instance, fnPtr](const Event& evt)
                                   {
                                       (instance->*fnPtr)(evt);
                                   });
        }

        void PollEvents();
    };

    class EventSystem
    {
        friend class EventListener;
        static std::list<EventListener*> listeners;
        static std::recursive_mutex listenersMutex;

    public:
        template <typename T>
        static void PushEvent(size_t eventId, T data)
        {

            Event event(eventId, data);

            std::lock_guard<std::recursive_mutex> lock(EventSystem::listenersMutex);
            for(auto& listener : listeners)
            {
                std::lock_guard<std::recursive_mutex> rawEventLock(listener->rawEventsMutex);
                listener->rawEvents.push(event);
            }
        }

        template <typename T>
        static void PushEvent(hashed_string eventId, T data)
        {

            Event event(eventId.value(), data);

            std::lock_guard<std::recursive_mutex> lock(EventSystem::listenersMutex);
            for(auto& listener : listeners)
            {
                std::lock_guard<std::recursive_mutex> rawEventLock(listener->rawEventsMutex);
                listener->rawEvents.push(event);
            }
        }

        template <typename T>
        static void PushEvent(const char* eventId, T data)
        {

            Event event(hashed_string(eventId).value(), data);

            std::lock_guard<std::recursive_mutex> lock(EventSystem::listenersMutex);
            for(auto& listener : listeners)
            {
                std::lock_guard<std::recursive_mutex> rawEventLock(listener->rawEventsMutex);
                listener->rawEvents.push(event);
            }
        }

        template <typename T>
        static void DirectEventCall(size_t eventId, T data)
        {
            Event event(eventId, data);
            std::lock_guard<std::recursive_mutex> lock(EventSystem::listenersMutex);
            for(auto& listener : listeners)
            {
                std::lock_guard<std::recursive_mutex> handlersLock(listener->handlersMutex);
                auto it = listener->handlers.find(eventId);
                if(it == listener->handlers.end())
                    continue;

                it->second(event);
            }
        }

        template <typename T>
        static void DirectEventCall(hashed_string eventId, T data)
        {
            Event event(eventId.value(), data);
            std::lock_guard<std::recursive_mutex> lock(EventSystem::listenersMutex);
            for(auto& listener : listeners)
            {
                std::lock_guard<std::recursive_mutex> handlersLock(listener->handlersMutex);
                auto it = listener->handlers.find(eventId);
                if(it == listener->handlers.end())
                    continue;

                it->second(event);
            }
        }
    };
} // namespace fts

#define PUSH_EVENT(eventName, data)        fts::EventSystem::PushEvent(hashed_string(eventName), data)       //; LOG(INFO)<<"PUSH_EVENT "<<eventName;
#define DIRECT_EVENT_CALL(eventName, data) fts::EventSystem::DirectEventCall(hashed_string(eventName), data) //; LOG(INFO)<<"DIRECT_CALL "<<eventName;

#endif
