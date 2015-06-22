/*
 *  Manage strings by using a generic, dynamic queue data structure.
 *
 *  Copyright (C) 2010  Efstathios Chatzikyriakidis (contact@efxa.org)
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

// include queue library header.
#include <QueueList.h>

class Test {
    public:
        Test(){};
        Test(int newType, String newName)
        {
            type = newType;
            name = newName;
        }
        ~Test(){};
        
        int type;
        String name;
};




// create a queue of strings messages.
QueueList <Test> queue;

// startup point entry (runs once).
void
setup () {
  // start serial communication.
  Serial.begin (9600);

    Test test1(0, "Test1");
    Test test2(0, "Test2");
    Test test3(1, "Test3");
    Test test4(1, "Test4");

  // push all the string messages to the queue.
  queue.push(test1);
  queue.push(test2);
  queue.push(test3);
  queue.push(test4);

}

// loop the main sketch.
void
loop () {

  // pop all the string messages from the queue.
  for (int i = 0; i < queue.count(); i++) {
        Serial.println(queue[i].name);
  }  

    Serial.println("Delay");
    delay(550);
  // nothing here.
}
