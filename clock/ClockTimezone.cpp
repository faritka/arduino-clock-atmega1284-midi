#include "ClockTimezone.h"

/**
 * Constructor
 */
ClockTimezone::ClockTimezone() 
{
};

const char PROGMEM ClockTimezone::kwajalein[]       = "-12:00 Eniwetok, Kwajalein";
const char PROGMEM ClockTimezone::apia[]            = "-11:00 Apia, Pago Pago";
const char PROGMEM ClockTimezone::honolulu[]        = "-10:00 Honolulu, Papeete";
const char PROGMEM ClockTimezone::anchorage[]       = "-9:00 Anchorage, Juneau";
const char PROGMEM ClockTimezone::losAngeles[]      = "-8:00 Los Angeles, San Francisco, Las Vegas, Vancouver";
const char PROGMEM ClockTimezone::calgary[]         = "-7:00 Calgary, Denver, Hermosillo";
const char PROGMEM ClockTimezone::phoenix[]         = "-7:00 Phoenix, Arizona";
const char PROGMEM ClockTimezone::chicago[]         = "-6:00 Chicago, Houston, Dallas, Minneapolis";
const char PROGMEM ClockTimezone::toronto[]         = "-5:00 New York City, Washington D.C., Toronto";
const char PROGMEM ClockTimezone::caracas[]         = "-4:30 Caracas";
const char PROGMEM ClockTimezone::asuncion[]        = "-4:00 Asuncion, San Juan, Santiago";
const char PROGMEM ClockTimezone::labrador[]        = "-3:30 St. John's, Newfoundland, Labrador";
const char PROGMEM ClockTimezone::buenos[]          = "-3:00 Buenos Aires, Montedevideo, Rio de Janeiro, Sao Paulo";
const char PROGMEM ClockTimezone::fernando[]        = "-2:00 Fernando de Noronha, Soutch Georgia";
const char PROGMEM ClockTimezone::azores[]          = "-1:00 Azores, Cape Verde";
const char PROGMEM ClockTimezone::london[]          = "GMT London, Lisbon, Casablanca, Dakar, Reykjavik, Tenerife";
const char PROGMEM ClockTimezone::paris[]           = "+1:00 Paris, Berlin, Rome, Madrid, Stockholm, Oslo";
const char PROGMEM ClockTimezone::istanbul[]        = "+2:00 Istanbul, Athens, Cairo, Helsinki, Jerusalem";
const char PROGMEM ClockTimezone::baghdad[]         = "+3:00 Baghdad";
const char PROGMEM ClockTimezone::moscow[]          = "+3:00 Moscow, Saint Petersburg";
const char PROGMEM ClockTimezone::tehran[]          = "+3:30 Tehran";
const char PROGMEM ClockTimezone::dubai[]           = "+4:00 Dubai";
const char PROGMEM ClockTimezone::kabul[]           = "+4:30 Kabul";
const char PROGMEM ClockTimezone::tashkent[]        = "+5:00 Tashkent, Karachi, Maldives, Islamabad";
const char PROGMEM ClockTimezone::colombo[]         = "+5:30 Colombo, Madras, New Delhi, Bombay, Calcutta";
const char PROGMEM ClockTimezone::kathmandu[]       = "+5:45 Kathmandu";
const char PROGMEM ClockTimezone::ufa[]             = "+6:00 Ufa, Ekaterinburg, Chelyabinsk";
const char PROGMEM ClockTimezone::cocos[]           = "+6:30 Cocos Islands, Yangon";
const char PROGMEM ClockTimezone::novosibirsk[]     = "+7:00 Novosibirsk, Omsk, Tomsk";
const char PROGMEM ClockTimezone::krasnoyarsk[]     = "+8:00 Krasnoyarsk";
const char PROGMEM ClockTimezone::irkutsk[]         = "+9:00 Irkutsk, Ulan Ude, Tokyo";
const char PROGMEM ClockTimezone::adelaide[]        = "+9:30 Adelaide, Darwin";
const char PROGMEM ClockTimezone::yakutsk[]         = "+10:00 Yakutsk, Chita, Blagoveschensk";
const char PROGMEM ClockTimezone::vladivostok[]     = "+11:00 Vladivostok, Khabarovsk";
const char PROGMEM ClockTimezone::magadan[]         = "+12:00 Magadan, Kamchatka";

const char* const ClockTimezone::timezoneNames[] = 
{
    ClockTimezone::kwajalein,
    ClockTimezone::apia,
    ClockTimezone::honolulu,
    ClockTimezone::anchorage,
    ClockTimezone::losAngeles,
    ClockTimezone::calgary,
    ClockTimezone::phoenix,
    ClockTimezone::chicago,
    ClockTimezone::toronto,
    ClockTimezone::caracas,
    ClockTimezone::asuncion,
    ClockTimezone::labrador,
    ClockTimezone::buenos,
    ClockTimezone::fernando,
    ClockTimezone::azores,
    ClockTimezone::london,
    ClockTimezone::paris,
    ClockTimezone::istanbul,
    ClockTimezone::baghdad,
    ClockTimezone::moscow,
    ClockTimezone::tehran,
    ClockTimezone::dubai,
    ClockTimezone::kabul,
    ClockTimezone::tashkent,
    ClockTimezone::colombo,
    ClockTimezone::kathmandu,
    ClockTimezone::ufa,
    ClockTimezone::cocos,
    ClockTimezone::novosibirsk,
    ClockTimezone::krasnoyarsk,
    ClockTimezone::irkutsk,
    ClockTimezone::adelaide,
    ClockTimezone::yakutsk,
    ClockTimezone::vladivostok,
    ClockTimezone::magadan
};

