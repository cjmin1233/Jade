#include "jdpch.h"

#include "Jade/Core/KeyCodes.h"

#include <imgui.h>

namespace Jade::Key
{
    std::string KeyCodeToString(KeyCode keycode)
    {
        switch (keycode)
        {
            // From glfw3.h
            case KeyCode::Space:         return "Space";
            case KeyCode::Apostrophe:    return "Apostrophe";
            case KeyCode::Comma:         return "Comma";
            case KeyCode::Minus:         return "Minus";
            case KeyCode::Period:        return "Period";
            case KeyCode::Slash:         return "Slash";

            case KeyCode::D0:            return "D0";
            case KeyCode::D1:            return "D1";
            case KeyCode::D2:            return "D2";
            case KeyCode::D3:            return "D3";
            case KeyCode::D4:            return "D4";
            case KeyCode::D5:            return "D5";
            case KeyCode::D6:            return "D6";
            case KeyCode::D7:            return "D7";
            case KeyCode::D8:            return "D8";
            case KeyCode::D9:            return "D9";

            case KeyCode::Semicolon:     return "Semicolon";
            case KeyCode::Equal:         return "Equal";

            case KeyCode::A:             return "A";
            case KeyCode::B:             return "B";
            case KeyCode::C:             return "C";
            case KeyCode::D:             return "D";
            case KeyCode::E:             return "E";
            case KeyCode::F:             return "F";
            case KeyCode::G:             return "G";
            case KeyCode::H:             return "H";
            case KeyCode::I:             return "I";
            case KeyCode::J:             return "J";
            case KeyCode::K:             return "K";
            case KeyCode::L:             return "L";
            case KeyCode::M:             return "M";
            case KeyCode::N:             return "N";
            case KeyCode::O:             return "O";
            case KeyCode::P:             return "P";
            case KeyCode::Q:             return "Q";
            case KeyCode::R:             return "R";
            case KeyCode::S:             return "S";
            case KeyCode::T:             return "T";
            case KeyCode::U:             return "U";
            case KeyCode::V:             return "V";
            case KeyCode::W:             return "W";
            case KeyCode::X:             return "X";
            case KeyCode::Y:             return "Y";
            case KeyCode::Z:             return "Z";

            case KeyCode::LeftBracket:   return "LeftBracket";
            case KeyCode::Backslash:     return "Backslash";
            case KeyCode::RightBracket:  return "RightBracket";
            case KeyCode::GraveAccent:   return "GraveAccent";

            case KeyCode::World1:        return "World1";
            case KeyCode::World2:        return "World2";

            // Function keys
            case KeyCode::Escape:        return "Escape";
            case KeyCode::Enter:         return "Enter";
            case KeyCode::Tab:           return "Tab";
            case KeyCode::Backspace:     return "Backspace";
            case KeyCode::Insert:        return "Insert";
            case KeyCode::Delete:        return "Delete";
            case KeyCode::Right:         return "Right";
            case KeyCode::Left:          return "Left";
            case KeyCode::Down:          return "Down";
            case KeyCode::Up:            return "Up";
            case KeyCode::PageUp:        return "PageUp";
            case KeyCode::PageDown:      return "PageDown";
            case KeyCode::Home:          return "Home";
            case KeyCode::End:           return "End";
            case KeyCode::CapsLock:      return "CapsLock";
            case KeyCode::ScrollLock:    return "ScrollLock";
            case KeyCode::NumLock:       return "NumLock";
            case KeyCode::PrintScreen:   return "PrintScreen";
            case KeyCode::Pause:         return "Pause";
            case KeyCode::F1:            return "F1";
            case KeyCode::F2:            return "F2";
            case KeyCode::F3:            return "F3";
            case KeyCode::F4:            return "F4";
            case KeyCode::F5:            return "F5";
            case KeyCode::F6:            return "F6";
            case KeyCode::F7:            return "F7";
            case KeyCode::F8:            return "F8";
            case KeyCode::F9:            return "F9";
            case KeyCode::F10:           return "F10";
            case KeyCode::F11:           return "F11";
            case KeyCode::F12:           return "F12";
            case KeyCode::F13:           return "F13";
            case KeyCode::F14:           return "F14";
            case KeyCode::F15:           return "F15";
            case KeyCode::F16:           return "F16";
            case KeyCode::F17:           return "F17";
            case KeyCode::F18:           return "F18";
            case KeyCode::F19:           return "F19";
            case KeyCode::F20:           return "F20";
            case KeyCode::F21:           return "F21";
            case KeyCode::F22:           return "F22";
            case KeyCode::F23:           return "F23";
            case KeyCode::F24:           return "F24";
            case KeyCode::F25:           return "F25";

            // Keypad
            case KeyCode::KP0:           return "KP0";
            case KeyCode::KP1:           return "KP1";
            case KeyCode::KP2:           return "KP2";
            case KeyCode::KP3:           return "KP3";
            case KeyCode::KP4:           return "KP4";
            case KeyCode::KP5:           return "KP5";
            case KeyCode::KP6:           return "KP6";
            case KeyCode::KP7:           return "KP7";
            case KeyCode::KP8:           return "KP8";
            case KeyCode::KP9:           return "KP9";
            case KeyCode::KPDecimal:     return "KPDecimal";
            case KeyCode::KPDivide:      return "KPDivide";
            case KeyCode::KPMultiply:    return "KPMultiply";
            case KeyCode::KPSubtract:    return "KPSubtract";
            case KeyCode::KPAdd:         return "KPAdd";
            case KeyCode::KPEnter:       return "KPEnter";
            case KeyCode::KPEqual:       return "KPEqual";

            case KeyCode::LeftShift:     return "LeftShift";
            case KeyCode::LeftControl:   return "LeftControl";
            case KeyCode::LeftAlt:       return "LeftAlt";
            case KeyCode::LeftSuper:     return "LeftSuper";
            case KeyCode::RightShift:    return "RightShift";
            case KeyCode::RightControl:  return "RightControl";
            case KeyCode::RightAlt:      return "RightAlt";
            case KeyCode::RightSuper:    return "RightSuper";
            case KeyCode::Menu:          return "Menu";

            default:                     return "Unknown";
        }
    }

