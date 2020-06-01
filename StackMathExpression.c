//
//  dev.noah0316
//  스택을 이용한 수식 계산
//

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define MAX_EXP 50

typedef int element;
typedef struct {
    element stack[MAX_EXP];
    int top;
} StackType;

// 스택 초기화 함수 선언
void init(StackType* s);
// 공백 상태 검출 함수 선언
int is_empty(StackType* s);
// 포화 상태 검출 함수 선언
int is_full(StackType* s);
// 삽입 함수 선언
void push(StackType* s, element item);
// 삭제 함수 선언
element pop(StackType* s);
// 피크 함수 선언
element peek(StackType* s);
// 연산자 우선순위 반환 함수 선언
int prec(char op);
// 중위 표기 수식 -> 후위 표기 수식 으로 변환하는 함수 선언
void infix_to_postfix(char exp[], char lee[]);
// 후위 표기 수식의 값을 계산하는 함수 선언
int eval(char exp[]);
// 중위 표기 수식 입력하는 함수 선언
void input_infix(char kim[]);
// 후위 표기 수식 출력하는 함수 선언
void print_post(char lee[]);

//메인 함수
int main(void) {
    int result;
    char kim[MAX_EXP];
    char lee[MAX_EXP];
    // 수식 입력
    input_infix(kim);
    // 후위 표기식 변환
    infix_to_postfix(kim, lee);
    // 후위 표기식 출력
    print_post(lee);
    result = eval(lee);
    printf("결과값은 %d", result);

    return 0;
}

// 스택 초기화 함수 정의
void init(StackType* s) {
    // 공백상태를 의미
    s->top = -1;
}

// 공백 상태 검출 함수
int is_empty(StackType* s) {

    // if문과 같은 의미이며, 스택의 top이 -1 이라면 공백상태라 가정하고 1을 반환
    return (s->top == -1);
}

// 포화 상태 검출 함수 정의
int is_full(StackType* s) {

    //if문과 같은 의미이며, 스택의 top이 MAX_EXP보다 1작다면 포화상태를 의미하므로 1을 반환
    return (s->top == (MAX_EXP - 1));
}

// 삽입 함수 정의
void push(StackType* s, element item) {
    // 삽입연산 전 포화상태 검사
    if (is_full(s)) {
        fprintf(stderr, "현재 스택이 포화 상태 입니다.\n");
        return;
    }

    else {
        // top 증가 후 삽입
        s->stack[++(s->top)] = item;
    }
}

// 삭제 함수 정의
element pop(StackType* s) {

    // 삭제 연산 전 공백상태 검사
    if (is_empty(s)) {
        fprintf(stderr, "스택이 공백 상태이므로 삭제할 것이 없습니다.\n");
        exit(1);
    }

    // 값 반환후 top 감소
    else {
        return s->stack[(s->top)--];
    }

}

//피크 함수 정의
element peek(StackType* s) {
    // 값을 읽어오기 전 공백상태 검사
    if (is_empty(s)) {
        fprintf(stderr, "스택이 공백 상태이므로 읽어올 data가 없습니다.\n");
        exit(1);
    }
    else {

        // 값만 반환
        return s->stack[s->top];
    }
}

// 연산자 반환 함수 정의
int prec(char op) {

    // 우선순위 ( ( or ) ) < ( + or - ) < ( * or / )

    // 인자로 전달받은 문자가 * 이거나 / 이면 2 반환
    if (op == '*' || op == '/') {
        return 2;
    }
    // 인자로 전달 받은 문자가 + 이거나 - 이면 1 반환
    else if (op == '+' || op == '-') {
        return 1;
    }
    // 인자로 전달 받은 문자가 ( or ) 이라면 0을 반환
    else if (op == '(' || op == ')') {
        return 0;
    }
    else {
        // + - * / ( ) 이외의 문자가 들어오면 에러메시지 출력
        fprintf(stderr, "prec함수는 연산자만 인자로 전달받습니다.");
        exit(1);
    }

}


