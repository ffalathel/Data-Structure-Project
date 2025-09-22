#include "linked_calc.hpp"
#include <sstream>
#include <stdexcept>
#include <vector>

// Default constructor definition
template <typename T>
LinkedCalc<T>::LinkedCalc() : head(nullptr) {}

// Destructor to deallocate memory
template <typename T>
LinkedCalc<T>::~LinkedCalc() {
    Node<T>* current = head;
    while (current != nullptr) {
        Node<T>* temp = current;
        current = current->next;
        delete temp;
    }
}

// Function to insert a new node at the end of the linked list
template <typename T>
void LinkedCalc<T>::insert(const T& value) {
    Node<T>* newNode = new Node<T>(value);
    if (head == nullptr) {
        head = newNode;
    } else {
        Node<T>* current = head;
        while (current->next != nullptr) {
            current = current->next;
        }
        current->next = newNode;
    }
}

// Helper function to determine if a character is a digit
template <typename T>
bool LinkedCalc<T>::isDigit(const T& c) {
    return (c >= '0' && c <= '9');
}

template <typename T>
bool LinkedCalc<T>::validateExpression() {
    if (head == nullptr)
        return false;  // Expression is empty

    bool lastWasDigit = false;
    bool digitFound = false;  // indicates if current number contains at least one digit
    bool decimalUsed = false; // track if the current number already contains a decimal

    Node<T>* current = head;

 
    if (!isDigit(current->data) && current->data != '.')
        return false;

    while (current != nullptr) {
        T c = current->data;
        if (isDigit(c)) {
            digitFound = true;
            lastWasDigit = true;
        }
        else if (c == '.') {
            if (decimalUsed)
                return false;  // two decimals in one number not allowed
            decimalUsed = true;
            lastWasDigit = false; // a decimal doesn't complete a number
        }
        else if (c == '+' || c == '-' || c == '*' || c == '/') {
            // Operator must follow a complete number.
            if (!digitFound)
                return false;
            // Reset flags for next number.
            lastWasDigit = false;
            digitFound = false;
            decimalUsed = false;
        }
        else {
            return false; // invalid character
        }
        current = current->next;
    }
    // Expression must end with a digit.
    return lastWasDigit;
}

template <typename T>
float LinkedCalc<T>::evaluateExpression() {
    std::vector<float> numbers;
    std::vector<char> operators;
    Node<T>* current = head;

    // Extract the first number inline:
    {
        std::string numStr;
        // Collect digits and decimal points.
        while (current != nullptr && (isDigit(current->data) || current->data == '.')) {
            numStr.push_back(current->data);
            current = current->next;
        }
        std::istringstream iss(numStr);
        float number = 0.0f;
        iss >> number;
        numbers.push_back(number);
    }
    
    // Process the rest of the list.
    while (current != nullptr) {

        if (current != nullptr &&
            (current->data == '+' || current->data == '-' ||
             current->data == '*' || current->data == '/')) {
            char op = current->data;
            operators.push_back(op);
            current = current->next;
            
            // Extract the next number inline:
            std::string numStr;
            while (current != nullptr && (isDigit(current->data) || current->data == '.')) {
                numStr.push_back(current->data);
                current = current->next;
            }
            std::istringstream iss(numStr);
            float number = 0.0f;
            iss >> number;
            numbers.push_back(number);
        }
    }
    
    // First pass: evaluate '*' and '/' (left-to-right).
    for (size_t i = 0; i < operators.size(); ) {
        if (operators[i] == '*' || operators[i] == '/') {
            float left = numbers[i];
            float right = numbers[i + 1];
            float result = 0.0f;
            if (operators[i] == '*') {
                result = left * right;
            } else { // division
                if (right == 0.0f)
                    throw std::runtime_error("Division by zero encountered in expression.");
                result = left / right;
            }
            numbers[i] = result;
            numbers.erase(numbers.begin() + i + 1);
            operators.erase(operators.begin() + i);
        } else {
            ++i;
        }
    }
    
    // Second pass: evaluate '+' and '-' (left-to-right).
    float result = numbers[0];
    for (size_t i = 0; i < operators.size(); i++) {
        char op = operators[i];
        float num = numbers[i + 1];
        if (op == '+')
            result += num;
        else if (op == '-')
            result -= num;
    }
    
    return result;
}