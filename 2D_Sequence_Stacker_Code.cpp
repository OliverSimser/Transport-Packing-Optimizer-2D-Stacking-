#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>

int singleMethod(
    const std::vector<double> &parts,
    int target) { // method to find the best value to pair up with longest part
                  // taken from the parts list, (add to total of 144)
  for (int i = 0; i < parts.size(); i++) {
    if (parts[i] <= target) { // find closest value to target that will fit
      return i;
    }
  }
  return -1; // no valid value found
}

std::vector<int> twoSumMethod(
    const std::vector<double> &parts, double target,
    int leftStart) { // method to find the best two values to pair up with
                     // longest part (closest result to 144 using 3 parts total)
  int leftPointer = leftStart;
  int rightPointer = (parts.size() - 1);
  std::vector<int> bestIndices = {-1, -1};
  double bestDif = 144;
  while (leftPointer < rightPointer) {
    double sum = parts[leftPointer] + parts[rightPointer];
    if (std::abs(target - sum) < std::abs(bestDif)) {
      if (sum <= target) {
        bestDif = (target - sum);
        bestIndices = {leftPointer, rightPointer};
        if (bestDif == 0)
          break;
      }
    }
    if (sum > target)
      leftPointer++;
    else
      rightPointer--;
  }
  return bestIndices;
}

std::vector<int> threeSumMethod(
    const std::vector<double> &parts,
    double target) { // method to find the best three values to pair up with
                     // longest part (closest result to 144 using 4 parts total)
  std::vector<int> bestIndices = {-1, -1, -1};
  int bestDif = 144;
  for (int i = 0; i < parts.size(); i++) {
    if (parts[i] > target + 16)
      continue;

    double complement = target - parts[i];
    std::vector<int> twosAdded = twoSumMethod(parts, complement, i + 1);

    if (twosAdded[0] == -1)
      continue;

    double sum = parts[i] + parts[twosAdded[0]] + parts[twosAdded[1]];
    double dif = target - sum;

    if (dif >= 0 && dif < bestDif) {
      bestDif = dif;
      bestIndices = {i, twosAdded[0], twosAdded[1]};

      if (dif == 0)
        break;
    }
  }
  return bestIndices;
}

int main() {
  std::vector<double> parts = {
      144,  144,  144,   144,   142,   142,   141.5, 139.5, 139,   136.5, 136.5,
      136,  119,  119,   119,   119,   119,   119,   119,   119,   119,   119,
      119,  119,  112.5, 110.5, 110.5, 110.5, 110.5, 110.5, 110.5, 110,   110,
      110,  110,  110,   73,    69,    69,    65,    64.5,  60.5,  60.5,  56.5,
      56,   54,   53.5,  53,    52.5,  52,    52,    52,    52,    51.5,  51.5,
      51,   51,   50.5,  50.5,  48,    29.5,  27.5,  27.5,  27.5,  27.5,  27.5,
      27.5, 27.5, 27.5,  27.5,  27.5,  27.5,  27.5,  27,    27,    27,    27,
      27,   27,   27,    27,    27,    27,    27,    27,    19.5,  19.5,  19.5,
      19.5, 19.5, 19.5,  19.5,  19.5,  19.5,  19.5,  19.5,  19.5,  15,    14.5,
      14,   14,   12.5,  12,    11.5,  11.5,  11,    11,    9,     9,     8.5,
      8.5,  8,    8,     8,     8,     8,     8,     8};
  std::sort(parts.begin(), parts.end(), std::greater<double>());

  std::vector<std::vector<double>>
      layers; // a vector that holds vectors for each layer

  while (!parts.empty()) {

    double firstPart = parts[0];
    parts.erase(parts.begin());

    double target = 144 - firstPart;

    std::vector<double> layer;
    layer.push_back(firstPart);

    if (target >= 8) {
      int addOne = singleMethod(parts, target);
      std::vector<int> addTwo = twoSumMethod(parts, target, 0);
      std::vector<int> addThree = threeSumMethod(parts, target);

      double bestSingle =
          (addOne == -1)
              ? -1
              : parts[addOne]; // (condition) ? value_it_becomes_if_true :
                               // value_it_becomes_if_false
      double bestTwo =
          (addTwo[0] == -1) ? -1 : parts[addTwo[0]] + parts[addTwo[1]];
      double bestThree =
          (addThree[0] == -1)
              ? -1
              : parts[addThree[0]] + parts[addThree[1]] + parts[addThree[2]];

      double best = std::max({bestSingle, bestTwo, bestThree});

      if (best == -1)
        continue;

      if (best == bestSingle) {
        layer.push_back(parts[addOne]);
        parts.erase(parts.begin() + addOne);
      } else if (best == bestTwo) {
        layer.push_back(parts[addTwo[0]]);
        layer.push_back(parts[addTwo[1]]);
        parts.erase(parts.begin() + addTwo[1]);
        parts.erase(parts.begin() + addTwo[0]);
      } else if (best == bestThree) {
        layer.push_back(parts[addThree[0]]);
        layer.push_back(parts[addThree[1]]);
        layer.push_back(parts[addThree[2]]);
        parts.erase(parts.begin() + addThree[2]);
        parts.erase(parts.begin() + addThree[1]);
        parts.erase(parts.begin() + addThree[0]);
      }
    }

    layers.push_back(layer);
  }

  // print layers
  std::cout << "\nLayers:\n\n";

  for (int i = 0; i < layers.size(); i++) {
    double sum = 0;

    std::cout << "Layer " << i + 1 << ": ";

    for (int j = 0; j < layers[i].size(); j++) {
      std::cout << layers[i][j] << " ";
      sum += layers[i][j];
    }

    std::cout << " | Total = " << sum << "\n";
  }
}