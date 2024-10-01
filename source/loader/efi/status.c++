#include <loader/efi/status.h++>

#include <climits>

// mask for the most significant bit
constexpr std::size_t msb = 1LLU << (sizeof msb * CHAR_BIT - 1);

efi::status_type_e efi::categorize_status(status_y status) {
    if (msb & status) {
        return status_type_e::error_code;
    } else if (status == 0) {
        return status_type_e::success;
    } else {
        return status_type_e::warning_code;
    }
}

bool efi::error_code(status_y status) {
    return (status & msb) != 0;
}

struct status_description {
    efi::status_y code;
    char16_t const *const description;
};

// the standardized status codes and their descriptions in the EFI specification.
// the descriptions are copied verbatim. These descriptions, perhaps to minimize
// the quantity of error codes, are really bad. "a parameter was incorrect." tells
// the user and the developer jack about which parameter was incorrect, these codes
// should be replaced with a more verbose information system, perhaps one that includes
// the function we tried to call and a list of the provided parameters.
constexpr status_description const statuses[] = {
    { 0,  u"The operation completed successfully." },   // EFI_SUCCESS
    { msb | 1, u"The image failed to load."},   // EFI_LOAD_ERROR
    { msb | 2, u"A parameter was incorrect."},  // EFI_INVALID_PARAMETER
    { msb | 3, u"The operation is not supported."}, // EFI_UNSUPPORTED
    { msb | 4, u"The buffer was not the proper size for the request."}, // EFI_BAD_BUFFER_SIZE
    { msb | 5, u"The buffer is not large enough to hold the requested data. The required buffer size is returned in the appropriate parameter when this error occurs."},    // EFI_BUFFER_TOO_SMALL
    { msb | 6, u"There is no data pending upon return."},   // EFI_NOT_READY
    { msb | 7, u"The physical device reported an error while attempting the operation."},   // EFI_DEVICE_ERROR
    { msb | 8, u"The device cannot be written to."},    // EFI_WRITE_PROTECTED
    { msb | 9, u"A resource has run out."}, // EFI_OUT_OF_RESOURCES
    // This is exactly as listed in the UEFI specification version 2.9, I believe that this
    // description should read "An inconsistancy was detected on the file system causing the operation to fail."
    // meaning that whoever was typing up table D-3 was perhaps typing too fast.
    { msb | 10, u"An inconstancy was detected on the file system causing the operating to fail."}, // EFI_VOLUME_CORRUPTED
    { msb | 11, u"There is no more space on the file system."}, // EFI_VOLUME_FULL
    { msb | 12, u"The device does not contain any medium to perform the operation."},   // EFI_NO_MEDIA
    { msb | 13, u"The medium in the device has changed since the last access."},    // EFI_MEDIA_CHANGED
    { msb | 14, u"The item was not found."},    // EFI_NOT_FOUND
    { msb | 15, u"Access was denied."}, // EFI_ACCESS_DENIED
    { msb | 16, u"The server was not found or did not respond to the request."},    // EFI_NO_RESPONSE
    { msb | 17, u"A mapping to a device does not exist."},  // EFI_NO_MAPPING
    { msb | 18, u"The timeout time expired."},  // EFI_TIMEOUT
    { msb | 19, u"The protocol has not been started."}, // EFI_NOT_STARTED
    { msb | 20, u"The protocol has already been started."}, // EFI_ALREADY_STARTED
    { msb | 21, u"The operation was aborted."},   // EFI_ABORTED
    { msb | 22, u"An ICMP error occurred during the network operation."},   // EFI_ICMP_ERROR
    { msb | 23, u"A TFTP error occurred during the network operation."},   // EFI_TFTP_ERROR
    { msb | 24, u"A protocol error occurred during the network operation."},   // EFI_PROTOCOL_ERROR
    { msb | 25, u"The function encountered an internal version that was incompatible with a version requested by the caller."},   // EFI_INCOMPATIBLE_VERSION
    { msb | 26, u"The function was not performed due to a security violation."},   // EFI_SECURITY_VIOLATION
    { msb | 27, u"A CRC error was detected."},   // EFI_CRC_ERROR
    { msb | 28, u"The beginning or end of media was reached."},   // EFI_END_OF_MEDIA
    { msb | 31, u"The end of the file was reached."},   // EFI_END_OF_FILE
    { msb | 32, u"The language specified was invalid."},   // EFI_INVALID_LANGUAGE
    { msb | 33, u"The security status of the data is unknown or compromised and the data must be updated or replaced  to restore a valid security status."},   // EFI_COMPROMISED_DATA
    // I'm not sure if this description makes a complete sentence.
    { msb | 34, u"There is an address conflict address allocation."},   // EFI_IP_ADDRESS_CONFLICT
    { msb | 35, u"An HTTP error occurred during the network operation."},   // EFI_HTTP_ERROR
    { 1, u"The string contained one or more characters that the device could not render and were skipped."},  // EFI_WARN_UNKNOWN_GLYPH
    { 2, u"The handle was closed, but the file was not deleted."},  // EFI_WARN_DELETE_FAILURE
    { 3, u"The handle was closed, but the data to the file was not flushed properly."},  // EFI_WARN_WRITE_FAILURE
    { 4, u"The resulting buffer was too small, and the data was truncated to the buffer size."},  // EFI_WARN_BUFFER_TOO_SMALL
    { 5, u"The data has not been updated within the timeframe set by local policy for this type of data."},  // EFI_WARN_STALE_DATA
    { 6, u"The resulting buffer contains UEFI-compliant file system."},  // EFI_WARN_FILE_SYSTEM
    { 7, u"The operation will be processed across a system reset."},  // EFI_WARN_RESET_REQUIRED
};

// an equally bad error message that will need reworking.
constexpr static char16_t const *const unknown_error_message = u"An unknown UEFI error occurred. Contact your device manufacturer for more details.";
constexpr static char16_t const *const unknown_warning_message = u"An unknown UEFI warning was encountered. Garbage OS might run face first into an error.";

char16_t const * efi::get_status_description(status_y status) {
    // check for the status in the list of known descriptions
    for(std::size_t i = 0; i < sizeof statuses / sizeof statuses[0]; i++) {
        if (statuses[i].code == status) {
            return statuses[i].description;
        }
    }
    // if the status is unknown, report the error message if it's an error
    // if it's not an error, then assume it's a warning (since only EFI_SUCCESS
    // is given that all-important title of a success code).
    if (status & msb) {
        return unknown_error_message;
    } else {
        return unknown_warning_message;
    }
}