// 중위 표기 수식 -> 후위 표기 수식 으로 변환하는 함수 정의
void infix_to_postfix(char exp[], char lee[]) {
    // main 함수를 건들지 않고(문제 그대로 풀기위헤), 안정성을 위해 초기화한 같은 사이즈의 배열을 lee에 대입 (혹시 모를 배열의 쓰레기 값에 대비하여)
    char init_lee[MAX_EXP] = {0,};
    strcpy(lee, init_lee);
    // i는 인자로 전달받은 expd의 인덱스, j는 후위 표기 수식을 저장할 배열의 인덱스를 의미한다.
    int i, j = 0;
    //인자로 전달 받은 exp배열의 문자를 하나씩 꺼내서 읽을 ch 변수와, 스택에 들어있는 top에 들어있는 값을 읽어올 top_op변수 선언
    char ch, top_op;
    // strlen함수의 반환형이 unsigned int 이므로 int형으로 캐스팅 해준다.
    int len = (int)strlen(exp);
    // 스택 선언
    StackType s;

    // 스택 초기화
    init(&s);

    // 인자로 전달받은 배열의 길이 만큼 반복
    for (i = 0; i < len; i++) {
        // exp에서 문자를 순서대로 한개씩 꺼내온다.
        ch = exp[i];

        //허용된 문자값 0~9, ( or ) +,-,*,/만 연산가능
        if ((ch >= 48 && ch <= 57) || (ch == '(' || ch == ')') || (ch == '+' || ch == '-' || ch == '*' || ch == '/')) {
            switch (ch) {
            // 연산자를 읽을 경우
            case '+': case '-': case '*': case '/':;
                // 스택이 공백이 아니고, 스택에 있는 연산자의 우선순위가 더 크거나 같다면
                while (!is_empty(&s) && (prec(ch) <= prec(peek(&s)))) {
                    // 스택에 있는 값을 하나 pop하고, 그 값을 lee배열 j번째에 저장
                    lee[j] = pop(&s);
                    // 다음 자리로 이동
                    j++;
                }
                // 읽은 문자의 현재 우선순위가 제일 높으므로 스택에 삽입
                push(&s, ch);
                break;

            // 왼쪽 괄호는 우선순위가 낮으므로 무조건 스택에 들어간다.
            case '(':
                push(&s, ch);
                break;

            // 오른쪽 괄호
            case ')':
                top_op = pop(&s);
                // 왼쪽 괄호를 만날때까지 스택을 비우며 읽은 값을 lee배열의 j번째에 저장
                while (top_op != '(') {
                    lee[j] = top_op;
                    top_op = pop(&s);
                    j++;
                }

                break;

            // 피연산자
            default:
                // 피연산자는 배열의 j번째에 바로 저장
                lee[j] = ch;
                j++;
                break;
            }



        }

        // 수식외에 다른 값이 들어오면 에러메시지 출력
        else {
            fprintf(stderr, "숫자, 연산자, ( or ) 외에는 입력될 수 없습니다.\n");
            exit(1);
        }

    }

    // 스택이 공백일때 까지 스택에 있는 값을 pop하고, 그 값을 lee배열 j번째에 저장
    while (!is_empty(&s)) {
        lee[j] = pop(&s);
        j++;
    }
}

// 후위 표기 수식의 값을 계산하는 함수 정의
int eval(char exp[]) {
    // 연산의 대상을 저장할 op1, op2 변수, char형 data를 int형으로 변환해 저장할 value변수, 인덱스 역할을 할 i변수 선언
    int op1, op2, value, i = 0;
    // strlen함수의 반환형이 unsigned int 이므로 int형으로 캐스팅 해준다.
    int len = (int)strlen(exp);
    // 인자로 전달 받은 exp배열의 문자를 하나씩 꺼내서 읽을 ch 변수선언
    char ch;
    StackType s;

    init(&s);
    for (i = 0; i < len; i++) {
        ch = exp[i];
        // 입력이 피연산자 이라면
        if ((ch != '+') && (ch != '-') && (ch != '*') && (ch != '/')) {
            // char형 data를 int형으로 바꾸는 연산
            value = ch - '0';
            // 스택에 int형으로 변환한 data 삽입
            push(&s, value);
        }
        // 연산자이면 피연산자를 스택에서 제거
        else {
            // 스택에 있는 값을 pop하고, op2에 저장 (second) 순서가 중요하다.
            op2 = pop(&s);
            // 스택에 있는 값을 pop하고, op1에 저장 (first)
            op1 = pop(&s);
            // 연산을 수행하고 값을 스택에 저장
            switch (ch) {
            case '+': push(&s, op1 + op2); break;
            case '-': push(&s, op1 - op2); break;
            case '*': push(&s, op1 * op2); break;
            case '/': push(&s, op1 / op2); break;
            }

        }
    }

    // 마지막에 있는값을 pop하며 값 반환
    return pop(&s);


}

// 중위 표기 수식 입력하는 함수 정의
void input_infix(char kim[]) {

    //main 함수를 건들지 않고(문제 그대로 풀기위헤), 안정성을 위해 초기화한 같은 사이즈의 배열을 lee에 대입 (혹시 모를 배열의 쓰레기 값에 대비하여)
    char init_kim[MAX_EXP] = {0,};
    strcpy(kim, init_kim);
    printf("<예> (2+3)*4+9 수식을 입력하시오 : ");
    // 문자열 kim에 저장
    scanf("%s", kim);


}

// 후위 표기 수식 출력하는 함수 정의
void print_post(char lee[]) {

    // 배열이 비어있지 않다면
    if (lee != '\0') {
        printf("%s\n", lee);
    }
    else {
        fprintf(stderr, "배열이 비어있으므로 출력할 내용이 없습니다.\n");
        return;
    }


}
