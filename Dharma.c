const int ENCODER = 180/(PI*2.75);
const int LIGHT_BLUE = 15;
const int DARK_BLUE = 3;
const int RED = 37;
const int WHITE = 19;
const int ROBOT = 1;
const int HUMAN = 2;
int arr[3][3];
int x, y;

void configureSensors()
{
	SensorType[S4] = sensorEV3_Gyro;
	wait1Msec(50);
	SensorType[S2] = sensorEV3_Touch;
	wait1Msec(50);
	SensorType[S3] = sensorEV3_Color;
	wait1Msec(50);

	SensorMode[S4] = modeEV3Gyro_Calibration;
	wait1Msec(50);
	SensorMode[S4] = modeEV3Gyro_RateAndAngle;
	wait1Msec(50);

	SensorMode[S3] = modeEV3Color_Reflected;
	wait1Msec(50);
}

void go(int motorPower)
{
	motor[motorA] = motor[motorD] = motorPower;
}

void rotate(int angle)
{
	motor[motorA] = 25;
	motor[motorD] = -25;

	resetGyro(S4);
	while(abs(getGyroDegrees(S4)) < abs(angle))
	{}
	go(0);
}

void activateClaw(int clawPower)
{
	time1[T1] = 0;
	motor[motorB] = clawPower;
	while(time1[T1] <= 1500)
	{}
	motor[motorB] = 0;
}

void encoderDist(int speed, int distance)
{
	nMotorEncoder[motorA] = 0;
	go(speed);
	while (abs(nMotorEncoder[motorA]) < distance*ENCODER)
	{}
	go(0);
}

void getChecker()
{
	activateClaw(30);
	go(30);
	while(SensorValue[S2]!= 1)
	{}
	go(0);

	activateClaw(-30);

	encoderDist(-20,20);
	rotate(85);
	go(30);
	while(abs(SensorValue[S3] - LIGHT_BLUE) >= 3)
	{}
	go(0);
}

void goHome()
{
	while(abs(SensorValue[S3] - WHITE) >= 5)
	{}
	rotate(260);
	go(30);
	activateClaw(-30);

	while(abs(SensorValue[S3] - RED) >= 1)
	{}
	go(0);
	rotate(80);
}

void drive(int x, int y)
{
	getChecker();
	go(30);
	for(int index = 1; index <= (2 - x); index++)
	{
		while(abs(SensorValue[S3] - DARK_BLUE) >= 1)
		{}
		while(abs(SensorValue[S3] - DARK_BLUE) <= 1)
		{}
	}
	encoderDist(20,35);
	rotate(260);
	go(-30);

	for(int index = 1; index <= (2 - y); index++)
	{
		while(abs(SensorValue[S3] - DARK_BLUE) >= 1)
		{}
		while(abs(SensorValue[S3] - DARK_BLUE) <= 1)
		{}
	}
	wait1Msec(850);
	activateClaw(30);
	goHome();
}

void humanInput()
{
	while(!getButtonPress(buttonAny))
	{}

	if(getButtonPress(buttonEnter))
	{
		x = 1;
		y = 1;
		while(getButtonPress(buttonEnter))
		{}
		arr[1][1] = HUMAN;
	}

	if(getButtonPress(buttonUp))
	{
		x = 0;
		y = 1;
		while(getButtonPress(buttonUp))
		{}
		arr[0][1] = HUMAN;
	}

	if(getButtonPress(buttonDown))
	{
		x = 2;
		y = 1;
		while(getButtonPress(buttonDown))
		{}
		arr[2][1] = HUMAN;
	}

	if(getButtonPress(buttonRight))
	{
		wait1Msec(1000);
		if(getButtonPress(buttonUp))
		{
			x = 0;
			y = 2;
			while(getButtonPress(buttonUp))
			{}
			arr[0][2] = HUMAN;
		}
		else if (getButtonPress(buttonDown))
		{
			x = 2;
			y = 2;
			while(getButtonPress(buttonDown))
			{}
			arr[2][2] = HUMAN;
		}
		else
		{
			x = 1;
			y = 2;
		}
		while(getButtonPress(buttonRight))
		{}
		arr[1][2] = HUMAN;
	}

	if(getButtonPress(buttonLeft))
	{
		wait1Msec(1000);
		if(getButtonPress(buttonUp))
		{
			x = 0;
			y = 0;
			while(getButtonPress(buttonUp))
			{}
			arr[0][0] = HUMAN;
		}
		else if (getButtonPress(buttonDown))
		{
			x = 2;
			y = 0;
			while(getButtonPress(buttonDown))
			{}
			arr[2][0] = HUMAN;
		}
		else
		{
			x = 1;
			y = 0;
		}
		while(getButtonPress(buttonLeft))
		{}
		arr[1][0] = HUMAN;
	}
}

bool winGame(int index)
{
	bool horizontal = false;
	bool vertical = false;
	bool diagonal = false;
	bool gameWin = false;
	int player;

	if (index % 2 == 0)
		player = ROBOT;
	else
		player = HUMAN;

	for (int a = 1; a <= 3; a++)
	{

		if (arr[0][a] == player && arr[1][a] == arr[0][a] && arr[2][a] == arr[1][a])
			vertical = true;

		if (arr[a][0] == player && arr[a][1] == arr[a][0] && arr[a][2] == arr[a][1])
			horizontal = true;

		if (arr[0][2] == player && arr[1][1] == arr[0][2] && arr[2][0] == arr[1][1])
			diagonal = true;

		if (arr[0][0] == player && arr[1][1] == arr[0][0] && arr[2][2] == arr[1][1])
			diagonal = true;
	}
	if (horizontal == 1 || diagonal == 1 || vertical == 1)
		gameWin = true;

	return gameWin;
}

