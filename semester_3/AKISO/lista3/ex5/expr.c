#include "types.h"
#include "stat.h"
#include "user.h"

int execute_operation(int firstOperand, char operator, int secondOperand) {
  switch (operator) {
    case '+':
      return firstOperand + secondOperand;
    case '-':
      return firstOperand - secondOperand;
    case '*':
      return firstOperand * secondOperand;
    case '/':
      if (secondOperand == 0) {
        printf(2, "Error: divide by zero\n");
        exit();
      }
      return firstOperand / secondOperand;
    default:
      printf(2, "Error: unknown operator '%c'\n", operator);
      exit();
  }
}

int main(int argc, char *argv[]) {
  if (argc != 4) {
    printf(2, "Usage: expr value1 operator value2\n");
    exit();
  }

  int num1 = atoi(argv[1]);
  char operator = argv[2][0];
  int num2 = atoi(argv[3]);

  int result = execute_operation(num1, operator, num2);
  printf(1, "%d\n", result);

  exit();
}
