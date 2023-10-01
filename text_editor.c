#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 1000

typedef struct node
{
    char letter;
    struct node *next;
} node;

typedef node* string;



void append_char(string, char);

int length(string);

void append_string(string, string);

void print(string);

string new_string(char[]);

int get_string(char[BUFFER_SIZE]);

void clear_buffer(char[BUFFER_SIZE]);

void free_string(string);

void clear_string(string);

void display_options(string);

void dashes(int);

void newline(int);

void take_choice(int*);

void display_text(string);

int search(string, string);

string create_segment_string(string);

int replace(string, string, string);

void clear_screen();

int is_separator(char);

int contains_separator(char []);

int is_alphanumeric(char);

int same_letter_insensitive(char a, char b);

void display_notification(char[]);


int main()
{
    char buffer[BUFFER_SIZE+2];
    clear_buffer(buffer);
    char notification[BUFFER_SIZE+2];
    clear_buffer(notification);

    int choice;

    string main_text = new_string("");
    start:
    clear_screen();
    display_notification(notification);
    display_text(main_text);
    display_options(main_text);
    retake_choices:
    take_choice(&choice);

    int main_text_len = length(main_text);
    if(choice == 1)
    {
        append:
        printf("Please enter the text that you would like to append: ");
        if(!get_string(buffer))
        {
            continue_prompt1:
            printf("Would you like to try again? (1 to try again | 0 to choose another operation) ");
            if(scanf("%d", &choice) != 1)
            {
                printf("\nInvalid input...\n");
                goto continue_prompt1;
            }
            if(choice == 1){
                int c;
                while ((c = getchar()) != '\n' && c != EOF);
                goto append;
            }
            else if(choice == 0) goto start;
            else {
                printf("\nInvalid input...\n");
                goto continue_prompt1;
            }
        }
        string input = new_string(buffer);
        clear_buffer(buffer);
        append_string(main_text, input);
        free_string(input);
    }
    else if(choice == 2 && main_text_len > 0)
    {
        search:
        printf("Please enter the word that you would like to find: ");
        if(!get_string(buffer))
        {
            continue_prompt2:
            printf("Would you like to try again? (1 = try again | 0 = back to options) ");
            if(scanf("%d", &choice) != 1)
            {
                printf("\nInvalid input...\n");
                goto continue_prompt2;
            }
            if(choice == 1){
                int c;
                while ((c = getchar()) != '\n' && c != EOF);
                goto search;
            }
            else if(choice == 0) goto start;
            else {
                printf("\nInvalid input...\n");
                goto continue_prompt2;
            }
        }
        if(contains_separator(buffer))
        {
            printf("\nYour word contains a separator, your input may only contain alphanumeric characters.\n");
            goto search;
        }
        char temp[BUFFER_SIZE+2];
        strcpy(temp, buffer);
        string input = new_string(buffer);
        int occs = search(main_text, input);
        if(occs <= 0)
        {
            strcpy(notification, "\nNo occurrences of the word \"");
            strcat(notification, temp);
            strcat(notification, "\" were found.\n");
        }
        else if(occs == 1)
        {
            strcpy(notification, "\n1 occurrence of the word \"");
            strcat(notification, temp);
            strcat(notification, "\" was found.\n");
        }
        else
        {
            strcpy(notification, "\n");

            char number[BUFFER_SIZE+2];
            sprintf(number, "%d", occs);

            strcat(notification, number);

            strcat(notification, " occurrences of the word \"");
            strcat(notification, temp);
            strcat(notification, "\" were found.\n");
        }
    }
    else if(choice == 3 && main_text_len > 0)
    {
        replace:
        printf("Please enter the word that you would like to replace: ");
        if(!get_string(buffer))
        {
            continue_prompt3:
            printf("Would you like to try again? (1 to try again | 0 to choose another operation) ");
            if(scanf("%d", &choice) != 1)
            {
                printf("\nInvalid input...\n");
                goto continue_prompt3;
            }
            if(choice == 1){
                int c;
                while ((c = getchar()) != '\n' && c != EOF);
                goto replace;
            }
            else if(choice == 0) goto start;
            else {
                printf("\nInvalid input...\n");
                goto continue_prompt3;
            }
        }
        if(contains_separator(buffer))
        {
            printf("\nYour input may only contain alphanumeric characters. (No spaces or symbols)\n\n");
            goto replace;
        }
        char temp1[BUFFER_SIZE+2];
        strcpy(temp1, buffer);
        string target = new_string(buffer);
        clear_buffer(buffer);
        replace_with:
        printf("What do you want to replace it with: ");
        if(!get_string(buffer))
        {
            continue_prompt4:
            printf("Would you like to try again? (1 to try again | 0 back to options) ");
            if(scanf("%d", &choice) != 1)
            {
                printf("\nInvalid input...\n");
                goto continue_prompt4;
            }
            if(choice == 1){
                int c;
                while ((c = getchar()) != '\n' && c != EOF);
                goto replace_with;
            }
            else if(choice == 0) goto start;
            else {
                printf("\nInvalid input...\n");
                goto continue_prompt4;
            }
        }
        char temp2[BUFFER_SIZE+2];
        strcpy(temp2, buffer);
        string replacement = new_string(buffer);
        clear_buffer(buffer);
        int replaced = replace(main_text, target, replacement);
        if(replaced <= 0)
        {
            strcpy(notification, "\nNo words were replaced.\n");
        }
        else if(replaced == 1)
        {
            strcpy(notification, "\n1 occurrence of the word \"");
            strcat(notification, temp1);
            strcat(notification, "\" was replaced with \"");
            strcat(notification, temp2);
            strcat(notification, "\".\n");
        }
        else
        {
            strcpy(notification, "\n");

            char number[BUFFER_SIZE+2];
            sprintf(number, "%d", replaced);

            strcat(notification, number);
            strcat(notification, " occurrences of the word \"");
            strcat(notification, temp1);
            strcat(notification, "\" were replaced with \"");
            strcat(notification, temp2);
            strcat(notification, "\".\n");
        }
        free_string(target);
        free_string(replacement);

    }
    else if(choice == 4 && main_text_len > 0)
    {
        delete_option:
        printf("Are you sure? All of your text will be lost. (1 = Delete | 0 = Cancel)\n");
        take_choice(&choice);
        if(choice == 1)
        {
            clear_string(main_text);
        }
        else if(choice == 0){}
        else
        {
            printf("\nInvalid input...\n");
            goto delete_option;
        }
    }
    else if(choice == 0)
    {
        exit_option:
        printf("Are you sure that you want to exit the program? (1 = Exit | 0 = Cancel)\n");
        take_choice(&choice);
        if(choice == 1)
        {
            printf("\nFarewell...\n");
            return 0;
        }
        else if(choice == 0){}
        else
        {
            printf("\nInvalid input...\n");
            goto exit_option;
        }
    }
    else
    {
        printf("\nInvalid input...\n");
        goto retake_choices;
    }
    goto start;
}

