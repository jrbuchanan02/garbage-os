#ifndef GOS_LOADER_EFI_SIMPLE_TEXT_INPUT_PROTOCOL_HPP
#define GOS_LOADER_EFI_SIMPLE_TEXT_INPUT_PROTOCOL_HPP

#include <cstdint>

#include <loader/efi/event.h++>
#include <loader/efi/macros.h++>
#include <loader/efi/status.h++>

namespace efi {
    EFI_STRUCT input_key {
        std::uint16_t scan_code;
        char16_t unicode_char;
    };
    static_assert(sizeof(input_key) == 4);

    EFI_STRUCT key_toggle_state {
        union {
            EFI_STRUCT {
                std::uint8_t scroll_lock : 1;
                std::uint8_t num_lock : 1;
                std::uint8_t caps_lock : 1;
                std::uint8_t : 3;
                std::uint8_t key_state_exposed : 1;
                std::uint8_t toggle_state_valid : 1;
            } toggle_flags;
            std::uint8_t toggle_state;
        };
    };

    EFI_STRUCT key_state {
        union {
            EFI_STRUCT {
                std::uint32_t rshift : 1;
                std::uint32_t lshift : 1;
                std::uint32_t rcontrol : 1;
                std::uint32_t lcontrol : 1;
                std::uint32_t ralt : 1;
                std::uint32_t lalt : 1;
                std::uint32_t rlogo : 1;
                std::uint32_t llogo : 1;
                std::uint32_t menu : 1;
                std::uint32_t sysreq : 1;
                std::uint32_t : 21;
                std::uint32_t valid : 1;
            } shift_flags;
            std::uint32_t shift_state;
        };
        key_toggle_state toggle_state;
    };
    static_assert(sizeof(key_state) == 5);

    EFI_STRUCT key_data {
        input_key key;
        key_state state;
    };

    static_assert(sizeof(key_data) == 9);

    EFIAPI using key_notify_function = status_y (*)(key_data);

    EFIAPI EFI_STRUCT simple_text_input_protocol {
    private:
        status_y (*_reset)(simple_text_input_protocol *, bool);
        status_y (*_read_key_stroke)(simple_text_input_protocol *, key_data *);
    public:
        event_y wait_for_key;

        inline status_y reset(bool extended_verification) {
            return this->_reset(this, extended_verification);
        }

        inline status_y read_key_stroke(key_data &data) {
            return this->_read_key_stroke(this, &data);
        }
    };

    static_assert(sizeof(simple_text_input_protocol) == 3 * sizeof(void *));

    EFIAPI EFI_STRUCT simple_text_input_ex_protocol : public simple_text_input_protocol {
    private:
        status_y (*_set_state)(simple_text_input_ex_protocol *, key_toggle_state *);
        status_y (*_register_key_notify)(simple_text_input_ex_protocol *, key_data *, key_notify_function, void **);
        status_y (*_unregister_key_notify)(simple_text_input_ex_protocol *, void *);
    public:
        inline status_y set_state(key_toggle_state state) {
            return this->_set_state(this, &state);
        }

        inline status_y register_key_notify(key_data *data, key_notify_function callback, void *&handle) {
            return this->_register_key_notify(this, data, callback, &handle);
        }

        inline status_y unregister_key_notify(void *handle) {
            return this->_unregister_key_notify(this, handle);
        }
    };

    static_assert(sizeof(simple_text_input_ex_protocol) == 6 * sizeof(void *));
}

#endif // ifndef GOS_LOADER_EFI_SIMPLE_TEXT_INPUT_PROTOCOL_HPP