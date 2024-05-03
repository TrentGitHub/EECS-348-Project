#include <iostream>
#include <string>
#include <vector>
using namespace std;

class Operators {
public:
    static char NOT(char a) { return (a == 'T') ? 'F' : 'T'; } // symbol: !

    static char AND(char a, char b) {
        return ((a == 'T') && (b == 'T')) ? 'T' : 'F';
    } // symbol: &

    static char NAND(char a, char b) { return NOT(AND(a, b)); } // symbol: @

    static char XOR(char a, char b) { return (a != b) ? 'T' : 'F'; } // symbol: $

    static char OR(char a, char b) {
        return ((a == 'T') || (b == 'T')) ? 'T' : 'F';
    } // symbol: |
};

//------------------------------------------------------------------------------------------------

class Evaluator{
    public:
    vector<char> parse(vector<char> v) { // parse vector and outputs T or F for logic gates
  for (int i = 0; i < v.size(); i++) { // NOT gate
    if (v[i] == '!') {
      v[i + 1] = Operators::NOT(v[i + 1]);
      v.erase(v.begin() + i);
    }
  }

  for (int i = 0; i < v.size(); i++) { // NAND gate
    if (v[i] == '@') {
      v[i] = Operators::NAND(v[i - 1], v[i + 1]); // grabs chars around gate symbol '@'
      v.erase(v.begin() + i -
              1); // make sure to erase correct spots, first erase the character
                  // behind v[i] that we have changed
      v.erase(v.begin() + i); // next erase i since we want to erase the
                              // character after v[i] which is now v[i-1]
    }
  }

  for (int i = 0; i < v.size(); i++) { // AND gate
    if (v[i] == '&') {
      v[i] = Operators::AND(v[i - 1], v[i + 1]);
      v.erase(v.begin() + i - 1);
      v.erase(v.begin() + i);
    }
  }

  for (int i = 0; i < v.size(); i++) { // XOR gate
    if (v[i] == '$') {
      v[i] = Operators::XOR(v[i - 1], v[i + 1]);
      v.erase(v.begin() + i - 1);
      v.erase(v.begin() + i);
    }
  }

  for (int i = 0; i < v.size(); i++) { // OR gate
    if (v[i] == '|') {
      v[i] = Operators::OR(v[i - 1], v[i + 1]);
      v.erase(v.begin() + i - 1);
      v.erase(v.begin() + i);
    }
  }

  return v;
}

vector<char> grouping(
    vector<char> v) { // grouping parentheses will send a temp vector to parse
                      // function and output T or F back into the original
                      // vector, erasing all values besides the new T or F, if
                      // no grouping parentheses exists, the whole vector will
                      // be send to parse function and returned as T or F
  int idx_closing = v.size() - 1; // track right hand parenthesis
  int idx_opening = 0;            // track left hand parenthesis
  int contains_closing = 0;       // track if there is a closing parenthesis
  int contains_opening = 0;       // track if there is an opening parenthesis

  for (int i = 0; i < v.size(); i++) { // grab index of first closing
                                       // parenthesis
    if (v[i] == ')') {
      idx_closing = i;
      contains_closing = 1;
      break;
    }
  }

  for (int i = idx_closing; i >= 0;
       i--) { // grab index of opening parenthesis tied to the above closing
              // parenthesis
    if (v[i] == '(') {
      idx_opening = i;
      contains_opening = 1;
      break;
    }
  }

  vector<char> a; // temp vector for grouping parentheses
  if (contains_opening &&
      contains_closing) { // check to make sure a valid group exists
    for (int i = idx_opening + 1; i < idx_closing;
         i++) { // push back all values between the opening and closing
                // parenthesis
      a.push_back(v[i]);
    }
    for (int i = idx_opening; i <= idx_closing;
         i++) { // erase all values between the opening and closing parenthesis
                // including the parentheses themselves
      v.erase(v.begin() + idx_opening);
    }
    a = parse(a); // calculate T or F for temp vector
    v.insert(v.begin() + idx_opening,
             a[0]); // insert the value of T or F into the original vector
  } else { // if there are no valid parentheses, then we calculate the value of
           // the entire vector
    v = parse(v);
  }

  return v;
}
};