const int16_t PROGMEM ClockTimezone::standardOffsets[] = 
{
    //kwajalein     "-12:00 Eniwetok, Kwajalein"
    -720,
    //apia             "-11:00 Apia, Pago Pago"
    -660,
    //honolulu         "-10:00 Honolulu, Papeete"
    -600,
    //anchorage     "-9:00 Anchorage, Juneau"
    -540,
    //losAngeles     "-8:00 Los Angeles, San Francisco, Las Vegas, Vancouver"
    -480,
    //calgary          "-7:00 Calgary, Denver, Hermosillo"
    -420,
    //phoenix          "-7:00 Phoenix, Arizona"
    -420,
    //chicago         "-6:00 Chicago, Mexico City, Houston, Dallas, Minneapolis"
    -360,
    //toronto         "-5:00 Toronto, New York City, Washington D.C., Havana"
    -300,
    //caracas         "-4:30 Caracas"
    -270,
    //asuncion         "-4:00 Asuncion, San Juan, Santiago"
    -240,
    //labrador         "-3:30 St. John's, Newfoundland, Labrador"
    -210,
    //buenos        "-3:00 Buenos Aires, Montedevideo, Rio de Janeiro, Sao Paulo"
    -180,
    //fernando         "-2:00 Fernando de Noronha, South Georgia"
    -120,
    //azores         "-1:00 Azores, Cape Verde"
    -60,
    //london         "GMT London, Lisbon, Casablanca, Dakar, Reykjavik, Tenerife"
    0,
    //paris         "+1:00 Paris, Berlin, Rome, Madrid, Stockholm"
    60,
    //istanbul         "+2:00 Kiev, Istanbul, Athens, Cairo, Helsinki, Jerusalem"
    120,
    //baghdad         "+3:00 Baghdad, Riyadh"
    180,    
    //Moscow         "+3:00 Moscow"
    180,    
    //tehran         "+3:30 Tehran"
    270,
    //moscow         "+4:00 Dubai, Baku"
    240,    
    //kabul         "-4:30 Kabul"
    270,
    //tashkent         "+5:00 Tashkent, Karachi, Maldives, Islamabad"
    300,
    //colombo         "+5:30 Colombo, Madras, New Delhi, Bombay, Calcutta"
    330,    
    //kathmandu         "+5:45 Kathmandu"
    345,
    //ufa             "+6:00 Ufa, Ekaterinburg, Chelyabinsk, Almata, Dhaka"
    360,
    //cocos         "+6:30 Cocos Islands, Yangon"
    390,
    //novosibirsk    "+7:00 Novosibirsk, Omsk, Tomsk, Bangkok, Jakarta, Hanoi"
    420,
    //krasnoyarsk     "+8:00 Krasnoyarsk, Beijing, Hong Kong, Manila, Taipei"
    480,
    //irkutsk         "+9:00 Irkutsk, Ulan Ude, Pyongyang, Seoul, Tokyo"
    540,
    //adelaide         "+9:30 Adelaide, Darwin"
    570,
    //yakutsk         "+10:00 Yakutsk, Chita, Blagoveschensk, Melbourne, Sydney"
    600,
    //vladivostok     "+11:00 Vladivostok, Khabarovsk, Noumea"
    660,
    //magadan         "+12:00 Magadan, Kamchatka"
    720        
};

/**
 * When Russian cities are present, no daylight saving
 */
