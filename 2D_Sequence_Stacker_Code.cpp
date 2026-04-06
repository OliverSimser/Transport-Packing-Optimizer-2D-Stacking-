#include <iostream>
#include <vector>

double singleTolerance = 3; //Tolerances to favor smaller groups, thus using up less "glueing" pieces
double doubleTolerance = 2;

int singleMethod(
    const std::vector<double> &parts,
    double target, int startIndex) { // method to find the best value to pair up with longest part
                  // taken from the parts list, (add to total of 144)
  for (int i = startIndex; i < parts.size(); i++) {
    if (parts[i] <= target) { // find closest value to target that will fit
      return i;
    }
  }
  return -1; // no valid value found
}

std::vector<int> twoSumMethod(
    const std::vector<double> &parts, double target,
    int leftStart) { // method to find the best two values to pair up with longest part (closest result to 144 using 3 parts total)
  int leftPointer = leftStart;
  int rightPointer = (parts.size() - 1); //Create pointers on both ends
  std::vector<int> bestIndices = {-1, -1};
  double bestDif = 144;
  while (leftPointer < rightPointer) {
    double sum = parts[leftPointer] + parts[rightPointer];
    if (std::abs(target - sum) < std::abs(bestDif)) { //if the sum of these two pointers indexs is a new best, save it and update the new best difference
      if (sum <= target) {
        bestDif = (target - sum);
        bestIndices = {leftPointer, rightPointer};
        if (bestDif == 0)
          break;
      }
    }
    if (sum > target) //If the sum is too large, move the left pointer in, so that we are dealing with a smaller number on this end of the sorted array
      leftPointer++;
    else 
      rightPointer--;
  }
  return bestIndices;
}

