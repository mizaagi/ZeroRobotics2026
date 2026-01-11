//Declare any variables shared between functions here

void init() {
	//This function is called once when your code is first loaded.
	//IMPORTANT: make sure to set any variables that need an initial value.
	//Do not assume variables will be set to 0 automatically!
	double[] waterTimes = new double[6];
	waterTimes[0],waterTimes[1],waterTimes[2],waterTimes[3],waterTimes[4],waterTimes[5] = 0,0,0,0,0,0;
	
}
void wCrop(int plotNum, int cropNum) {
    game.MovePlot(plotNum);
    game.PlantCrop(plotNum, cropNum);
    waterTimes[plotNum-1] = game.GetTime();
    game.WaterCrop(plotNum);
}

void loop() {
	//This function is called once per second.  Use it to control the satellite.
  // The below wCrop calls are eventually going to be automated based on the location of the robot
  wCrop(1,4);
	wCrop(3,1);
	wCrop(4,3);
	// Add automatic time checking code that determines which plot needs to get watered and compares it to the
	// current distance between the robot and the plots-- some sort of "is it worth it" function
	game.MoveAstronaut();
  int bonus = game.GetBonusCrop();
  game.LeaveAstronaut();
	wCrop(2,bonus);
	game.MovePlot(3);
	game.WaterCrop(3);
	game.MoveWatering();
  game.MovePlot(1);
  game.WaterCrop(1);
	game.MovePlot(3);
	game.HarvestCrop(3);
	game.MovePlot(1);
	game.HarvestCrop(1);
}
