#include <stdio.h>
#include <string.h>
#include <stdlib.h> // atoi �Լ� ����� ���� �߰�

#define MAX_TEXT_LENGTH 50
#define MAX_KEY_LENGTH 16

void main() {
    int index, keyIndex, keyLength, textLength, operation;
    char text[MAX_TEXT_LENGTH + 1] = { 0 }, key[MAX_KEY_LENGTH + 1] = { 0 }, selectionBuffer[5];

    // ����ڷκ��� ��ȣ�� �Ǵ� �� �Է� �ޱ�
    printf("��ȣ�� �Ǵ� ���� �Է�: ");
    fgets(text, sizeof(text), stdin);
    // fgets�� ���� ���� ���� ����
    text[strcspn(text, "\n")] = 0;

    // ��ȣȭ �Ǵ� ��ȣȭ ����
    printf("��ȣ�� 1��, ��ȣ�� 2�� ����: ");
    fgets(selectionBuffer, sizeof(selectionBuffer), stdin);
    operation = atoi(selectionBuffer); // ���ڿ��� ���ڷ� ��ȯ

    // ��ȣ Ű �Է�
    printf("Ű �� �Է�(�ҹ��ڸ�): ");
    fgets(key, sizeof(key), stdin);
    key[strcspn(key, "\n")] = 0; // ���� ���� ����

    textLength = strlen(text);
    keyLength = strlen(key);

    // Ű�� ���̰� �Է� �ؽ�Ʈ�� ���̺��� �� ���, Ű�� ���̸� ����
    if (keyLength > textLength) keyLength = textLength;

    // Ű�� 0~25 ������ ������ ��ȯ
    for (keyIndex = 0; keyIndex < keyLength; keyIndex++) {
        key[keyIndex] = key[keyIndex] - 'a';
    }

    for (index = 0; index < textLength; index++) {
        keyIndex = index % keyLength;

        if (operation == 1) { // ��ȣȭ
            if (text[index] >= 'a' && text[index] <= 'z') {
                text[index] = (text[index] - 'a' + key[keyIndex] + 26) % 26 + 'a';
            }
            else if (text[index] >= 'A' && text[index] <= 'Z') {
                text[index] = (text[index] - 'A' + key[keyIndex] + 26) % 26 + 'A';
            }
        }
        else if (operation == 2) { // ��ȣȭ
            if (text[index] >= 'a' && text[index] <= 'z') {
                text[index] = (text[index] - 'a' - key[keyIndex] + 26) % 26 + 'a';
            }
            else if (text[index] >= 'A' && text[index] <= 'Z') {
                text[index] = (text[index] - 'A' - key[keyIndex] + 26) % 26 + 'A';
            }
        }
    }

    // ��� ���
    printf("\n��ȣȭ �Ǵ� ��ȣȭ�� ��� ���: ");
    puts(text);
}
