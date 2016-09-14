#Cpp-Windows-Programming
By Packt

This is the code repository for [Cpp-Windows-Programming](https://www.packtpub.com/application-development/c-windows-programming), published by [Packt Publishing](https://www.packtpub.com/). It contains all the required files to run the code.

###Instructions and Navigation
This book is for application developers who want a head-first approach into Windows programming. It will teach you how to develop an object-oriented class library in C++ and enhanced applications in Windows. Basic knowledge of C++ and the object-oriented framework is assumed to get the most out of this book.

**Code Snippet:**

```
#include "..\\SmallWindows\\SmallWindows.h"
#include "HelloWindow.h"

HelloWindow::HelloWindow(WindowShow windowShow)
 :Window(LogicalWithScroll, ZeroSize, nullptr,
         OverlappedWindow, NoStyle, windowShow) {
  SetHeader(TEXT("Hello Window"));
}

```

###Software Requirements

* Visual Studio Express
 

###Related C Plus Plus Books

* [Beginning C++ Game Programming](https://www.packtpub.com/game-development/beginning-c-game-programming?utm_source=github&utm_medium=repository&utm_campaign=9781786466198)

* [Windows Phone 7.5 Application Development with F#](https://www.packtpub.com/application-development/windows-phone-75-application-development-f?utm_source=github&utm_medium=repository&utm_campaign=9781849687843)
