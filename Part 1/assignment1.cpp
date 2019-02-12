

// include header filers here
#include"LinkedStack.h"
#include"Token.h"
#include<vector>



using namespace std;

int getPriority(Token token){
    if(token.isOperator()){
        Token::Operator tempoperator = token.getOperator();
        if(tempoperator == Token::OPEN_PARENTHESIS_OPERATOR)
            return 0;
        else if(tempoperator == Token::MULTIPLICATION_OPERATOR || tempoperator == Token::DIVISION_OPERATOR)
            return 2;
        else if(tempoperator == Token::ADDITION_OPERATOR || tempoperator == Token::SUBTRACTION_OPERATOR)
            return 3;
        else
            throw runtime_error("not correct operator");
    }
    else{
        throw runtime_error("in getpriority token is not operator");
    }
}


int getInStackPriority(Token token) {
    if(token.isOperator()){
        Token::Operator tempoperator = token.getOperator();
        if(tempoperator == Token::OPEN_PARENTHESIS_OPERATOR)
            return 8;
        else if(tempoperator == Token::MULTIPLICATION_OPERATOR || tempoperator == Token::DIVISION_OPERATOR)
            return 2;
        else if(tempoperator == Token::ADDITION_OPERATOR || tempoperator == Token::SUBTRACTION_OPERATOR)
            return 3;
        else
            throw runtime_error("not correct operator in getinstackpriority");
    }
    else{
        throw runtime_error("in getinstackpriority token is not operator");
    }
}


vector<Token> postfix(const vector<Token>& expression) {
    vector<Token> answer;
    LinkedStack<Token> tempstack;
    tempstack.push(Token("("));
    int len = expression.size();
    Token temptoken;
    int balance = 0;
    for(int i = 0; i < len; i++){
        temptoken = expression[i];
        if(temptoken.isConstant())
            answer.push_back(temptoken);
        else if(temptoken.getOperator() == Token::CLOSE_PARENTHESIS_OPERATOR){
            balance--;
            while(tempstack.top().getOperator() != Token::OPEN_PARENTHESIS_OPERATOR){
                answer.push_back(tempstack.top());
                tempstack.pop();
            }
            tempstack.pop();
        }
        else{
            if(temptoken.getOperator() == Token::OPEN_PARENTHESIS_OPERATOR)
                balance++;
            while(getInStackPriority(tempstack.top()) <= getPriority(temptoken)){
                answer.push_back(tempstack.top());
                tempstack.pop();
            }
            tempstack.push(temptoken);
        }
        if(balance < 0)
            throw runtime_error("bad placement of parenthesises");
    }
    if(balance != 0)
        throw runtime_error("not enought close parenthesises");
    while(tempstack.size() > 1){
        answer.push_back(tempstack.top());
        tempstack.pop();
    }
    return answer;
}


int eval(const vector<Token>& expression) {
    LinkedStack<Token> tempstack;
    Token temptoken;
    int len = expression.size();
    for(int i = 0; i < len; i++){
        temptoken = expression[i];
        if(temptoken.isConstant())
            tempstack.push(temptoken);
        else{
            if(tempstack.empty())
                throw runtime_error("not enough operands");
            int operand2 = tempstack.top().getConstant();
            tempstack.pop();
            if(tempstack.empty())
                throw runtime_error("not enough operands");
            int operand1 = tempstack.top().getConstant();
            tempstack.pop();
            int ans;
            if(temptoken.getOperator() == Token::ADDITION_OPERATOR)
                ans = operand1 + operand2;
            else if(temptoken.getOperator() == Token::SUBTRACTION_OPERATOR)
                ans = operand1 - operand2;
            else if(temptoken.getOperator() == Token::MULTIPLICATION_OPERATOR)
                ans = operand1 * operand2;
            else if(temptoken.getOperator() == Token::DIVISION_OPERATOR){
                if(operand2 == 0)
                    throw runtime_error("can not divide by 0");
                ans = operand1 / operand2;
            }
            tempstack.push(Token(ans));
        }
    }
    if(tempstack.size() != 1)
        throw runtime_error("not correct postfix expression");
    return tempstack.top().getConstant();
}