    ImGuiKey KeyCodeToImGuiKey(KeyCode keycode)
    {
        switch (keycode)
        {
        case KeyCode::Tab: return ImGuiKey_Tab;
        case KeyCode::Left: return ImGuiKey_LeftArrow;
        case KeyCode::Right: return ImGuiKey_RightArrow;
        case KeyCode::Up: return ImGuiKey_UpArrow;
        case KeyCode::Down: return ImGuiKey_DownArrow;
        case KeyCode::PageUp: return ImGuiKey_PageUp;
        case KeyCode::PageDown: return ImGuiKey_PageDown;
        case KeyCode::Home: return ImGuiKey_Home;
        case KeyCode::End: return ImGuiKey_End;
        case KeyCode::Insert: return ImGuiKey_Insert;
        case KeyCode::Delete: return ImGuiKey_Delete;
        case KeyCode::Backspace: return ImGuiKey_Backspace;
        case KeyCode::Space: return ImGuiKey_Space;
        case KeyCode::Enter: return ImGuiKey_Enter;
        case KeyCode::Escape: return ImGuiKey_Escape;
        case KeyCode::Apostrophe: return ImGuiKey_Apostrophe;
        case KeyCode::Comma: return ImGuiKey_Comma;
        case KeyCode::Minus: return ImGuiKey_Minus;
        case KeyCode::Period: return ImGuiKey_Period;
        case KeyCode::Slash: return ImGuiKey_Slash;
        case KeyCode::Semicolon: return ImGuiKey_Semicolon;
        case KeyCode::Equal: return ImGuiKey_Equal;
        case KeyCode::LeftBracket: return ImGuiKey_LeftBracket;
        case KeyCode::Backslash: return ImGuiKey_Backslash;
        case KeyCode::World1: return ImGuiKey_Oem102;
        case KeyCode::World2: return ImGuiKey_Oem102;
        case KeyCode::RightBracket: return ImGuiKey_RightBracket;
        case KeyCode::GraveAccent: return ImGuiKey_GraveAccent;
        case KeyCode::CapsLock: return ImGuiKey_CapsLock;
        case KeyCode::ScrollLock: return ImGuiKey_ScrollLock;
        case KeyCode::NumLock: return ImGuiKey_NumLock;
        case KeyCode::PrintScreen: return ImGuiKey_PrintScreen;
        case KeyCode::Pause: return ImGuiKey_Pause;
        case KeyCode::KP0: return ImGuiKey_Keypad0;
        case KeyCode::KP1: return ImGuiKey_Keypad1;
        case KeyCode::KP2: return ImGuiKey_Keypad2;
        case KeyCode::KP3: return ImGuiKey_Keypad3;
        case KeyCode::KP4: return ImGuiKey_Keypad4;
        case KeyCode::KP5: return ImGuiKey_Keypad5;
        case KeyCode::KP6: return ImGuiKey_Keypad6;
        case KeyCode::KP7: return ImGuiKey_Keypad7;
        case KeyCode::KP8: return ImGuiKey_Keypad8;
        case KeyCode::KP9: return ImGuiKey_Keypad9;
        case KeyCode::KPDecimal: return ImGuiKey_KeypadDecimal;
        case KeyCode::KPDivide: return ImGuiKey_KeypadDivide;
        case KeyCode::KPMultiply: return ImGuiKey_KeypadMultiply;
        case KeyCode::KPSubtract: return ImGuiKey_KeypadSubtract;
        case KeyCode::KPAdd: return ImGuiKey_KeypadAdd;
        case KeyCode::KPEnter: return ImGuiKey_KeypadEnter;
        case KeyCode::KPEqual: return ImGuiKey_KeypadEqual;
        case KeyCode::LeftShift: return ImGuiKey_LeftShift;
        case KeyCode::LeftControl: return ImGuiKey_LeftCtrl;
        case KeyCode::LeftAlt: return ImGuiKey_LeftAlt;
        case KeyCode::LeftSuper: return ImGuiKey_LeftSuper;
        case KeyCode::RightShift: return ImGuiKey_RightShift;
        case KeyCode::RightControl: return ImGuiKey_RightCtrl;
        case KeyCode::RightAlt: return ImGuiKey_RightAlt;
        case KeyCode::RightSuper: return ImGuiKey_RightSuper;
        case KeyCode::Menu: return ImGuiKey_Menu;
        case KeyCode::D0: return ImGuiKey_0;
        case KeyCode::D1: return ImGuiKey_1;
        case KeyCode::D2: return ImGuiKey_2;
        case KeyCode::D3: return ImGuiKey_3;
        case KeyCode::D4: return ImGuiKey_4;
        case KeyCode::D5: return ImGuiKey_5;
        case KeyCode::D6: return ImGuiKey_6;
        case KeyCode::D7: return ImGuiKey_7;
        case KeyCode::D8: return ImGuiKey_8;
        case KeyCode::D9: return ImGuiKey_9;
        case KeyCode::A: return ImGuiKey_A;
        case KeyCode::B: return ImGuiKey_B;
        case KeyCode::C: return ImGuiKey_C;
        case KeyCode::D: return ImGuiKey_D;
        case KeyCode::E: return ImGuiKey_E;
        case KeyCode::F: return ImGuiKey_F;
        case KeyCode::G: return ImGuiKey_G;
        case KeyCode::H: return ImGuiKey_H;
        case KeyCode::I: return ImGuiKey_I;
        case KeyCode::J: return ImGuiKey_J;
        case KeyCode::K: return ImGuiKey_K;
        case KeyCode::L: return ImGuiKey_L;
        case KeyCode::M: return ImGuiKey_M;
        case KeyCode::N: return ImGuiKey_N;
        case KeyCode::O: return ImGuiKey_O;
        case KeyCode::P: return ImGuiKey_P;
        case KeyCode::Q: return ImGuiKey_Q;
        case KeyCode::R: return ImGuiKey_R;
        case KeyCode::S: return ImGuiKey_S;
        case KeyCode::T: return ImGuiKey_T;
        case KeyCode::U: return ImGuiKey_U;
        case KeyCode::V: return ImGuiKey_V;
        case KeyCode::W: return ImGuiKey_W;
        case KeyCode::X: return ImGuiKey_X;
        case KeyCode::Y: return ImGuiKey_Y;
        case KeyCode::Z: return ImGuiKey_Z;
        case KeyCode::F1: return ImGuiKey_F1;
        case KeyCode::F2: return ImGuiKey_F2;
        case KeyCode::F3: return ImGuiKey_F3;
        case KeyCode::F4: return ImGuiKey_F4;
        case KeyCode::F5: return ImGuiKey_F5;
        case KeyCode::F6: return ImGuiKey_F6;
        case KeyCode::F7: return ImGuiKey_F7;
        case KeyCode::F8: return ImGuiKey_F8;
        case KeyCode::F9: return ImGuiKey_F9;
        case KeyCode::F10: return ImGuiKey_F10;
        case KeyCode::F11: return ImGuiKey_F11;
        case KeyCode::F12: return ImGuiKey_F12;
        case KeyCode::F13: return ImGuiKey_F13;
        case KeyCode::F14: return ImGuiKey_F14;
        case KeyCode::F15: return ImGuiKey_F15;
        case KeyCode::F16: return ImGuiKey_F16;
        case KeyCode::F17: return ImGuiKey_F17;
        case KeyCode::F18: return ImGuiKey_F18;
        case KeyCode::F19: return ImGuiKey_F19;
        case KeyCode::F20: return ImGuiKey_F20;
        case KeyCode::F21: return ImGuiKey_F21;
        case KeyCode::F22: return ImGuiKey_F22;
        case KeyCode::F23: return ImGuiKey_F23;
        case KeyCode::F24: return ImGuiKey_F24;
        default: return ImGuiKey_None;
        }
    }

    bool IsNamedKey(ImGuiKey key)
    {
        return key >= ImGuiKey_NamedKey_BEGIN
            && key < ImGuiKey_NamedKey_END;
    }

    bool GetKeyDown(ImGuiIO& io, ImGuiKey key)
    {
        return io.KeysData[key - ImGuiKey_NamedKey_BEGIN].Down;
    }
}