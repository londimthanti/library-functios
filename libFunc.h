#ifndef LIBFUNC_H_INCLUDED
#define LIBFUNC_H_INCLUDED
#include <cstdlib>
#include <iostream>
#include <ctime>
#include <sstream>
#include <cassert>

using namespace std;
namespace SeaSpace
{
    ///Two day array of integers
      typedef int** t_2DArray;
      typedef int* t_1DArray;

     enum enumFeatures {MINE, CRUSH_0, CRUSH_10, CRUSH_30};  ///Various features that can exist in the 2D
     enum enumState {ONGOING, WON, DIED, EXPLODED, QUIT};    ///State of the game
     enum enumSinkState {SUNK,FLOAT};
     enum enumErrors                                         ///Differenct return values

      {
          ERR_ARG_COUNT = -1,
          ERR_CONVERSION = -2,
          ERR_RANGE = -3,
          SUCCESS = 0
      };

       const char CH_FEATURES[] = {'*','.','-','='};
       const char CH_PLAYER = 'P';                             ///Character for the submarine

       const int CHANCE_SINK = 20;                             ///Chance that the submarine will sink
       const int CHANCE_10 = 10;                               ///Chance that the submarine will implode in 10%
       const int CHANCE_30 = 30;                               ///Chance that the submarine will implode in 30%

        ///Structure used to describe a specific location
        struct strucLocation
       {
            int row;
            int col;
        };

        ///Structure used to describe the game
       struct strucGame
        {
            t_2DArray arrSea;               ///The 2D array with all the features of the world
            strucLocation playerLocation;   ///Used to keep track of the submarine location
            int intTRows;                   ///Total Rows
            int intTCols;                   ///Total Columns
            enumState state;                ///State of the game
            enumSinkState sinkState;        ///Did we sink or not?
        };

        ///Prototype functions used by the main program
        strucGame Initialise(int intTRows, int intTCols, int intMineChance);
        void PrintScreen(strucGame gameWorld);
        void MovePlayer(strucGame& gameWorld, char chInput);
        void Deallocate(strucGame& gameWorld);
        void Implode(strucGame& gameWorld);
    }

#endif // LIBFUNC_H_INCLUDED
