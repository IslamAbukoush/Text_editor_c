#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 10000

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

int get_string(char[BUFFER_SIZE], char[], string);

void clear_buffer(char[BUFFER_SIZE]);

void free_string(string);

void clear_string(string);

void display_options(string);

void dashes(int);

void newline(int);

int take_choice(int *, char[], string);

void display_text(string);

int search(string, string);

string create_segment_string(string);

int replace(string, string, string);

void clear_screen();

int contains_separator(char[], char[]);

int is_alphanumeric(char);

int same_letter_insensitive(char a, char b);

void display_notification(char[]);

node *new_node(char c);

void flush();

void search_ntf(int, char[], char[]);

void replace_ntf(int, char[], char[], char[]);

void refresh(string, char[]);


int main()
{
    int choice;
    char buffer[BUFFER_SIZE+2];
    char notification[BUFFER_SIZE+2];
    clear_buffer(notification);
    string main_text = new_string("");

    start:
    clear_buffer(buffer);
    refresh(main_text, notification);
    if(!take_choice(&choice, notification, main_text)) goto start;
    int main_text_len = length(main_text);

    switch(choice)
    {
        case 1:
            goto append;
        case 2:
            goto search;
        case 3:
            goto replace;
        case 4:
            goto delete_option;
        case 0:
            goto exit_option;
        default:
            strcpy(notification, "\nInvalid input...\n");
            goto start;
    }

    append:
    printf("Please enter the text that you would like to append(Max %d characters per append):\n", BUFFER_SIZE);
    if(!get_string(buffer, notification, main_text)) goto append;
    string input = new_string(buffer);
    append_string(main_text, input);
    free_string(input);
    goto start;

    search:
    if(main_text_len <= 0)
    {
        strcpy(notification, "\nInvalid input...\n");
        goto start;
    }
    printf("Please enter the word that you would like to find: ");
    if(!get_string(buffer, notification, main_text) || contains_separator(buffer, notification))
    {
        refresh(main_text, notification);
        goto search;
    }
    string input2 = new_string(buffer);
    int occs = search(main_text, input2);
    search_ntf(occs, buffer, notification);
    free_string(input2);
    goto start;

    replace:
    if(main_text_len <= 0)
    {
        strcpy(notification, "\nInvalid input...\n");
        goto start;
    }
    printf("Please enter the word that you would like to replace: ");
    if(!get_string(buffer, notification, main_text) || contains_separator(buffer, notification))
    {
        refresh(main_text, notification);
        goto replace;
    }
    char replacable[BUFFER_SIZE+2];
    strcpy(replacable, buffer);
    string target = new_string(buffer);
    clear_buffer(buffer);

    replace_with:
    printf("What do you want to replace it with: ");
    if(!get_string(buffer, notification, main_text)) goto replace_with;
    string replacement = new_string(buffer);
    int replaced = replace(main_text, target, replacement);
    replace_ntf(replaced, replacable, buffer, notification);
    free_string(target);
    free_string(replacement);
    clear_buffer(buffer);
    goto start;

    delete_option:
    if(main_text_len <= 0)
    {
        strcpy(notification, "\nInvalid input...\n");
        goto start;
    }
    printf("Are you sure? All of your text will be lost. (1 = Delete | 0 = Cancel)\n");
    if(!take_choice(&choice, notification, main_text)) 
    {
        refresh(main_text, notification);
        goto delete_option;
    }
    switch(choice)
    {
    case 1:
        clear_string(main_text);
        goto start;
    case 0:
        goto start;
    default:
        strcpy(notification, "\nInvalid input...\n");
        refresh(main_text, notification);
        goto delete_option;
    }

    exit_option:
    printf("Are you sure that you want to exit the program? (1 = Exit | 0 = Cancel)\n");
    if(!take_choice(&choice, notification, main_text))
    {
        refresh(main_text, notification);
        goto exit_option;
    }
    switch(choice)
    {
    case 1:
        clear_screen();
        printf("\nFarewell...\n");
        return 0;
    case 0:
        goto start;
    default:
        strcpy(notification, "\nInvalid input...\n");
        refresh(main_text, notification);
        goto exit_option;
    }
}

node *new_node(char c)
{
    node *result_node = (node*)malloc(sizeof(node));
    result_node->letter = c;
    result_node->next = NULL;
    return result_node;
}