const int16_t PROGMEM ClockTimezone::daylightOffsets[] = 
{
    //kwajalein     "-12:00 Eniwetok, Kwajalein"
    -720,
    //apia             "-11:00 Apia, Pago Pago"
    -660,
    //honolulu         "-10:00 Honolulu, Papeete"
    -600,
    //anchorage     "-9:00 Anchorage, Juneau"
    -480,
    //losAngeles     "-8:00 Los Angeles, San Francisco, Las Vegas, Vancouver"
    -420,
    //calgary          "-7:00 Calgary, Denver, Hermosillo"
    -360,
    //phoenix          "-7:00 Phoenix, Arizona"
    -420,
    //chicago         "-6:00 Chicago, Mexico City, Houston, Dallas, Minneapolis"
    -300,
    //toronto         "-5:00 Toronto, New York City, Washington D.C., Havana"
    -240,
    //caracas         "-4:30 Caracas"
    -270,
    //asuncion         "-4:00 Asuncion, San Juan, Santiago"
    -240,
    //labrador         "-3:30 St. John's, Newfoundland, Labrador"
    -150,
    //buenos        "-3:00 Buenos Aires, Montedevideo, Rio de Janeiro, Sao Paulo"
    -180,
    //fernando         "-2:00 Fernando de Noronha, South Georgia"
    -120,
    //azores         "-1:00 Azores, Cape Verde"
    0,
    //london         "GMT London, Lisbon, Casablanca, Dakar, Reykjavik, Tenerife"
    60,
    //paris         "+1:00 Paris, Berlin, Rome, Madrid, Stockholm"
    120,
    //istanbul         "+2:00 Kiev, Istanbul, Athens, Cairo, Helsinki, Jerusalem"
    180,
    //baghdad         "+3:00 Kaliningrad, Baghdad, Riyadh"
    180,
    //moscow         "+3:00 Moscow, Saint Petersburg"
    180,    
    //tehran         "+3:30 Tehran"
    210,
    //moscow         "+4:00 Baku"
    240,    
    //kabul         "-4:30 Kabul"
    270,
    //tashkent         "+5:00 Tashkent, Karachi, Maldives, Islamabad"
    300,
    //colombo         "+5:30 Colombo, Madras, New Delhi, Bombay, Calcutta"
    330,    
    //kathmandu         "+5:45 Kathmandu"
    345,
    //ufa             "+6:00 Ufa, Ekaterinburg, Chelyabinsk, Almata, Dhaka"
    360,
    //cocos         "+6:30 Cocos Islands, Yangon"
    390,
    //novosibirsk    "+7:00 Novosibirsk, Omsk, Tomsk, Bangkok, Jakarta, Hanoi"
    420,
    //krasnoyarsk     "+8:00 Krasnoyarsk, Beijing, Hong Kong, Manila, Taipei"
    480,
    //irkutsk         "+9:00 Irkutsk, Ulan Ude, Pyongyang, Seoul, Tokyo"
    540,
    //adelaide         "+9:30 Adelaide, Darwin"
    570,
    //yakutsk         "+10:00 Yakutsk, Chita, Blagoveschensk, Melbourne, Sydney"
    600,
    //vladivostok     "+11:00 Vladivostok, Khabarovsk, Noumea"
    660,
    //magadan         "+12:00 Magadan, Kamchatka"
    720        
};

const int8_t PROGMEM ClockTimezone::standardWeeks[] = 
{
    //kwajalein     "-12:00 Eniwetok, Kwajalein"
    First,
    //apia             "-11:00 Apia, Pago Pago"
    First,
    //honolulu         "-10:00 Honolulu, Papeete"
    First,
    //anchorage     "-9:00 Anchorage, Juneau"
    First,
    //losAngeles     "-8:00 Los Angeles, San Francisco, Las Vegas, Vancouver"
    First,
    //calgary          "-7:00 Calgary, Denver, Hermosillo"
    First,
    //phoenix          "-7:00 Phoenix, Arizona"
    First,
    //chicago         "-6:00 Chicago, Mexico City, Houston, Dallas, Minneapolis"
    First,
    //toronto         "-5:00 Toronto, New York City, Washington D.C., Havana"
    First,
    //caracas         "-4:30 Caracas"
    First,
    //asuncion         "-4:00 Asuncion, San Juan, Santiago"
    First,
    //labrador         "-3:30 St. John's, Newfoundland, Labrador"
    First,
    //buenos        "-3:00 Buenos Aires, Montedevideo, Rio de Janeiro, Sao Paulo"
    First,
    //fernando         "-2:00 Fernando de Noronha, South Georgia"
    First,
    //azores         "-1:00 Azores, Cape Verde"
    First,
    //london         "GMT London, Lisbon, Casablanca, Dakar, Reykjavik, Tenerife"
    Last,
    //paris         "+1:00 Paris, Berlin, Rome, Madrid, Stockholm"
    Last,
    //istanbul         "+2:00 Kiev, Istanbul, Athens, Cairo, Helsinki, Jerusalem"
    Last,
    //baghdad         "+3:00 Kaliningrad, Baghdad, Riyadh"
    First,    
    //moscow         "+3:00 Moscow, Saint Petersburg"
    First,    
    //tehran         "+3:30 Tehran"
    Last,
    //dubai         "+4:00 Dubai, Baku"
    First,    
    //kabul         "-4:30 Kabul"
    First,
    //tashkent         "+5:00 Tashkent, Karachi, Maldives, Islamabad"
    First,
    //colombo         "+5:30 Colombo, Madras, New Delhi, Bombay, Calcutta"
    First,
    //kathmandu         "+5:45 Kathmandu"
    First,
    //ufa             "+6:00 Ufa, Ekaterinburg, Chelyabinsk, Almata, Dhaka"
    First,
    //cocos         "+6:30 Cocos Islands, Yangon"
    First,
    //novosibirsk    "+7:00 Novosibirsk, Omsk, Tomsk, Bangkok, Jakarta, Hanoi"
    First,
    //krasnoyarsk     "+8:00 Krasnoyarsk, Beijing, Hong Kong, Manila, Taipei"
    First,
    //irkutsk         "+9:00 Irkutsk, Ulan Ude, Pyongyang, Seoul, Tokyo"
    First,
    //adelaide         "+9:30 Adelaide, Darwin"
    First,
    //yakutsk         "+10:00 Yakutsk, Chita, Blagoveschensk, Melbourne, Sydney"
    First,
    //vladivostok     "+11:00 Vladivostok, Khabarovsk, Noumea"
    First,
    //magadan         "+12:00 Magadan, Kamchatka"
    First
};

