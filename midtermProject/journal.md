# Midterm Project Journal - Give Me Attention

https://youtu.be/v-6Q0RtPAas

## Ideation
For the midterm project, I wanted to build a robot that consistently seeks for the user's attention. While thinking of which robot I should create, I remembered an occasion where a small kid continues to seek for his parent's attention. Whenever the kid gets an attention, he would show a movement of satisfaction. However, when his parents were focused on something else, the kid started to panic and sought actively for his parent's attention by rapidly moving his arms. So, I decided to replicate the following instance using a robot that involes a NeoPixel for the different facial expressions.

<img width="472" alt="스크린샷 2021-10-13 오전 6 09 37" src="https://user-images.githubusercontent.com/15336672/137054777-c13c6af4-7af5-464d-a333-d5909f05acdb.png">

### So, What does the robot do? And, how will it respond to the audience?
For the sequence of the robot action, when a user presses on the button that trasmits data to the receiver (here, in this case, is the robot) the "dull" face of the robot switches to "happy". While being in the "Happy" mode, the robot raises its arm to the full extent and slowly pulls it down to its original position. When it reaches its original position, the mode of the robot changes back to the "dull" mode. However, if the dull-mode is continued for more than 5 seconds, the robot enters a "sad" mode where the face changes along with rapid movement of both arms to indicate either sadness or anger to keep pressing the button to provide it with the button. The mode continues until the user presses the button, in which it returns to the "Happy" mode. Thus, the robot basically responds based on the user's press of the button

For the robot to be expressive, I have mainly used the face-resembling pixel order (dull, happy, and sad) to describe the emotion that the robot is feeling. Also, by adding on the movement of the arms of the robot depending on the emotion state, it helps the audience to understand comprehensively what the robot is feeling (or what the robot wants the user to do).

## Process

### Initial Production Phase
In order to get the parts working, I mainly focused on the codebase as well as the circuit to double-check whether there is any short circuits. Also, I had to soldier any other parts that were lose so that when I move the circuit into the container for the robot it will not create any problems from wire falling off from the components. Once all the circuit was complete, I moved on to the main arduino code for the logic behind the movement of the robot.

### Arduino Production Phase
In order to create different facial expression of the robot based on its respective emotion state, I created three arrays that represent the position of lit up LEDs for each faces. The ones that should be off are denoted as 0, and the ones that should light up are denoted as 1.

![IMG_1595](https://user-images.githubusercontent.com/15336672/137061019-a086ba96-405b-4613-b9eb-c9f00d15d42c.jpeg)
![IMG_1596](https://user-images.githubusercontent.com/15336672/137061026-5780565f-81f2-4d30-96c9-0777acb2e42b.jpeg)
![IMG_1597](https://user-images.githubusercontent.com/15336672/137061038-4909eb91-f143-4862-9697-bfbde4b31789.jpeg)

https://youtu.be/ZXVaTOwCm3w

```
int DULL_FACE[] = {
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 1, 1, 0, 0, 1, 1, 0,
  0, 1, 1, 0, 0, 1, 1, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 1, 1, 1, 1, 1, 1, 0,
  0, 1, 1, 1, 1, 1, 1, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
};

int HAPPY_FACE[] = {
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 1, 0, 0, 0, 0, 1, 0,
  0, 0, 1, 0, 0, 1, 0, 0,
  0, 1, 0, 0, 0, 0, 1, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 1, 0, 0, 0, 0, 1, 0,
  0, 1, 1, 1, 1, 1, 1, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
};

int SAD_FACE[] = {
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 1, 0, 0, 1, 0, 0,
  0, 1, 1, 0, 0, 1, 1, 0,
  0, 1, 0, 0, 0, 0, 1, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 1, 1, 1, 1, 0, 0,
  0, 1, 0, 0, 0, 0, 1, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
};
```
And, for the waiting duration between dull mode and sad mode, I used millis() to calculate exactly 5 seconds after the robot has begun to be in a dull mode. Moreover, in order to facilitate each set of movements in a linear and coordinated fashion, I used different boolean states for each section such as, but not limited to the following (See Below). By turning on and off the different phase states, I was able to successfully coordinate the actions that the robot made.
```
  bool hasStarted = false;
  bool needFaceUpdate = false;
  bool needHappyArmPosUpdate = false;
  bool needSadArmPosUpdate = false;
  bool isHoldAfterSequence = false;
```
### Physical Production Phase
Producing the frame for the robot was one of the most strenuous task as it was hard to make it balanced without andding any supports on its exterior. Before building the body part, I have calculated the required space needed to store the circuit but as I was shifting around with different cables as I faced a short-circuit due to incorrect placement of wires I realized that the body frame was too small. Therefore, I had to leave the back open. For the other parts of the robot, I used glue gun to get all the cardboards together as one piece.

![IMG_1600](https://user-images.githubusercontent.com/15336672/137061277-e2c102ef-e156-4856-bdec-be5e6fad5f5d.jpeg)

## Obstacles
1. Limited Body Frame Size: Even though I had calculated the size of the body frame to hold the circuit, when I had to fit in all the components within the body, I realized that the frame was too small to hold all the components. So, I was not able to close the back part of the robot - which made it look incomplete. For next project, I will make sure to calculate all dimensions properly "after" I have finished the circuit and connected all the components

2. Servo Motor Direction Difference: When I was coding the arm-swing motion of the robot, I noticed since each motor has to be placed facing outwards opposite to one another one of the motor has to start of with the maximum angle (180 degrees) and decrement its angle to have the same motion as the other motor incrementing from 0 degrees. I was not aware of this issue until I placed each servo within the body for a test run. For future projects, before proceeding onto the coding section, I should be more detailed on the ideation and planning to prevent these types of issues from happening.