std::vector<int> threeSumMethod(
    const std::vector<double> &parts,
    double target, int startIndex) { // method to find the best three values to pair up with longest part (closest result to 144 using 4 parts total)
  std::vector<int> bestIndices = {-1, -1, -1};
  double bestDif = 144;
  for (int i = startIndex; i < parts.size(); i++) { //Go down the list and get each numbers compliment, then search twos sum for the rest of the array to see which pair along with the initial part gets closest to target
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
    
void makeNewLayer (std::vector<double> &parts, //Build the layers by choosing best sum method considering our tolerances
                   std::vector<std::vector<double>> &layers,
                   int startIndex){
                       
    double firstPart = parts[startIndex];
    parts.erase(parts.begin() + startIndex) ;

    double target = 144 - firstPart;

    std::vector<double> layer;
    layer.push_back(firstPart);
    
    double best{-2};
    
    if (target >= 8) {
      int addOne = singleMethod(parts, target, startIndex);
      std::vector<int> addTwo = twoSumMethod(parts, target, startIndex);
      std::vector<int> addThree = threeSumMethod(parts, target, startIndex);

      double bestSingle =
          (addOne == -1) ? -1: parts[addOne]; // (condition) ? value_it_becomes_if_true :
                               // value_it_becomes_if_false
      double bestTwo =
          (addTwo[0] == -1) ? -1 : parts[addTwo[0]] + parts[addTwo[1]];
      double bestThree =
          (addThree[0] == -1)
              ? -1
              : parts[addThree[0]] + parts[addThree[1]] + parts[addThree[2]];

      best = std::max({bestSingle, bestTwo, bestThree});

      if (best == -1){}
      else if (best == bestSingle || bestSingle >= (best - singleTolerance)) {
        layer.push_back(parts[addOne]);
        parts.erase(parts.begin() + addOne);
      } else if (best == bestTwo || bestTwo >= (best - doubleTolerance)) {
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
    
    if (parts[0] <= 60){ //To use up extra space in the final layers if they full of the last small pieces of our list and has extra space
        double currentSum = 0;
        for (double v : layer)
        currentSum += v;
        
        while (true){ //keep on adding parts until adding a new part will exceed 144
            double targetNew = 144 - currentSum;
            int addOneNew = singleMethod(parts, targetNew, 0);
            std::vector<int> addTwoNew = twoSumMethod(parts, targetNew, 0);
            double bestSingleNew =
            (addOneNew == -1) ? -1: parts[addOneNew]; // (condition) ? value_it_becomes_if_true :
                               // value_it_becomes_if_false
            double bestTwoNew =
            (addTwoNew[0] == -1) ? -1 : parts[addTwoNew[0]] + parts[addTwoNew[1]];
            double bestNew = std::max({bestSingleNew, bestTwoNew});
            if (bestNew == -1) break;
            if (bestNew == bestSingleNew) {
                currentSum += parts[addOneNew];
                layer.push_back(parts[addOneNew]);
                parts.erase(parts.begin() + addOneNew);
            }
            else {
                currentSum += parts[addTwoNew[0]] + parts[addTwoNew[1]];
                layer.push_back(parts[addTwoNew[0]]);
                layer.push_back(parts[addTwoNew[1]]);
                parts.erase(parts.begin() + addTwoNew[1]);
                parts.erase(parts.begin() + addTwoNew[0]);
            }
        }
    }
    layers.push_back(layer);
}

int main() {
    //Test vector 1:
//   std::vector<double> parts = {
//       144,  144,  144,   144,   142,   142,   141.5, 139.5, 139,   136.5, 136.5,
//       136,  119,  119,   119,   119,   119,   119,   119,   119,   119,   119,
//       119,  119,  112.5, 110.5, 110.5, 110.5, 110.5, 110.5, 110.5, 110,   110,
//       110,  110,  110,   73,    69,    69,    65,    64.5,  60.5,  60.5,  56.5,
//       56,   54,   53.5,  53,    52.5,  52,    52,    52,    52,    51.5,  51.5,
//       51,   51,   50.5,  50.5,  48,    29.5,  27.5,  27.5,  27.5,  27.5,  27.5,
//       27.5, 27.5, 27.5,  27.5,  27.5,  27.5,  27.5,  27,    27,    27,    27,
//       27,   27,   27,    27,    27,    27,    27,    27,    19.5,  19.5,  19.5,
//       19.5, 19.5, 19.5,  19.5,  19.5,  19.5,  19.5,  19.5,  19.5,  15,    14.5,
//       14,   14,   12.5,  12,    11.5,  11.5,  11,    11,    9,     9,     8.5,
//       8.5,  8,    8,     8,     8,     8,     8,     8};
    //Test vector 2:
//  std::vector<double> parts = {
//       8,10,10,18.5,22,22,22,25,25,29,29.5,30,31.5,33,35,37,40.5,44.5,45,45.5,45.5,45.5,46,46,47,48,53,62.5,68.5,69,69,81,84,88,92.5,92.5,92.5,92.5,94,96,104.5,132,138,144,144,144,144,144,144,144,144,144,144,144,144,144,144,144,144,144,144,144,144,144,10.5,10.5,10.5,10.5,25,25,25.5,25.5,76,76};
//  Test vector 3:
    std::vector<double> parts = {
       27.5, 27.5, 27.5, 27.5, 27.5, 27.5, 110.5, 110.5, 110.5, 110.5, 110.5, 110.5, 120, 120, 120, 120, 120, 120, 144, 144, 144, 144, 144, 144, 144, 8.5, 8.5, 8.5, 8.5, 8.5, 8.5, 9.5, 9.5, 9.5, 9.5, 9.5, 9.5, 12, 12, 12, 12, 12, 12, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 69, 69, 69, 69, 69, 69, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 12.5, 12.5, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 17, 18, 18, 19.5, 19.5, 20, 20, 20.5, 20.5, 20.5, 20.5, 21, 21, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 24.5, 27, 28, 29.5, 30.5, 31.5, 31.5, 31.5, 31.5, 31.5, 31.5, 32, 32, 32, 32, 32, 32, 32, 32, 32.5, 34, 35, 35, 35, 35.5, 37.5, 38.5, 40, 40.5, 40.5, 42, 42.5, 42.5, 42.5, 43.5, 46.5, 55, 57, 57.5, 61, 62, 64, 64, 67, 67.5, 67.5, 68.5, 68.5, 69.5, 69.5, 69.5, 69.5, 69.5, 69.5, 70, 72, 76, 84, 85.5, 91.5, 92, 92.5, 95.5, 99.5, 100.5, 117, 117, 117, 117, 117, 117, 120.5, 123, 124.5, 132, 132, 135, 135.5, 135.5, 136, 137.5, 138.5, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144};
  std::sort(parts.begin(), parts.end(), std::greater<double>());
  
    std::vector<std::vector<double>>
    layers; // a vector that holds vectors for each layer
  
    bool useMidBand{0};
  
    double midCount = count_if(parts.begin(), parts.end(), [](double x) {
    return x >= 35 && x <= 85;});

                        
    double largeCount = count_if(parts.begin(), parts.end(),
    [](double x){ return x >= 100 && x <= 136; });

    if (largeCount > 0 && midCount > largeCount * 1.5 && midCount > 15) useMidBand = true; //If there is a lot of middle pieces compared to the number of large pieces..
    
    double akwardPieceStartRange = 75;
    
    double upperMidCount = count_if(parts.begin(), parts.end(),
    [](double x){ return x >= 75 && x <= 110; });
    
    if (upperMidCount > 15 && upperMidCount > largeCount * 0.55) { //If many of the akward pieces have length 110 to 75, start by using this first
    akwardPieceStartRange = 108.5;
    }

int midStart = 0;
  while (!parts.empty()) {
      if (useMidBand){
      for(int i = 0; i < parts.size(); i++){
          if(parts[i] <= akwardPieceStartRange){ //Start sorting at the highest value found above of the start of the akward pieces
              midStart = i;
              break;
          }
      }
      while (parts[midStart] >= 47.5){
      makeNewLayer(parts, layers, midStart); //Then we start building the layers by using up the middle pieces first, to get rid of akward middle pieces
      }
      }
      else{
          singleTolerance = 6; //If there is not significantly more mid-length parts, we add looser tolerances, to allow these limited amount of middle pieces to be used up sooner with this new favorability to smaller groups (using more of these longer, mid-length pieces rather than many small ones), so we are not left with only akward middle pieces at the end
          doubleTolerance = 5;
      }
    makeNewLayer(parts, layers, 0); //Otherwise, start building layers from largest piece
  }

  // Print layers   
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
