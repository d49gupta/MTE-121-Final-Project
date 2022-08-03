const int ENCODER = 180/(PI*2.75);
const int LIGHT_BLUE = 15;
const int DARK_BLUE = 3;
const int RED = 37;
const int WHITE = 19;
const int PLAYER1 = 1;
const int PLAYER2 = 2;
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
	encoderDist(20,33);
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

void humanInput(int & index)
{
	int player;

	if (index % 2 == 0)
		player = PLAYER2;
	else
		player = PLAYER1;

	while(!getButtonPress(buttonAny))
	{}

	if(getButtonPress(buttonEnter))
	{
		x = 1;
		y = 1;
		while(getButtonPress(buttonEnter))
		{}
	}

	if(getButtonPress(buttonUp))
	{
		x = 0;
		y = 1;
		while(getButtonPress(buttonUp))
		{}
	}

	if(getButtonPress(buttonDown))
	{
		x = 2;
		y = 1;
		while(getButtonPress(buttonDown))
		{}
	}

	if(getButtonPress(buttonRight))
	{
		wait1Msec(2500);
		if(getButtonPress(buttonUp))
		{
			x = 0;
			y = 2;
			while(getButtonPress(buttonUp))
			{}
		}
		else if (getButtonPress(buttonDown))
		{
			x = 2;
			y = 2;
			while(getButtonPress(buttonDown))
			{}
		}
		else
		{
			x = 1;
			y = 2;
		}

		while(getButtonPress(buttonRight))
		{}
	}

	if(getButtonPress(buttonLeft))
	{
		wait1Msec(2500);
		if(getButtonPress(buttonUp))
		{
			x = 0;
			y = 0;
			while(getButtonPress(buttonUp))
			{}
		}
		else if (getButtonPress(buttonDown))
		{
			x = 2;
			y = 0;
			while(getButtonPress(buttonDown))
			{}
		}
		else
		{
			x = 1;
			y = 0;
		}
		while(getButtonPress(buttonLeft))
		{}
	}
	if (arr[x][y] != PLAYER1 && arr[x][y] != PLAYER2)
	{
		drive(x,y);
		arr[x][y] = player;
	}
	else
	{
		displayString(10,"TRY AGAIN");
		index--;
	}
}

bool winGame(int index)
{
	int diagonal = 0;
	bool horizontal = false;
	bool vertical = false;
	bool diagonal1 = false;
	bool gameWin = false;
	int player;

	if (index % 2 == 0)
		player = PLAYER2;
	else
		player = PLAYER1;

	for(int a = 0; a < 3; a++)
	{
		if(arr[a][a] == player)
			diagonal++;

		if (arr[0][a] == player && arr[1][a] == arr[0][a] && arr[2][a] == arr[1][a])
			vertical = true;

		if (arr[a][0] == player && arr[a][1] == arr[a][0] && arr[a][2] == arr[a][1])
			horizontal = true;

		if (arr[0][2] == player && arr[1][1] == arr[0][2] && arr[2][0] == arr[1][1])
			diagonal1 = true;
	}
	if (diagonal == 3 || horizontal == 1 || diagonal1 == 1 || vertical == 1)
		gameWin = true;

	return gameWin;
}


task main()
{
	configureSensors();

	displayString(2,"Start Game");

	for(int index = 1; index <= 9; index++)
	{
		index = 9;
		displayString(index + 2, "Move %d", index);
		humanInput(index);

		if(winGame(index))
		{
			if(index % 2 != 0)
				displayString(10,"PLAYER 1 WON!");
			else
				displayString(10,"PLAYER 2 WON!");

			while (!getButtonPress(buttonAny))
			{}
			while (getButtonPress(buttonAny))
			{}
			index = 10;
		}
		else if (index == 9)
		{
			displayString(10,"TIE GAME");
			while (!getButtonPress(buttonAny))
			{}
			while (getButtonPress(buttonAny))
			{}
		}
	}
}
