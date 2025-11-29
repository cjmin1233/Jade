#include "jdpch.h"

#include "Jade/Core/KeyCodes.h"

#include <imgui.h>

namespace Jade
{
    std::string KeyToString(Key key)
    {
        switch (key)
        {
            // From glfw3.h
            case Key::Space:         return "Space";
            case Key::Apostrophe:    return "Apostrophe";
            case Key::Comma:         return "Comma";
            case Key::Minus:         return "Minus";
            case Key::Period:        return "Period";
            case Key::Slash:         return "Slash";

            case Key::D0:            return "D0";
            case Key::D1:            return "D1";
            case Key::D2:            return "D2";
            case Key::D3:            return "D3";
            case Key::D4:            return "D4";
            case Key::D5:            return "D5";
            case Key::D6:            return "D6";
            case Key::D7:            return "D7";
            case Key::D8:            return "D8";
            case Key::D9:            return "D9";

            case Key::Semicolon:     return "Semicolon";
            case Key::Equal:         return "Equal";

            case Key::A:             return "A";
            case Key::B:             return "B";
            case Key::C:             return "C";
            case Key::D:             return "D";
            case Key::E:             return "E";
            case Key::F:             return "F";
            case Key::G:             return "G";
            case Key::H:             return "H";
            case Key::I:             return "I";
            case Key::J:             return "J";
            case Key::K:             return "K";
            case Key::L:             return "L";
            case Key::M:             return "M";
            case Key::N:             return "N";
            case Key::O:             return "O";
            case Key::P:             return "P";
            case Key::Q:             return "Q";
            case Key::R:             return "R";
            case Key::S:             return "S";
            case Key::T:             return "T";
            case Key::U:             return "U";
            case Key::V:             return "V";
            case Key::W:             return "W";
            case Key::X:             return "X";
            case Key::Y:             return "Y";
            case Key::Z:             return "Z";

            case Key::LeftBracket:   return "LeftBracket";
            case Key::Backslash:     return "Backslash";
            case Key::RightBracket:  return "RightBracket";
            case Key::GraveAccent:   return "GraveAccent";

            case Key::World1:        return "World1";
            case Key::World2:        return "World2";

            // Function keys
            case Key::Escape:        return "Escape";
            case Key::Enter:         return "Enter";
            case Key::Tab:           return "Tab";
            case Key::Backspace:     return "Backspace";
            case Key::Insert:        return "Insert";
            case Key::Delete:        return "Delete";
            case Key::Right:         return "Right";
            case Key::Left:          return "Left";
            case Key::Down:          return "Down";
            case Key::Up:            return "Up";
            case Key::PageUp:        return "PageUp";
            case Key::PageDown:      return "PageDown";
            case Key::Home:          return "Home";
            case Key::End:           return "End";
            case Key::CapsLock:      return "CapsLock";
            case Key::ScrollLock:    return "ScrollLock";
            case Key::NumLock:       return "NumLock";
            case Key::PrintScreen:   return "PrintScreen";
            case Key::Pause:         return "Pause";
            case Key::F1:            return "F1";
            case Key::F2:            return "F2";
            case Key::F3:            return "F3";
            case Key::F4:            return "F4";
            case Key::F5:            return "F5";
            case Key::F6:            return "F6";
            case Key::F7:            return "F7";
            case Key::F8:            return "F8";
            case Key::F9:            return "F9";
            case Key::F10:           return "F10";
            case Key::F11:           return "F11";
            case Key::F12:           return "F12";
            case Key::F13:           return "F13";
            case Key::F14:           return "F14";
            case Key::F15:           return "F15";
            case Key::F16:           return "F16";
            case Key::F17:           return "F17";
            case Key::F18:           return "F18";
            case Key::F19:           return "F19";
            case Key::F20:           return "F20";
            case Key::F21:           return "F21";
            case Key::F22:           return "F22";
            case Key::F23:           return "F23";
            case Key::F24:           return "F24";
            case Key::F25:           return "F25";

            // Keypad
            case Key::KP0:           return "KP0";
            case Key::KP1:           return "KP1";
            case Key::KP2:           return "KP2";
            case Key::KP3:           return "KP3";
            case Key::KP4:           return "KP4";
            case Key::KP5:           return "KP5";
            case Key::KP6:           return "KP6";
            case Key::KP7:           return "KP7";
            case Key::KP8:           return "KP8";
            case Key::KP9:           return "KP9";
            case Key::KPDecimal:     return "KPDecimal";
            case Key::KPDivide:      return "KPDivide";
            case Key::KPMultiply:    return "KPMultiply";
            case Key::KPSubtract:    return "KPSubtract";
            case Key::KPAdd:         return "KPAdd";
            case Key::KPEnter:       return "KPEnter";
            case Key::KPEqual:       return "KPEqual";

            case Key::LeftShift:     return "LeftShift";
            case Key::LeftControl:   return "LeftControl";
            case Key::LeftAlt:       return "LeftAlt";
            case Key::LeftSuper:     return "LeftSuper";
            case Key::RightShift:    return "RightShift";
            case Key::RightControl:  return "RightControl";
            case Key::RightAlt:      return "RightAlt";
            case Key::RightSuper:    return "RightSuper";
            case Key::Menu:          return "Menu";

            default:                     return "Unknown";
        }
    }

