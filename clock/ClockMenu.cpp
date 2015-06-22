#include "ClockMenu.h"

//menu item titles
//for alarms
const char ClockMenuAlarm::titleActive[] PROGMEM = "Active";
const char ClockMenuAlarm::titleMusic[] PROGMEM = "Music";
const char ClockMenuAlarm::titleSongName[] PROGMEM = "Name";
const char ClockMenuAlarm::titleSongRandom[] PROGMEM = "Random";
const char ClockMenuAlarm::titleSongTime[] PROGMEM = "Max Time";
const char ClockMenuAlarm::titleHour[] PROGMEM = "Hour";
const char ClockMenuAlarm::titleMinute[] PROGMEM = "Minute";
const char ClockMenuAlarm::titleAmPm[] PROGMEM = "AM/PM";
const char ClockMenuAlarm::titleWhen[] PROGMEM = "When";
const char ClockMenuAlarm::titleWeekdays[] PROGMEM = "Weekday";
const char ClockMenuAlarm::titleRepeat[] PROGMEM = "Repeat";
const char ClockMenuAlarm::titleDate[] PROGMEM = "Date";
const char ClockMenuAlarm::titleYear[] PROGMEM = "Year";
const char ClockMenuAlarm::titleMonth[] PROGMEM = "Month";
const char ClockMenuAlarm::titleDay[] PROGMEM = "Day";
const char ClockMenuAlarm::titleSunday[] PROGMEM = "Sun";
const char ClockMenuAlarm::titleMonday[] PROGMEM = "Mon";
const char ClockMenuAlarm::titleTuesday[] PROGMEM = "Tue";
const char ClockMenuAlarm::titleWednesday[] PROGMEM = "Wed";
const char ClockMenuAlarm::titleThursday[] PROGMEM = "Thu";
const char ClockMenuAlarm::titleFriday[] PROGMEM = "Fri";
const char ClockMenuAlarm::titleSaturday[] PROGMEM = "Sat";
const char ClockMenuAlarm::titleRepeatTimes[] PROGMEM = "Times";
const char ClockMenuAlarm::titleRepeatInterval[] PROGMEM = "Interval";


const char ClockMenu::titleForecast[] PROGMEM = "";
const char ClockMenu::titleTimeAndDate[] PROGMEM = "Set Time";
const char ClockMenu::titleHourFormat[] PROGMEM = "12/24h";
const char ClockMenu::titleHour[] PROGMEM = "Hour";
const char ClockMenu::titleMinute[] PROGMEM = "Minute";
const char ClockMenu::titleAmPm[] PROGMEM = "AM/PM";
const char ClockMenu::titleDay[] PROGMEM = "Day";
const char ClockMenu::titleMonth[] PROGMEM = "Month";
const char ClockMenu::titleYear[] PROGMEM = "Year";
const char ClockMenu::titleTimezone[] PROGMEM = "TimeZone";
const char ClockMenu::titleScreen[] PROGMEM = "Screen";
const char ClockMenu::titleColor[] PROGMEM = "Color";
const char ClockMenu::titleBrightnessMin[] PROGMEM = "MinBright";
const char ClockMenu::titleBrightnessMax[] PROGMEM = "MaxBright";
const char ClockMenu::titleLightSensor[] PROGMEM = "Sensor";
const char ClockMenu::titleLightSensorEnabled[] PROGMEM = "Enabled";
const char ClockMenu::titleLightSensorCalibration[] PROGMEM = "Calibrate";
const char ClockMenu::titleInternet[] PROGMEM = "Internet";
const char ClockMenu::titleInternetEnabled[] PROGMEM = "Enabled";
const char ClockMenu::titleWifiSsid[] PROGMEM = "WiFi ID";
const char ClockMenu::titleWifiPassword[] PROGMEM = "Password";
const char ClockMenu::titleWeather[] PROGMEM = "Weather";
const char ClockMenu::titleWeatherEnabled[] PROGMEM = "Enabled";
const char ClockMenu::titleWeatherApiKey[] PROGMEM = "API Key";
const char ClockMenu::titleWeatherLocation[] PROGMEM = "Location";
const char ClockMenu::titleWeatherTemperatureScale[] PROGMEM = "Temperature";
const char ClockMenu::titleWeatherForecastHour[] PROGMEM = "Hour Tom";
const char ClockMenu::titleSync[] PROGMEM = "Sync SD";
const char ClockMenu::titleSyncServerName[] PROGMEM = "Server";
const char ClockMenu::titleSyncServerUrl[] PROGMEM = "URL";
const char ClockMenu::titleSyncStart[] PROGMEM = "Start";

const char ClockMenu::titleAlarms[] PROGMEM = "Alarms";
const char ClockMenu::titleAlarm1[] PROGMEM = "Alarm 1";
const char ClockMenu::titleAlarm2[] PROGMEM = "Alarm 2";
const char ClockMenu::titleAlarm3[] PROGMEM = "Alarm 3";
const char ClockMenu::titleAlarm4[] PROGMEM = "Alarm 4";
const char ClockMenu::titleAlarm5[] PROGMEM = "Alarm 5";

const char *ClockMenu::titleAlarmNames[5] = {titleAlarm1, titleAlarm2, titleAlarm3, titleAlarm4, titleAlarm5};


//up to MENU_VALUE_LENGTH characters
const char ClockMenu::stringHelp[]            = {'S','e','l','e','c','t',' ','a',' ','c','h','a','r','a','c','t','e','r',',',
    ' ','p','u','s','h',' ','E','n','t','e','r','.',' ','F','i','n','i','s','h',' ','b','y',' ',
    'e','n','t','e','r','i','n','g',' ','t','h','e',' ','s','y','m','b','o','l',' ', enterCharacter, '\0'};
const char ClockMenu::weatherApiKeyHelp[]   = "Get the API Key from http://www.worldweatheronline.com";
const char ClockMenu::weatherLocationHelp[] = "Enter your city or ZIP";
const char ClockMenu::weatherForecastHourHelp[] = "The hour after which the forecast is displayed for tomorrow";
const char ClockMenu::lightSensorHelp[]     = "Expose the light sensor to the brightest light and then the darkest";
const char ClockMenu::syncHelp[]     = "Synchronizing the SD card contents from the Internet web server";


/**
 * Constructor
 */
ClockMenu::ClockMenu(ClockConfig *clockConfig, ClockScreen *clockScreen, 
    ClockTime *clockTime, ClockMusic *clockMusic, ClockLightSensor *clockLightSensor, ClockWeather *clockWeather) 
    : config(clockConfig), screen(clockScreen), time(clockTime), music(clockMusic), lightSensor(clockLightSensor),
      weather(clockWeather)
{
    menuSetup();

    isAdjusting = isStringAdjusting = false;
    itemValue = 0;
    itemValueString = "";
    itemValueStringPosition = 0;

    buttonRepeat = repeatFirst;
}

const char ClockMenu::yesNo[2][4] = {"No", "Yes"};

bool ClockMenu::isActive(time_t currentTime)
{
    if (menu->getCurrent() == menu->getRoot()) {
        return false;
    }

    //if the menu wasn't used for the last 5 minutes, go to the root menu
    if (currentTime > (lastUseTime + 5 * SECS_PER_MIN)) {
#ifdef DEBUG
        Serial.println(F("Moved to the root menu element"));
#endif
        isAdjusting = isStringAdjusting = false;
        menu->moveToLevel(0);
    }

    return true;
}

void ClockMenu::goForecast()
{
    menu->moveToLevel(forecastLevel);
}

void ClockMenu::initializeInterrupt()
{
    //turn off interrupts while messing with their settings
    cli();
    
    //enable the pin change interrupt 1
    //the button pins PCINT8, PCINT9, PCINT11 are processed by the interrupt 1
    PCICR |= 0x02;
    //enables interrupts on the pins PCINT8, PCINT9, PCINT11
    PCMSK1 |= 0b00001011;
    
    //turn on interrupts back
    sei();    
}