//------------------------------------------------------------------------------------------------
class Handler {
public:
    bool isValidOperand(char a) { return (a == 'T' || a == 'F'); }

    bool isValidOperator(char a) {
        return (a == '!' || a == '&' || a == '@' || a == '$' || a == '|');
    }

    bool isValidParenthesis(char a) { return (a == '(' || a == ')'); }

    int parenthesis_match(vector<char>& v) {
        int x = 0;
        for (char a : v) {
            if (a == '(') {
                x++;
            } else if (a == ')') {
                x--;
            }
        }
        return x;
    }

    bool other_empty(vector<char>& v) {
        int x = 0;
        for (int i = 0; i < v.size(); i++) {
            if (!isValidOperator(v[i]) && !isValidOperand(v[i]) && isValidParenthesis(v[i])) {
                x++;
            }
        }
        return (x == v.size());
    }

    void errorHandling(vector<char>& v) {
    int parentheses_count = 0;
    if (v.empty()) {
        throw runtime_error("Invalid Expression: Empty Expression");
    }
    if (other_empty(v)) {
        throw runtime_error("Invalid Expression: Empty Expression, No Operators or Operands found");
    }
    if (parenthesis_match(v)) {
        parentheses_count = parenthesis_match(v);
        if (parentheses_count > 0) {
            throw runtime_error("Invalid Expression: Missing closing parenthesis");
        } else if (parentheses_count < 0) {
            throw runtime_error("Invalid Expression: Missing opening parenthesis");
        }
    }

    for (int i = 0; i < v.size(); i++) {
        if (!isValidOperand(v[i]) && !isValidOperator(v[i]) && !isValidParenthesis(v[i])) {
            if (v[i] == 't' || v[i] == 'f') {
                throw runtime_error("Invalid Expression: Using Lowercase Letters instead of T and F");
            } else {
                throw runtime_error("Invalid Expression: Unknown Character: " + string(1, v[i]));
            }
        }

        if (i > 0 && v.size() > 1 && (isValidOperand(v[i - 1])) &&
            (!isValidOperator(v[i]) && v[i] != ')' && v[i] != '(')) {
            throw runtime_error("Invalid Expression: Missing Operand or Parenthesis after: '" + string(1, v[i - 1]) + "'");
        }
        if (i < v.size() - 1 &&
            (isValidOperator(v[i]) && isValidOperator(v[i + 1])) &&
            (v[i + 1] != '!')) {
            throw runtime_error("Invalid Expression: Two consecutive Operators");
        }

        if (isValidOperator(v[0])) {
            if (v[0] != '!') {
                throw runtime_error("Invalid Expression: Missing Operand Before " + string(1, v[0]));
            }
        }

        if ((i < v.size() - 1) && (isValidOperator(v[i])) && (v[i] != '!') &&
            (!isValidOperand(v[i + 1]) && v[i + 1] != '!')) {
            if (i == 0) {
                throw runtime_error("Invalid Expression: Cannot begin expression with Operator besides '!'");
            }
        }
        if (i == v.size() - 1 && isValidOperator(v[i])) {
            throw runtime_error("Invalid Expression: Cannot end expression with operation");
        }

        if (v[i] == '!' && i != 0 && isValidOperand(v[i - 1])) {
            throw runtime_error("Invalid Expression: Operator '!' between Operands");
        }
    }
}

};

int main() {
    Handler handler;
    Operators op;
    Evaluator eval;

    string line;
    cout << "Expression: ";
    getline(cin, line);

    vector<char> v;
    for (int i = 0; i < line.size(); i++) {
        if (line[i] != ' ') {
            v.push_back(line[i]);
        }
    }

    try {
        handler.errorHandling(v);

        while (v.size() > 1) {
            v = eval.grouping(v);
        }

        cout << "Evaluation: ";
        if (v[0] == 'T') {
            cout << "True";
        } else {
            cout << "False";
        }
    } catch (const runtime_error& x) {
        cerr << x.what() << endl;
    }

    return 0;
}