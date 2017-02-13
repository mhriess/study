#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"

struct node {
  int data;
  struct node* next;
};

/*
  Test Utilities
*/

void Context(char* message) {
  printf("%sContext: %s\n", KNRM, message);
}

void It(char* message, int pass) {
  if (pass) {
    printf("%sSuccess: %s\n", KGRN, message);
  } else {
    printf("%sFailure: %s\n", KRED, message);
  }
}


void PrintList(struct node* head) {
  if (head == NULL) {
    printf("NULL\n");
  } else {
    printf("%d -> ", head->data);

    PrintList(head->next);
  }
}

/*
  Problems
*/

// Set the headRef to a new node containing data at the head of the linked list.
void Push(struct node** headRef, int data) {
  struct node* newNode = malloc(sizeof(struct node));

  newNode->data = data;
  newNode->next = *headRef;
  *headRef = newNode;
}

// Return the length of the linked list.
int Length(struct node* head) {
  struct node* current = head;
  int count = 0;

  while (current != NULL) {
    count++;
    current = current->next;
  }

  return count;
}

// Build a linked list {1,2,3}
struct node* BuildOneTwoThree() {
  struct node* list = NULL;

  for (int i = 3; i > 0; --i)
  {
    Push(&list, i);
  }

  return list;
}

// Return the occurrences of target in a linked list
int Count(struct node* head, int target) {
  struct node* current = head;
  int count = 0;

  while (current != NULL) {
    if (current->data == target) {
      count++;
    }

    current = current->next;
  }

  return count;
}

// Return the nth element's data.
int GetNth(struct node* head, int target) {
  struct node* current = head;
  int index = 0;
  int term;

  while (current != NULL) {
    assert(index <= target);

    if (index == target) {
      term = current->data;
      break;
    }

    index++;
    current = current->next;
  }

  return term;
}

// Free all nodes in the list and set head to NULL.
void DeleteList(struct node** headRef) {
  struct node* current = *headRef;

  while (current != NULL) {
    struct node** currentRef = &current;

    current = current->next;
    free(*currentRef);
  }

  *headRef = NULL;
}

// Return the data of the first element in the list and set head to the next.
int Pop(struct node** headRef) {
  struct node* head = *headRef;

  assert(head != NULL);

  int data = head->data;
  struct node* next = head->next;

  free(head);
  *headRef = next;

  return data;
}

struct node* InsertNthRecurInner(struct node** headRef, int target, int index, int data) {
  struct node* current = *headRef;

  if (target == index) {
    Push(&current, data);
  } else {
    struct node* next = current->next;

    next = InsertNthRecurInner(&next, target + 1, index, data);
    current->next = next;
  }

  return(current);
}

// Insert the data element at the nth position.
void InsertNthRecur(struct node** headRef, int index, int data) {
  *headRef = InsertNthRecurInner(headRef, 0, index, data);
}

// Insert the data element at the nth position.
void InsertNthIter(struct node** headRef, int index, int data) {
  struct node* current = *headRef;
  struct node* last = NULL;
  int currentIndex = 0;

  while (currentIndex < index) {
    last = current;
    current = current->next;
    currentIndex++;
  }

  Push(&current, data);

  if (last == NULL) {
    *headRef = current;
  } else {
    last->next = current;
  }
}

void InsertNth(struct node** headRef, int index, int data) {
  InsertNthIter(headRef, index, data);
}

// Adds newNode to the list in sorted order ascending.
void SortedInsert(struct node** headRef, struct node* newNode) {
  struct node* current = *headRef;
  struct node* last = NULL;
  int index = 0;

  // Iterate until current is > new
  while (current != NULL && current->data <= newNode->data) {
    index++;
    last = current;
    current = current->next;
  }

  newNode->next = current;

  if (last == NULL) {
    *headRef = newNode;
  } else {
    last->next = newNode;
  }
}

// Given a list, change it to be in sorted order (using SortedInsert()).
void InsertSort(struct node** headRef) {
  struct node* newHead = NULL;
  struct node* current = *headRef;
  struct node* next;

  while (current != NULL) {
    next = current->next;

    SortedInsert(&newHead, current);

    current = next;
  }

  *headRef = newHead;
}

/*
 Tests
*/

