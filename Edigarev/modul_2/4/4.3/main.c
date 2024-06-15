#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Contact {
    char lastName[50];
    char firstName[50];
    char middleName[50];
    char workPlace[100];
    char position[100];
    char phoneNumber[20];
    char email[100];
    char socialMedia[100];
    char messengerProfile[100];
} Contact;

typedef struct TreeNode {
    Contact contact;
    struct TreeNode* left;
    struct TreeNode* right;
    int height;
} TreeNode;

// Функция для создания нового контакта
Contact createContact(const char* lastName, const char* firstName, const char* middleName,
                      const char* workPlace, const char* position, const char* phoneNumber,
                      const char* email, const char* socialMedia, const char* messengerProfile) {
    Contact contact;
    strcpy(contact.lastName, lastName);
    strcpy(contact.firstName, firstName);
    strcpy(contact.middleName, middleName);
    strcpy(contact.workPlace, workPlace);
    strcpy(contact.position, position);
    strcpy(contact.phoneNumber, phoneNumber);
    strcpy(contact.email, email);
    strcpy(contact.socialMedia, socialMedia);
    strcpy(contact.messengerProfile, messengerProfile);
    return contact;
}

// Функция для создания нового узла дерева
TreeNode* createTreeNode(Contact contact) {
    TreeNode* newNode = (TreeNode*)malloc(sizeof(TreeNode));
    newNode->contact = contact;
    newNode->left = NULL;
    newNode->right = NULL;
    newNode->height = 1; // Начальная высота нового узла
    return newNode;
}

// Функция для получения высоты узла
int height(TreeNode* node) {
    if (node == NULL) {
        return 0;
    }
    return node->height;
}

// Функция для получения максимума из двух чисел
int max(int a, int b) {
    return (a > b) ? a : b;
}

// Правый поворот поддерева с корнем y
TreeNode* rightRotate(TreeNode* y) {
    TreeNode* x = y->left;
    TreeNode* T2 = x->right;

    // Выполняем поворот
    x->right = y;
    y->left = T2;

    // Обновляем высоты
    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    // Возвращаем новый корень
    return x;
}

// Левый поворот поддерева с корнем x
TreeNode* leftRotate(TreeNode* x) {
    TreeNode* y = x->right;
    TreeNode* T2 = y->left;

    // Выполняем поворот
    y->left = x;
    x->right = T2;

    // Обновляем высоты
    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    // Возвращаем новый корень
    return y;
}

// Получение коэффициента баланса узла
int getBalance(TreeNode* node) {
    if (node == NULL) {
        return 0;
    }
    return height(node->left) - height(node->right);
}

// Вставка контакта в дерево с балансировкой
TreeNode* insertTreeNode(TreeNode* node, Contact contact) {
    if (node == NULL) {
        return createTreeNode(contact);
    }

    if (strcmp(contact.lastName, node->contact.lastName) < 0) {
        node->left = insertTreeNode(node->left, contact);
    } else if (strcmp(contact.lastName, node->contact.lastName) > 0) {
        node->right = insertTreeNode(node->right, contact);
    } else {
        return node; // Дубликаты не разрешены
    }

    node->height = 1 + max(height(node->left), height(node->right));

    int balance = getBalance(node);

    // Левый левый случай
    if (balance > 1 && strcmp(contact.lastName, node->left->contact.lastName) < 0) {
        return rightRotate(node);
    }

    // Правый правый случай
    if (balance < -1 && strcmp(contact.lastName, node->right->contact.lastName) > 0) {
        return leftRotate(node);
    }

    // Левый правый случай
    if (balance > 1 && strcmp(contact.lastName, node->left->contact.lastName) > 0) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    // Правый левый случай
    if (balance < -1 && strcmp(contact.lastName, node->right->contact.lastName) < 0) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

// Поиск минимального узла в дереве
TreeNode* minValueNode(TreeNode* node) {
    TreeNode* current = node;

    while (current->left != NULL) {
        current = current->left;
    }

    return current;
}

// Удаление контакта из дерева с балансировкой
TreeNode* deleteTreeNode(TreeNode* root, const char* lastName) {
    if (root == NULL) {
        return root;
    }

    if (strcmp(lastName, root->contact.lastName) < 0) {
        root->left = deleteTreeNode(root->left, lastName);
    } else if (strcmp(lastName, root->contact.lastName) > 0) {
        root->right = deleteTreeNode(root->right, lastName);
    } else {
        if ((root->left == NULL) || (root->right == NULL)) {
            TreeNode* temp = root->left ? root->left : root->right;

            if (temp == NULL) {
                temp = root;
                root = NULL;
            } else {
                *root = *temp;
            }
            free(temp);
        } else {
            TreeNode* temp = minValueNode(root->right);
            root->contact = temp->contact;
            root->right = deleteTreeNode(root->right, temp->contact.lastName);
        }
    }

    if (root == NULL) {
        return root;
    }

    root->height = 1 + max(height(root->left), height(root->right));

    int balance = getBalance(root);

    if (balance > 1 && getBalance(root->left) >= 0) {
        return rightRotate(root);
    }

    if (balance > 1 && getBalance(root->left) < 0) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    if (balance < -1 && getBalance(root->right) <= 0) {
        return leftRotate(root);
    }

    if (balance < -1 && getBalance(root->right) > 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}

// Функция для отображения контактов в дереве (симметричный обход)
void displayContacts(TreeNode* root) {
    if (root != NULL) {
        displayContacts(root->left);
        printf("Фамилия: %s\n", root->contact.lastName);
        printf("Имя: %s\n", root->contact.firstName);
        printf("Отчество: %s\n", root->contact.middleName);
        printf("Место работы: %s\n", root->contact.workPlace);
        printf("Должность: %s\n", root->contact.position);
        printf("Номер телефона: %s\n", root->contact.phoneNumber);
        printf("Электронная почта: %s\n", root->contact.email);
        printf("Социальные сети: %s\n", root->contact.socialMedia);
        printf("Профиль в мессенджере: %s\n\n", root->contact.messengerProfile);
        displayContacts(root->right);
    }
}

int main() {
    TreeNode* root = NULL;

    Contact contact1 = createContact("Иванов", "Иван", "Иванович", "Компания А", "Менеджер", "89001234567", "ivanov@example.com", "ivanov_vk", "ivanov_telegram");
    Contact contact2 = createContact("Петров", "Петр", "Петрович", "Компания Б", "Директор", "89007654321", "petrov@example.com", "petrov_vk", "petrov_telegram");
    Contact contact3 = createContact("Сидоров", "Сидор", "Сидорович", "Компания В", "Инженер", "89001234568", "sidorov@example.com", "sidorov_vk", "sidorov_telegram");
        Contact contact4 = createContact("Сидоров1", "Сидор1", "Сидорович1", "Компания В1", "Инженер1", "890012345681", "sidorov@example.com1", "sidorov_vk1", "sidorov_telegram1");

    root = insertTreeNode(root, contact1);
    root = insertTreeNode(root, contact2);
    root = insertTreeNode(root, contact3);
    root = insertTreeNode(root, contact4);

    printf("Контакты после добавления:\n");
    displayContacts(root);

    root = deleteTreeNode(root, "Петров");

    printf("Контакты после удаления Петрова:\n");
    displayContacts(root);

    // Освобождение памяти
    while (root != NULL) {
        root = deleteTreeNode(root, root->contact.lastName);
    }

    return 0;
}

