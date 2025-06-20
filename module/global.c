#include "../lib/global.h"

int GLOBAL_printLineInFile(char *filename, char *line)
{
    FILE *file = fopen(filename, "a");
    if (file == NULL)
    {
        printf("ERROR: Could not open file %s for writing.\n", filename);
        return -1;
    }

    fprintf(file, "%s\n", line);
    fclose(file);
    return 0;
}

int GLOBAL_validatePassword(char* password) {
    int uppercase = 0, lowercase = 0, special = 0, length = 0;

    for (int i = 0; password[i] != '\0'; i++) {
        length++;

        if (password[i] >= 'A' && password[i] <= 'Z') {
            uppercase = 1;
        }

        else if (password[i] >= 'a' && password[i] <= 'z') {
            lowercase = 1;
        }

        else if ((password[i] >= '!' && password[i] <= '/') || (password[i] >= ':' && password[i] <= '@') || (password[i] >= '[' && password[i] <= '`') || (password[i] >= '{' && password[i] <= '~')) {
            special = 1;
        }
    }

	if (length >= 8 && uppercase && lowercase && special) {
        return 1;
    } else {
        return 0;
    }
}

char GLOBAL_isValidEmailChar(char c) {
    return ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == '_' || c == '-');
}

char GLOBAL_validateEmail(char *email) {
    char valid = 0;
    int at_count = 0;
    int at_position = -1;
    char dot_count = 0;
    int length = strlen(email);

    if (length == 0) {
		valid = 0;
    }

    for (int i = 0; i < length; i++) {
        switch (email[i]) {
            case '@':
                at_count++;

				if (at_count > 1) {
				    valid = 0;
				}
                
				at_position = i;
                break;
            case '.':
                if (at_position != -1 && i > at_position) {
                    dot_count = 1;
                }

                break;
            default:
                if (!GLOBAL_isValidEmailChar(email[i])) {
                    return (0);
                }

                break;
        }
    }
    
    if (at_count == 1 && at_position > 1 && at_position < length - 3 && dot_count) {
        valid = 1;
    }

    return valid;
}

char *GLOBAL_toLower(const char *str)
{
    if (!str)
        return NULL;

    char *lower_str = strdup(str);
    for (int i = 0; lower_str[i]; i++)
    {
        if (lower_str[i] >= 'A' && lower_str[i] <= 'Z')
            lower_str[i] = lower_str[i] + ('a' - 'A');
    }
    return lower_str;
}

int GLOBAL_containsIgnoreCase(const char *text, const char *filter)
{
    if (!text || !filter)
        return 0;

    char *lower_text = GLOBAL_toLower(text);
    char *lower_filter = GLOBAL_toLower(filter);

    int result = strstr(lower_text, lower_filter) != NULL;

    free(lower_text);
    free(lower_filter);

    return result;
}