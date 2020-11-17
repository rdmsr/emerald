#include <iostream>
#include <cmath>


int main()
{
    int num1, num2;
    char op;


    std::cout << "Enter first Number: ";
    std::cin >> num1;
    std::cout << "Enter the operator: ";
    std::cin >> op;
    std::cout << "Enter the second Number: ";
    std::cin >> num2;
    int result;
    if(op == '+'){

        result = num1 + num2;
        }
    else if(op == '-'){
    result = num1 - num2;
    }

    else if(op == '/'){
    result = num1 / num2;
    }

    else if(op == '*'){
    result = num1 * num2;
    } else {
     std::cout << "Invalid Operator" << std::endl;


    }


    std::cout << result << std::endl;

    std::cout << "HERE YA GO" << std::endl;
  
    

    return 0;



}