void robotInput(int index)
{
	bool center = false;

	if (index == 1) //first human move
	{
		if (x == 1 && y == 1) //center game
		{
			arr[0][2] = ROBOT;
			drive(0,2);
			center = true;
		}

		else // not center game
		{
			if (x == 0 || y == 0)
			{
				arr[2][2] = ROBOT;
				drive(2,2);
			}
			else
			{
				arr[0][0] = ROBOT;
				drive(0,0);
			}
		}
	}
	if (index == 2) //second human move
	{
		if (center == 1)
		{
			if (x == 0 && y == 0)
			{
				arr[2][2] = ROBOT;
				drive(2,2);
			}
			else if (x == 2 && y == 2)
			{
				arr[0][0] = ROBOT;
				drive(0,0);
			}

			else if (x == 0 && y == 1)
			{
				arr[2][1] = ROBOT;
				drive(2,1);
			}
			else if (x == 1 && y == 0)
			{
				arr[1][2] = ROBOT;
				drive(1,2);
			}
			else if (x == 1 && y == 2)
			{
				arr[1][0] = ROBOT;
				drive(1,0);
			}
			else if (x == 2 && y == 1)
			{
				arr[0][1] = ROBOT;
				drive(0,1);
			}
		}
		if (center == 0)
		{
			if (arr[2][2] == ROBOT)
			{
				if (x == 2 && y == 1)
				{
					if(arr[0][0] == HUMAN)
					{
						arr[0][2] = ROBOT;
						drive(0,2);
					}
					if (arr[0][2] == HUMAN)
					{
						arr[0][0] = ROBOT;
						drive(0,0);
					}
				}
			}
			else
			{
				arr[2][1] = ROBOT; //WINNER
				drive(2,1);
			}
		}
		if (arr[0][0] == ROBOT)
		{
			if (x == 1 && y == 0)
			{
				if (arr[1][2] == HUMAN)
				{
					arr[1][1] = ROBOT;
					drive(1,1);
				}
				else if (arr[2][1] == HUMAN || arr[2][2] == HUMAN)
				{
					arr[0][2] = ROBOT;
					drive(0,2);
				}
			}
			else
			{
				arr[0][1] = ROBOT; //WINNER
				drive(0,1);
			}
		}
	}
	if (index == 3) //third human move
	{
		if(center == 1)
		{
			if(arr[0][0] == HUMAN) //WINNER
			{
				if(x == 1 && y == 2)
				{
					arr[2][1] = ROBOT;
					drive(2,1);
				}
				else
				{
					arr[1][2] = ROBOT;
					drive(1,2);
				}
			}
			if(arr[2][2] == HUMAN) //WINNER
			{
				if(x == 0 && y == 1)
				{
					arr[1][0] = ROBOT;
					drive(1,0);
				}
				else
				{
					arr[0][1] = ROBOT;
					drive(0,1);
				}
			}

			if(x == 2 && y == 2)
			{
				arr[0][0] = ROBOT;
				drive(0,0);
			}
			if (x == 0 && y == 0)
			{
				arr[2][2] = ROBOT;
				drive(2,2);
			}
			if (x == 0 && y == 1)
			{
				arr[2][1] = ROBOT;
				drive(2,1);
			}
			if (x == 2 && y == 1)
			{
				arr[0][1] = ROBOT;
				drive(0,1);
			}
		}
		if(center == 0) //WINNER
		{
			if(arr[1][1] == ROBOT)
			{
				if(x == 0 && y == 0)
				{
					arr[0][2] = ROBOT;
					drive(0,2);
				}
				else
				{
					arr[0][0] = ROBOT;
					drive(0,0);
				}
			}
			if (arr[0][2] == ROBOT)
			{
				if(x == 1 && y == 1)
				{
					arr[1][2] = ROBOT;
					drive(1,2);
				}
				else
				{
					arr[1][1] = ROBOT;
					drive(1,1);
				}
			}
			if (arr[0][0] == ROBOT)
			{
				if(x == 0 && y == 1)
				{
					arr[1][1] = ROBOT;
					drive(1,1);
				}
				else
				{
					arr[0][1] = ROBOT;
					drive(0,1);
				}
			}
		}
		if (index == 4) //fourth human move
		{
			for(int row = 0; row < 3; row++)
			{
				for (int col = 0; col < 3; col++)
				{
					if(arr[row][col] != ROBOT && arr[row][col] != HUMAN)
						arr[row][col] = ROBOT;
					drive(row,col);
				}
			}
		}
	}
}

task main()
{
	configureSensors();

	displayString(2,"Start Game");

	displayString(3,"Move 1");
	arr[2][0] = ROBOT;
	drive(2,0);

	for(int index = 1; index <= 4; index++)
	{
		displayString(index + 3, "Move %d", index + 1);
		humanInput();

	//if (arr[x][y] == ROBOT && arr[x][y] == HUMAN)
		robotInput(index);

	/*	if(winGame(index) == true)
		{
			displayString(10,"ROBOT WON!");
			index = 5;
		}
		else if (index == 4)
			displayString(10,"TIE GAME");
			*/

	}
}