const int8_t PROGMEM ClockTimezone::daylightWeeks[] = 
{
    //kwajalein     "-12:00 Eniwetok, Kwajalein"
    First,
    //apia             "-11:00 Apia, Pago Pago"
    First,
    //honolulu         "-10:00 Honolulu, Papeete"
    First,
    //anchorage     "-9:00 Anchorage, Juneau"
    Second,
    //losAngeles     "-8:00 Los Angeles, San Francisco, Las Vegas, Vancouver"
    Second,
    //calgary          "-7:00 Calgary, Denver, Hermosillo"
    Second,
    //phoenix          "-7:00 Phoenix, Arizona"
    First,
    //chicago         "-6:00 Chicago, Mexico City, Houston, Dallas, Minneapolis"
    Second,
    //toronto         "-5:00 Toronto, New York City, Washington D.C., Havana"
    Second,
    //caracas         "-4:30 Caracas"
    First,
    //asuncion         "-4:00 Asuncion, San Juan, Santiago"
    First,
    //labrador         "-3:30 St. John's, Newfoundland, Labrador"
    First,
    //buenos        "-3:00 Buenos Aires, Montedevideo, Rio de Janeiro, Sao Paulo"
    First,
    //fernando         "-2:00 Fernando de Noronha, South Georgia"
    First,
    //azores         "-1:00 Azores, Cape Verde"
    First,
    //london         "GMT London, Lisbon, Casablanca, Dakar, Reykjavik, Tenerife"
    Last,
    //paris         "+1:00 Paris, Berlin, Rome, Madrid, Stockholm"
    Last,
    //istanbul         "+2:00 Kiev, Istanbul, Athens, Cairo, Helsinki, Jerusalem"
    Last,
    //baghdad         "+3:00 Kaliningrad, Baghdad, Riyadh"
    First,
    //moscow         "+3:00 Moscow, Saint Petersburg"
    First,
    //tehran         "+3:30 Tehran"
    Last,
    //dubai         "+4:00 Dubai, Baku"
    First,
    //kabul         "-4:30 Kabul"
    First,
    //tashkent         "+5:00 Tashkent, Karachi, Maldives, Islamabad"
    First,
    //colombo         "+5:30 Colombo, Madras, New Delhi, Bombay, Calcutta"
    First,    
    //kathmandu         "+5:45 Kathmandu"
    First,
    //ufa             "+6:00 Ufa, Ekaterinburg, Chelyabinsk, Almata, Dhaka"
    First,
    //cocos         "+6:30 Cocos Islands, Yangon"
    First,
    //novosibirsk    "+7:00 Novosibirsk, Omsk, Tomsk, Bangkok, Jakarta, Hanoi"
    First,
    //krasnoyarsk     "+8:00 Krasnoyarsk, Beijing, Hong Kong, Manila, Taipei"
    First,
    //irkutsk         "+9:00 Irkutsk, Ulan Ude, Pyongyang, Seoul, Tokyo"
    First,
    //adelaide         "+9:30 Adelaide, Darwin"
    First,
    //yakutsk         "+10:00 Yakutsk, Chita, Blagoveschensk, Melbourne, Sydney"
    First,
    //vladivostok     "+11:00 Vladivostok, Khabarovsk, Noumea"
    First,
    //magadan         "+12:00 Magadan, Kamchatka"
    First
};

