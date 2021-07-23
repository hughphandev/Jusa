#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum token_type
{
    TOKEN_EOS = 0,
    TOKEN_PLUS = '+',
    TOKEN_MINUS = '-',
    TOKEN_DIVIDE = '/',

    TOKEN_BITWISE_AND = '&',
    TOKEN_BITWISE_OR = '|',

    TOKEN_OPEN_PAREN = '(',
    TOKEN_CLOSE_PAREN = ')',

    TOKEN_OPEN_BRACE = '{',
    TOKEN_CLOSE_BRACE = '}',

    TOKEN_OPEN_BRACKET = '[',
    TOKEN_CLOSE_BRACKET = ']',

    TOKEN_COLON = ':',
    TOKEN_SEMI = ';',

    TOKEN_ASTERISK = '*',
    TOKEN_HASH = '#',

    TOKEN_IDENTIFIER = 256,
    TOKEN_NUMBER,
    TOKEN_STRING,
};

struct token
{
    size_t textLength;
    char* text;
    token_type type;

};

bool IsLiteral(char c)
{
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9');
}

bool IsEndOfLine(char c)
{
    return (c == '\n' || c == '\r');
}

bool IsWhiteSpace(char c)
{
    return (c == ' ' || c == '\t' || IsEndOfLine(c));
}

bool IsAlpha(char c)
{
    return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
}

bool IsNumber(char c)
{
    return (c >= '0' && c <= '9');
}


char* LoadFile(char* fileName)
{
    FILE* file = fopen(fileName, "r");
    char* result = 0;
    if (file)
    {
        fseek(file, 0, SEEK_END);
        size_t size = ftell(file);
        rewind(file);
        result = (char*)malloc(size + 1);
        fread(result, 1, size, file);
        result[size] = '\0';

        fclose(file);
    }
    return result;
}

struct tokenizer
{
    char* at;
};


void EatAllWhiteSpaces(tokenizer* tknizer)
{
    for (;;)
    {
        if (IsWhiteSpace(tknizer->at[0]))
        {
            ++tknizer->at;
        }
        else if (tknizer->at[0] == '/' && tknizer->at[1] == '/')
        {
            tknizer += 2;
            while (tknizer->at[0] && !IsEndOfLine(tknizer->at[0]))
            {
                ++tknizer->at;
            }
        }
        else if (tknizer->at[0] == '/' && tknizer->at[1] == '*')
        {
            tknizer += 2;
            while (!IsEndOfLine(tknizer->at[0]))
            {
                ++tknizer;
            }
        }
        else
        {
            break;
        }
    }
}

token GetToken(tokenizer* tknizer)
{
    token result = {};
    EatAllWhiteSpaces(tknizer);

    if (IsAlpha(tknizer->at[0]))
    {
        char* base = tknizer->at;
        result.type = TOKEN_IDENTIFIER;
        result.text = tknizer->at;
        while (!IsWhiteSpace(tknizer->at[0]))
        {
            ++tknizer->at;
        }
        result.textLength = tknizer->at - base;
    }
    else if (tknizer->at[0] == '"')
    {
        ++tknizer->at;
        result.text = tknizer->at;
        result.type = TOKEN_STRING;
        char* base = tknizer->at;

        while (tknizer->at[0] && tknizer->at[0] != '"')
        {
            if (tknizer->at[0] == '\\' && tknizer->at[1])
            {
                //NOTE: special character
                ++tknizer->at;
            }
            ++tknizer->at;
        }
        result.textLength = tknizer->at - base;
        ++tknizer->at;
    }
    else
    {
        result.type = (token_type)tknizer->at[0];
        result.textLength = 1;
        result.text = tknizer->at;
        ++tknizer->at;
    }

    return result;
}

int main(int argc, char* argv[])
{
    if (argc > 1)
    {
        char* src = LoadFile(argv[1]);

        tokenizer tknizer = {};
        tknizer.at = src;

        bool parsing = true;
        while (parsing)
        {
            token tk = GetToken(&tknizer);
            switch (tk.type)
            {
                default:
                {
                    printf("%i: %.*s\n", tk.type, tk.textLength, tk.text);
                } break;

                case TOKEN_EOS:
                {
                    parsing = false;
                } break;
            }
        }
    }
    else
    {
        puts("not enough parameters");
    }

    return 0;
}
