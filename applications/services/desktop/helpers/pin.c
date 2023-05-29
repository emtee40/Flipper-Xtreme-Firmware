#include "pin.h"

#include <notification/notification.h>
#include <notification/notification_messages.h>
#include <stddef.h>
#include <furi.h>
#include <furi_hal.h>
#include <gui/gui.h>

#include "../desktop_i.h"

static const NotificationSequence sequence_pin_fail = {
    &message_display_backlight_on,

    &message_red_255,
    &message_vibro_on,
    &message_delay_100,
    &message_vibro_off,
    &message_red_0,

    &message_delay_250,

    &message_red_255,
    &message_vibro_on,
    &message_delay_100,
    &message_vibro_off,
    &message_red_0,
    NULL,
};

void desktop_pin_lock_error_notify() {
    NotificationApp* notification = furi_record_open(RECORD_NOTIFICATION);
    notification_message(notification, &sequence_pin_fail);
    furi_record_close(RECORD_NOTIFICATION);
}

uint32_t desktop_pin_lock_get_fail_timeout() {
    uint32_t pin_fails = furi_hal_rtc_get_pin_fails();
    if(pin_fails < 3) {
        return 0;
    }
    return 30 * pow(2, pin_fails - 3);
}

bool desktop_pin_compare(const PinCode* pin_code1, const PinCode* pin_code2) {
    furi_assert(pin_code1);
    furi_assert(pin_code2);
    bool result = false;

    if(pin_code1->length == pin_code2->length) {
        result = !memcmp(pin_code1->data, pin_code2->data, pin_code1->length);
    }

    return result;
}