const int8_t PROGMEM ClockTimezone::standardDayOfWeeks[] = 
{
    //kwajalein     "-12:00 Eniwetok, Kwajalein"
    Sun,
    //apia             "-11:00 Apia, Pago Pago"
    Sun,
    //honolulu         "-10:00 Honolulu, Papeete"
    Sun,
    //anchorage     "-9:00 Anchorage, Juneau"
    Sun,
    //losAngeles     "-8:00 Los Angeles, San Francisco, Las Vegas, Vancouver"
    Sun,
    //calgary          "-7:00 Calgary, Denver, Hermosillo"
    Sun,
    //phoenix          "-7:00 Phoenix, Arizona"
    Sun,    
    //chicago         "-6:00 Chicago, Mexico City, Houston, Dallas, Minneapolis"
    Sun,
    //toronto         "-5:00 Toronto, New York City, Washington D.C., Havana"
    Sun,
    //caracas         "-4:30 Caracas"
    Sun,
    //asuncion         "-4:00 Asuncion, San Juan, Santiago"
    Sun,
    //labrador         "-3:30 St. John's, Newfoundland, Labrador"
    Sun,
    //buenos        "-3:00 Buenos Aires, Montedevideo, Rio de Janeiro, Sao Paulo"
    Sun,
    //fernando         "-2:00 Fernando de Noronha, South Georgia"
    Sun,
    //azores         "-1:00 Azores, Cape Verde"
    Sun,
    //london         "GMT London, Lisbon, Casablanca, Dakar, Reykjavik, Tenerife"
    Sun,
    //paris         "+1:00 Paris, Berlin, Rome, Madrid, Stockholm"
    Sun,
    //istanbul         "+2:00 Kiev, Istanbul, Athens, Cairo, Helsinki, Jerusalem"
    Sun,
    //baghdad         "+3:00 Kaliningrad, Baghdad, Riyadh"
    Sun,    
    //moscow         "+3:00 Moscow, Saint Petersburg"
    Sun,    
    //tehran         "+3:30 Tehran"
    Sun,
    //dubai         "+4:00 Dubai, Baku"
    Sun,    
    //kabul         "-4:30 Kabul"
    Sun,
    //tashkent         "+5:00 Tashkent, Karachi, Maldives, Islamabad"
    Sun,
    //colombo         "+5:30 Colombo, Madras, New Delhi, Bombay, Calcutta"
    Sun,    
    //kathmandu         "+5:45 Kathmandu"
    Sun,
    //ufa             "+6:00 Ufa, Ekaterinburg, Chelyabinsk, Almata, Dhaka"
    Sun,
    //cocos         "+6:30 Cocos Islands, Yangon"
    Sun,
    //novosibirsk    "+7:00 Novosibirsk, Omsk, Tomsk, Bangkok, Jakarta, Hanoi"
    Sun,
    //krasnoyarsk     "+8:00 Krasnoyarsk, Beijing, Hong Kong, Manila, Taipei"
    Sun,
    //irkutsk         "+9:00 Irkutsk, Ulan Ude, Pyongyang, Seoul, Tokyo"
    Sun,
    //adelaide         "+9:30 Adelaide, Darwin"
    Sun,
    //yakutsk         "+10:00 Yakutsk, Chita, Blagoveschensk, Melbourne, Sydney"
    Sun,
    //vladivostok     "+11:00 Vladivostok, Khabarovsk, Noumea"
    Sun,
    //magadan         "+12:00 Magadan, Kamchatka"
    Sun
};

const int8_t PROGMEM ClockTimezone::daylightDayOfWeeks[] = 
{
    //kwajalein     "-12:00 Eniwetok, Kwajalein"
    Sun,
    //apia             "-11:00 Apia, Pago Pago"
    Sun,
    //honolulu         "-10:00 Honolulu, Papeete"
    Sun,
    //anchorage     "-9:00 Anchorage, Juneau"
    Sun,
    //losAngeles     "-8:00 Los Angeles, San Francisco, Las Vegas, Vancouver"
    Sun,
    //calgary          "-7:00 Calgary, Denver, Hermosillo"
    Sun,
    //phoenix          "-7:00 Phoenix, Arizona"
    Sun,
    //chicago         "-6:00 Chicago, Mexico City, Houston, Dallas, Minneapolis"
    Sun,
    //toronto         "-5:00 Toronto, New York City, Washington D.C., Havana"
    Sun,
    //caracas         "-4:30 Caracas"
    Sun,
    //asuncion         "-4:00 Asuncion, San Juan, Santiago"
    Sun,
    //labrador         "-3:30 St. John's, Newfoundland, Labrador"
    Sun,
    //buenos        "-3:00 Buenos Aires, Montedevideo, Rio de Janeiro, Sao Paulo"
    Sun,
    //fernando         "-2:00 Fernando de Noronha, South Georgia"
    Sun,
    //azores         "-1:00 Azores, Cape Verde"
    Sun,
    //london         "GMT London, Lisbon, Casablanca, Dakar, Reykjavik, Tenerife"
    Sun,
    //paris         "+1:00 Paris, Berlin, Rome, Madrid, Stockholm"
    Sun,
    //istanbul         "+2:00 Kiev, Istanbul, Athens, Cairo, Helsinki, Jerusalem"
    Sun,
    //baghdad         "+3:00 Kaliningrad, Baghdad, Riyadh"
    Sun,    
    //moscow         "+3:00 Moscow, Saint Petersburg"
    Sun,    
    //tehran         "+3:30 Tehran"
    Sun,
    //dubai         "+4:00 Dubai, Baku"
    Sun,    
    //kabul         "-4:30 Kabul"
    Sun,
    //tashkent         "+5:00 Tashkent, Karachi, Maldives, Islamabad"
    Sun,
    //colombo         "+5:30 Colombo, Madras, New Delhi, Bombay, Calcutta"
    Sun,    
    //kathmandu         "+5:45 Kathmandu"
    Sun,
    //ufa             "+6:00 Ufa, Ekaterinburg, Chelyabinsk, Almata, Dhaka"
    Sun,
    //cocos         "+6:30 Cocos Islands, Yangon"
    Sun,
    //novosibirsk    "+7:00 Novosibirsk, Omsk, Tomsk, Bangkok, Jakarta, Hanoi"
    Sun,
    //krasnoyarsk     "+8:00 Krasnoyarsk, Beijing, Hong Kong, Manila, Taipei"
    Sun,
    //irkutsk         "+9:00 Irkutsk, Ulan Ude, Pyongyang, Seoul, Tokyo"
    Sun,
    //adelaide         "+9:30 Adelaide, Darwin"
    Sun,
    //yakutsk         "+10:00 Yakutsk, Chita, Blagoveschensk, Melbourne, Sydney"
    Sun,
    //vladivostok     "+11:00 Vladivostok, Khabarovsk, Noumea"
    Sun,
    //magadan         "+12:00 Magadan, Kamchatka"
    Sun
};

