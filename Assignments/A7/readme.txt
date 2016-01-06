Eric Sheeder - esheeder@mines.edu
Assignment 7 - To Arms! A Battle is Brewing

This program is a game where you control the hero and must move to dodge incoming enemies that roll towards you and will kill you if they touch you. You can also die if you move outside the boundaries of the grid in the world. The goal of the game is to survive until all 20 enemies have fallen off the edge of the world.

To compile the program, just go into the directory and type "make." Use the WASD keys to move around, left mouse to rotate the camera, and right mouse to zoom in and out.

Sadly, the game is very easy because of a bug that causes the enemies to move in the exact opposite direction of the hero instead of towards him. I spent a while trying to figure out what was going wrong, and although it feels like it should be an easy fix, I couldn't get it to work. Any time I tried to fix it the balls would just roll back and forth in place instead.

Unfortunately, this makes it difficult to tell whether or not there is collision detection in the game as the balls tend to fall off the edge fairly quickly, but it does exist as I saw the enemies bouncing off of each other earlier in development, and if you drive into one of the balls you will fall through the grid and die. The code for collision detection between enemies was taken straight from lab 11.

The lab was pretty helpful for the assignment in terms of collision detection, I'd give it 8/10.

This assignment was fun, but would have been a lot better if I were able to actually get the enemy movement working. I think it will be a good starting point for enemy AI in the final project though. 8/10.