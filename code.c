
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Student {
    char name[20];
    int kor;
    int eng;
    int math;
    int total;
    float avg;
    int rank;
    struct Student* next;
} Student;

/* 함수 선언 */
void addStudent(Student** head, Student** tail);
void printStudents(Student* head);
void calculate(Student* head);
void saveFile(Student* head);
void loadFile(Student** head, Student** tail);
void freeList(Student** head);

/* 학생 추가 */
void addStudent(Student** head, Student** tail) {
    Student* newNode = (Student*)malloc(sizeof(Student));

    printf("이름: ");
    scanf_s("%s", newNode->name);
    printf("국어 점수: ");
    scanf_s("%d", &newNode->kor);
    printf("영어 점수: ");
    scanf_s("%d", &newNode->eng);
    printf("수학 점수: ");
    scanf_s("%d", &newNode->math);

    newNode->total = 0;
    newNode->avg = 0;
    newNode->rank = 1;
    newNode->next = NULL;

    if (*head == NULL) {
        *head = *tail = newNode;
    }
    else {
        (*tail)->next = newNode;
        *tail = newNode;
    }
}

/* 총점, 평균, 등수 계산 */
void calculate(Student* head) {
    Student* p, * q;

    for (p = head; p != NULL; p = p->next) {
        p->total = p->kor + p->eng + p->math;
        p->avg = p->total / 3.0f;
        p->rank = 1;
    }

    for (p = head; p != NULL; p = p->next) {
        for (q = head; q != NULL; q = q->next) {
            if (q->total > p->total)
                p->rank++;
        }
    }
}

/* 성적 출력 */
void printStudents(Student* head) {
    Student* p = head;

    calculate(head);

    printf("-------------------------------------------------\n");
    printf("이름\t국어\t영어\t수학\t총점\t평균\t등수\n");
    printf("-------------------------------------------------\n");

    while (p != NULL) {
        printf("%s\t%d\t%d\t%d\t%d\t%.1f\t%d\n",
            p->name, p->kor, p->eng, p->math,
            p->total, p->avg, p->rank);
        p = p->next;
    }
}

/* 파일 저장 */
void saveFile(Student* head) {
    FILE* fp = fopen("student.dat", "wb");
    Student* p = head;

    if (!fp) {
        printf("파일 저장 실패\n");
        return;
    }

    while (p != NULL) {
        fwrite(p, sizeof(Student) - sizeof(Student*), 1, fp);
        p = p->next;
    }

    fclose(fp);
    printf("파일 저장 완료\n");
}

/* 파일 읽기 */
void loadFile(Student** head, Student** tail) {
    FILE* fp = fopen("student.dat", "rb");
    Student temp;

    if (!fp) {
        printf("파일이 없습니다.\n");
        return;
    }

    freeList(head);
    *tail = NULL;

    while (fread(&temp, sizeof(Student) - sizeof(Student*), 1, fp)) {
        Student* newNode = (Student*)malloc(sizeof(Student));
        *newNode = temp;
        newNode->next = NULL;

        if (*head == NULL)
            *head = *tail = newNode;
        else {
            (*tail)->next = newNode;
            *tail = newNode;
        }
    }

    fclose(fp);
    printf("파일 읽기 완료\n");
}

/* 메모리 해제 */
void freeList(Student** head) {
    Student* p;
    while (*head != NULL) {
        p = *head;
        *head = (*head)->next;
        free(p);
    }
}

/* 메인 함수 */
int main(void) {
    Student* head = NULL;
    Student* tail = NULL;
    int menu;

    while (1) {
        printf("\n[Menu]\n");
        printf("1. .dat 파일에서 데이터 읽기\n");
        printf("2. 추가 학생 정보 입력\n");
        printf("3. .dat 파일 저장\n");
        printf("4. 성적 확인 (평균, 등수)\n");
        printf("5. 종료\n");
        printf("선택(1~5): ");
        scanf_s("%d", &menu);

        switch (menu) {
        case 1:
            loadFile(&head, &tail);
            break;
        case 2:
            addStudent(&head, &tail);
            break;
        case 3:
            saveFile(head);
            break;
        case 4:
            printStudents(head);
            break;
        case 5:
            freeList(&head);
            printf("프로그램 종료\n");
            return 0;
        default:
            printf("잘못된 선택입니다.\n");
        }
    }
}
