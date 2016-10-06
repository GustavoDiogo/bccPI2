#include <stdio.h>
#include <stdlib.h>

typedef struct Questions {
    int answer_num;
    char *main_question;
    char *correct_answer;
    char *answer2;
    char *answer3;
    char *answer4;
    char *answer5;
}Question;


int main()
{
    char q1[] = "Primeira pergunta aqui";
    char ca[] = "resposta certa aqui";
    char wa1[] = "Resposta errada 1";
    char wa2[] = "Resposta errada 2";
    char wa3[] = "Resposta errada 3";

    char newline[] = "\r\n";
    size_t size = sizeof(newline) - sizeof(char);

    FILE *questions_file = fopen("questions.txt", "wb");

    fwrite("4", sizeof(char), 1, questions_file);
    fwrite(newline, size, 1, questions_file);
    fwrite(q1, sizeof(q1), 1, questions_file);
    fwrite(newline, size, 1, questions_file);
    fwrite(ca, sizeof(ca), 1, questions_file);
    fwrite(newline, size, 1, questions_file);
    fwrite(wa1, sizeof(wa1), 1, questions_file);
    fwrite(newline, size, 1, questions_file);
    fwrite(wa2, sizeof(wa2), 1, questions_file);
    fwrite(newline, size, 1, questions_file);
    fwrite(wa3, sizeof(wa3), 1, questions_file);
    fwrite(newline, size, 1, questions_file);

    fclose(questions_file);

    free(questions_file);
    questions_file = NULL;

    questions_file = fopen("questions.txt", "r");

    char str[100];
    fgets(str, 100, questions_file);

    int num = str[0] - 48;
    int x = 0;

    Question new_question;
    new_question.answer_num = num;

    char qstr[100];
    fgets(qstr, 100, questions_file);
    new_question.main_question = qstr;

    char astr1[100];
    fgets(astr1, 100, questions_file);
    new_question.correct_answer = astr1;

    char astr2[100];
    fgets(astr2, 100, questions_file);
    new_question.answer2 = astr2;

    if(num > 2) {

        char astr3[100];
        fgets(astr3, 100, questions_file);
        new_question.answer3 = astr3;

        if(num > 3) {

            char astr4[100];
            fgets(astr4, 100, questions_file);
            new_question.answer4 = astr4;

            if(num > 4){

                char astr5[100];
                fgets(astr5, 100, questions_file);
                new_question.answer5 = astr5;
            }
        }
    }

    fclose(questions_file);

    printf("%s\n", new_question.main_question);
    printf("%s\n", new_question.correct_answer);
    printf("%s\n", new_question.answer2);
    printf("%s\n", new_question.answer3);
    printf("%s\n", new_question.answer4);

    return 0;
}