void ClockMenu::menuSetup()
{
    //setting up the control buttons
    pinMode(BUTTON_PIN_UP, INPUT);
    pinMode(BUTTON_PIN_ENTER, INPUT);
    pinMode(BUTTON_PIN_DOWN, INPUT);

    buttonUp = new Button(BUTTON_PIN_UP, true, true, BUTTON_DEBOUNCE_MS); 
    buttonEnter = new Button(BUTTON_PIN_ENTER, true, true, BUTTON_DEBOUNCE_MS); 
    buttonDown = new Button(BUTTON_PIN_DOWN, true, true, BUTTON_DEBOUNCE_MS); 
    
    //init interrupts on the button pins
    initializeInterrupt();

#ifdef DEBUG
      Serial.println(F("Setting up the menu"));
#endif 

     /**
      * The main menu and submenus are parallel.
      * A user can move up and down each menu.
      * When she clicks on Enter and there is a submenu to the right, she can now move up and down in the submenu.
      * If the user moves up after the first item in the submenu, she'll be moved left to the main menu.
      * 
      * Main 1---
      * Main 2  |
      * Main 3  Submenu 1 --
      *         Submenu 2  |
      *         Submenu 3  Submenu 1
      */

    //this controls the menu backend and the event generation
    menu = new MenuBackend(this);

    //beneath is list of menu items needed to build the menu      
    //showing the forecast
    itemForecast            = new MenuItem(menu, titleForecast, FORECAST_LEVEL);

    //setting the clock time and date
    itemTimeAndDate         = new MenuItem(menu, titleTimeAndDate, 1);
    //the submenu with the elements of time
    itemHourFormat          = new MenuItem(menu, titleHourFormat, 2);
    itemHour                = new MenuItem(menu, titleHour, 2);
    itemMinute              = new MenuItem(menu, titleMinute, 2);
    itemAmPm                = new MenuItem(menu, titleAmPm, 2);

    itemYear                = new MenuItem(menu, titleYear, 2);
    itemMonth               = new MenuItem(menu, titleMonth, 2);
    itemDay                 = new MenuItem(menu, titleDay, 2);

    //the timezone
    itemTimezone            = new MenuItem(menu, titleTimezone, 1);

    //the screen settings
    itemScreen              = new MenuItem(menu, titleScreen, 1);
    //the colors
    itemColor               = new MenuItem(menu, titleColor, 2);
    //the screen brightness
    itemBrightnessMin       = new MenuItem(menu, titleBrightnessMin, 2);
    itemBrightnessMax       = new MenuItem(menu, titleBrightnessMax, 2);

    //the light sensor menu
    itemLightSensor         = new MenuItem(menu, titleLightSensor, 1);    
    itemLightSensorEnabled  = new MenuItem(menu, titleLightSensorEnabled, 2);
    itemLightSensorCalibration = new MenuItem(menu, titleLightSensorCalibration, 2);

    //the Internet setup
    itemInternet            = new MenuItem(menu, titleInternet, 1);
    itemInternetEnabled     = new MenuItem(menu, titleInternetEnabled, 2);
    //The Wi-Fi SSID and password
    itemWifiSsid            = new MenuItem(menu, titleWifiSsid, 2);
    itemWifiPassword        = new MenuItem(menu, titleWifiPassword, 2);

    //the Weather setup
    itemWeather             = new MenuItem(menu, titleWeather, 1);
    itemWeatherEnabled      = new MenuItem(menu, titleWeatherEnabled, 2);
    //The weather API key and location
    itemWeatherApiKey       = new MenuItem(menu, titleWeatherApiKey, 2);
    itemWeatherLocation     = new MenuItem(menu, titleWeatherLocation, 2);
    itemWeatherTemperatureScale = new MenuItem(menu, titleWeatherTemperatureScale, 2);
    itemWeatherForecastHour = new MenuItem(menu, titleWeatherForecastHour, 2);

    //the SD Synchonization
    itemSync                = new MenuItem(menu, titleSync, 1);
    //start the synchronization process
    itemSyncStart           = new MenuItem(menu, titleSyncStart, 2);
    //The Synchronization Server Name and URL
    itemSyncServerName      = new MenuItem(menu, titleSyncServerName, 2);
    itemSyncServerUrl       = new MenuItem(menu, titleSyncServerUrl, 2);    

    
    //the list of alarms    
    itemAlarms            = new MenuItem(menu, titleAlarms, 1);
      
      //init alarms
    for (int i = 0; i < NUMBER_ALARMS; i++) {
        menuAlarms[i].alarm          = new MenuItem(menu, titleAlarmNames[i], 2, i);
        menuAlarms[i].active         = new MenuItem(menu, ClockMenuAlarm::titleActive, 3, i);
        menuAlarms[i].hour           = new MenuItem(menu, ClockMenuAlarm::titleHour, 3, i);
        menuAlarms[i].minute         = new MenuItem(menu, ClockMenuAlarm::titleMinute, 3, i);
        menuAlarms[i].amPm           = new MenuItem(menu, ClockMenuAlarm::titleAmPm, 3, i);
        menuAlarms[i].when           = new MenuItem(menu, ClockMenuAlarm::titleWhen, 3, i);
        menuAlarms[i].music          = new MenuItem(menu, ClockMenuAlarm::titleMusic, 3, i);
        menuAlarms[i].songName       = new MenuItem(menu, ClockMenuAlarm::titleSongName, 4, i);
        menuAlarms[i].songRandom     = new MenuItem(menu, ClockMenuAlarm::titleSongRandom, 4, i);
        menuAlarms[i].songTime       = new MenuItem(menu, ClockMenuAlarm::titleSongTime, 4, i);
        menuAlarms[i].weekdays       = new MenuItem(menu, ClockMenuAlarm::titleWeekdays, 3, i);
        menuAlarms[i].repeat         = new MenuItem(menu, ClockMenuAlarm::titleRepeat, 3, i);
        menuAlarms[i].date           = new MenuItem(menu, ClockMenuAlarm::titleDate, 4, i);

        //the repeat
        menuAlarms[i].repeatTimes    = new MenuItem(menu, ClockMenuAlarm::titleRepeatTimes, 4, i);
        menuAlarms[i].repeatInterval = new MenuItem(menu, ClockMenuAlarm::titleRepeatInterval, 4, i);
        
        //the alarm date    
        menuAlarms[i].year           = new MenuItem(menu, ClockMenuAlarm::titleYear, 5, i);
        menuAlarms[i].month          = new MenuItem(menu, ClockMenuAlarm::titleMonth, 5, i);
        menuAlarms[i].day            = new MenuItem(menu, ClockMenuAlarm::titleDay, 5, i);
        
        //the alarm weekdays    
        menuAlarms[i].sunday         = new MenuItem(menu, ClockMenuAlarm::titleSunday, 4, i);
        menuAlarms[i].monday         = new MenuItem(menu, ClockMenuAlarm::titleMonday, 4, i);
        menuAlarms[i].tuesday        = new MenuItem(menu, ClockMenuAlarm::titleTuesday, 4, i);
        menuAlarms[i].wednesday      = new MenuItem(menu, ClockMenuAlarm::titleWednesday, 4, i);
        menuAlarms[i].thursday       = new MenuItem(menu, ClockMenuAlarm::titleThursday, 4, i);
        menuAlarms[i].friday         = new MenuItem(menu, ClockMenuAlarm::titleFriday, 4, i);
        menuAlarms[i].saturday       = new MenuItem(menu, ClockMenuAlarm::titleSaturday, 4, i);
    }
  
    //add items into the main menu
    menu->getRoot()
        ->add(itemForecast)
        ->add(itemTimeAndDate)
        ->add(itemAlarms)
        ->add(itemScreen)
        ->add(itemLightSensor)
        ->add(itemInternet)
        ->add(itemWeather)
        ->add(itemSync);
      
    //the date & time setting menu
    itemTimeAndDate->addRight(itemHourFormat)
        ->add(itemHour)
        ->add(itemMinute)
        ->add(itemYear)
        ->add(itemMonth)
        ->add(itemDay)
        ->add(itemTimezone);
    //loop back if left on time & date
    itemHourFormat->addLeft(itemTimeAndDate);

    //the screen settings menu
    itemScreen->addRight(itemColor)
        ->add(itemBrightnessMin)
        ->add(itemBrightnessMax);
    itemColor->addLeft(itemScreen);

    //the light sensor menu
    itemLightSensor->addRight(itemLightSensorEnabled)
        ->add(itemLightSensorCalibration);
    itemLightSensorEnabled->addLeft(itemLightSensor);

    //the Internet menu
    itemInternet->addRight(itemInternetEnabled)
        ->add(itemWifiSsid)
        ->add(itemWifiPassword);
    itemInternetEnabled->addLeft(itemInternet);

    //the Weather menu
    itemWeather->addRight(itemWeatherEnabled)
        ->add(itemWeatherApiKey)
        ->add(itemWeatherLocation)
        ->add(itemWeatherTemperatureScale)
        ->add(itemWeatherForecastHour);
    itemWeatherEnabled->addLeft(itemWeather);

    //the SD Synchronization menu
    itemSync->addRight(itemSyncStart)
        ->add(itemSyncServerName)
        ->add(itemSyncServerUrl);
    itemSyncStart->addLeft(itemSync);

    //alarms
    if (NUMBER_ALARMS > 0) {
        //add the first alarm in the "Alarms" menu
        itemAlarms->addRight(menuAlarms[0].alarm);
        menuAlarms[0].alarm->addLeft(itemAlarms);

        for (int i = 0; i < NUMBER_ALARMS; i++) {
            //add the second, etc alarms into the "Alarms" menu
            if (i > 0) {
                menuAlarms[i - 1].alarm->add(menuAlarms[i].alarm);
            }

            //the main menu for the alarms
            menuAlarms[i].alarm->addRight(menuAlarms[i].active)
                ->add(menuAlarms[i].hour)
                ->add(menuAlarms[i].minute)
                ->add(menuAlarms[i].when)
                ->add(menuAlarms[i].music)
                ->add(menuAlarms[i].date)
                ->add(menuAlarms[i].weekdays)
                ->add(menuAlarms[i].repeat);
            menuAlarms[i].active->addLeft(menuAlarms[i].alarm);

            //the alarm date submenu
            menuAlarms[i].date->addRight(menuAlarms[i].year)
                ->add(menuAlarms[i].month)
                ->add(menuAlarms[i].day);
            menuAlarms[i].year->addLeft(menuAlarms[i].date);

            //the repeat submenu
            menuAlarms[i].repeat->addRight(menuAlarms[i].repeatTimes)
                ->add(menuAlarms[i].repeatInterval);
            menuAlarms[i].repeatTimes->addLeft(menuAlarms[i].repeat);

            //the music submenu
            menuAlarms[i].music->addRight(menuAlarms[i].songName)
                ->add(menuAlarms[i].songRandom)
                ->add(menuAlarms[i].songTime);
            menuAlarms[i].songName->addLeft(menuAlarms[i].music);

            //the alarm weekdays
            menuAlarms[i].weekdays->addRight(menuAlarms[i].monday)
                ->add(menuAlarms[i].tuesday)
                ->add(menuAlarms[i].wednesday)
                ->add(menuAlarms[i].thursday)
                ->add(menuAlarms[i].friday)
                ->add(menuAlarms[i].saturday)
                ->add(menuAlarms[i].sunday);
            menuAlarms[i].monday->addLeft(menuAlarms[i].weekdays);    
        }
    }

    //add the AM/PM item for the 12-hour format
    addItemAmPm();

}