void append_char(string str, char letter)
{
    node *curr = str;
    while(curr->next != NULL)
    {
        curr = curr->next;
    }
    curr->next = malloc(sizeof(node));
    curr->next->letter = letter;
    curr->next->next = NULL;
}

void print(string str)
{
    node *curr = str->next;
    while(curr != NULL)
    {
        printf("%c", curr->letter);
        curr = curr->next;
    }
}

int length(string str)
{
    int len = 0;
    node *curr = str->next;
    while(curr != NULL)
    {
        len++;
        curr = curr->next;
    }
    return len;
}

void append_string(string str1, string str2)
{
    node *curr = str1;
    while(curr->next != NULL)
    {
        curr = curr->next;
    }
    int len = length(str2);
    node *curr2 = str2;
    for(int i = 0; i < len; i++)
    {
        curr->next = malloc(sizeof(node));
        curr->next->next = NULL;
        curr->next->letter = curr2->next->letter;
        curr = curr->next;
        curr2 = curr2->next;
    }
}

void free_string(string str)
{
    node *prev = str;
    node *curr = str;
    while(curr != NULL)
    {
        prev = curr;
        curr = curr->next;
        free(prev);
    }
}

string new_string(char str[])
{

    node *head = malloc(sizeof(node));
    head->next = NULL;
    head->letter = '\0';
    int len = strlen(str);
    if(len > 0) {
        for(int i = 0; i < len; i++)
        {
            append_char(head, str[i]);
        }
    }
    return head;
}

void clear_string(string str)
{
    free_string(str->next);
    str->next = NULL;
}

int get_string(char buffer[])
{
    clear_buffer(buffer);
    if(fgets(buffer, BUFFER_SIZE, stdin) == NULL) {
        printf("\nInvalid input...\n");
        return 0;
    }
    if(strchr(buffer, '\n') == NULL)
    {
        printf("\nThe input exceeded the maximum amount of characters (%d chars)\n", BUFFER_SIZE);
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
        clear_buffer(buffer);
        return 0;
    }
    buffer[strcspn(buffer, "\n")] = 0;
    return 1;
}

void clear_buffer(char buffer[])
{
    memset(buffer,0,BUFFER_SIZE+2);
}

void dashes(int n)
{
    for(int i = 0; i < n; i++)
    {
        printf("-");
    }
}

void newline(int n)
{
    for(int i = 0; i < n; i++)
    {
        printf("\n");
    }
}

