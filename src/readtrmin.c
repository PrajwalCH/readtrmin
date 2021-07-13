#define READTRMIN_NSTRINGOPTIONS_COLL
#include "readtrmin.h"

#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

#include "str_util.h"

#define NULL_BYTE 1
#define MIN_BUFFER_SIZE 2
#define INT_MAX_BUFFER_SIZE 10

bool
readtrmin_int(long *pointer_arg, size_t max_input_len)
{
    assert(pointer_arg != NULL);
    assert(max_input_len < INT_MAX_BUFFER_SIZE);

    if (max_input_len >= INT_MAX_BUFFER_SIZE)
        max_input_len = INT_MAX_BUFFER_SIZE - 1;

    char buffer[INT_MAX_BUFFER_SIZE];
    clear_buffer(buffer, INT_MAX_BUFFER_SIZE);

    char *error_msg = "";
    size_t buffer_length = 0;
    size_t input_length = max_input_len + NULL_BYTE;

    if (!get_input(buffer, input_length))
        return false;

    if (has_buffer_overflow(buffer, INT_MAX_BUFFER_SIZE)) {
        flush_input_buffer();
        set_null_terminator(buffer, input_length - 1);
        buffer_length = input_length - 1;
    } else {
        buffer_length = replace_LF_with_NUL(buffer, input_length, input_length - 1);
    }

    if (is_null_input(buffer)) {
        error_msg = "expected some input but found nothing";
        goto RETURN_FAILURE;
    }

    errno = 0;
    char *endPtr = NULL;
    long converted_buffer = strtol(buffer, &endPtr, 10);

    if (errno != 0) {
        error_msg = "unable to convert the buffer into number";
        goto RETURN_FAILURE;
    }

    if (strcmp(endPtr, buffer) == 0) {
        error_msg = "unable to find valid data that can be convert into numbers";
        goto RETURN_FAILURE;
    }

    *pointer_arg = converted_buffer;
    return true;

RETURN_FAILURE:
    print_error(error_msg);
    return false;
}

bool
readtrmin_string(char *buffer_arg,
                 size_t buffer_size,
                 size_t  max_input_len,
                 StringOptions *string_option)
{
    assert(buffer_arg != NULL);
    assert(max_input_len < buffer_size);
    assert(string_option != NULL);

    if (max_input_len >= buffer_size)
        max_input_len = buffer_size - 1;

    clear_buffer(buffer_arg, buffer_size);
    char *error_msg = "";
    size_t buffer_length = 0;
    size_t input_length = max_input_len + NULL_BYTE;

    if (!get_input(buffer_arg, input_length))
        return false;

    if (has_buffer_overflow(buffer_arg, buffer_size)) {
        flush_input_buffer();
        set_null_terminator(buffer_arg, input_length - 1);
        buffer_length = input_length - 1;
    }
    else {
        // it will return num of items it found on buffer if new line is found
        // else it will return expected index we passed to it
        buffer_length = replace_LF_with_NUL(buffer_arg, input_length, input_length - 1);
    }

    if (is_null_input(buffer_arg)) {
        error_msg = "expected some input but found nothing";
        goto RETURN_FAILURE;
    }

    struct CharacterSets found_char_set = init_CharacterSets_struct();
    search_character_sets(buffer_arg, buffer_length, &found_char_set);

    if (!string_option->allow_space) {
        if (found_char_set.space) {
            error_msg = "spaces are not allowed";
            goto RETURN_FAILURE;
        }
    }

    if (!string_option->allow_number) {
        if (found_char_set.number) {
            error_msg = "numbers are not allowed";
            goto RETURN_FAILURE;
        }
    }

    if (!string_option->allow_symbol) {
        if (found_char_set.symbol) {
            error_msg = "special characters are not allowed";
            goto RETURN_FAILURE;
        }
    }

    if (!string_option->allow_uppercase) {
        if (found_char_set.uppercase) {
            error_msg = "uppercase are not allowed, as it is disabled on option";
            goto RETURN_FAILURE;
        }
    }

    if (!string_option->allow_lowercase) {
        if (found_char_set.lowercase) {
            error_msg = "lowecase are not allowed, as it is disabled on option";
            goto RETURN_FAILURE;
        }
    }

    return true;

RETURN_FAILURE:
    print_error(error_msg);
    clear_buffer(buffer_arg, buffer_size);
    return false;
}

bool
readtrmin_char(char *pointer_arg)
{
    *pointer_arg = 0;
    char *error_msg = "";
    char buffer[MIN_BUFFER_SIZE];
    clear_buffer(buffer, MIN_BUFFER_SIZE);

    if(!get_input(buffer, MIN_BUFFER_SIZE)) {
        return false;
    }

    if (has_buffer_overflow(buffer, MIN_BUFFER_SIZE)) {
        flush_input_buffer();
        set_null_terminator(buffer, 1);
    } else {
        replace_LF_with_NUL(buffer, MIN_BUFFER_SIZE, 1);
    }

    if (is_null_input(buffer)) {
        error_msg = "expected some input but found nothing";
        goto RETURN_FAILURE;
    }

    if (!isalpha(buffer[0])) {
        error_msg = "other then alphabet are not allowed";
        goto RETURN_FAILURE;
    }

    assert(isalpha(buffer[0]));
    assert(buffer[1] == '\0');
    *pointer_arg = buffer[0];
    return true;

RETURN_FAILURE:
    print_error(error_msg);
    return false;
}