void ClockMenu::addItemAmPm()
{
    //if the 24-hour format is used, delete the AM/PM item from the menu
    if (config->getHourFormat() == 0) {
        itemAmPm->deleteFromMenu();
    }    
    //if it doesn't exist in the menu, add it
    else if ((config->getHourFormat() == 1) & !itemAmPm->hasChildren()) {
        itemMinute->add(itemAmPm);
        itemYear->addBefore(itemAmPm);
    }

    for (int i = 0; i < NUMBER_ALARMS; i++) {
        if (config->getHourFormat() == 0) {
            menuAlarms[i].amPm->deleteFromMenu();
        }
        //if it doesn't exist in the menu, add it
        else if ((config->getHourFormat() == 1) & !menuAlarms[i].amPm->hasChildren()) {
            menuAlarms[i].minute->add(menuAlarms[i].amPm);
            menuAlarms[i].when->addBefore(menuAlarms[i].amPm);
        }
    }

}

void ClockMenu::menuUse(MenuUseEvent used)
{

#ifdef DEBUG
    Serial.println(F("The menu use event"));
#endif

    //if the itemValue was changed
    bool changed = false;

    //the text value for display
    const byte textLength = MENU_VALUE_LENGTH + 1;
    char itemValueText[textLength];
    memset(itemValueText, 0, textLength);

    //setting the hour format: 12 or 24
    if (used.item == itemHourFormat) {
        if (isAdjusting) {
            config->setHourFormat(itemValue)
                ->saveConfig();
            //add the AM/PM menu
            addItemAmPm();
        }
        else {
            itemValue = config->getHourFormat();
        }
        config->getHourFormatText(itemValueText, itemValue);
        changed = true;
    }
    //setting the hour
    else if (used.item == itemHour) {
        if (isAdjusting) {
            time->setHour(itemValue);
        }
        else {
            itemValue = time->getHour();
        }
        changed = true;
    }
    //setting the minute
    else if (used.item == itemMinute) {
        if (isAdjusting) {
            time->setMinute(itemValue);
        }
        else {
            itemValue = time->getMinute();
        }
        changed = true;
    }
    //setting the AM/PM
    else if (used.item == itemAmPm) {
        if (isAdjusting) {
            time->setAmPm(itemValue);
        }
        else {
            itemValue = time->getAmPm();
        }
        time->getAmPmText(itemValueText, itemValue);
        changed = true;
    }
    //setting the year
    else if (used.item == itemYear) {
        if (isAdjusting) {
            time->setYear(itemValue);
        }
        else {
            itemValue = time->getYear();
        }
        changed = true;
    }
    //setting the month
    else if (used.item == itemMonth) {
        if (isAdjusting) {
            time->setMonth(itemValue);
        }
        else {
            itemValue = time->getMonth();
        }
        strncpy(itemValueText, ClockTime::getMonthName(itemValue), textLength);
        changed = true;
    }
    //setting the day
    else if (used.item == itemDay) {
        if (isAdjusting) {
            time->setDay(itemValue);
        }
        else {
            itemValue = time->getDay();
        }
        changed = true;
    }
    //setting the screen color
    else if (used.item == itemColor) {
        if (isAdjusting) {
            config->setColor(ClockScreen::getColorIdByNumber(itemValue))
                ->saveConfig();
        }
        else {
            itemValue = ClockScreen::getColorNumberById(config->getColor());
        }
        strncpy(itemValueText, ClockScreen::getColorNameByNumber(itemValue), textLength);
        changed = true;
        //the color was changed, redraw the title using the new color
        screen->showMenuTitle(used.item.getName());
    }
    //setting the minimum screen brightness
    else if (used.item == itemBrightnessMin) {
        if (isAdjusting) {
            config->setBrightnessMin(itemValue)
                ->saveConfig();
        }
        else {
            itemValue = config->getBrightnessMin();
        }
        changed = true;
        //the brighness was changed, redraw the title using the new brightness
        screen->showMenuTitle(used.item.getName());
    }
    //setting the maximum screen brightness
    else if (used.item == itemBrightnessMax) {
        if (isAdjusting) {
            config->setBrightnessMax(itemValue)
                ->saveConfig();
        }
        else {
            itemValue = config->getBrightnessMax();
        }
        changed = true;
        //the brighness was changed, redraw the title using the new brightness
        screen->showMenuTitle(used.item.getName());
    }
    //setting if the light sensor is enabled or not
    else if (used.item == itemLightSensorEnabled) {
        if (isAdjusting) {
            config->setLightSensorEnabled(itemValue)
                ->saveConfig();
        }
        else {
            itemValue = config->getLightSensorEnabled();
        }
        strncpy(itemValueText, getYesNo(itemValue), textLength);
        changed = true;
        //the brighness might have changed because of the sensor, redraw the title using the new brightness
        screen->showMenuTitle(used.item.getName());
    }
    //setting the light sensor minimum and maximum calibration values
    else if (used.item == itemLightSensorCalibration) {
        //show the help
        showHelp(itemValueText, lightSensorHelp);

        isAdjusting = true;
        lightSensor->calibrate(screen);
        snprintf(itemValueText, textLength, "%d/%d", config->getLightSensorMin(), config->getLightSensorMax());

        changed = true;
        //the brighness was changed, redraw the title using the new brightness
        screen->showMenuTitle(used.item.getName());
    }
    //setting the timezone
    else if (used.item == itemTimezone) {
        ClockTimezone zone;

        if (isAdjusting) {
            zone.setTimezoneIntoConfig(config, itemValue);
            config->setTimezoneNumber(itemValue)
                ->saveConfig();
            //reset the new timezone into the time object
            time->setTimezone();
        }
        else {
            itemValue = config->getTimezoneNumber();
        }
        changed = true;

        zone.getTimezoneNameByNumber(itemValueText, MENU_VALUE_LENGTH, itemValue);
    }
    //setting if the Internet is enabled or not
    else if (used.item == itemInternetEnabled) {
        if (isAdjusting) {
            config->setInternetEnabled(itemValue)
                ->saveConfig();
        }
        else {
            itemValue = config->getInternetEnabled();
        }
        strncpy(itemValueText, getYesNo(itemValue), textLength);
        changed = true;
    }
    //setting the Wi-Fi SSID
    else if (used.item == itemWifiSsid) {
        if (isAdjusting) {        
            config->setWifiSsid(itemValueString)
                ->saveConfig();
            //no character editing
            itemValueStringPosition = -1;
        }
        else {
            showHelp(itemValueText, stringHelp);

            //sets the string adjusting mode
            isStringAdjusting = true;

            //set the initial value
            itemValueString = "a";
            itemValueStringPosition = 0;
        }
        changed = true;
    }
    //setting the Wi-Fi Password
    else if (used.item == itemWifiPassword) {
        if (isAdjusting) {
            config->setWifiPassword(itemValueString)
                ->saveConfig();
            itemValueStringPosition = -1;
        }
        else {
            showHelp(itemValueText, stringHelp);

            isStringAdjusting = true;
            itemValueString = "a";
            itemValueStringPosition = 0;
        }
        changed = true;
    }
    //setting if the Weather Feed is enabled or not
    else if (used.item == itemWeatherEnabled) {
        if (isAdjusting) {
            config->setWeatherEnabled(itemValue)
                ->saveConfig();
        }
        else {
            itemValue = config->getWeatherEnabled();
        }
        strncpy(itemValueText, getYesNo(itemValue), textLength);
        changed = true;
    }
    //setting the Weather API Key
    else if (used.item == itemWeatherApiKey) {
        if (isAdjusting) {
            config->setWeatherApiKey(itemValueString)
                ->saveConfig();
            itemValueStringPosition = -1;
        }
        else {
            //show the help
            showHelp(itemValueText, stringHelp);
            showHelp(itemValueText, weatherApiKeyHelp);

            isStringAdjusting = true;

            itemValueString = "a";
            itemValueStringPosition = 0;
        }
        changed = true;
    }
    //setting the Weather Location
    else if (used.item == itemWeatherLocation) {
        if (isAdjusting) {
            config->setWeatherLocation(itemValueString)
                ->saveConfig();
            itemValueStringPosition = -1;
        }
        else {
            //show the help
            showHelp(itemValueText, stringHelp);
            showHelp(itemValueText, weatherLocationHelp);

            isStringAdjusting = true;

            itemValueString = "a";
            itemValueStringPosition = 0;
        }
        changed = true;
    }
    //setting the temperature scale
    else if (used.item == itemWeatherTemperatureScale) {
        if (isAdjusting) {
            config->setWeatherTemperatureScale(itemValue)
                ->saveConfig();
        }
        else {
            itemValue = config->getWeatherTemperatureScale();
        }
        strncpy(itemValueText, ClockWeather::getTemperatureScaleName(itemValue), textLength);
        changed = true;
    }
    //setting the weather forecast switch hour
    else if (used.item == itemWeatherForecastHour) {
        if (isAdjusting) {
            config->setWeatherForecastHour(itemValue)
                ->saveConfig();
        }
        else {
            showHelp(itemValueText, weatherForecastHourHelp);
            itemValue = config->getWeatherForecastHour();
        }
        changed = true;
    }
    //setting the SD Synchronization server name
    else if (used.item == itemSyncServerName) {
        if (isAdjusting) {        
            config->setSyncServerName(itemValueString)
                ->saveConfig();
            //no character editing
            itemValueStringPosition = -1;
        }
        else {
            showHelp(itemValueText, stringHelp);

            //sets the string adjusting mode
            isStringAdjusting = true;

            //set the initial value
            itemValueString = "a";
            itemValueStringPosition = 0;
        }
        changed = true;
    }
    //setting the SD Synchronization server URL
    else if (used.item == itemSyncServerUrl) {
        if (isAdjusting) {
            config->setSyncServerUrl(itemValueString)
                ->saveConfig();
            //no character editing
            itemValueStringPosition = -1;
        }
        else {
            showHelp(itemValueText, stringHelp);

            //sets the string adjusting mode
            isStringAdjusting = true;

            //set the initial value
            itemValueString = "a";
            itemValueStringPosition = 0;
        }
        changed = true;
    }
    //synchronizing the SD card from the server
    else if (used.item == itemSyncStart) {
        //show the help
        showHelp(itemValueText, syncHelp);

        //run the sync process
        ClockSync sync(config, music, screen);
        sync.update();

        changed = false;
    }


    //check if the alarm settings were adjusted
    for (int i = 0; i < NUMBER_ALARMS; i++) {
        bool alarmChanged = false;
        //skip if the event was already found
        if (changed) {
            break;
        }

        //setting if the alarm is active
        if (used.item == menuAlarms[i].active) {
            if (isAdjusting) {
                config->alarm[i].setActive(itemValue);
                alarmChanged = true;

                //in case the alarm is enabled for the event tomorrow
                if (itemValue && (config->alarm[i].getWhen() == ALARM_WHEN_TOMORROW)) {
                    unsigned int year;
                    byte month, day;
                    time->getTomorrow(year, month, day);
                    config->alarm[i].setYear(year)->setMonth(month)->setDay(day);
                }
            }
            else {
                itemValue = config->alarm[i].getActive();
            }
            strncpy(itemValueText, getYesNo(itemValue), textLength);
            changed = true;
        }
        //setting the hour of the alarm
        else if (used.item == menuAlarms[i].hour) {
            if (isAdjusting) {
                config->alarm[i].setHour(itemValue);
                alarmChanged = true;
            }
            else {
                itemValue = config->alarm[i].getHour(config->getHourFormat());
            }
            changed = true;
        }
        //setting the minute of the alarm
        else if (used.item == menuAlarms[i].minute) {
            if (isAdjusting) {
                config->alarm[i].setMinute(itemValue);
                alarmChanged = true;
            }
            else {
                itemValue = config->alarm[i].getMinute();
            }        
            changed = true;
        }
        //setting the AM/PM of the alarm
        else if (used.item == menuAlarms[i].amPm) {
            if (isAdjusting) {
                config->alarm[i].setAmPm(itemValue);
                alarmChanged = true;
            }
            else {
                itemValue = config->alarm[i].getAmPm();
            }
            time->getAmPmText(itemValueText, itemValue);
            changed = true;
        }
        //setting the year of the alarm
        else if (used.item == menuAlarms[i].year) {
            if (isAdjusting) {
                config->alarm[i].setYear(itemValue);
                alarmChanged = true;
            }
            else {
                itemValue = config->alarm[i].getYear();
            }
            changed = true;
        }
        //setting the month of the alarm
        else if (used.item == menuAlarms[i].month) {
            if (isAdjusting) {
                config->alarm[i].setMonth(itemValue);
                alarmChanged = true;
            }
            else {
                itemValue = config->alarm[i].getMonth();
            }
            changed = true;
        }
        //setting the day of the alarm
        else if (used.item == menuAlarms[i].day) {
            if (isAdjusting) {
                config->alarm[i].setDay(itemValue);
                alarmChanged = true;
            }
            else {
                itemValue = config->alarm[i].getDay();
            }
            changed = true;
        }
        //setting When the alarm will be activated: each day, once, a date
        else if (used.item == menuAlarms[i].when) {
            if (isAdjusting) {
                config->alarm[i].setWhen(itemValue);
                //when once, set the date as tomorrow
                if (itemValue == ALARM_WHEN_TOMORROW) {
                    unsigned int year;
                    byte month, day;
                    time->getTomorrow(year, month, day);
                    config->alarm[i].setYear(year)->setMonth(month)->setDay(day);
                }
                alarmChanged = true;
            }
            else {
                itemValue = config->alarm[i].getWhen();
            }
            strncpy(itemValueText, ClockAlarm::getWhenName(itemValue), textLength);
            changed = true;
        }

        //setting the song name of the alarm
        else if (used.item == menuAlarms[i].songName) {
            if (isAdjusting) {
                config->alarm[i].setSongName(music->getSongNameByNumber(itemValue));
                alarmChanged = true;
            }
            else {
                itemValue = music->getSongNumberByName(config->alarm[i].getSongName());
            }
            strncpy(itemValueText, config->alarm[i].getSongName(), textLength);

            //play the song
            //music->playSong(config->alarm[i].getSongName(), config->alarm[i].getSongTime());

            changed = true;
        }
        //setting if the song name is random
        else if (used.item == menuAlarms[i].songRandom) {
            if (isAdjusting) {
                config->alarm[i].setSongRandom(itemValue);
                alarmChanged = true;
            }
            else {
                itemValue = config->alarm[i].getSongRandom();
            }
            strncpy(itemValueText, getYesNo(itemValue), textLength);
            changed = true;
        }
        //setting the maximum playing time of the song
        else if (used.item == menuAlarms[i].songTime) {
            if (isAdjusting) {
                config->alarm[i].setSongTime(itemValue);
                alarmChanged = true;
            }
            else {
                itemValue = config->alarm[i].getSongTime();
            }
            changed = true;
        }
        //setting the repeat times of the alarm
        else if (used.item == menuAlarms[i].repeatTimes) {
            if (isAdjusting) {
                config->alarm[i].setRepeatTimes(itemValue);
                alarmChanged = true;
            }
            else {
                itemValue = config->alarm[i].getRepeatTimes();
            }
            changed = true;
        }
        //setting the repeat interval of the alarm
        else if (used.item == menuAlarms[i].repeatInterval) {
            if (isAdjusting) {
                config->alarm[i].setRepeatInterval(itemValue);
                alarmChanged = true;
            }
            else {
                itemValue = config->alarm[i].getRepeatInterval();
            }
            changed = true;
        }
        //setting if the alarm is active on Monday
        else if (used.item == menuAlarms[i].monday) {
            if (isAdjusting) {
                config->alarm[i].setMonday(itemValue);
                alarmChanged = true;
            }
            else {
                itemValue = config->alarm[i].getMonday();
            }
            strncpy(itemValueText, getYesNo(itemValue), textLength);
            changed = true;
        }
        //setting if the alarm is active on Tuesday
        else if (used.item == menuAlarms[i].tuesday) {
            if (isAdjusting) {
                config->alarm[i].setTuesday(itemValue);
                alarmChanged = true;
            }
            else {
                itemValue = config->alarm[i].getTuesday();
            }
            strncpy(itemValueText, getYesNo(itemValue), textLength);
            changed = true;
        }
        //setting if the alarm is active on Wednesday
        else if (used.item == menuAlarms[i].wednesday) {
            if (isAdjusting) {
                config->alarm[i].setWednesday(itemValue);
                alarmChanged = true;
            }
            else {
                itemValue = config->alarm[i].getWednesday();
            }
            strncpy(itemValueText, getYesNo(itemValue), textLength);
            changed = true;
        }
        //setting if the alarm is active on Thursday
        else if (used.item == menuAlarms[i].thursday) {
            if (isAdjusting) {
                config->alarm[i].setThursday(itemValue);
                alarmChanged = true;
            }
            else {
                itemValue = config->alarm[i].getThursday();
            }
            strncpy(itemValueText, getYesNo(itemValue), textLength);
            changed = true;
        }
        //setting if the alarm is active on Friday
        else if (used.item == menuAlarms[i].friday) {
            if (isAdjusting) {
                config->alarm[i].setFriday(itemValue);
                alarmChanged = true;
            }
            else {
                itemValue = config->alarm[i].getFriday();
            }
            strncpy(itemValueText, getYesNo(itemValue), textLength);
            changed = true;
        }
        //setting if the alarm is active on Saturday
        else if (used.item == menuAlarms[i].saturday) {
            if (isAdjusting) {
                config->alarm[i].setSaturday(itemValue);
                alarmChanged = true;
            }
            else {
                itemValue = config->alarm[i].getSaturday();
            }
            strncpy(itemValueText, getYesNo(itemValue), textLength);
            changed = true;
        }
        //setting if the alarm is active on Sunday
        else if (used.item == menuAlarms[i].sunday) {
            if (isAdjusting) {
                config->alarm[i].setSunday(itemValue);
                alarmChanged = true;
            }
            else {
                itemValue = config->alarm[i].getSunday();
            }
            strncpy(itemValueText, getYesNo(itemValue), textLength);
            changed = true;
        }

        //save the config in EEPROM
        if (alarmChanged) {
            config->saveConfig();
            //recalculate the next alarm time
            time->calculateNextAlarm();
        }
    }

    if (changed) {
        toggleAdjusting();
        if (strlen(itemValueText)) {
            screen->showMenuValue(itemValueText, isAdjusting);
        }
        else if (itemValueString && (itemValueString.length() > 0)) {
            screen->showMenuValueString(itemValueString, true, itemValueStringPosition);
        }
        else {
            screen->showMenuValue(itemValue, isAdjusting);
        }
    }

    //clear the item value string
    if (!isAdjusting) {
        itemValueString = "";
        itemValueStringPosition = 0;
    }
}

