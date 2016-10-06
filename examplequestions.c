#include <stdio.h>
#include <stdlib.h>




int main()
{
    char q1[] = "Primeira pergunta aqui";
    char ca[] = "resposta certa aqui";
    char wa1[] = "Resposta errada 1";
    char wa2[] = "Resposta errada 2";
    char wa3[] = "Resposta errada 3";

    char newline[] = "\r\n";
    size_t size = sizeof(newline) - sizeof(char);

    FILE *questions = fopen("questions.txt", "wb");

    fwrite("4", sizeof(char), 1, questions);
    fwrite(newline, size, 1, questions);
    fwrite(q1, sizeof(q1), 1, questions);
    fwrite(newline, size, 1, questions);
    fwrite(ca, sizeof(ca), 1, questions);
    fwrite(newline, size, 1, questions);
    fwrite(wa1, sizeof(wa1), 1, questions);
    fwrite(newline, size, 1, questions);
    fwrite(wa2, sizeof(wa2), 1, questions);
    fwrite(newline, size, 1, questions);
    fwrite(wa3, sizeof(wa3), 1, questions);
    fwrite(newline, size, 1, questions);

    fclose(questions);

    free(questions);
    questions = NULL;

    questions = fopen("questions.txt", "r");

    char str[100];
    fgets(str, 100, questions);

    printf("%s", str);

    int num = str[0] - 48;
    int x = 0;

    for(x = 0; x <= num; x++) {
        char qstr[100];
        fgets(qstr, 100, questions);

        printf("%s\n", qstr);
    }

    fclose(questions);

    return 0;
}
