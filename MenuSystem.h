/*
  MenuSystem.h - Library for creating a menu system on ILI9341.
  Created by Chris Alessandro(Groguard), June 15, 2017.
  Released into the public domain.
*/

#ifndef MenuSystem_h
#define MenuSystem_h

#include <SPI.h>
#include <ILI9341_t3.h>

#define UP 1
#define DOWN 2
#define SELECT 3
typedef void (* CurrentFuncPtr) (); // Function pointer for menus


class MenuSystem
{
    public:
        MenuSystem(ILI9341_t3 tft);
        void start(void);
        void displayMenu(void); // display the current menu
        void setMenu(char** menu, uint8_t size); // set the current menu
        void setFunc(CurrentFuncPtr* funcs, uint8_t size);
        void action(uint8_t action); // process user input
        int returnSelected(void);
    private:
        uint8_t menu_top = 35; // top space between title and items
        uint8_t selected; // currently selected menu item
        uint8_t current_menu_index = 1; // current position in the array
        uint8_t current_menu_size;
        uint8_t previous_menu_size;
        CurrentFuncPtr menu_func[256] = {0};
        char* current_menu[256] = {0};
        bool page_change = false;
        void menuHighlight(uint8_t index); // highlight a menu item based on index
        void setPreviousMenu(char** menu, uint8_t size);
};

#endif