void ClockMenu::menuChange(MenuChangeEvent changed)
{

#ifdef DEBUG
    Serial.println(F("The menu change event."));
#endif

    //the text value for display
    const byte textLength = MENU_VALUE_LENGTH + 1;
    char itemValueText[textLength];
    memset(itemValueText, 0, textLength);

    //clear the string value
    itemValueString = "";

    //show the title of the selected item
    screen->showMenuTitle(changed.to.getName());

    //the root element
    if (changed.to == menu->getRoot()) {
        screen->showMenuTitle("", false);
        screen->showMenuValue("", false);
    }
    //showing the full forecast
    else if (changed.to == itemForecast) {
        screen->showMenuTitle("", false);
        screen->showMenuValue("", false);
        screen->showWeather(weather, time->getCurrentTime(), true);
    }
    //showing the current clock hour format
    else if (changed.to == itemHourFormat) {
        config->getHourFormatText(itemValueText, config->getHourFormat());
        screen->showMenuValue(itemValueText, false);
    }
    //showing the current clock hour
    else if (changed.to == itemHour) {
        screen->showMenuValue(time->getHour(), false);
    }
    //showing the current clock minute
    else if (changed.to == itemMinute) {
        screen->showMenuValue(time->getMinute(), false);
    }
    //showing the current clock AM/PM
    else if (changed.to == itemAmPm) {
        time->getAmPmText(itemValueText, time->getAmPm());
        screen->showMenuValue(itemValueText, false);
    }
    //showing the current clock year
    else if (changed.to == itemYear) {
        screen->showMenuValue(time->getYear(), false);
    }
    //showing the current clock month
    else if (changed.to == itemMonth) {
        screen->showMenuValue(ClockTime::getMonthName(time->getMonth()), false);
    }
    //showing the current clock day
    else if (changed.to == itemDay) {
        screen->showMenuValue(time->getDay(), false);
    }
    //showing the screen color
    else if (changed.to == itemColor) {
        screen->showMenuValue(ClockScreen::getColorNameByNumber(
            ClockScreen::getColorNumberById(config->getColor())), false);
    }
    //showing the minimum screen brightness
    else if (changed.to == itemBrightnessMin) {
        screen->showMenuValue(config->getBrightnessMin(), false);
    }
    //showing the maximum screen brightness
    else if (changed.to == itemBrightnessMax) {
        screen->showMenuValue(config->getBrightnessMax(), false);
    }
    //showing if the light sensor enabled
    else if (changed.to == itemLightSensorEnabled) {
        screen->showMenuValue(getYesNo(config->getLightSensorEnabled()), false);
    }
    //showing the light sensor minimum and maximum values
    else if (changed.to == itemLightSensorCalibration) {
        snprintf(itemValueText, textLength, "%d/%d", config->getLightSensorMin(), config->getLightSensorMax());
        screen->showMenuValue(itemValueText, false);
    }
    //showing the timezone
    else if (changed.to == itemTimezone) {
        ClockTimezone zone;
        zone.getTimezoneNameByNumber(itemValueText, MENU_VALUE_LENGTH, config->getTimezoneNumber());
        screen->showMenuValueScrolling(itemValueText, false);
    }
    //showing if the Internet is enabled
    else if (changed.to == itemInternetEnabled) {
        screen->showMenuValue(getYesNo(config->getInternetEnabled()), false);
    }
    //showing the Wi-Fi SSID
    else if (changed.to == itemWifiSsid) {
        screen->showMenuValue(config->getWifiSsid(), false);
    }
    //showing the Wi-Fi password
    else if (changed.to == itemWifiPassword) {
        //hide the password
        screen->showMenuValue("+++", false);
    }
    //showing if the Weather Feed is enabled
    else if (changed.to == itemWeatherEnabled) {
        screen->showMenuValue(getYesNo(config->getWeatherEnabled()), false);
    }
    //showing the Weather API Key
    else if (changed.to == itemWeatherApiKey) {
        screen->showMenuValue(config->getWeatherApiKey(), false);
    }
    //showing the Weather Location
    else if (changed.to == itemWeatherLocation) {
        screen->showMenuValue(config->getWeatherLocation(), false);
    }    
    //showing the weather temperature scale
    else if (changed.to == itemWeatherTemperatureScale) {
        screen->showMenuValue(ClockWeather::getTemperatureScaleName(config->getWeatherTemperatureScale()), false);    
    }
    //showing the Weather forecast switch hour
    else if (changed.to == itemWeatherForecastHour) {
        screen->showMenuValue(config->getWeatherForecastHour(), false);
    }    
    //showing the Sync Server Name
    else if (changed.to == itemSyncServerName) {
        screen->showMenuValue(config->getSyncServerName(), false);
    }
    //showing the Sync Server Url
    else if (changed.to == itemSyncServerUrl) {
        screen->showMenuValue(config->getSyncServerUrl(), false);
    }

    
    //show the alarm settings
    for (int i = 0; i < NUMBER_ALARMS; i++) {
        //showing the alarm
        if (changed.to == menuAlarms[i].alarm) {
              sprintf(itemValueText, "%d:%02d", config->alarm[i].getHour(config->getHourFormat()), 
                  config->alarm[i].getMinute()); 
            screen->showMenuValue(itemValueText, false);
        }
        //showing if the alarm is active
        else if (changed.to == menuAlarms[i].active) {
            screen->showMenuValue(getYesNo(config->alarm[i].getActive()), false);
        }
        //showing the hour of the alarm
        else if (changed.to == menuAlarms[i].hour) {
            screen->showMenuValue(config->alarm[i].getHour(config->getHourFormat()), false);
        }
        //showing the minute of the alarm
        else if (changed.to == menuAlarms[i].minute) {
            screen->showMenuValue(config->alarm[i].getMinute(), false);
        }
        //showing the AM/PM of the alarm
        else if (changed.to == menuAlarms[i].amPm) {
            time->getAmPmText(itemValueText, config->alarm[i].getAmPm());
            screen->showMenuValue(itemValueText, false);
        }
        //showing the date of the alarm
        else if (changed.to == menuAlarms[i].date) {
              sprintf(itemValueText, "%d %s", config->alarm[i].getDay(), 
                  ClockTime::getMonthName(config->alarm[i].getMonth()));
            screen->showMenuValue(itemValueText, false);
        }
        //showing the year of the alarm
        else if (changed.to == menuAlarms[i].year) {
            screen->showMenuValue(config->alarm[i].getYear(), false);
        }
        //showing the month of the alarm
        else if (changed.to == menuAlarms[i].month) {
            screen->showMenuValue(config->alarm[i].getMonth(), false);
        }
        //showing the day of the alarm
        else if (changed.to == menuAlarms[i].day) {
            screen->showMenuValue(config->alarm[i].getDay(), false);
        }
        //showing the song name of the alarm
        else if (changed.to == menuAlarms[i].songName) {
            screen->showMenuValue(config->alarm[i].getSongName(), false);
        }
        //showing the song name of the alarm
        else if (changed.to == menuAlarms[i].songRandom) {
            screen->showMenuValue(getYesNo(config->alarm[i].getSongRandom()), false);
        }
        //showing the maximum song time 
        else if (changed.to == menuAlarms[i].songTime) {
              sprintf(itemValueText, "%d secs", config->alarm[i].getSongTime());
            screen->showMenuValue(itemValueText, false);
        }
        //showing the repeat menu
        else if (changed.to == menuAlarms[i].repeat) {
              sprintf(itemValueText, "%d times", config->alarm[i].getRepeatTimes());
            screen->showMenuValue(itemValueText, false);
        }
        //showing the repeat times of the alarm
        else if (changed.to == menuAlarms[i].repeatTimes) {
            screen->showMenuValue(config->alarm[i].getRepeatTimes(), false);
        }
        //showing the repeat interval of the alarm
        else if (changed.to == menuAlarms[i].repeatInterval) {
            screen->showMenuValue(config->alarm[i].getRepeatInterval(), false);
        }
        //showing when the alarm    
        else if (changed.to == menuAlarms[i].when) {
            screen->showMenuValue(ClockAlarm::getWhenName(config->alarm[i].getWhen()), false);
        }
        //showing when the alarm will be activate
        else if (changed.to == menuAlarms[i].minute) {
            screen->showMenuValue(ClockAlarm::getWhenName(config->alarm[i].getWhen()), false);
        }
        //showing if the alarm is active of week days
        else if (changed.to == menuAlarms[i].weekdays) {
            config->alarm[i].getWeekdays(itemValueText);
            screen->showMenuValue(itemValueText, false);
        }
        //showing if the alarm is active on Monday
        else if (changed.to == menuAlarms[i].monday) {
            screen->showMenuValue(getYesNo(config->alarm[i].getMonday()), false);
        }
        //showing if the alarm is active on Tuesday
        else if (changed.to == menuAlarms[i].tuesday) {
            screen->showMenuValue(getYesNo(config->alarm[i].getTuesday()), false);
        }
        //showing if the alarm is active on Wednesday
        else if (changed.to == menuAlarms[i].wednesday) {
            screen->showMenuValue(getYesNo(config->alarm[i].getWednesday()), false);
        }
        //showing if the alarm is active on Thursday
        else if (changed.to == menuAlarms[i].thursday) {
            screen->showMenuValue(getYesNo(config->alarm[i].getThursday()), false);
        }
        //showing if the alarm is active on Friday
        else if (changed.to == menuAlarms[i].friday) {
            screen->showMenuValue(getYesNo(config->alarm[i].getFriday()), false);
        }
        //showing if the alarm is active on Saturday
        else if (changed.to == menuAlarms[i].saturday) {
            screen->showMenuValue(getYesNo(config->alarm[i].getSaturday()), false);
        }
        //showing if the alarm is active on Sunday
        else if (changed.to == menuAlarms[i].sunday) {
            screen->showMenuValue(getYesNo(config->alarm[i].getSunday()), false);
        }

    }

}

