/**
 * The base interface for menus
 */
#ifndef __MENUINTERFACE_H
#define __MENUINTERFACE_H

#include <Arduino.h>
#include "MenuBackend.h"


//forward declaration
class MenuBackend;
class MenuItem;
struct MenuChangeEvent;
struct MenuUseEvent;
struct MenuItemChangeEvent;

/*
*/
class MenuInterface
{
    public:
          MenuInterface(){};

    /**
     * Handles all events
     * This is where you define a behaviour for a menu item
     */
    virtual void menuUse(MenuUseEvent used) = 0;

    /**
     * Here we get a notification whenever the user changes the menu
     * That is, when the menu is navigated
     */
    virtual void menuChange(MenuChangeEvent changed) = 0;

    /**
     * Here we get a notification whenever
     */
    virtual void menuItemChange(MenuItemChangeEvent changed) = 0;

};


// __MENUINTERFACE_H
#endif
