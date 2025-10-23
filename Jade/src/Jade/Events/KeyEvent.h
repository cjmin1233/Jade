#pragma once
#include "Jade/Core/KeyCodes.h"
#include "Event.h"

namespace Jade
{
    class JADE_API KeyEvent : public Event
    {
    public:
        inline Key::KeyCode GetKeyCode() const { return m_KeyCode; }

        EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)

    protected:
        KeyEvent(const Key::KeyCode keycode)
            : m_KeyCode(keycode) {}

        Key::KeyCode m_KeyCode;
    };

    class JADE_API KeyPressedEvent : public KeyEvent
    {
    public:
        KeyPressedEvent(const Key::KeyCode keycode, bool isRepeat = false)
            : KeyEvent(keycode), m_IsRepeat(isRepeat) {}

        inline bool IsRepeat() const { return m_IsRepeat; }

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "KeyPressedEvent: " << Key::KeyCodeToString(m_KeyCode) << " (repeat = " << m_IsRepeat << ")";
            return ss.str();
        }

        EVENT_CLASS_TYPE(KeyPressed)

    private:
        bool m_IsRepeat;
    };

    class JADE_API KeyReleasedEvent : public KeyEvent
    {
    public:
        KeyReleasedEvent(const Key::KeyCode keycode)
            : KeyEvent(keycode) {}

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "KeyReleasedEvent: " << Key::KeyCodeToString(m_KeyCode);
            return ss.str();
        }

        EVENT_CLASS_TYPE(KeyReleased)
    };

    class JADE_API KeyTypedEvent : public KeyEvent
    {
    public:
        KeyTypedEvent(const Key::KeyCode keycode)
            : KeyEvent(keycode) {}

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "KeyTypedEvent: " << Key::KeyCodeToString(m_KeyCode);
            return ss.str();
        }

        EVENT_CLASS_TYPE(KeyTyped)
    };
}