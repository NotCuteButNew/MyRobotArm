#ifndef QUEUE_H_
#define QUEUE_H_
#include "WString.h"
#include "logger.h"

template <typename Element> class Queue {
public:
  Queue(int alen);
  ~Queue();
  bool push(Element elem);
  Element pop();
  bool isFull() const;
  bool isEmpty() const;
  int getFreeSpace() const;
  int getMaxLength() const;
  inline int getUsedSpace() const;
  inline int getRealCount() const;

private:
  Queue(Queue<Element> &q); // copy const.
  Element *data;
  int len;
  int start;
  int count;
  int realcount;
};

template <typename Element> Queue<Element>::Queue(int alen) {
  data = new Element[alen];
  len = alen;
  start = 0;
  count = 0;
  realcount = 0;
}

template <typename Element> Queue<Element>::~Queue() { delete data; }

template <typename Element> bool Queue<Element>::push(Element elem) {
  data[(start + count++) % len] = elem;
  realcount++;
}

template <typename Element> Element Queue<Element>::pop() {
  count--;
  int s = start;
  start = (start + 1) % len;
  return data[(s) % len];
}

template <typename Element> bool Queue<Element>::isFull() const {
  return count >= len;
}

template <typename Element> bool Queue<Element>::isEmpty() const {
  return count <= 0;
}

template <typename Element> int Queue<Element>::getFreeSpace() const {
  return len - count;
}

template <typename Element> int Queue<Element>::getMaxLength() const {
  return len;
}

template <typename Element> int Queue<Element>::getUsedSpace() const {
  return count;
}

template <typename Element> int Queue<Element>::getRealCount() const {
  return realcount;
}

#endif
