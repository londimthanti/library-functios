#include "libFunc.h"

namespace SeaSpace
{
     ///Generate a random number between intUpper and intLower
        int GenRand(int intLower, int intUpper)
        {
         assert(intUpper > intLower);                ///Ensure the upper parameter is greater than the lower parameter

           int intRange = intUpper - intLower + 1;
           return rand()%intRange + intLower;
       }

       ///Iniliase the game
       strucGame Initialise(int intTRows, int intTCols, int intMineChance)
       {
           strucGame gameWorld;

           ///This variable is used to determine how many rows must be added to the middel zone
           int intZ1 = 0;
           ///See if the game rows are divisible by three to ensure my zones are correct
           if(intTRows%3!=0)
               intZ1++;

           gameWorld.arrSea = new t_1DArray[intTRows];
           ///Allocate memory AND initialise array
           for(int r=0;r<intTRows;r++)
           {
               gameWorld.arrSea[r] = new int[intTCols];
               ///Initialise initial state
               for(int c=0;c<intTCols;c++)
               {
                   if(GenRand(1,100)<=intMineChance)                   ///See if mine must appear
                       gameWorld.arrSea[r][c] = enumFeatures::MINE;
                   else if(r<intTRows/3)                               ///Are we in the top zone
                       gameWorld.arrSea[r][c] = enumFeatures::CRUSH_0;
                   else if(r>=intTRows/3 && r<((intTRows-intTRows/3)-intZ1)) ///Are we in the middle zone
                       gameWorld.arrSea[r][c] = enumFeatures::CRUSH_10;
                   else
                       gameWorld.arrSea[r][c] = enumFeatures::CRUSH_30;
               }
           }

           ///Place the player
           int intMiddleRow = intTRows / 2;                        ///If there is an even number, the player will be placed in the upper middle row

           int intCol = GenRand(0,intTCols-1);

           ///Get a location where the player wont explode
           while(gameWorld.arrSea[intMiddleRow][intCol]==enumFeatures::MINE)
           {
               intCol = GenRand(0,intTCols-1);
           }

           ///Set the rest of the values
           gameWorld.playerLocation.row = intMiddleRow;
           gameWorld.playerLocation.col = intCol;
           gameWorld.intTCols = intTCols;
           gameWorld.intTRows = intTRows;
           gameWorld.state = enumState::ONGOING;
           gameWorld.sinkState = enumSinkState::FLOAT;

           return gameWorld;

}
            ///Output the game world.
      void PrintScreen(strucGame gameWorld)
      {
          assert(gameWorld.arrSea!=nullptr);              ///Make sure we have allocated memory for the 2D

          system("cls");
          for(int r=0;r<gameWorld.intTRows;r++)
          {
              for(int c=0;c<gameWorld.intTCols;c++)
              {
                  ///See if the player is in the current location
                  if(gameWorld.playerLocation.row == r &&
                     gameWorld.playerLocation.col == c)
                      cout << CH_PLAYER;
                  else
                  {
                      ///Print out the feature in the game world
                      cout << CH_FEATURES[gameWorld.arrSea[r][c]];
                      cout << " ";
                  }
              }
              cout << endl;
          }
          cout << "Move up: w" << endl;
          cout << "Move left: a" << endl;
          cout << "Move right: d" << endl;
          cout << "Quit: x" << endl;
          ///See if we should highlight that we sunk
          if(gameWorld.sinkState==SUNK)
              cout << "You sunk down.  Hold on!!!" << endl;

      }

      ///Returns true if row and col is in the game
      bool IsInWorld(strucGame gameWorld, int row, int col)
      {
          return(row>=0 && row<gameWorld.intTRows &&
                 col>=0 && col<gameWorld.intTCols);
      }

      ///See if the player must sin
      void MoveDownWards(strucGame& gameWorld)
      {
          if(GenRand(1,100) <= CHANCE_SINK)
          {
              ///Update the sinkState value to help indicate to the player that the sub moved downwards
              gameWorld.sinkState = SUNK;
              ///Calculate how far to sink
              int intRows = GenRand(1,2);
              ///Set a destination location
              strucLocation locationDest = gameWorld.playerLocation;
              ///Calculate the new destination location
              locationDest.row = locationDest.row + intRows;
              if(IsInWorld(gameWorld,locationDest.row,locationDest.col))
              {
                  ///Always see if there is a mine directly underneath the player
                  ///This is necessary in cases where we move two locations1
               if(gameWorld.arrSea[gameWorld.playerLocation.row+1][gameWorld.playerLocation.col]==MINE)
                      gameWorld.state = EXPLODED;
                  ///See if you sunk into a mine (This handles the case if the player might have moved two

                  if(gameWorld.arrSea[locationDest.row][locationDest.col] == enumFeatures::MINE)
                      gameWorld.state = enumState::EXPLODED; ///See if you died
                  else
                      gameWorld.playerLocation = locationDest;
             }

             }
          else
              ///If we don't sink then we just floated.
              gameWorld.sinkState = FLOAT;
      }

      ///This function tests to see if the submarine will implode of the pressure.
      void Implode(strucGame& gameWorld)
      {
          ///intZone is used to help determine the zones in cases where the rows are not a multiple of 3
          int intZone = (gameWorld.intTRows%3);

          ///See if the submarine is in the 10% zone
         if(gameWorld.playerLocation.row >= (gameWorld.intTRows/3) &&
             gameWorld.playerLocation.row < (gameWorld.intTRows-(gameWorld.intTRows/3))-intZone)
          {
              if(GenRand(1,100)<=CHANCE_10)
                  gameWorld.state = enumState::DIED;
          }
          ///See if the submarine is in the 30% zone
          else if(gameWorld.playerLocation.row >= (gameWorld.intTRows-(gameWorld.intTRows/3))-intZone)
          {
              if(GenRand(1,100)<=CHANCE_30)
                  gameWorld.state = enumState::DIED;
          }
      }

      ///Move the player left, right an up
      void MovePlayer(strucGame& gameWorld, char chInput)
      {
          ///Make sure that the world has been initialised
          assert(gameWorld.arrSea != nullptr);
          ///Set destination location
          strucLocation locationDest = gameWorld.playerLocation;
          ///Modify the destination location
          switch(toupper(chInput))
          {
          case 'W':
              locationDest.row--;
              break;
          case 'A':
              locationDest.col--;
              break;
          case 'D':
              locationDest.col++;
              break;
          case 'X':
              gameWorld.state = enumState::QUIT;
          }

          ///Make sure the player does not move into a mine
          if(IsInWorld(gameWorld,locationDest.row,locationDest.col))
          {
              if(gameWorld.arrSea[locationDest.row][locationDest.col] == enumFeatures::MINE)
                  gameWorld.state = enumState::EXPLODED;
              else
                  gameWorld.playerLocation = locationDest;
          }
          ///Handle the possiblity of sinking
          MoveDownWards(gameWorld);
      }

      ///Deallocate memory for the 2D array
      void Deallocate(strucGame& gameWorld)
      {
          assert(gameWorld.arrSea != nullptr);
          for(int r=0;r<gameWorld.intTRows;r++)
          {
              delete[] gameWorld.arrSea[r];
          }
          delete[] gameWorld.arrSea;
          gameWorld.arrSea = nullptr;
      }
  }
