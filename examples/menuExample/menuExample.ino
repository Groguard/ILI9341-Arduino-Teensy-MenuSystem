#include <ILI9341_t3.h>
#include <Bounce2.h>
#include <SPI.h>
#include <MenuSystem.h>

#define TFT_DC  9
#define TFT_CS 10
#define TFT_RST 255

#define BL_PIN 3
#define BRIGHTNESS 255   // how bright the tft is

#define up 26
#define down 25
#define select 24

bool appRunning = false;
int selected; // currently selected menu item

ILI9341_t3 tft = ILI9341_t3(TFT_CS, TFT_DC);

MenuSystem menu = MenuSystem(tft);

typedef void (* MenuFuncPtr) (); // Function pointer for Main Menu
typedef void (* Test1FuncPtr) (); // Function pointer for Test Menu 1
typedef void (* Test2FuncPtr) (); // Function pointer for Test Menu 2

char* mainMenu[3] = {"    Main Menu",
                    "Test Menu 1   ",
                    "Test Menu 2   "
                   };

char* testMenu1[4] = {"   Test Menu 1",
                    "Option 1   ",
                    "Option 2   ",
                    "Back       "
                   };

char* testMenu2[4] = {"   Test Menu 2",
                    "Option 1   ",
                    "Option 2   ",
                    "Back       "
                   };

// Predefine functions
void buildTestMenu1(void);
void buildTestMenu2(void);
void returnMainMenu(void);

// Main menu func pointer
MenuFuncPtr menu_func[10] = {0,
                            buildTestMenu1,
                            buildTestMenu2};
// Test menu Func Pointer
Test1FuncPtr test1_func[10] = {0,
                            0,
                            0,
                            returnMainMenu};

Test2FuncPtr test2_func[10] = {0,
                            0,
                            0,
                            returnMainMenu};

// Instantiate a Bounce object
// up
Bounce debouncer1 = Bounce();
// down
Bounce debouncer2 = Bounce();
// select
Bounce debouncer3 = Bounce();

void setup() {
  Serial.begin(9600);

  // Button Setup
  pinMode(up, INPUT_PULLUP);
  debouncer1.attach(up);
  debouncer1.interval(5); // interval in ms

  pinMode(down, INPUT_PULLUP);
  debouncer2.attach(down);
  debouncer2.interval(5); // interval in ms

  pinMode(select, INPUT_PULLUP);
  debouncer3.attach(select);
  debouncer3.interval(5); // interval in ms

  // Tft Setup
  pinMode(TFT_RST, OUTPUT);
  digitalWrite(TFT_RST, HIGH);
  analogWrite(BL_PIN, BRIGHTNESS);   // Turn Backlight on full

  tft.begin();
  menu.start();
  menu.setMenu(mainMenu, sizeof(mainMenu)/sizeof(char *)-1);
  menu.setFunc(menu_func, sizeof(menu_func));
  menu.displayMenu();
}

void menuInput(void) {
  debouncer1.update(); // Check for button presses
  debouncer2.update(); // Check for button presses
  debouncer3.update(); // Check for button presses

  if (debouncer1.fell()) {
    // Up
    menu.action(UP);
  }
  else if (debouncer2.fell()) {
    // Down
    menu.action(DOWN);
  }
  else if (debouncer3.fell()) {
    selected = menu.returnSelected();
    menu.action(SELECT);
    Serial.println(selected);
  }
  delay(100);
}

void loop() {
  while(!appRunning) {
    menuInput();
  }
}

void returnMainMenu(void) {
  menu.setMenu(mainMenu, sizeof(mainMenu)/sizeof(char *)-1);
  menu.setFunc(menu_func, sizeof(menu_func));
  menu.displayMenu();
}

void buildTestMenu1(void) {
  menu.setMenu(testMenu1, sizeof(testMenu1)/sizeof(char *)-1);
  menu.setFunc(test1_func, sizeof(test1_func));
  menu.displayMenu();
}

void buildTestMenu2(void) {
  menu.setMenu(testMenu2, sizeof(testMenu2)/sizeof(char *)-1);
  menu.setFunc(test2_func, sizeof(test2_func));
  menu.displayMenu();
}
