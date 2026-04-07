#include <iostream>
#include <vector>

double singleTolerance = 3.5; //Tolerances to favor smaller groups, thus using up less "glueing" pieces
double doubleTolerance = 2.5;

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
    
    double currentSum = 0;
    for (double v : layer)
    currentSum += v;
    
    if (currentSum <= 136){ //To use up extra space in the final layers if they full of the last small pieces of our list and has extra space
        
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
//  Test vector 2:
//  std::vector<double> parts = {
//       8,10,10,18.5,22,22,22,25,25,29,29.5,30,31.5,33,35,37,40.5,44.5,45,45.5,45.5,45.5,46,46,47,48,53,62.5,68.5,69,69,81,84,88,92.5,92.5,92.5,92.5,94,96,104.5,132,138,144,144,144,144,144,144,144,144,144,144,144,144,144,144,144,144,144,144,144,144,144,10.5,10.5,10.5,10.5,25,25,25.5,25.5,76,76};
//  Test vector 3:
//  std::vector<double> parts = {
//       22.5, 85, 98.5, 98.5, 98.5, 98.5, 98.5, 100.5, 102.5, 112.5, 116, 116, 119, 122, 122, 126, 126, 126, 126.5, 128, 139, 139, 142, 142, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 8, 8, 8.5, 8.5, 9, 9.5, 10.5, 11, 11, 11.5, 11.5, 11.5, 11.5, 11.5, 11.5, 11.5, 11.5, 11.5, 11.5, 11.5, 11.5, 12, 12, 12.5, 12.5, 13, 13.5, 14.5, 16.5, 17.5, 19, 23.5, 24, 24, 24.5, 25.5, 26, 27, 31.5, 37.5, 42.5, 43.5, 50, 65, 67.5, 69, 75, 77, 77, 77, 77, 77, 78, 83, 85, 90, 112, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 8, 8.5, 8.5, 8.5, 8.5, 8.5, 9, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 20.5, 64.5, 65.5, 77.5, 78, 88, 92, 107};
    std::sort(parts.begin(), parts.end(), std::greater<double>());
  
    std::vector<std::vector<double>>
    layers; // a vector that holds vectors for each layer
  
    bool useMidBand{0};
    
    double total = parts.size();
  
    double midCount = count_if(parts.begin(), parts.end(), [](double x) {
    return x >= 40 && x <= 90;});

                        
    double awkwardCount = count_if(parts.begin(), parts.end(),
    [](double x){ return x >= 90 && x <= 118; });

    if (midCount > total * 0.30 || awkwardCount > total * 0.95) useMidBand = true; //If there is a lot of middle pieces compared to the number of large pieces..
    
    double awkwardPieceStartRange = 80;
    double smallSum = 0;
    int smallCount = 0;

    for (double x : parts) {
        if (x < 40) {
            smallSum += x;
            smallCount++;
        }
    }
    
    if (smallCount > 0){
        double avgSmall = smallSum / smallCount;
        double smallRatio = static_cast<double>(smallCount) / parts.size(); 
        awkwardPieceStartRange = 144 - (avgSmall * 4.5 * smallRatio); //If most of the small pieces are 8 for example, then we will start sorting our layers, using up awkward pieces that are much higher, like at 132, as there are more small pieces like 8 to help deal with this, so we will deal with it sooner
        if (awkwardPieceStartRange < 85) awkwardPieceStartRange = 85;
        if (awkwardPieceStartRange > 124) awkwardPieceStartRange = 124;
    }
    
    if (smallCount > total * 0.4) {
        // MANY glue pieces → pack tightly
        singleTolerance = 2.5;
        doubleTolerance = 2;
    }
    else if (awkwardCount > total * 0.15) {
        // MANY awkward pieces → need flexibility
        singleTolerance = 5.5;
        doubleTolerance = 4.5;
    }
    else if (awkwardCount > total * 0.1) {
        // MANY awkward pieces → need flexibility
        singleTolerance = 4;
        doubleTolerance = 3;
    }

    int midStart = 0;
    while (!parts.empty()) {
        if (useMidBand){
            for(int i = 0; i < parts.size(); i++){
                if(parts[i] <= awkwardPieceStartRange){ //Start sorting at the highest value found above of the start of the awkward pieces
                midStart = i;
                break;
                }
            }
            while (parts[midStart] >= 47.5){
                makeNewLayer(parts, layers, midStart); //Then we start building the layers by using up the middle pieces first, to get rid of awkward middle pieces
            }
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
