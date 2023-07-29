/*
 * main.cpp
 *
 *  Created on: 19 Jul 2023
 *      Author: rw123
 */

#include<stdio.h>
#include<stdint.h>
#include<list>
#include <unordered_map>
using namespace std;

int hashFunc(uint64_t key){
  return key % 0xff; //256;
}

void test1();
void test2();
void test3();

enum dataTypes{
    CHAR = 0,
    INT
};

class Node
{
public:
  void* pData;
  Node* previous;
  Node* next;
  Node(void* pData): pData(pData){
    this->previous = nullptr;
    this->next = nullptr;
  }
  void debugPrint(dataTypes type){
    if(type == CHAR){
        uint64_t n = (uint64_t) this->pData;
        char c = (char) n;
        printf("data: %c    this: %p    previous: %p    next: %p\n", c, this, this->previous, this->next);
        //sprintf(str, "data: %c\n", c);
    }
  }
  uint64_t getAddress(){
    return (uint64_t) this;
  }
};

class LinkedList
{
public:
  Node* current;
  Node* head;
  Node* end;
  //-------------------------------------------
  LinkedList(){
    this->head = nullptr;
    this->current = nullptr;
    this->end = nullptr;
  }
  //-------------------------------------------
  void insert(Node* node)
  {
    if(this->head == nullptr){
        // list is empty
        printf("add head\n");
        this->head = node;
        this->end = node;
        this->current = node;
        this->current->previous = nullptr;
        this->current->next = nullptr;
    }
    else if(this->current->next == nullptr){
        // Adding at the end
        printf("add end\n");
        Node* above = this->current;

        this->current->next = node;


        this->current = node; // go to new node
        this->current->previous = above;
        this->current->next = nullptr;

        this->end = this->current;
    }
    else{
        // Adding in the middle
        printf("add middle\n");

        Node* above = this->current;
        Node* below = this->current->next;
        this->current->next = node;

        this->current = node; // go to new node
        this->current->previous = above;
        this->current->next = below;

        below->previous = this->current;
    }
  }
  //-------------------------------------------
  void goToHead(){this->current = this->head;}
  //-------------------------------------------
  Node* getCurrentNode(){
    return this->current;
  }
  //-------------------------------------------
  bool goDown(){
    if(this->current->next == nullptr){
      return true;
    }
    else{
      this->current = this->current->next;
      return false;
    }
  }
  //-------------------------------------------
  bool goUp(){
    if(this->current->previous == nullptr){
        return true;
    }
    else{
      this->current = this->current->previous;
      return false;
    }
  }
  //-------------------------------------------
  void makeCircular(){
    this->head->previous = this->end;
    this->end->next = this->head;
  }
  //-------------------------------------------
  // Makes a loop from the end to the current
  void makeLoop(){
    this->end->next = this->current;
    this->current->previous = this->end;
  }
};

int main()
{
  printf("Main Start 3\n");

  //test1();
  test2();
  //test3();

  printf("Main End\n");
}
//-------------------------------------------------------------------
void test1()
{
  LinkedList list;
  Node* node;
  char s[256];

  node = new Node((void*) 'A');
  list.insert(node);
  node = new Node((void*) 'B');
  list.insert(node);
  node = new Node((void*) 'C');
  list.insert(node);
  list.goUp();
  node = new Node((void*) 'D');
  list.insert(node);
  node = new Node((void*) 'E');
  list.insert(node);

  list.goToHead();
//  node = list.getCurrentNode();
//  node->debugPrint(CHAR, s);

  node = nullptr;
  do{
    node = list.getCurrentNode();
    node->debugPrint(CHAR);
  }
  while(!list.goDown());
}
//-------------------------------------------------------------------
void test2()
{
  LinkedList list;
  Node* node;
  char s[256];

  node = new Node((void*) 'A');
  list.insert(node);
  node = new Node((void*) 'B');
  list.insert(node);
  node = new Node((void*) 'C');
  list.insert(node);
  //list.goUp();
  node = new Node((void*) 'D');
  list.insert(node);
  node = new Node((void*) 'E');
  list.insert(node);

  list.makeCircular();
  list.goToHead();

  for(int i=0; i<12; i++)
  {
    node = list.getCurrentNode();
    node->debugPrint(CHAR);
    list.goDown();
  }
}
//-------------------------------------------------------------------
void test3()
{
  LinkedList list;
  Node* node;
  unordered_map<int, int> hashMap;

  node = new Node((void*) 'A');
  list.insert(node);
  //hashMap[node->getAddress(), 0];
  node = new Node((void*) 'B');
  list.insert(node);
  //hashMap[node->getAddress(), 0];
  node = new Node((void*) 'C');
  list.insert(node);
  uint64_t addrC = node->getAddress();
  //hashMap[node->getAddress(), 0];
  node = new Node((void*) 'D');
  list.insert(node);
  //hashMap[node->getAddress(), 0];
  node = new Node((void*) 'E');
  list.insert(node);
  //hashMap[node->getAddress(), 0];

  list.goToHead();
  list.goDown();
  list.makeLoop();

  list.goToHead();

  for(int i=0; i<12; i++)
  {
    node = list.getCurrentNode();
    node->debugPrint(CHAR);
    //int n = node->getAddress() % 256; printf("n: %d\n", n);
    if(list.goDown()){
      // end of list found - no loop
      printf("No loop found\n");
      break;
    }
    else{
      uint64_t addr = node->getAddress();
      int hashValue = hashFunc(addr);

      if(hashMap.find(hashValue) == hashMap.end())
      {
        hashMap.insert({hashValue, 1});
      }
      else
      {
        int count = hashMap.at(hashValue);
        hashMap[hashValue] = ++count;
      }

      //hashMap.insert({addr, 1});
      if(hashMap.at(hashValue) > 1){
        printf("Loop found\n");
        break;
      }
    }
  }
}
//-------------------------------------------------------------------
