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

// Append 'b' onto the end of 'a', and then set 'b' to NULL.
void Append(struct node** aRef, struct node** bRef) {
  if (*aRef == NULL) {
    *aRef = *bRef;
  } else {
    struct node* a = *aRef;
    struct node* b = *bRef;

    while (a->next != NULL) {
      a = a->next;
    }

    while (b != NULL) {
      Push(&(a->next), b->data);
      a = a->next;
      b = b->next;
    }
  }

  *bRef = NULL;
}

/*
 Split the nodes of the given list into front and back halves,
 and return the two lists using the reference parameters.
 If the length is odd, the extra node should go in the front list.
*/
void FrontBackSplit(struct node* source, struct node** frontRef, struct node** backRef) {
  int length = Length(source);
  struct node* current = source;

  if (length < 2) {
    *frontRef = source;
    *backRef = NULL;
  } else {
    int split = (length - 1) / 2;

    for (int i = 0; i < split; i++) {
      current = current->next;
    }

    *frontRef = source;
    *backRef = current->next;
    current->next = NULL;
  }
}

// Remove duplicates from a sorted list.
void RemoveDuplicates(struct node* head) {
  struct node* current = head;

  while (current != NULL) {
    struct node* next = current->next;

    while (next != NULL && next->data == current->data) {
      struct node* nextNext = next->next;
      free(next);
      next = nextNext;
    }

    current->next = next;
    current = current->next;
  }
}

/*
 Take the node from the front of the source, and move it to
 the front of the dest.
 It is an error to call this with the source list empty.
*/
void MoveNode(struct node** destRef, struct node** sourceRef) {
  struct node* head = *destRef;
  struct node* target = *sourceRef;

  *sourceRef = target->next;
  target->next = head;
  *destRef = target;
}

/*
 Given the source list, split its nodes into two shorter lists.
 If we number the elements 0, 1, 2, ... then all the even elements
 should go in the first list, and all the odd elements in the second.
 The elements in the new lists may be in any order.
*/
void AlternatingSplit(struct node* source, struct node** aRef, struct node** bRef) {
  struct node* current = source;
  int index = 0;

  while (current != NULL) {
    struct node* next = current->next;

    if (index % 2 == 0) {
      MoveNode(aRef, &current);
    } else {
      MoveNode(bRef, &current);
    }

    index++;
    current = next;
  }
}

/*
 Merge the nodes of the two lists into a single list taking a node
 alternately from each list, and return the new list.
*/
struct node* ShuffleMerge(struct node* a, struct node* b) {
  struct node* merged = NULL;
  struct node* headA = a;
  struct node* headB = b;
  struct node* last = merged;

  while (headA != NULL && headB != NULL) {
    if (merged == NULL) {
      merged = headA;
    } else {
      last->next = headA;
    }

    struct node* nextA = headA->next;
    struct node* nextB = headB->next;

    headA->next = headB;
    headB->next = NULL;
    last = headB;

    if (nextA == NULL) {
      last->next = nextB;
      nextB = NULL;
    } else if (nextB == NULL) {
      last->next = nextA;
      nextA = NULL;
    }

    headA = nextA;
    headB = nextB;
  }

