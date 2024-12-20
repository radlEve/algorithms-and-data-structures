#include <iostream>
#include <stdexcept>
#include <random>
#include <chrono>

template <typename T>
class LinkedList {
private:
    struct Node {
        T data;
        Node* next;

        Node(const T& data) : data(data), next(nullptr) {}
    };

    Node* head;
    size_t size;

public:
    LinkedList() : head(nullptr), size(0) {}

    LinkedList(const LinkedList& other) : head(nullptr), size(0) {
        Node* currentOther = other.head;
        if (currentOther) {
            do {
                push_tail(currentOther->data);
                currentOther = currentOther->next;
            } while (currentOther != other.head);
        }
    }

    LinkedList(size_t count, T min_val, T max_val, unsigned int seed = std::random_device{}()) : head(nullptr), size(0) {
        std::default_random_engine gen(seed);

        if (std::is_integral_v<T>) {
            std::uniform_int_distribution<T> dis(min_val, max_val);
            while (size < count) {
                T value = dis(gen);
                push_tail(value);
            }
        }
        else {
            std::uniform_real_distribution<double> dis(static_cast<double>(min_val), static_cast<double>(max_val));
            while (size < count) {
                T value = static_cast<T>(dis(gen));
                push_tail(value);
            }
        }
    }

    ~LinkedList() {
        clear();
    }

    LinkedList& operator=(const LinkedList& other) {
        if (this != &other) {
            clear();
            Node* currentOther = other.head;
            if (currentOther) {
                do {
                    push_tail(currentOther->data);
                    currentOther = currentOther->next;
                } while (currentOther != other.head);
            }
        }
        return *this;
    }

    void push_tail(const T& data) {
        Node* newNode = new Node(data);
        if (!head) {
            head = newNode;
            head->next = head;
        }
        else {
            Node* tail = head;
            while (tail->next != head) {
                tail = tail->next;
            }
            tail->next = newNode;
            newNode->next = head;
        }
        size++;
    }

    void push_tail(const LinkedList& other) {
        Node* currentOther = other.head;
        if (currentOther) {
            do {
                push_tail(currentOther->data);
                currentOther = currentOther->next;
            } while (currentOther != other.head);
        }
    }

    void push_head(const T& data) {
        Node* newNode = new Node(data);
        if (!head) {
            head = newNode;
            head->next = head;
        }
        else {
            Node* tail = head;
            while (tail->next != head) {
                tail = tail->next;
            }
            newNode->next = head;
            head = newNode;
            tail->next = head;
        }
        size++;
    }

    void push_head(const LinkedList& other) {
        LinkedList temp = other;
        temp.reverse();
        Node* currentOther = temp.head;
        if (currentOther) {
            do {
                push_head(currentOther->data);
                currentOther = currentOther->next;
            } while (currentOther != temp.head);
        }
    }

    void pop_head() {
        if (!head) {
            throw std::runtime_error("List is empty");
        }
        if (head->next == head) {
            delete head;
            head = nullptr;
        }
        else {
            Node* tail = head;
            while (tail->next != head) {
                tail = tail->next;
            }
            Node* temp = head;
            head = head->next;
            tail->next = head;
            delete temp;
        }
        size--;
    }

    void pop_tail() {
        if (!head) {
            throw std::runtime_error("List is empty");
        }
        if (head->next == head) {
            delete head;
            head = nullptr;
        }
        else {
            Node* tail = head;
            Node* prev_tail = nullptr;
            while (tail->next != head) {
                prev_tail = tail;
                tail = tail->next;
            }
            prev_tail->next = head;
            delete tail;
        }
        size--;
    }

    void delete_node(const T& data) {
        if (!head) return;

        Node* current = head;
        Node* prev = nullptr;

        do {
            if (current->data == data) {
                if (current == head) {
                    if (head->next == head) {
                        delete head;
                        head = nullptr;
                        current = nullptr;
                        size--;
                        break;
                    }
                    else {
                        Node* tail = head;
                        while (tail->next != head) {
                            tail = tail->next;
                        }
                        head = head->next;
                        tail->next = head;
                        delete current;
                        current = head;
                        size--;
                    }
                }
                else {
                    prev->next = current->next;
                    delete current;
                    current = prev->next;
                    size--;
                }
            }
            else {
                prev = current;
                current = current->next;
            }

        } while (current != head);

    }


    const T& operator[](size_t index) const {
        if (index >= size) {
            throw std::out_of_range("Index out of range");
        }
        Node* current = head;
        for (size_t i = 0; i < index; ++i) {
            current = current->next;
        }
        return current->data;
    }
    T& operator[](size_t index) {
        if (index >= size) {
            throw std::out_of_range("Index out of range");
        }
        Node* current = head;
        for (size_t i = 0; i < index; ++i) {
            current = current->next;
        }
        return current->data;
    }


    void reverse() {
        if (size <= 1) return;

        Node* prev = nullptr;
        Node* current = head;
        Node* next = nullptr;
        do {
            next = current->next;
            current->next = prev == nullptr ? head : prev;
            prev = current;
            current = next;
        } while (current != head);

        head->next = prev;
        head = prev;

    }

    void print() const {
        if (!head) {
            std::cout << "List is empty." << std::endl;
            return;
        }
        Node* current = head;
        do {
            std::cout << current->data << " ";
            current = current->next;
        } while (current != head);
        std::cout << std::endl;
    }

private:
    void clear() {
        while (head)
        {
            pop_head();
        }
        size = 0;
    }
};



int main() {
    try {
        LinkedList<int> list;
        std::cout << "Initial list: " << std::endl;
        list.print(); // Empty list

        list.push_tail(1);
        list.push_tail(2);
        list.push_tail(3);
        list.push_tail(4);
        std::cout << "List after push_tail: " << std::endl;
        list.print(); // 1 2 3 4

        list.push_head(0);
        std::cout << "List after push_head: " << std::endl;
        list.print(); // 0 1 2 3 4


        LinkedList<int> list2;
        list2.push_tail(5);
        list2.push_tail(6);

        list.push_tail(list2);
        std::cout << "List after push_tail another list: " << std::endl;
        list.print(); // 0 1 2 3 4 5 6

        LinkedList<int> list3;
        list3.push_tail(7);
        list3.push_tail(8);

        list.push_head(list3);
        std::cout << "List after push_head another list: " << std::endl;
        list.print(); // 7 8 0 1 2 3 4 5 6

        list.reverse();
        std::cout << "Reversed list: " << std::endl;
        list.print(); // 6 5 4 3 2 1 0 8 7

        list.pop_head();
        std::cout << "List after pop_head: " << std::endl;
        list.print(); // 5 4 3 2 1 0 8 7

        list.pop_tail();
        std::cout << "List after pop_tail: " << std::endl;
        list.print(); // 5 4 3 2 1 0 8

        list.delete_node(3);
        std::cout << "List after delete_node(3): " << std::endl;
        list.print(); // 5 4 2 1 0 7

        LinkedList<int> list4(5, 10, 120);
        std::cout << "List generated by random values: " << std::endl;
        list4.print();

        LinkedList<int> list5 = list4;
        std::cout << "Copied List from random value: " << std::endl;
        list5.print();

        std::cout << "Access by index : " << list5[2] << std::endl;
        list5[2] = 100;
        std::cout << "Access by index and set new value: " << std::endl;
        list5.print();


    }
    catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}