void PushTest() {
  Context("Push()");

  struct node* current = NULL;

  Push(&current, 1);
  It("should set the first node in an empty list.", current->data == 1);

  Push(&current, 2);
  It("should set the head node in a non-empty list.", current->data == 2);
}

void LengthTest() {
  Context("Length()");

  struct node* current = NULL;

  It("should return 0 when the list is empty.", Length(current) == 0);

  Push(&current, 1);
  It("should return 1 when the list has 1 element.", Length(current) == 1);
}

void BuildOneTwoThreeTest() {
  Context("BuildOneTwoThree()");

  struct node* list = BuildOneTwoThree();

  It("should return 1 for the data of the head element.", list->data == 1);
  It("should return 2 for the data of the second element.", list->next->data == 2);
  It("should return 3 for the data of the third element.", list->next->next->data == 3);
  It("should have the third element as the last element.", list->next->next->next == NULL);
}

void CountTest() {
  Context("Problem 1: Count()");

  struct node* list = BuildOneTwoThree();

  It("should return 1 as the number of twos in the list.", Count(list, 2) == 1);

  Push(&list, 2);
  It("should return 2 as the number of twos in the list.", Count(list, 2) == 2);
}

void GetNthTest() {
  Context("Problem 2: GetNth()");

  struct node* list = BuildOneTwoThree();

  It("should return 2 as the 1th data element.", GetNth(list, 1) == 2);
  It("should return 3 as the 2th data element.", GetNth(list, 2) == 3);
}

void DeleteListTest() {
  Context("Problem 3: DeleteList()");

  struct node* list = BuildOneTwoThree();

  DeleteList(&list);
  It("should set the head pointer to NULL.", list == NULL);
  // not sure how to test freed nodes
}

void PopTest() {
  Context("Problem 4: Pop()");

  struct node* list = BuildOneTwoThree();

  int a = Pop(&list);

  It("should return the data of the first element in the list.", a == 1);
  It("should set list to the next element in the list.", list->data == 2);
}

void InsertNthTest() {
  Context("Problem 5: InsertNth() - Recursive");

  struct node* listRecur = NULL;

  InsertNthRecur(&listRecur, 0, 13);
  InsertNthRecur(&listRecur, 1, 42);
  InsertNthRecur(&listRecur, 1, 5);

  It("should insert the data elements at the specified indices", (
    listRecur->data == 13 && listRecur->next->data == 5 && listRecur->next->next->data == 42
  ));

  Context("Problem 5: InsertNth() - Iterative");

  struct node* listIter = NULL;

  InsertNthIter(&listIter, 0, 13);
  InsertNthIter(&listIter, 1, 42);
  InsertNthIter(&listIter, 1, 5);

  It("should insert the data elements at the specified indices", (
    listIter->data == 13 && listIter->next->data == 5 && listIter->next->next->data == 42
  ));
}

void SortedInsertTest() {
  Context("Problem 6: SortedInsert()");

  struct node* list = NULL;
  struct node* newNode = malloc(sizeof(struct node));

  Push(&list, 10);
  Push(&list, 5);
  Push(&list, 2);

  newNode->data = 6;

  SortedInsert(&list, newNode);
  It("should insert newNode at the second indice", GetNth(list, 2) == 6);

  struct node* newNodeTwo = malloc(sizeof(struct node));
  newNodeTwo->data = 11;

  SortedInsert(&list, newNodeTwo);
  It("should insert newNode at the last indice", GetNth(list, 4) == 11);

  struct node* newNodeThree = malloc(sizeof(struct node));
  newNodeThree->data = 1;

  SortedInsert(&list, newNodeThree);
  It("should insert newNode at the first indice", GetNth(list, 0) == 1);
}

void InsertSortTest() {
  Context("Problem 7: InsertSort()");

  struct node* list = NULL;

  Push(&list, 8);
  Push(&list, 3);
  Push(&list, 6);

  InsertSort(&list);

  It("should sort the list in ascending order",
    GetNth(list, 0) == 3 &&
    GetNth(list, 1) == 6 &&
    GetNth(list, 2) == 8
  );
}

int main(void) {

  PushTest();
  LengthTest();
  BuildOneTwoThreeTest();
  CountTest();
  GetNthTest();
  DeleteListTest();
  PopTest();
  InsertNthTest();
  SortedInsertTest();
  InsertSortTest();

  return 0;
}
