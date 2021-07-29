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
    TOKEN_COMA = ',',

    TOKEN_ASTERISK = '*',
    TOKEN_HASH = '#',

    TOKEN_IDENTIFIER = 256,
    TOKEN_NUMBER,
    TOKEN_KEYWORD,
    TOKEN_STRING,
};

struct token
{
    size_t textLength;
    char* text;
    token_type type;
};

struct node
{
    //TODO: AST parsing
    token value;

    node* left;
    node* right;
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
    //NOTE: Read in text mode convert \r\n to \n cause the fread to read less than the file size!
    FILE* file = fopen(fileName, "rb");
    char* result = 0;
    if (file)
    {
        fseek(file, 0, SEEK_END);
        size_t size = ftell(file);
        rewind(file);
        result = (char*)malloc(size + 1);
        size = fread(result, sizeof(char), size, file);
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
        while (IsAlpha(tknizer->at[0]) || IsNumber(tknizer->at[0]) || tknizer->at[0] == '_')
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

bool TokenEqual(token tk, char* match)
{
    for (int i = 0; i < tk.textLength; ++i)
    {
        if (tk.text[i] != match[i])
        {
            return false;
        }
    }

    return (match[tk.textLength] == '\0');
}

int main(int argc, char* argv[])
{
    if (argc > 1)
    {
        char* src = LoadFile(argv[1]);

        tokenizer tknizer = {};
        tknizer.at = src;

        //TODO: Dynamic allocate token!
#define MAX_TOKEN_COUNT 1000
        token tokens[MAX_TOKEN_COUNT] = {};

        int tokenCount = 0;
        int index = 0;
        while (tokenCount < MAX_TOKEN_COUNT)
        {
            token tk = GetToken(&tknizer);
            if(tk.type == TOKEN_EOS) break;
            tokens[index++] = tk;
            ++tokenCount;
        }

        for (int i = 0; i < tokenCount; ++i)
        {
            printf("%i: %.*s\n", tokens[i].type, (int)tokens[i].textLength, tokens[i].text);
        }
    }
    return 0;
}
