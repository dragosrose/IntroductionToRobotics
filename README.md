# IntroductionToRobotics

Introduction to Robotics laboratory homeworks, taken in the 3rd year at the Faculty of Mathematics and Computer Science, University of Bucharest. Each homework includes requirements, implementation details, code and image files.

<!-- TABLE OF CONTENTS -->
<details open>
  <summary><h3>Table of Contents</h3></summary>
  <ol>
    <li><a href="#getting-started">Getting Started</a></li>
    <li><a href="#homework-1">Homework 1</a></li>
    <li><a href="#homework-2">Homework 2</a></li>
    <li><a href="#homework-3">Homework 3</a></li>
    <li><a href="#homework-4">Homework 4</a></li>
    <li><a href="#homework-5">Homework 5</a></li>
  </ol>
</details>

## Getting Started

Code regarding the last homework is written in the "main.ino" file. To view the source codes for the other homeworks, please navigate to the "homework" folder.

<br>

## Homework 1

<b>Technical task: </b> Use a separate potentiometer in controlling each of the
color of the RGB led (Red, Green and Blue). The control must be done
with digital electronics (aka you must read the value of the potentiometer with Arduino, and write a mapped value to each of the pins connected
to the led.)

<b>Components:</b> 1 RBG LED, 3 potentiometers,
resistors and wires
<br><br>

<h3> Picture of the setup: </h3>
<img src="https://drive.google.com/uc?export=view&id=1kQ3rH5mv4lF6WDAjuVzAqp-AwAdwAlaR" alt="setup picture" />
<br><br>
<h3> Picture of the design behind it: </h3>
<img src="https://drive.google.com/uc?export=view&id=1riGbG8wzaon06seESVv3LTx6KJwgI7Cb" alt="design"/>
<sub>1. Real setup has the green and blue pins of the LED inversed, due to its different design from the Tinkercad model. <br>
2. <a href="https://www.tinkercad.com/things/iskBdIcFpQn?sharecode=rFOlXGFIqA9Q1KxTsPaeFc4YmNW5atJbWIF2ZmMmFVI">Tinkercad simulation</a></sub>
<br><br>
<h3> Videos showcasing the setup: </h3>
<ul>
    <li><a href="https://drive.google.com/file/d/1kK_IDuUVnUMwUWfNwKOu2DZBJ50rHQK8/view?usp=sharing">Daytime</a>
    </li>
    <li>
    <a href="https://drive.google.com/file/d/1kEMd9oamkOFXMtUuCpt9NlEhctBrVOUH/view?usp=sharing">Nightime</a>
    </li>
</ul>

<br>

## Homework 2

<b>Technical task: </b> Setup that simulates the behavior of real-life traffic lights for a crosswalk. 2 LEDs (red and green) represent the traffic lights for people, whereas 3 LEDs (red, yellow and green) are for the cars.

<b>Components:</b> 5 LEDs, 1 push buttons, resistors and wires

<h3> Picture of the setup: </h3>
<img src="https://drive.google.com/uc?export=view&id=1q-50skNGcgDhvHY4rlQmZJzDv3XvLUIs" alt="setup picture" />

<br>

<h3> Short video showcasing the setup: </h3>
<a href="https://drive.google.com/file/d/1prVPlxDzc1HlFhmHoC4C1chddjBHiGcd/view?usp=sharing">Video</a>

<br>

## Homework 3

<b>Technical task: </b> "Drawing" on a 7-digit LED display using a joystick. The joystick is used to move the cursor on the display, and the push button is used to draw. The cursor is represented by a blinking segment and upon selecting said segment it can be turnt on or off by joystick movement. The whole display is cleared by pressing the push button for more than 0.5 seconds.

<h3>Picture of the setup: </h3>
<img src="https://drive.google.com/uc?export=view&id=1vpdlEugEEhexd5KGP4MRiqesWY2ZpBPp" alt="setup picture" />
<br>

<h3>Short video showcasing the setup: </h3>
<a href= "https://drive.google.com/file/d/1vnB6PrdBQqzXwLHaoxE45b_ltmIOdHG1/view?usp=share_link">Video</a>
<br>

## Homework 4

<b>Technical task: </b> Using the joystick to move through the 4 digit 7 segment display, and using the push button to select a digit. The selected digit is then incremented or decremented by the joystick movement. The whole display is cleared by pressing the push button for more than 0.5 seconds.

<b>Components:</b> a 4 digit 7-segment displays, 1 joystick, 1 74hc595 shift register, wires

<h3>Picture of the setup: </h3>

<h3>Short video showcasing the setup: </h3>

https://user-images.githubusercontent.com/79592589/203647423-68d1bf6f-cae9-4e29-a928-178dc054c4f5.mp4

## Homework 5

<b>Technical task: </b> Menu for the game, where the user can scroll on the LCD and select options using the joystick.

<h3>Picture of the setup: </h3>

<h3>Short video showcasing the setup: </h3>