    ImGuiKey KeyToImGuiKey(Key key)
    {
        switch (key)
        {
        case Key::Tab: return ImGuiKey_Tab;
        case Key::Left: return ImGuiKey_LeftArrow;
        case Key::Right: return ImGuiKey_RightArrow;
        case Key::Up: return ImGuiKey_UpArrow;
        case Key::Down: return ImGuiKey_DownArrow;
        case Key::PageUp: return ImGuiKey_PageUp;
        case Key::PageDown: return ImGuiKey_PageDown;
        case Key::Home: return ImGuiKey_Home;
        case Key::End: return ImGuiKey_End;
        case Key::Insert: return ImGuiKey_Insert;
        case Key::Delete: return ImGuiKey_Delete;
        case Key::Backspace: return ImGuiKey_Backspace;
        case Key::Space: return ImGuiKey_Space;
        case Key::Enter: return ImGuiKey_Enter;
        case Key::Escape: return ImGuiKey_Escape;
        case Key::Apostrophe: return ImGuiKey_Apostrophe;
        case Key::Comma: return ImGuiKey_Comma;
        case Key::Minus: return ImGuiKey_Minus;
        case Key::Period: return ImGuiKey_Period;
        case Key::Slash: return ImGuiKey_Slash;
        case Key::Semicolon: return ImGuiKey_Semicolon;
        case Key::Equal: return ImGuiKey_Equal;
        case Key::LeftBracket: return ImGuiKey_LeftBracket;
        case Key::Backslash: return ImGuiKey_Backslash;
        case Key::World1: return ImGuiKey_Oem102;
        case Key::World2: return ImGuiKey_Oem102;
        case Key::RightBracket: return ImGuiKey_RightBracket;
        case Key::GraveAccent: return ImGuiKey_GraveAccent;
        case Key::CapsLock: return ImGuiKey_CapsLock;
        case Key::ScrollLock: return ImGuiKey_ScrollLock;
        case Key::NumLock: return ImGuiKey_NumLock;
        case Key::PrintScreen: return ImGuiKey_PrintScreen;
        case Key::Pause: return ImGuiKey_Pause;
        case Key::KP0: return ImGuiKey_Keypad0;
        case Key::KP1: return ImGuiKey_Keypad1;
        case Key::KP2: return ImGuiKey_Keypad2;
        case Key::KP3: return ImGuiKey_Keypad3;
        case Key::KP4: return ImGuiKey_Keypad4;
        case Key::KP5: return ImGuiKey_Keypad5;
        case Key::KP6: return ImGuiKey_Keypad6;
        case Key::KP7: return ImGuiKey_Keypad7;
        case Key::KP8: return ImGuiKey_Keypad8;
        case Key::KP9: return ImGuiKey_Keypad9;
        case Key::KPDecimal: return ImGuiKey_KeypadDecimal;
        case Key::KPDivide: return ImGuiKey_KeypadDivide;
        case Key::KPMultiply: return ImGuiKey_KeypadMultiply;
        case Key::KPSubtract: return ImGuiKey_KeypadSubtract;
        case Key::KPAdd: return ImGuiKey_KeypadAdd;
        case Key::KPEnter: return ImGuiKey_KeypadEnter;
        case Key::KPEqual: return ImGuiKey_KeypadEqual;
        case Key::LeftShift: return ImGuiKey_LeftShift;
        case Key::LeftControl: return ImGuiKey_LeftCtrl;
        case Key::LeftAlt: return ImGuiKey_LeftAlt;
        case Key::LeftSuper: return ImGuiKey_LeftSuper;
        case Key::RightShift: return ImGuiKey_RightShift;
        case Key::RightControl: return ImGuiKey_RightCtrl;
        case Key::RightAlt: return ImGuiKey_RightAlt;
        case Key::RightSuper: return ImGuiKey_RightSuper;
        case Key::Menu: return ImGuiKey_Menu;
        case Key::D0: return ImGuiKey_0;
        case Key::D1: return ImGuiKey_1;
        case Key::D2: return ImGuiKey_2;
        case Key::D3: return ImGuiKey_3;
        case Key::D4: return ImGuiKey_4;
        case Key::D5: return ImGuiKey_5;
        case Key::D6: return ImGuiKey_6;
        case Key::D7: return ImGuiKey_7;
        case Key::D8: return ImGuiKey_8;
        case Key::D9: return ImGuiKey_9;
        case Key::A: return ImGuiKey_A;
        case Key::B: return ImGuiKey_B;
        case Key::C: return ImGuiKey_C;
        case Key::D: return ImGuiKey_D;
        case Key::E: return ImGuiKey_E;
        case Key::F: return ImGuiKey_F;
        case Key::G: return ImGuiKey_G;
        case Key::H: return ImGuiKey_H;
        case Key::I: return ImGuiKey_I;
        case Key::J: return ImGuiKey_J;
        case Key::K: return ImGuiKey_K;
        case Key::L: return ImGuiKey_L;
        case Key::M: return ImGuiKey_M;
        case Key::N: return ImGuiKey_N;
        case Key::O: return ImGuiKey_O;
        case Key::P: return ImGuiKey_P;
        case Key::Q: return ImGuiKey_Q;
        case Key::R: return ImGuiKey_R;
        case Key::S: return ImGuiKey_S;
        case Key::T: return ImGuiKey_T;
        case Key::U: return ImGuiKey_U;
        case Key::V: return ImGuiKey_V;
        case Key::W: return ImGuiKey_W;
        case Key::X: return ImGuiKey_X;
        case Key::Y: return ImGuiKey_Y;
        case Key::Z: return ImGuiKey_Z;
        case Key::F1: return ImGuiKey_F1;
        case Key::F2: return ImGuiKey_F2;
        case Key::F3: return ImGuiKey_F3;
        case Key::F4: return ImGuiKey_F4;
        case Key::F5: return ImGuiKey_F5;
        case Key::F6: return ImGuiKey_F6;
        case Key::F7: return ImGuiKey_F7;
        case Key::F8: return ImGuiKey_F8;
        case Key::F9: return ImGuiKey_F9;
        case Key::F10: return ImGuiKey_F10;
        case Key::F11: return ImGuiKey_F11;
        case Key::F12: return ImGuiKey_F12;
        case Key::F13: return ImGuiKey_F13;
        case Key::F14: return ImGuiKey_F14;
        case Key::F15: return ImGuiKey_F15;
        case Key::F16: return ImGuiKey_F16;
        case Key::F17: return ImGuiKey_F17;
        case Key::F18: return ImGuiKey_F18;
        case Key::F19: return ImGuiKey_F19;
        case Key::F20: return ImGuiKey_F20;
        case Key::F21: return ImGuiKey_F21;
        case Key::F22: return ImGuiKey_F22;
        case Key::F23: return ImGuiKey_F23;
        case Key::F24: return ImGuiKey_F24;
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