const int8_t PROGMEM ClockTimezone::standardMonths[] = 
{
    //kwajalein     "-12:00 Eniwetok, Kwajalein"
    Jan,
    //apia             "-11:00 Apia, Pago Pago"
    Jan,
    //honolulu         "-10:00 Honolulu, Papeete"
    Jan,
    //anchorage     "-9:00 Anchorage, Juneau"
    Nov,
    //losAngeles     "-8:00 Los Angeles, San Francisco, Las Vegas, Vancouver"
    Nov,
    //calgary          "-7:00 Calgary, Denver, Hermosillo"
    Nov,
    //phoenix          "-7:00 Phoenix, Arizona"
    Jan,    
    //chicago         "-6:00 Chicago, Mexico City, Houston, Dallas, Minneapolis"
    Nov,
    //toronto         "-5:00 Toronto, New York City, Washington D.C., Havana"
    Nov,
    //caracas         "-4:30 Caracas"
    Jan,
    //asuncion         "-4:00 Asuncion, San Juan, Santiago"
    Jan,
    //labrador         "-3:30 St. John's, Newfoundland, Labrador"
    Jan,
    //buenos        "-3:00 Buenos Aires, Montedevideo, Rio de Janeiro, Sao Paulo"
    Jan,
    //fernando         "-2:00 Fernando de Noronha, South Georgia"
    Jan,
    //azores         "-1:00 Azores, Cape Verde"
    Jan,
    //london         "GMT London, Lisbon, Casablanca, Dakar, Reykjavik, Tenerife"
    Oct,
    //paris         "+1:00 Paris, Berlin, Rome, Madrid, Stockholm"
    Oct,
    //istanbul         "+2:00 Kiev, Istanbul, Athens, Cairo, Helsinki, Jerusalem"
    Oct,
    //baghdad         "+3:00 Kaliningrad, Baghdad, Riyadh"
    Jan,
    //moscow         "+3:00 Moscow, Saint Petersburg"
    Jan,
    //tehran         "+3:30 Tehran"
    Oct,
    //dubai         "+4:00 Dubai, Baku"
    Jan,
    //kabul         "-4:30 Kabul"
    Jan,
    //tashkent         "+5:00 Tashkent, Karachi, Maldives, Islamabad"
    Jan,
    //colombo         "+5:30 Colombo, Madras, New Delhi, Bombay, Calcutta"
    Jan,
    //kathmandu         "+5:45 Kathmandu"
    Jan,
    //ufa             "+6:00 Ufa, Ekaterinburg, Chelyabinsk, Almata, Dhaka"
    Jan,
    //cocos         "+6:30 Cocos Islands, Yangon"
    Jan,
    //novosibirsk    "+7:00 Novosibirsk, Omsk, Tomsk, Bangkok, Jakarta, Hanoi"
    Jan,
    //krasnoyarsk     "+8:00 Krasnoyarsk, Beijing, Hong Kong, Manila, Taipei"
    Jan,
    //irkutsk         "+9:00 Irkutsk, Ulan Ude, Pyongyang, Seoul, Tokyo"
    Jan,
    //adelaide         "+9:30 Adelaide, Darwin"
    Jan,
    //yakutsk         "+10:00 Yakutsk, Chita, Blagoveschensk, Melbourne, Sydney"
    Jan,
    //vladivostok     "+11:00 Vladivostok, Khabarovsk, Noumea"
    Jan,
    //magadan         "+12:00 Magadan, Kamchatka"
    Jan
};