void ClockMenu::itemValueChange(MenuItem *currentItem, bool upPressed, bool downPressed)
{
      //the text of the integer value for display
      const byte textLength = MENU_VALUE_LENGTH + 1;
      char itemValueText[textLength];
      memset(itemValueText, 0, textLength);

    //setting the clock hour format
    if (currentItem == itemHourFormat) {
        setItemValue(upPressed, downPressed, 0, 1, true);
        config->getHourFormatText(itemValueText, itemValue);
        screen->showMenuValue(itemValueText, true);
    }
    //setting the clock hour
    else if (currentItem == itemHour) {
        //the maximum value depends on 12 or 24 hour format
        setItemValue(upPressed, downPressed, 
            (config->getHourFormat() == 0 ? 0 : 1), 
            (config->getHourFormat() == 0 ? 23 : 12), 
            true
        );
        screen->showMenuValue(itemValue, true);
    }
    //setting the clock minute
    else if (currentItem == itemMinute) {
        setItemValue(upPressed, downPressed, 0, 59, true);
        screen->showMenuValue(itemValue, true);    
    }
    //setting the clock AM/PM
    else if (currentItem == itemAmPm) {
        setItemValue(upPressed, downPressed, 0, 1, true);
        time->getAmPmText(itemValueText, itemValue);
        screen->showMenuValue(itemValueText, true);
    }
    //setting the clock year
    else if (currentItem == itemYear) {
        setItemValue(upPressed, downPressed, 2013, 2112, false);
        screen->showMenuValue(itemValue, true);
    }
    //setting the clock month
    else if (currentItem == itemMonth) {
        setItemValue(upPressed, downPressed, 1, 12, true);
        screen->showMenuValue(ClockTime::getMonthName(itemValue), true);
    }
    //setting the clock day
    else if (currentItem == itemDay) {
        setItemValue(upPressed, downPressed, 1, 31, true);
        screen->showMenuValue(itemValue, true);
    }
    //setting the colors
    else if (currentItem == itemColor) {
        //skip the key BLACK = 0
        setItemValue(upPressed, downPressed, 1, 4, true);
        screen->showMenuValue(ClockScreen::getColorNameByNumber(itemValue), true);
    }
    //setting the minimum brightness
    else if (currentItem == itemBrightnessMin) {
        setItemValue(upPressed, downPressed, 0, 15, true);
        screen->showMenuValue(itemValue, true);
    }
    //setting the maximum brightness
    else if (currentItem == itemBrightnessMax) {
        setItemValue(upPressed, downPressed, 0, 15, true);
        screen->showMenuValue(itemValue, true);
    }
    //setting the timezone
    else if (currentItem == itemTimezone) {
        setItemValue(upPressed, downPressed, 0, ClockTimezone::NUMBER_OF_TIMEZONES - 1, true);
        ClockTimezone zone;
        zone.getTimezoneNameByNumber(itemValueText, MENU_VALUE_LENGTH, itemValue);
        screen->showMenuValueScrolling(itemValueText, true);
    }
    //setting if the light sensor is enabled
    else if (currentItem == itemLightSensorEnabled) {
        setItemValue(upPressed, downPressed, 0, 1, true);
        screen->showMenuValue(getYesNo(itemValue), true);
    }
    //setting if the Internet is enabled
    else if (currentItem == itemInternetEnabled) {
        setItemValue(upPressed, downPressed, 0, 1, true);
        screen->showMenuValue(getYesNo(itemValue), true);
    }
    //setting the Wi-Fi SSID
    else if (currentItem == itemWifiSsid) {
        setItemValueString(upPressed, downPressed);
        screen->showMenuValueString(itemValueString, true, itemValueStringPosition);
    }
    //setting the Wi-Fi Password
    else if (currentItem == itemWifiPassword) {
        setItemValueString(upPressed, downPressed);
        screen->showMenuValueString(itemValueString, true, itemValueStringPosition);
    }
    //setting if the Weather Feed is enabled
    else if (currentItem == itemWeatherEnabled) {
        setItemValue(upPressed, downPressed, 0, 1, true);
        screen->showMenuValue(getYesNo(itemValue), true);
    }
    //setting the Weather API Key
    else if (currentItem == itemWeatherApiKey) {
        setItemValueString(upPressed, downPressed);
        screen->showMenuValueString(itemValueString, true, itemValueStringPosition);
    }
    //setting the Weather location
    else if (currentItem == itemWeatherLocation) {
        setItemValueString(upPressed, downPressed);
        screen->showMenuValueString(itemValueString, true, itemValueStringPosition);
    }
    //setting the weather temperature scale
    else if (currentItem == itemWeatherTemperatureScale) {
        setItemValue(upPressed, downPressed, WEATHER_CELSIUS, WEATHER_FAHRENHEIT, true);
        screen->showMenuValue(ClockWeather::getTemperatureScaleName(itemValue), true);
    }
    //the hour after which the forecast is displayed for tomorrow
    else if (currentItem == itemWeatherForecastHour) {
        setItemValue(upPressed, downPressed, 0, 23, true);
        screen->showMenuValue(itemValue, true);
    }
    //setting the Sync Server Name
    else if (currentItem == itemSyncServerName) {
        setItemValueString(upPressed, downPressed);
        screen->showMenuValueString(itemValueString, true, itemValueStringPosition);
    }
    //setting the Sync Server Url
    else if (currentItem == itemSyncServerUrl) {
        setItemValueString(upPressed, downPressed);
        screen->showMenuValueString(itemValueString, true, itemValueStringPosition);
    }

    //check if the alarm settings were adjusted
    for (int i = 0; i < NUMBER_ALARMS; i++) {
        //setting if the alarm is active    
        if (currentItem == menuAlarms[i].active) {
            setItemValue(upPressed, downPressed, 0, 1, true);
            screen->showMenuValue(getYesNo(itemValue), true);
        }
        //setting the hour of the alarm    
        else if (currentItem == menuAlarms[i].hour) {
            //the maximum value depends on 12 or 24 hour format
            setItemValue(upPressed, downPressed, 
                (config->getHourFormat() == 0 ? 0 : 1), 
                (config->getHourFormat() == 0 ? 23 : 12), 
                true
            );    
            screen->showMenuValue(itemValue, true);
        }
        //setting the minute of the alarm
        else if (currentItem == menuAlarms[i].minute) {
            setItemValue(upPressed, downPressed, 0, 59, true);
            screen->showMenuValue(itemValue, true);
        }
        //setting the AM/PM of the alarm
        else if (currentItem == menuAlarms[i].amPm) {
            setItemValue(upPressed, downPressed, 0, 1, true);
            time->getAmPmText(itemValueText, itemValue);
            screen->showMenuValue(itemValueText, true);
        }
        //setting the year of the alarm
        else if (currentItem == menuAlarms[i].year) {
            setItemValue(upPressed, downPressed, 2014, 2100, false);
            screen->showMenuValue(itemValue, true);
        }
        //setting the month of the alarm
        else if (currentItem == menuAlarms[i].month) {
            setItemValue(upPressed, downPressed, 1, 12, true);
            screen->showMenuValue(itemValue, true);
        }
        //setting the day of the alarm
        else if (currentItem == menuAlarms[i].day) {
            setItemValue(upPressed, downPressed, 1, 31, true);
            screen->showMenuValue(itemValue, true);
        }
        //setting the song name of the alarm
        else if (currentItem == menuAlarms[i].songName) {
            setItemValue(upPressed, downPressed, 0, music->getNumberSongs() - 1, true);
            screen->showMenuValue(music->getSongNameByNumber(itemValue), true);

            //play the song
            music->playSong(music->getSongNameByNumber(itemValue), config->alarm[i].getSongTime());
        }
        //if songs are selected randomly
        else if (currentItem == menuAlarms[i].songRandom) {
            setItemValue(upPressed, downPressed, 0, 1, true);
            screen->showMenuValue(getYesNo(itemValue), true);
        }
        //setting the maximum time of each song
        else if (currentItem == menuAlarms[i].songTime) {
            setItemValue(upPressed, downPressed, 10, 600, true);
            screen->showMenuValue(itemValue, true);
        }
        //setting the repeat interval of the alarm
        else if (currentItem == menuAlarms[i].repeatInterval) {
            setItemValue(upPressed, downPressed, 1, 30, true);
            screen->showMenuValue(itemValue, true);
        }
        //setting the repeat times of the alarm
        else if (currentItem == menuAlarms[i].repeatTimes) {
            setItemValue(upPressed, downPressed, 0, 10, true);
            screen->showMenuValue(itemValue, true);
        }
        //setting the repeat interval of the alarm
        else if (currentItem == menuAlarms[i].repeatInterval) {
            setItemValue(upPressed, downPressed, 1, 30, true);
            screen->showMenuValue(itemValue, true);
        }
        //setting When the alarm
        else if (currentItem == menuAlarms[i].when) {
            setItemValue(upPressed, downPressed, 0, 2, true);
            screen->showMenuValue(ClockAlarm::getWhenName(itemValue), true);
        }

        //setting the if the alarm is active on Monday
        else if (currentItem == menuAlarms[i].monday) {
            setItemValue(upPressed, downPressed, 0, 1, true);
            screen->showMenuValue(getYesNo(itemValue), true);
        }
        //setting the if the alarm is active on Tuesday
        else if (currentItem == menuAlarms[i].tuesday) {
            setItemValue(upPressed, downPressed, 0, 1, true);
            screen->showMenuValue(getYesNo(itemValue), true);
        }
        //setting the if the alarm is active on Wednesday
        else if (currentItem == menuAlarms[i].wednesday) {
            setItemValue(upPressed, downPressed, 0, 1, true);
            screen->showMenuValue(getYesNo(itemValue), true);
        }
        //setting the if the alarm is active on Thursday
        else if (currentItem == menuAlarms[i].thursday) {
            setItemValue(upPressed, downPressed, 0, 1, true);
            screen->showMenuValue(getYesNo(itemValue), true);
        }
        //setting the if the alarm is active on Friday
        else if (currentItem == menuAlarms[i].friday) {
            setItemValue(upPressed, downPressed, 0, 1, true);
            screen->showMenuValue(getYesNo(itemValue), true);
        }
        //setting the if the alarm is active on Saturday
        else if (currentItem == menuAlarms[i].saturday) {
            setItemValue(upPressed, downPressed, 0, 1, true);
            screen->showMenuValue(getYesNo(itemValue), true);
        }
        //setting the if the alarm is active on Sunday
        else if (currentItem == menuAlarms[i].sunday) {
            setItemValue(upPressed, downPressed, 0, 1, true);
            screen->showMenuValue(getYesNo(itemValue), true);
        }
    }

}

