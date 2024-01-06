#include <iostream>
#include "libFunc.h"

using namespace std;
using namespace SeaSpace;


int main()
{
    int ConvertString(string strWord);
    int intNum = 0;
    stringstream ss(strWord);
    ss >> intNum;
    if(ss.fail())
    {
        cerr << "couldnot convert";
        exit(ERR_CONVERSION);
    }
    return intNum;
}

     int main(int argc, char** argv)
  {
      srand(time(nullptr));
      ///Test for 3 command line args
     if(argc!=4)
      {
         cerr << "Please run Sinking.exe <TotalRows> <TotalCols> <MineChance>";
          exit(ERR_ARG_COUNT);
      }
      ///Converting command line args to variables.
      int intTRows = ConvertString(argv[1]);
      int intTCols = ConvertString(argv[2]);
      int intMineChance = ConvertString(argv[3]);

      ///Range checks
      if(intTRows <5 || intTRows > 40 ||
         intTCols <5 || intTCols > 40)
      {
          cerr << "Please make sure the total rows and cols are greater than 5 and less than 40";
          exit(ERR_RANGE);
      }
      if(intMineChance <0 || intMineChance >100)
      {
          cerr << "Please make sure the chance of mine appearing is between 0 and 100";
          exit(ERR_RANGE);
      }

      ///Initialisation
      strucGame gameWorld = Initialise(intTRows,intTCols,intMineChance);
      char chInput = '\0';
      bool blnContinue = true;

      ///Main loop
      do
      {
          ///Print Screen
          PrintScreen(gameWorld);
         ///Handle Player Movement
          cin >> chInput;
          MovePlayer(gameWorld,chInput);
          ///Check for end states
          Implode(gameWorld);
          ///See if the player is in the top row
          if(gameWorld.playerLocation.row == 0)
              gameWorld.state = enumState::WON;
          ///See if the main loop should stop.
          if(gameWorld.state != ONGOING)
              blnContinue = false;
      }while(blnContinue);
      ///Final refresh of the screen
      PrintScreen(gameWorld);
      ///Output end message
      switch(gameWorld.state)
      {
      case enumState::EXPLODED:
          cout << "You exploded against a mine!" << endl;
          break;
      case enumState::DIED:
          cout << "You got crushed!." << endl;
          break;
     case enumState::QUIT:
          cout << "You quit the game." << endl;
          break;
      case enumState::WON:
          cout << "You escaped!" << endl;
         break;
      default:
         ///Doesn't do anything, but it gets rid of a compiler warning
          break;
     }
      ///Deallocate memory allocated to 2D array.
     Deallocate(gameWorld);

     return SUCCESS;
  }

