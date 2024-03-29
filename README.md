**WORK IN PROGRESS**

# Yield of Norway Spruce in Sweden

A c++ implementation (in progress) of Harry Erikssons yield model for Norway Spruce from 1976.

Reference:
*Eriksson, Harry (1976). Granens produktion i Sverige (Yield of Norway spruce in Sweden). Research Notes. No. 41. Dept. of Forest Yield Research. Royal College of Forestry. Stockholm.*

## File description

* ErikssonStemDistributions.h implements an approximation of the skew-normal distribution described by Cramér. Correction for classes is done. 

* HagglundHeightSpruce.h implements a few of the height functions.

* Stand.h implements the general StandObject.

* ThinningManager.h holds management rules and checks for a StandObject.

* StandManager.h oversees interaction between StandObjects and their ThinningManager. Implements the incremental process (thin and then increment..)

## TODO:
* Remove superfluous #includes for Eriksson1976v2.cpp (Eriksson_1976.cpp is deprecated but not removed for ongoing comparisons between major version changes.)
* Implement StandObject::cut() - to conduct a thinning and update vals.
* Implement StandObject::result() - to print results.
* Implement ThinningManager. - Currently no effect. Based on variables and objects from earlier practice runs.
* Implement StandManager. Currently no effect. as ThinningManager.