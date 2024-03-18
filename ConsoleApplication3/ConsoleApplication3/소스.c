#include <stdio.h>
#include <string.h>
#include <stdlib.h> // atoi 함수 사용을 위해 추가

#define MAX_TEXT_LENGTH 50
#define MAX_KEY_LENGTH 16

void main() {
    int index, keyIndex, keyLength, textLength, operation;
    char text[MAX_TEXT_LENGTH + 1] = { 0 }, key[MAX_KEY_LENGTH + 1] = { 0 }, selectionBuffer[5];

    // 사용자로부터 암호문 또는 평문 입력 받기
    printf("암호문 또는 평문을 입력: ");
    fgets(text, sizeof(text), stdin);
    // fgets로 인한 개행 문자 제거
    text[strcspn(text, "\n")] = 0;

    // 암호화 또는 복호화 선택
    printf("암호는 1번, 복호는 2번 선택: ");
    fgets(selectionBuffer, sizeof(selectionBuffer), stdin);
    operation = atoi(selectionBuffer); // 문자열을 숫자로 변환

    // 암호 키 입력
    printf("키 값 입력(소문자만): ");
    fgets(key, sizeof(key), stdin);
    key[strcspn(key, "\n")] = 0; // 개행 문자 제거

    textLength = strlen(text);
    keyLength = strlen(key);

    // 키의 길이가 입력 텍스트의 길이보다 긴 경우, 키의 길이를 조정
    if (keyLength > textLength) keyLength = textLength;

    // 키를 0~25 사이의 값으로 변환
    for (keyIndex = 0; keyIndex < keyLength; keyIndex++) {
        key[keyIndex] = key[keyIndex] - 'a';
    }

    for (index = 0; index < textLength; index++) {
        keyIndex = index % keyLength;

        if (operation == 1) { // 암호화
            if (text[index] >= 'a' && text[index] <= 'z') {
                text[index] = (text[index] - 'a' + key[keyIndex] + 26) % 26 + 'a';
            }
            else if (text[index] >= 'A' && text[index] <= 'Z') {
                text[index] = (text[index] - 'A' + key[keyIndex] + 26) % 26 + 'A';
            }
        }
        else if (operation == 2) { // 복호화
            if (text[index] >= 'a' && text[index] <= 'z') {
                text[index] = (text[index] - 'a' - key[keyIndex] + 26) % 26 + 'a';
            }
            else if (text[index] >= 'A' && text[index] <= 'Z') {
                text[index] = (text[index] - 'A' - key[keyIndex] + 26) % 26 + 'A';
            }
        }
    }

    // 결과 출력
    printf("\n암호화 또는 복호화된 결과 출력: ");
    puts(text);
}
