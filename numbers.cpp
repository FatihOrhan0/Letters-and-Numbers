#include <algorithm>
#include <iostream>
#include <numeric>
#include <unordered_map>
#include <vector>
#include <climits>
#include <cfloat>

std::pair<int, int> get_pair(int num, int base) {
    return std::make_pair(num / base, 
      num % base >= num / base ? num % base + 1 : num % base);
}

//this is only because the c++ version I use has issues with precision on the default abs function
double abs(double d) { 
  if (d >= 0) return d;
  return -1 * d;
}

std::vector<std::pair<int, int> > find_seq(int num) {
  int first = num / 2880;
  num -= first * 2880;
  std::pair<int, int> p1 = get_pair(first, 5);
  int second = num / 144;
  std::pair<int, int> p2 = get_pair(second, 4);
  num -= second * 144;
  int third = num / 12;
  std::pair<int, int> p3 = get_pair(third, 3);
  num -= third * 12;
  int fourth = num / 2;
  std::pair<int, int> p4 = get_pair(fourth, 2);
  num -= fourth * 2;
  int fifth = num % 2;
  std::pair<int, int> p5 = get_pair(fifth, 1);
  return { p1, p2, p3, p4, p5 };
}

std::vector<int> extract_bits(int number) {
    std::vector<int> result;
    int divisor = 256;
    while (number >= 0 && result.size() < 5) { 
      result.push_back(number / divisor);
      number -= divisor * result.back();
      divisor /= 4;
    }
    return result;
}

int main() {
  std::vector<std::vector<std::pair<int, int> > > sequences(86400);
  for (int i = 0; i < 86400; i++) { 
    sequences[i] = find_seq(i);
  }
  std::vector<std::vector<int> > operators_list(1024);
  for (int i = 0; i < 1024; i++) { 
    operators_list[i] = extract_bits(i);
  }
  std::vector<double> numbers = {100, 25, 2, 5, 7, 75};
  double target = 35800000000, closest_diff = DBL_MAX, closest_maker = 0, closest_operators = 0; 

  for (int i = 0; i < 86400; i++) {
    // if (i % 1000 == 0) std::cout << i / 1000 << std::endl;
    auto v = sequences[i];
    for (int k = 0; k < 1024; k++) { 
      auto operators = operators_list[k];
      std::vector<double> temp = numbers; 
      for (int j = 0; j < 5; j++) { 
        double new_num;
        if (operators[j] == 0) { 
          new_num = temp[v[j].first] + temp[v[j].second]; 
        }
        else if (operators[j] == 1) { 
          new_num = temp[v[j].first] * temp[v[j].second]; 
        }
        else if (operators[j] == 2) { 
          new_num = temp[v[j].first] - temp[v[j].second]; 
        }
        else { 
          if (temp[v[j].second] == 0) break;
          new_num = temp[v[j].first] / temp[v[j].second]; 
        }
        temp.erase(temp.begin() + std::max(v[j].first, v[j].second)); 
        temp[std::min(v[j].first, v[j].second)] = new_num;

        if (abs(target - new_num) < closest_diff) { 
          closest_diff = abs(target - new_num);
          closest_maker = i;
          closest_operators = k;
          if (closest_diff == 0.0) { 
            goto exact;
          }
        }
      }
    }
  }

  exact:
    auto v = find_seq(closest_maker);
    auto operators = extract_bits(closest_operators);
    for (int j = 0; j < 5; j++) { 
      double new_num;
      if (operators[j] == 0) { 
        new_num = numbers[v[j].first] + numbers[v[j].second]; 
        std::cout << numbers[v[j].first] << '+' << numbers[v[j].second] << '=' << new_num << std::endl;
      }
      else if (operators[j] == 1) { 
        new_num = numbers[v[j].first] * numbers[v[j].second]; 
        std::cout << numbers[v[j].first] << '*' << numbers[v[j].second] << '=' << new_num << std::endl;
      }
      else if (operators[j] == 2) { 
        new_num = numbers[v[j].first] - numbers[v[j].second]; 
        std::cout << numbers[v[j].first] << '-' << numbers[v[j].second] << '=' << new_num << std::endl;
      }
      else { 
        if (numbers[v[j].second] == 0) break;
        new_num = numbers[v[j].first] / numbers[v[j].second]; 
        std::cout << numbers[v[j].first] << '/' << numbers[v[j].second] << '=' << new_num << std::endl;
      }
      numbers.erase(numbers.begin() + std::max(v[j].first, v[j].second)); 
      numbers[std::min(v[j].first, v[j].second)] = new_num;
      if (abs(new_num - target) == closest_diff) return 1;  
    }
    return 1; 
}