const int8_t PROGMEM ClockTimezone::daylightMonths[] = 
{
    //kwajalein     "-12:00 Eniwetok, Kwajalein"
    Jan,
    //apia             "-11:00 Apia, Pago Pago"
    Jan,
    //honolulu         "-10:00 Honolulu, Papeete"
    Jan,
    //anchorage     "-9:00 Anchorage, Juneau"
    Mar,
    //losAngeles     "-8:00 Los Angeles, San Francisco, Las Vegas, Vancouver"
    Mar,
    //calgary          "-7:00 Calgary, Denver, Hermosillo"
    Mar,
    //phoenix          "-7:00 Phoenix, Arizona"
    Jan,    
    //chicago         "-6:00 Chicago, Mexico City, Houston, Dallas, Minneapolis"
    Mar,
    //toronto         "-5:00 Toronto, New York City, Washington D.C., Havana"
    Mar,
    //caracas         "-4:30 Caracas"
    Jan,
    //asuncion         "-4:00 Asuncion, San Juan, Santiago"
    Jan,
    //labrador         "-3:30 St. John's, Newfoundland, Labrador"
    Jan,
    //buenos        "-3:00 Buenos Aires, Montedevideo, Rio de Janeiro, Sao Paulo"
    Jan,
    //fernando         "-2:00 Fernando de Noronha, South Georgia"
    Jan,
    //azores         "-1:00 Azores, Cape Verde"
    Jan,
    //london         "GMT London, Lisbon, Casablanca, Dakar, Reykjavik, Tenerife"
    Mar,
    //paris         "+1:00 Paris, Berlin, Rome, Madrid, Stockholm"
    Mar,
    //istanbul         "+2:00 Kiev, Istanbul, Athens, Cairo, Helsinki, Jerusalem"
    Mar,
    //baghdad         "+3:00 Kaliningrad, Baghdad, Riyadh"
    Jan,
    //moscow         "+3:00 Moscow, Saint Petersburg"
    Jan,
    //tehran         "+3:30 Tehran"
    Mar,
    //dubai         "+4:00 Dubai, Baku"
    Jan,
    //kabul         "-4:30 Kabul"
    Jan,
    //tashkent         "+5:00 Tashkent, Karachi, Maldives, Islamabad"
    Jan,
    //colombo         "+5:30 Colombo, Madras, New Delhi, Bombay, Calcutta"
    Jan,
    //kathmandu         "+5:45 Kathmandu"
    Jan,
    //ufa             "+6:00 Ufa, Ekaterinburg, Chelyabinsk, Almata, Dhaka"
    Jan,
    //cocos         "+6:30 Cocos Islands, Yangon"
    Jan,
    //novosibirsk    "+7:00 Novosibirsk, Omsk, Tomsk, Bangkok, Jakarta, Hanoi"
    Jan,
    //krasnoyarsk     "+8:00 Krasnoyarsk, Beijing, Hong Kong, Manila, Taipei"
    Jan,
    //irkutsk         "+9:00 Irkutsk, Ulan Ude, Pyongyang, Seoul, Tokyo"
    Jan,
    //adelaide         "+9:30 Adelaide, Darwin"
    Jan,
    //yakutsk         "+10:00 Yakutsk, Chita, Blagoveschensk, Melbourne, Sydney"
    Jan,
    //vladivostok     "+11:00 Vladivostok, Khabarovsk, Noumea"
    Jan,
    //magadan         "+12:00 Magadan, Kamchatka"
    Jan
};

const int8_t PROGMEM ClockTimezone::standardHours[] = 
{
    //kwajalein     "-12:00 Eniwetok, Kwajalein"
    0,
    //apia             "-11:00 Apia, Pago Pago"
    0,
    //honolulu         "-10:00 Honolulu, Papeete"
    0,
    //anchorage     "-9:00 Anchorage, Juneau"
    2,
    //losAngeles     "-8:00 Los Angeles, San Francisco, Las Vegas, Vancouver"
    2,
    //calgary          "-7:00 Calgary, Denver, Hermosillo"
    2,
    //phoenix          "-7:00 Phoenix, Arizona"
    0,
    //chicago         "-6:00 Chicago, Mexico City, Houston, Dallas, Minneapolis"
    2,
    //toronto         "-5:00 Toronto, New York City, Washington D.C., Havana"
    2,
    //caracas         "-4:30 Caracas"
    0,
    //asuncion         "-4:00 Asuncion, San Juan, Santiago"
    0,
    //labrador         "-3:30 St. John's, Newfoundland, Labrador"
    0,
    //buenos        "-3:00 Buenos Aires, Montedevideo, Rio de Janeiro, Sao Paulo"
    0,
    //fernando         "-2:00 Fernando de Noronha, South Georgia"
    0,
    //azores         "-1:00 Azores, Cape Verde"
    0,
    //london         "GMT London, Lisbon, Casablanca, Dakar, Reykjavik, Tenerife"
    1,
    //paris         "+1:00 Paris, Berlin, Rome, Madrid, Stockholm"
    1,
    //istanbul         "+2:00 Kiev, Istanbul, Athens, Cairo, Helsinki, Jerusalem"
    1,
    //baghdad         "+3:00 Kaliningrad, Baghdad, Riyadh"
    0,    
    //moscow         "+3:00 Moscow, Saint Petersburg"
    0,    
    //tehran         "+3:30 Tehran"
    1,
    //dubai         "+4:00 Dubai, Baku"
    0,    
    //kabul         "-4:30 Kabul"
    0,
    //tashkent         "+5:00 Tashkent, Karachi, Maldives, Islamabad"
    0,
    //colombo         "+5:30 Colombo, Madras, New Delhi, Bombay, Calcutta"
    0,    
    //kathmandu         "+5:45 Kathmandu"
    0,
    //ufa             "+6:00 Ufa, Ekaterinburg, Chelyabinsk, Almata, Dhaka"
    0,
    //cocos         "+6:30 Cocos Islands, Yangon"
    0,
    //novosibirsk    "+7:00 Novosibirsk, Omsk, Tomsk, Bangkok, Jakarta, Hanoi"
    0,
    //krasnoyarsk     "+8:00 Krasnoyarsk, Beijing, Hong Kong, Manila, Taipei"
    0,
    //irkutsk         "+9:00 Irkutsk, Ulan Ude, Pyongyang, Seoul, Tokyo"
    0,
    //adelaide         "+9:30 Adelaide, Darwin"
    0,
    //yakutsk         "+10:00 Yakutsk, Chita, Blagoveschensk, Melbourne, Sydney"
    0,
    //vladivostok     "+11:00 Vladivostok, Khabarovsk, Noumea"
    0,
    //magadan         "+12:00 Magadan, Kamchatka"
    0
};

