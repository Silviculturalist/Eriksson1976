#include "HagglundHeightSpruce.h"

int main()
{

  std::cout << "South G8 : " <<  HagglundTimeToBreastHeightSpruceSouth(8) << "\n"
            << "South G16 : " <<  HagglundTimeToBreastHeightSpruceSouth(16) << "\n"
            << "North G8, Lat 61, Planted : " <<  HagglundTimeToBreastHeightSpruceNorth(8,61,1) << "\n"
            << "North G16, Lat 61, Planted : " <<  HagglundTimeToBreastHeightSpruceNorth(16,61,1) << "\n"
            << std::endl;

  std::cout << "South G8, 100 yrs Total: " <<  HagglundHeightSpruceSouth(8,100-HagglundTimeToBreastHeightSpruceSouth(8),100-HagglundTimeToBreastHeightSpruceSouth(8)) << "\n"
            << "South G16, 100 yrs Total: " <<  HagglundHeightSpruceSouth(16,100-HagglundTimeToBreastHeightSpruceSouth(16),100-HagglundTimeToBreastHeightSpruceSouth(16)) << "\n"
            << "North G8, Lat 61, Planted, 100 yrs Total: " <<  HagglundHeightSpruceNorth(8,100-HagglundTimeToBreastHeightSpruceNorth(8,61,1),100-HagglundTimeToBreastHeightSpruceNorth(8,61,1),61,1) << "\n"
            << "North G16, Lat 61, Planted, 100 yrs Total: " <<  HagglundHeightSpruceNorth(16,100-HagglundTimeToBreastHeightSpruceNorth(16,61,1),100-HagglundTimeToBreastHeightSpruceNorth(16,61,1),61,1) << "\n"
            << std::endl;

  return 0;
}