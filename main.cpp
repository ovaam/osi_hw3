#include <iostream>
#include <vector>

//std::string getStringFromStack(std::vector<char> temp) {
//  std::string res;
//  for (char letter : temp) {
//    res += letter;
//  }
//  return res;
//}

bool checkIfPollindrom(std::vector<char> temp) {
  std::vector<char> new_temp = temp;
  std::reverse(new_temp.begin(), new_temp.end());
  for (int i = 0; i < temp.size(); i++) {
    if (temp[i] != new_temp[i]) {
      return false;
    }
  }
  return true;
}


int main()
{
    std::string input;
    std::string output;
    std::cin >> input;
    std::vector<char> temp;
    bool flag = false;

    for (char letter : input) {
      temp.push_back(letter);
    }

    for (char letter : input) {
      temp.erase(temp.begin());
      temp.insert(temp.end(), letter);
      if (checkIfPollindrom(temp)) {
        std::cout << "yes" << std::endl;
        flag = true;
        break;
      }
    }

    if (!flag) {
      std::cout << "no" << std::endl;
    }

    return 0;
}