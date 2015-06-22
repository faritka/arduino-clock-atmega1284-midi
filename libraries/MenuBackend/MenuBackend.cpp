/* $Id: MenuBackend.cpp 1198 2011-06-14 21:08:27Z bhagman $
||
|| @author         Alexander Brevig <abrevig@wiring.org.co>
|| @url            http://wiring.org.co/
|| @contribution   Adrian Brzezinski
|| @contribution   Bernhard Benum
|| @contribution   Brett Hagman <bhagman@wiring.org.co>
||
|| @description
|| | Provides an easy way of making menus.
|| |
|| | Wiring Cross-platform Library
|| #
||
|| @license Please see cores/Common/License.txt.
||
*/

#include "MenuBackend.h"

void MenuItem::deleteFromMenu()
{
    //no links
    if (!hasChildren()) {
        return;
    }
    
    //in the middle vertical
    if (before && after) {
        before->after = after;
        after->before = before;
    }
    else if (before) {
        before->after = 0;
    }
    else if (after) {
        after->before = 0;
    }
    
    //in the middle horizontal
    if (left && right) {
        left->right = right;
        right->left = left;
    }
    else if (left) {
        left->right = 0;
    }
    else if (right) {
        right->left = 0;
    }
        
    before = right = after = left = back = 0;
}


void MenuItem::setValue(int v)
{
    if (v != value) {
        v = value;
        menuBackend->fireItemChangedEvent(this);
    }
}

int MenuItem::increment(int v)
{
    if (v != 0 && menuBackend) {
        value += v;
        menuBackend->fireItemChangedEvent(this);
    }
    return value;
}

int MenuItem::decrement(int v)
{
    if (v != 0 && menuBackend) {
        value -= v;
        menuBackend->fireItemChangedEvent(this);
    }
    return value;
}


void MenuBackend::use()
{
    //current->use();
    MenuUseEvent mue = { *current };
    menu->menuUse(mue);
}

void MenuBackend::moveToLevel(unsigned char level)
{
    MenuItem *cur = current;
    recursiveSearchForLevel(level, current);
    if (cur != current) {
        MenuChangeEvent mce = { *cur, *current };
        menu->menuChange(mce);
    }
}

void MenuBackend::fireItemChangedEvent(MenuItem *mi)
{
    MenuItemChangeEvent mice = { *mi };
    menu->menuItemChange(mice);
}

void MenuBackend::setCurrent(MenuItem *next)
{
    if (next) {
        MenuChangeEvent mce = { *current, *next };
        menu->menuChange(mce);
        current = next;
    }
}

