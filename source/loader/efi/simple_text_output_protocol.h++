#ifndef GOS_LOADER_EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL_HPP
#define GOS_LOADER_EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL_HPP

#include <cstdint>

#include <loader/efi/event.h++>
#include <loader/efi/macros.h++>
#include <loader/efi/status.h++>

namespace efi {
    EFI_STRUCT simple_text_output_mode {
        std::int32_t max_mode;
        std::int32_t mode;
        std::int32_t attribute;
        std::int32_t cursor_column;
        std::int32_t cursor_row;
        bool cursor_visible;
    };
    static_assert(sizeof(simple_text_output_mode) == 21);

    EFIAPI EFI_STRUCT simple_text_output_protocol {
    private:
        status_y (*_reset)(simple_text_output_protocol *, bool);
        status_y (*_output_string)(simple_text_output_protocol *, char16_t *);
        status_y (*_test_string)(simple_text_output_protocol *, char16_t *);
        status_y (*_query_mode)(simple_text_output_protocol *, std::size_t, std::size_t *, std::size_t *);
        status_y (*_set_mode)(simple_text_output_protocol *, std::size_t);
        status_y (*_set_attribute)(simple_text_output_protocol *, std::size_t);
        status_y (*_clear_screen)(simple_text_output_protocol *);
        status_y (*_set_cursor_position)(simple_text_output_protocol *, std::size_t, std::size_t);
        status_y (*_enable_cursor)(simple_text_output_protocol *, bool);
    public:

        inline status_y reset(bool extended_verification) {
            return this->_reset(this, extended_verification);
        }

        inline status_y output_string(char16_t *string) {
            return this->_output_string(this, string);
        }

        inline status_y test_string(char16_t *string) {
            return this->_test_string(this, string);
        }

        inline status_y query_mode(std::size_t mode_number, std::size_t &columns, std::size_t &rows) {
            return this->_query_mode(this, mode_number, &columns, &rows);
        }

        inline status_y set_mode(std::size_t mode_number) {
            return this->_set_mode(this, mode_number);
        }

        inline status_y set_attribute(std::size_t attributes) {
            return this->_set_attribute(this, attributes);
        }

        inline status_y clear_screen() {
            return this->_clear_screen(this);
        }

        inline status_y set_cursor_position(std::size_t column, std::size_t row) {
            return this->_set_cursor_position(this, column, row);
        }

        inline status_y enable_cursor(bool visible) {
            return this->_enable_cursor(this, visible);
        }
    };

    static_assert(sizeof(simple_text_output_protocol) == 9 * sizeof(void*));
}

#endif // ifndef GOS_LOADER_EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL_HPP