void ClockMenu::menuItemChange(MenuItemChangeEvent changed) 
{
}

void ClockMenu::checkButtons()
{
    //button events
    bool upPressed = false;
    bool downPressed = false;
    bool enterPressed = false;

    //read the buttons
    buttonUp->read();
    buttonEnter->read();
    buttonDown->read();

    enterPressed = buttonEnter->wasPressed();

    //the UP button was pressed once
    if (buttonUp->wasPressed()) {
        upPressed = true;
    }
    else if (buttonUp->wasReleased()) {
        //reset the long press interval
        buttonRepeat = repeatFirst;
    }
    //check for long press
    else if (buttonUp->pressedFor(buttonRepeat)) {
        //increment the long press interval for the next check
        buttonRepeat += repeatIncr;
        upPressed = true;
    }
    //the DOWN button was pressed once
    else if (buttonDown->wasPressed()) {
        downPressed = true;
#ifdef DEBUG
        Serial.println(F("The button Down was pressed"));
#endif
    }
    else if (buttonDown->wasReleased()) {
        //reset the long press interval
        buttonRepeat = repeatFirst;
    }
    //check for the long press
    else if (buttonDown->pressedFor(buttonRepeat)) {
        //increment the long press interval for the next check
        buttonRepeat += repeatIncr;
        downPressed = true;
#ifdef DEBUG
        Serial.println(F("The button down was pressed for long time"));
#endif
    }

    //no button was pressed
    if (!upPressed && !enterPressed && !downPressed) {
        return;
    }
    
#ifdef DEBUG
    //Serial.print(F("Button upPressed: "));
    //Serial.println(upPressed);
    //Serial.print(F("Button enterPressed: "));
    //Serial.println(enterPressed);
    //Serial.print(F("Button downPressed: "));
    //Serial.println(downPressed);
#endif

    //get the current menu item
    MenuItem *currentItem = menu->getCurrent();

    //go up in the menu
    if (!isAdjusting && upPressed) {
        //the root element, show the real menu
        if (currentItem == menu->getRoot()) {
#ifdef DEBUG
            Serial.println(F("The root menu"));
#endif
        }
        //if there is an item to the left 
        //(usually, to the left of the first item in the submenu), move to it
        else if (currentItem->getLeft()) {
            menu->moveLeft();
        }
        else {
            menu->moveUp();
        }
    }
    //go down in the menu
    else if (!isAdjusting && downPressed) {
        menu->moveDown();
    }
    //adjust the value of the item in the menu
    else if (isAdjusting && (upPressed || downPressed)) {
        itemValueChange(currentItem, upPressed, downPressed);
    }
    //adjust the string value of the item in the menu
    else if (isStringAdjusting && enterPressed) {
        enterStringValueCharacter();
    }
    else if (enterPressed) {
        //the root element, show the real menu
        if (currentItem == menu->getRoot()) {
            menu->moveDown();
        }
        //if there is a menu item to the right, move to it
        else if (currentItem->getRight()) {
            menu->moveRight();
        }
        else {
            menu->use();
        }
    }

    //set the last use time
    lastUseTime = time->getCurrentTime();    
}


