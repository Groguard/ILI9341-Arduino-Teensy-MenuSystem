#include <MenuSystem.h>

ILI9341_t3 _tft(10, 9);

MenuSystem::MenuSystem(ILI9341_t3 tft)
{
    _tft=tft;
}

void MenuSystem::start(void)
{
    _tft.begin();
    _tft.setTextWrap(false);
    _tft.setRotation(3);
    _tft.setTextSize(3);
}

void MenuSystem::setMenu(char** menu, uint8_t size) {
    memset(current_menu, 0, sizeof(current_menu));
    int i;
    for(i=0; i<=size; i++) {
        current_menu[i] = menu[i];
    }
    selected = 1;
    current_menu_size = i-2;
}

void MenuSystem::setFunc(CurrentFuncPtr* funcs, uint8_t size) {
    memset(menu_func, 0, sizeof(menu_func));
    int i;
    for(i=0; i<=size; i++) {
        menu_func[i] = funcs[i];
    }
}

void MenuSystem::displayMenu(void) {
    int i;
    _tft.fillScreen(ILI9341_BLACK);
    _tft.setCursor(0, 0);
    _tft.setTextColor(ILI9341_GREEN, ILI9341_BLACK); 
    _tft.println(current_menu[0]);
    _tft.drawLine(0, 25, _tft.width()-1, 25, ILI9341_GREEN);
    
    _tft.setTextColor(ILI9341_YELLOW, ILI9341_BLACK);  
    for(i = current_menu_index; i <= (current_menu_index + 6); i++) {
        _tft.setCursor(0, ((i - current_menu_index) * 30) + menu_top);    
        _tft.println(current_menu[i]);
    }
    menuHighlight(selected);
}

void MenuSystem::menuHighlight(uint8_t index) {
    if (page_change == false) {
        _tft.setCursor(0, ((selected - current_menu_index) * 30) + menu_top);
        _tft.setTextColor(ILI9341_YELLOW, ILI9341_BLACK);
        _tft.println(current_menu[selected]);
    }
    _tft.setCursor(0, ((index - current_menu_index) * 30) + menu_top);
    _tft.setTextColor(ILI9341_YELLOW, ILI9341_BLUE);
    _tft.println(current_menu[index]);
    selected = index;
    page_change = false;
}

int MenuSystem::returnSelected(void) {
    return selected;
}

void MenuSystem::action(uint8_t action) {
    switch (action) {
        case 1: { // up
            if (selected <= (current_menu_index + 6) && (selected > current_menu_index)) {
                menuHighlight(selected - 1);
            }
            else if ((current_menu_index > 1) && (selected <= current_menu_index)) {
                current_menu_index = (current_menu_index - 1);
                selected = selected - 1;
                page_change = true;
                displayMenu();
            }
            break;
        }
        case 2: { // down
            if (selected >= (current_menu_index + 6) && 
                (current_menu_size > 6) && (selected <= current_menu_size)) {
                current_menu_index = (current_menu_index + 1);
                selected = selected + 1;
                page_change = true;
                displayMenu();
            }
            else if ((selected < (current_menu_index + 6)) &&
                    (selected <= current_menu_size)) {
                menuHighlight(selected + 1);
            }
            break;
        }
        case 3: // select
            menu_func[selected]();//
            break;
    }
}
