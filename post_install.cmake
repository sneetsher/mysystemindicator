execute_process(COMMAND glib-compile-schemas /usr/share/glib-2.0/schemas/
                RESULT_VARIABLE Result
                OUTPUT_VARIABLE Output
                ERROR_VARIABLE Error)