void ClockMenu::setItemValue(bool upPressed, bool downPressed, 
    int minValue, int maxValue, bool circle)
{
    if (upPressed) {
        itemValue--;
        if (itemValue < minValue) {
            itemValue = (circle ? maxValue : minValue);
        }
    }
    else if (downPressed) {
        itemValue++;
        if (itemValue > maxValue) {
            itemValue = (circle ? minValue : maxValue);
        }
    }
}

void ClockMenu::setItemValueString(bool upPressed, bool downPressed)
{
    //first and last ASCII characters
    const int firstAscii = 32;
    const int lastAscii = 127;

    unsigned char c = '0';

    //if there is a character already
    if (itemValueString.length() > itemValueStringPosition) {
        c = itemValueString[itemValueStringPosition];
    }

    if (upPressed) {
        c--;
        if (c < firstAscii) {
            c = lastAscii;
        }
    }
    else if (downPressed) {
        c++;
        if (c > lastAscii) {
            c = firstAscii;
        }
    }

    //grow the string
    if (itemValueString.length() <= itemValueStringPosition) {
        itemValueString += " ";
    }
    //set the new character
    itemValueString.setCharAt(itemValueStringPosition, c);

#ifdef DEBUG
    Serial.print(F("Character: "));
    Serial.println((char)c);

    Serial.print(F("Character position: "));
    Serial.println(itemValueStringPosition);
#endif

}

void ClockMenu::enterStringValueCharacter()
{
    //read the entered character
    unsigned c = itemValueString[itemValueStringPosition];

    if ((int)c == enterCharacter) {
        //clear the Enter character and all trailing spaces
        itemValueString = itemValueString.substring(0, itemValueStringPosition);
        itemValueString.trim();

        //disable the string adjusting mode
        isStringAdjusting = false;

        //call the menuUse method
        menu->use();
    }
    else {
        //move to the next character
        itemValueStringPosition++;
        //add the default character as a
        itemValueString += "a";

        screen->showMenuValueString(itemValueString, true, itemValueStringPosition);
    }
}

void ClockMenu::showHelp(char *buf, const char PROGMEM *helpText)
{
    memset(buf, 0, MENU_VALUE_LENGTH + 1);

    //copy the text from the flash
    strncpy_P(buf, helpText, MENU_VALUE_LENGTH);
    screen->showMenuValueScrolling(buf, false);

    //clear the buffer
    memset(buf, 0, MENU_VALUE_LENGTH + 1);
}