void append_char(string str, char letter)
{
    node *curr = str;
    while(curr->next != NULL)
    {
        curr = curr->next;
    }
    curr->next = new_node(letter);
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
        curr->next = new_node(curr2->next->letter);
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

    node *head = new_node('\0');
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

int get_string(char buffer[], char ntf[], string main_text)
{
    clear_buffer(buffer);
    if(fgets(buffer, BUFFER_SIZE, stdin) == NULL) {
        strcpy(ntf, "\nInvalid input...\n");
        refresh(main_text, ntf);
        return 0;
    }
    if(strchr(buffer, '\n') == NULL)
    {
        strcpy(ntf, "\nThe input exceeded the maximum amount of characters (");

        char number[BUFFER_SIZE+2];
        sprintf(number, "%d", BUFFER_SIZE);

        strcat(ntf, number);
        strcat(ntf, " chars)\n");
        flush();
        clear_buffer(buffer);
        refresh(main_text, ntf);
        return 0;
    }
    buffer[strcspn(buffer, "\n")] = 0;
    return 1;
}

void clear_buffer(char buffer[])
{
    memset(buffer, 0, BUFFER_SIZE+2);
}

void dashes(int n)
{
    for(int i = 0; i < n; i++) printf("-");
    printf("\n");
}

void newline(int n)
{
    for(int i = 0; i < n; i++) printf("\n");
}

void display_options(string txt)
{
    newline(1);
    dashes(20);
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
}

void display_text(string str)
{
    if(length(str) < 1) return;
    newline(1);
    printf("Your text: \n");
    dashes(10);
    print(str);
    newline(2);
}

int take_choice(int *choice, char ntf[], string main_text)
{
    printf("Your choice: ");
    if(scanf("%d", choice) != 1)
    {
        strcpy(ntf, "\nInvalid input... your input may only contain numeric values.\n");
        *choice = 0;
        flush();
        return 0;
    }
    getchar();
    refresh(main_text, ntf);
    return 1;
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
    node *copy_head = new_node('\0');
    node *copy = copy_head;
    while(curr != NULL)
    {
        copy->letter = curr->letter;
        copy->next = curr->next == NULL ? NULL : new_node('\0');
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

int is_alphanumeric(char c)
{
    return (c > 47 && c < 58) || (c > 64 && c < 91) || (c > 96 && c < 123);
}

int contains_separator(char str[], char ntf[])
{
    int i = 0;
    while(str[i] != '\0')
    {
        if(!is_alphanumeric(str[i]))
        {
            strcpy(ntf, "\nYour input may only contain alphanumeric characters. (No spaces or symbols)\n\n");
            return 1;
        }
        i++;
    }
    return 0;
}

int same_letter_insensitive(char a, char b)
{
    return (a == b) || ((a > 96 && a < 123) && (a - 32) == b) || ((a > 64 && a < 91) && (a + 32) == b);
}

void display_notification(char str[])
{
    if(strlen(str) <= 0) return;
    printf("Notifications:\n");
    dashes(14);
    printf("%s",str);
    newline(1);
    dashes(20);
    clear_buffer(str);
}

void flush()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void search_ntf(int occs, char buffer[], char ntf[])
{
    if(occs <= 0)
    {
        strcpy(ntf, "\nNo occurrences of the word \"");
        strcat(ntf, buffer);
        strcat(ntf, "\" were found.\n");
    }
    else if(occs == 1)
    {
        strcpy(ntf, "\n1 occurrence of the word \"");
        strcat(ntf, buffer);
        strcat(ntf, "\" was found.\n");
    }
    else
    {
        strcpy(ntf, "\n");

        char number[BUFFER_SIZE+2];
        sprintf(number, "%d", occs);

        strcat(ntf, number);

        strcat(ntf, " occurrences of the word \"");
        strcat(ntf, buffer);
        strcat(ntf, "\" were found.\n");
    }
}

void refresh(string main_text, char ntf[])
{
    clear_screen();
    display_notification(ntf);
    display_text(main_text);
    display_options(main_text);
}

void replace_ntf(int replaced, char replacable[], char buffer[], char ntf[])
{
    if(replaced <= 0)
    {
        strcpy(ntf, "\nNo words were replaced. (the word \"");
        strcat(ntf, replacable);
        strcat(ntf, "\" was not found in the text)\n");
    }
    else if(replaced == 1)
    {
        strcpy(ntf, "\n1 occurrence of the word \"");
        strcat(ntf, replacable);
        strcat(ntf, "\" was replaced with \"");
        strcat(ntf, buffer);
        strcat(ntf, "\".\n");
    }
    else
    {
        strcpy(ntf, "\n");

        char number[BUFFER_SIZE+2];
        sprintf(number, "%d", replaced);

        strcat(ntf, number);
        strcat(ntf, " occurrences of the word \"");
        strcat(ntf, replacable);
        strcat(ntf, "\" were replaced with \"");
        strcat(ntf, buffer);
        strcat(ntf, "\".\n");
    }
}
