//Declare any variables shared between functions here
// double waterTimes[6] = {0, 0, 0, 0, 0, 0};
// double PT[6] = {0, 0, 0, 0, 0, 0};
// int charge[6] = {0, 0, 0, 0, 0, 0};

void init(){
	//This function is called once when your code is first loaded.
    double waterTimes[6] = {0,0,0,0,0,0};
// 	waterTimes[0],waterTimes[1],waterTimes[2],waterTimes[3],waterTimes[4],waterTimes[5] = 0,0,0,0,0,0;
//  Plot-Plant Water Time
	double PT[6] = {0,0,0,0,0,0};
// 	PT[0],PT[1],PT[2],PT[3],PT[4],PT[5] = 0,0,0,0,0,0;
//  Charges of water on a given plot
	double charge[6] = {0,0,0,0,0,0};
// 	charge[0], charge[1], charge[2], charge[3], charge[4], charge[5] = 0,0,0,0,0,0;
	//IMPORTANT: make sure to set any variables that need an initial value.
	//Do not assume variables will be set to 0 automatically!
	wCrop(1,4, waterTimes, PT);
	wCrop(3,1, waterTimes, PT);
	// Add automatic time checking code that determines which plot needs to get watered and compares it to the
	// current distance between the robot and the plots-- some sort of "is it worth it" function
	game.MoveAstronaut();
    int bonus = game.GetBonusCrop();
    game.LeaveAstronaut();
	wCrop(2,bonus, waterTimes, PT);
	WaterCrop(3, charge, waterTimes);
	game.MoveWatering();
    WaterCrop(1, charge, waterTimes);
	HarvestCrop(3, charge, waterTimes, PT);
	HarvestCrop(1, charge, waterTimes, PT);
// 	while (game.GetTime() < 10) {
// 	     for (int i = 0; i < 3; i++) {
//             if (game.GetTime()-waterTimes[i] > PT[i] && PT[i] != 0) {
//                 if (charge[i] == 2) HarvestCrop(i, charge, waterTimes, PT);
//                 else WaterCrop(i, charge, waterTimes);
//             }
//         }
// 	}
}

void wCrop(int plotNum, int cropNum, double wt[], double PT[]) {
    game.MovePlot(plotNum);
    game.PlantCrop(plotNum, cropNum);
    wt[plotNum-1] = game.GetTime();
    PT[plotNum-1] = cropNum == 1 ? 3.0 : cropNum == 2 ? 5.0 : cropNum == 3 ? 6.0 : 8.0;
    game.WaterCrop(plotNum);
    int water = game.GetWaterUnits();
    if (water == 0) game.MoveWatering();
}
void WaterCrop(int plotNum, double charge[], double wt[]) {
    game.MovePlot(plotNum);
    game.WaterCrop(plotNum);
    wt[plotNum-1] = game.GetTime();
    charge[plotNum-1]++;
    int water = game.GetWaterUnits();
    if (water == 0) game.MoveWatering();
}
void HarvestCrop(int plotNum, double charge[], double wt[], double PT[]) {
    game.MovePlot(plotNum);
    game.HarvestCrop(plotNum);
    charge[plotNum-1] = 0;
    wt[plotNum-1] = 0;
    PT[plotNum-1] = 0;
}

void loop() {
	//This function is called once per second.  Use it to control the satellite.
  // The below wCrop calls are eventually going to be automated based on the location of the robot
  game.GetWaterUnits();
}