  return merged;
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

void AppendTest() {
  Context("Problem 8: Append()");

  struct node* listOne = NULL;
  struct node* listTwo = BuildOneTwoThree();

  Append(&listOne, &listTwo);
  It("should set the first list to the second if the first list is empty",
    Length(listOne) == 3 &&
    GetNth(listOne, 1) == 2
  );

  struct node* listThree = BuildOneTwoThree();
  struct node* listFour = BuildOneTwoThree();

  Append(&listThree, &listFour);
  It("should append the second list to the first",
    Length(listThree) == 6 &&
    GetNth(listThree, 4) == 2
  );
  It("should set the second list to null", listFour == NULL);
}

void FrontBackSplitTest() {
  Context("Problem 9: FrontBackSplit()");

  struct node* emptyList = NULL;
  struct node* front = NULL;
  struct node* back = NULL;

  FrontBackSplit(emptyList, &front, &back);
  It("should keep all lists as NULL", emptyList == NULL && front == NULL && back == NULL);

  struct node* listTwo = NULL;
  struct node* frontTwo = NULL;
  struct node* backTwo = NULL;

  Push(&listTwo, 4);
  Push(&listTwo, 3);

  FrontBackSplit(listTwo, &frontTwo, &backTwo);
  It("should split a length two list into two equal lists",
    Length(frontTwo) == 1 && GetNth(frontTwo, 0) == 3 &&
    Length(backTwo) == 1 && GetNth(backTwo, 0) == 4
  );

  struct node* listThree = NULL;
  struct node* frontThree = NULL;
  struct node* backThree = NULL;

  Push(&listThree, 4);
  Push(&listThree, 3);
  Push(&listThree, 2);

  FrontBackSplit(listThree, &frontThree, &backThree);
  It("should split a length three list into the first two elements and the last",
    Length(frontThree) == 2 && GetNth(frontThree, 0) == 2 && GetNth(frontThree, 1) == 3 &&
    Length(backThree) == 1 && GetNth(backThree, 0) == 4
  );


  struct node* listFour = NULL;
  struct node* frontFour = NULL;
  struct node* backFour = NULL;

  Push(&listFour, 4);
  Push(&listFour, 3);
  Push(&listFour, 2);
  Push(&listFour, 1);

  FrontBackSplit(listFour, &frontFour, &backFour);
  It("should split a length four list into the first two elements and the last two elements",
    Length(frontFour) == 2 && GetNth(frontFour, 0) == 1 && GetNth(frontFour, 1) == 2 &&
    Length(backFour) == 2 && GetNth(backFour, 0) == 3 && GetNth(backFour, 1) == 4
  );
}

void RemoveDuplicatesTest() {
  Context("Problem 10: RemoveDuplicates()");

  struct node* list = BuildOneTwoThree();
  Push(&list, 1);

  RemoveDuplicates(list);
  It("should remove duplicates from the beginning of the list",
    Length(list) == 3 && GetNth(list, 0) == 1 && GetNth(list, 1) == 2
  );

  InsertNth(&list, 1, 2);
  RemoveDuplicates(list);
  It("should remove duplicates from the middle of the list",
    Length(list) == 3 && GetNth(list, 1) == 2 && GetNth(list, 2) == 3
  );

  InsertNth(&list, 2, 3);
  RemoveDuplicates(list);
  It("should remove duplicates from the end of the list",
    Length(list) == 3 && GetNth(list, 1) == 2 && GetNth(list, 2) == 3
  );
}

void MoveNodeTest() {
  Context("Problem 11: MoveNode()");

  struct node* a = BuildOneTwoThree(); // the list {1, 2, 3}
  struct node* b = BuildOneTwoThree();

  MoveNode(&a, &b);
  It("should move the head node of the second list to the front of the first",
    Length(a) == 4 &&
    Length(b) == 2 &&
    GetNth(a, 0) == 1 &&
    GetNth(a, 1) == 1 &&
    GetNth(b, 0) == 2
  );
}

void AlternatingSplitTest() {
  Context("Problem 12: AlternatingSplit()");

  struct node* list = BuildOneTwoThree();
  struct node* a = NULL;
  struct node* b = NULL;

  AlternatingSplit(list, &a, &b);
  It("should split the list in two",
    Length(a) == 2 && GetNth(a, 0) == 3 && GetNth(a, 1) == 1 &&
    Length(b) == 1 && GetNth(b, 0) == 2
  );
}

void ShuffleMergeTest() {
  Context("Problem 13: ShuffleMerge()");

  struct node* a = BuildOneTwoThree();
  struct node* b = BuildOneTwoThree();

  struct node* merged = ShuffleMerge(a, b);
  It("should alternately merge the nodes",
    Length(merged) == 6 &&
    GetNth(merged, 0) == 1 &&
    GetNth(merged, 1) == 1 &&
    GetNth(merged, 2) == 2 &&
    GetNth(merged, 3) == 2
  );

  struct node* c = BuildOneTwoThree();
  struct node* d = NULL;

  Push(&d, 1);

  struct node* merged2 = ShuffleMerge(c, d);
  It("should alternately merge the nodes",
    Length(merged2) == 4 &&
    GetNth(merged2, 0) == 1 &&
    GetNth(merged2, 1) == 1 &&
    GetNth(merged2, 2) == 2 &&
    GetNth(merged2, 3) == 3
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
  AppendTest();
  FrontBackSplitTest();
  RemoveDuplicatesTest();
  MoveNodeTest();
  AlternatingSplitTest();
  ShuffleMergeTest();

  return 0;
}