const int8_t PROGMEM ClockTimezone::daylightHours[] = 
{
    //kwajalein     "-12:00 Eniwetok, Kwajalein"
    0,
    //apia          "-11:00 Apia, Pago Pago"
    0,
    //honolulu      "-10:00 Honolulu, Papeete"
    0,
    //anchorage     "-9:00 Anchorage, Juneau"
    2,
    //losAngeles    "-8:00 Los Angeles, San Francisco, Las Vegas, Vancouver"
    2,
    //calgary       "-7:00 Calgary, Denver, Hermosillo"
    2,
    //phoenix       "-7:00 Phoenix, Arizona"
    0,
    //chicago       "-6:00 Chicago, Mexico City, Houston, Dallas, Minneapolis"
    2,
    //toronto       "-5:00 Toronto, New York City, Washington D.C., Havana"
    2,
    //caracas       "-4:30 Caracas"
    0,
    //asuncion      "-4:00 Asuncion, San Juan, Santiago"
    0,
    //labrador      "-3:30 St. John's, Newfoundland, Labrador"
    0,
    //buenos        "-3:00 Buenos Aires, Montedevideo, Rio de Janeiro, Sao Paulo"
    0,
    //fernando      "-2:00 Fernando de Noronha, South Georgia"
    0,
    //azores        "-1:00 Azores, Cape Verde"
    0,
    //london        "GMT London, Lisbon, Casablanca, Dakar, Reykjavik, Tenerife"
    1,
    //paris         "+1:00 Paris, Berlin, Rome, Madrid, Stockholm"
    1,
    //istanbul      "+2:00 Kiev, Istanbul, Athens, Cairo, Helsinki, Jerusalem"
    1,
    //baghdad       "+3:00 Kaliningrad, Baghdad, Riyadh"
    0,
    //moscow        "+4:00 Moscow, Saint Petersburg"
    0,
    //tehran        "+3:30 Tehran"
    1,
    //dubai        "+4:00 Dubai, Baku"
    0,
    //kabul         "-4:30 Kabul"
    0,
    //tashkent      "+5:00 Tashkent, Karachi, Maldives, Islamabad"
    0,
    //colombo       "+5:30 Colombo, Madras, New Delhi, Bombay, Calcutta"
    0,
    //kathmandu     "+5:45 Kathmandu"
    0,
    //ufa           "+6:00 Ufa, Ekaterinburg, Chelyabinsk, Almata, Dhaka"
    0,
    //cocos         "+6:30 Cocos Islands, Yangon"
    0,
    //novosibirsk   "+7:00 Novosibirsk, Omsk, Tomsk, Bangkok, Jakarta, Hanoi"
    0,
    //krasnoyarsk   "+8:00 Krasnoyarsk, Beijing, Hong Kong, Manila, Taipei"
    0,
    //irkutsk       "+9:00 Irkutsk, Ulan Ude, Pyongyang, Seoul, Tokyo"
    0,
    //adelaide      "+9:30 Adelaide, Darwin"
    0,
    //yakutsk       "+10:00 Yakutsk, Chita, Blagoveschensk, Melbourne, Sydney"
    0,
    //vladivostok   "+11:00 Vladivostok, Khabarovsk, Noumea"
    0,
    //magadan       "+12:00 Magadan, Kamchatka"
    0
};


void ClockTimezone::getTimezoneNameByNumber(char *buf, const int bufSize, const byte timezoneNumber)
{
    strncpy_P(buf, (char*)pgm_read_word(&(ClockTimezone::timezoneNames[timezoneNumber])), bufSize - 1);
}


void ClockTimezone::setTimezoneIntoConfig(ClockConfig *config, byte timezoneNumber)
{
    TimeChangeRule standardTimeStart = {
        "",
        pgm_read_byte_near(ClockTimezone::standardWeeks + timezoneNumber),
        pgm_read_byte_near(ClockTimezone::standardDayOfWeeks + timezoneNumber),
        pgm_read_byte_near(ClockTimezone::standardMonths + timezoneNumber),
        pgm_read_byte_near(ClockTimezone::standardHours + timezoneNumber),
        pgm_read_word_near(ClockTimezone::standardOffsets + timezoneNumber)
    };

    TimeChangeRule daylightTimeStart = {
        "",
        pgm_read_byte_near(ClockTimezone::daylightWeeks + timezoneNumber),
        pgm_read_byte_near(ClockTimezone::daylightDayOfWeeks + timezoneNumber),
        pgm_read_byte_near(ClockTimezone::daylightMonths + timezoneNumber),
        pgm_read_byte_near(ClockTimezone::daylightHours + timezoneNumber),
        pgm_read_word_near(ClockTimezone::daylightOffsets + timezoneNumber)
    };

    config->setStandardTimeStart(standardTimeStart)
        ->setDaylightTimeStart(daylightTimeStart);
}