void display_options(string txt)
{
    newline(1);
    dashes(20);
    newline(1);
    printf("Options:\n");
    printf("1- Enter text.\n");
    if(length(txt) > 0)
    {
        printf("2- Search for word.\n");
        printf("3- Replace word.\n");
        printf("4- Delete.\n");
    }
    printf("0- Exit.\n");
    dashes(20);
    newline(1);
}

void display_text(string str)
{
    if(length(str) < 1) return;
    newline(1);
    printf("Your text: \n");
    dashes(10);
    newline(1);
    print(str);
    newline(1);
    newline(1);
}

void take_choice(int *choice)
{
    beginning:
    printf("Your choice: ");
    if(scanf("%d", choice) != 1)
    {
        printf("\nInvalid input... your input may only contain numeric values.\n");
        *choice = 0;
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
        goto beginning;
    }
    getchar();
}

int search(string text, string target)
{
    int result = 0;
    node *prev_prev = text;
    node *curr = text->next;
    node *curr2 = target->next;
    while(curr != NULL)
    {
        int skip = 0;
        node *prev = curr;
        while(same_letter_insensitive(curr->letter, curr2->letter))
        {
            if(curr2->next == NULL)
            {
                skip = 1;
                if((prev_prev->letter == '\0' || !is_alphanumeric(prev_prev->letter)) && (curr->next == NULL || !is_alphanumeric(curr->next->letter)))
                {
                    result++;
                }
                break;
            }
            else if(curr->next == NULL) return result;
            curr = curr->next;
            curr2 = curr2->next;
        }
        if(!skip) {
            curr = prev->next;
            prev_prev = prev;
        } else {
            curr = curr->next;
            prev_prev = prev_prev->next;
        }
        curr2 = target->next;
    }
    return result;
}

string create_segment_string(string str)
{
    node *curr = str->next;
    node *copy_head = malloc(sizeof(node));
    node *copy = copy_head;
    while(curr != NULL)
    {
        copy->letter = curr->letter;
        copy->next = curr->next == NULL ? NULL : malloc(sizeof(node));
        copy = copy->next;
        curr = curr->next;
    }
    return copy_head;
}

int replace(string text, string target, string replacement)
{
    int replaced = 0;
    node *prev_prev = text;
    node *curr = text->next;
    node *curr2 = target->next;
    while(curr != NULL)
    {
        int found = 0;
        while(same_letter_insensitive(curr->letter, curr2->letter))
        {
            if(curr2->next == NULL)
            {
                if((prev_prev->letter == '\0' || !is_alphanumeric(prev_prev->letter)) && (curr->next == NULL || !is_alphanumeric(curr->next->letter)))
                {
                    found = 1;
                }
                break;
            }
            else if(curr->next == NULL) return replaced;
            curr = curr->next;
            curr2 = curr2->next;
        }
        if(found) {
            string replacement_copy = create_segment_string(replacement);
            int tar_len = length(target);
            int rep_len = length(replacement);
            node *to_delete = prev_prev->next;
            prev_prev->next = replacement_copy;
            node *tail = replacement_copy;
            for(int i = 0; i < rep_len-1; i++)
            {
                tail = tail->next;
            }
            node *delete_now = to_delete;
            for(int i = 0; i < tar_len; i++)
            {
                to_delete = delete_now->next;
                free(delete_now);
                delete_now = to_delete;
            }
            tail->next = to_delete;
            curr = to_delete;
            prev_prev = tail;
            replaced++;
        } else {
            curr = prev_prev->next->next;
            prev_prev = prev_prev->next;
        }
        curr2 = target->next;
    }
    return replaced;
}

void clear_screen()
{
    printf("\e[1;1H\e[2J");
}

int is_separator(char c)
{
    char separators[] = " !\"#$&'()*+,-./:;<=>?@[\\]^_`{|}~";
    int i = 0;
    while(separators[i] != '\0')
    {
        if(c == separators[i])
        {
            printf("\n|THE SEPARATOR: %c|\n", separators[i]);
            return 1;
        }
        i++;
    }
    return 0;
}

int is_alphanumeric(char c)
{
    return (c > 47 && c < 58) || (c > 64 && c < 91) || (c > 96 && c < 123);
}

int contains_separator(char str[])
{
    int i = 0;
    while(str[i] != '\0')
    {
        if(!is_alphanumeric(str[i]))
        {
            return 1;
        }
        i++;
    }
    return 0;
}

int same_letter_insensitive(char a, char b)
{
    if((a == b) || ((a > 96 && a < 123) && (a - 32) == b) || ((a > 64 && a < 91) && (a + 32) == b)) return 1;
    return 0;
}

void display_notification(char str[])
{
    if(strlen(str) <= 0) return;
    printf("Notifications:\n");
    dashes(14);
    newline(1);
    printf("%s",str);
    newline(1);
    dashes(20);
    newline(1);
    clear_buffer(str);
}
