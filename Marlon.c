hen the plot was last watered
double waterTimes[6];
//Plant time for the certain plot
double PT[6];
//Holds the plant's grow time
double PlantTime[6];
//Contains the amount of times watered on a plot used to determine whether to plant, water, or harvest
int charge[6];
//The current value of crops in correct id order
double cropValue[6];
// variable to check which plot of the cycle in loop I am checking.
int count;
// How many times I have visited Astronaut
int astro;
// stores the crop id of the bonus crop, set to 0 if no bonus crop
int bonus;
float plotCoordinates[6][2];
int targetPlot; // Plot to perform actions on
int cycle;

void init(){
    cycle = 0;
bonus = 0;
    astro = 0;
    count = 0;
waterTimes[0] = 0.0;
    waterTimes[1] = 0.0;
    waterTimes[2] = 0.0;
    waterTimes[3] = 0.0;
    waterTimes[4] = 0.0;
    waterTimes[5] = 0.0;
    
    PT[0] = 0.0;
    PT[1] = 0.0;
    PT[2] = 0.0;
    PT[3] = 0.0;
    PT[4] = 0.0;
    PT[5] = 0.0;
    
    PlantTime[0] = 6.0;
    PlantTime[1] = 8.0;
    PlantTime[2] = 5.0;
    PlantTime[3] = 12.0;
    PlantTime[4] = 3.0;
    PlantTime[5] = 9.0;
    
    charge[0] = 0;
    charge[1] = 0;
    charge[2] = 0;
    charge[3] = 0;
    charge[4] = 0;
    charge[5] = 0;

    plotCoordinates[0][0] = -0.35;
    plotCoordinates[0][1] = 0.60;
    plotCoordinates[1][0] = 0.35;
    plotCoordinates[1][1] = 0.60;
    plotCoordinates[2][0] = -0.15;
    plotCoordinates[2][1] = 1.00;
    plotCoordinates[3][0] = -0.35;
    plotCoordinates[3][1] = 1.40;
    plotCoordinates[4][0] = 0.35;
    plotCoordinates[4][1] = 1.40;
    plotCoordinates[5][0] = 0.15;
    plotCoordinates[5][1] = 1.00;
    
cropValue[0] = 6.0;
cropValue[1] = 7.0;
cropValue[2] = 5.0;
cropValue[3] = 11.0;
cropValue[4] = 3.0;
cropValue[5] = 8.0; 

targetPlot = 0;
}
void wCrop(int plotNum, int cropNum) {
    game.MovePlot(plotNum);
    if (game.GetTime() > 240) return;
    game.PlantCrop(plotNum, cropNum);
    waterTimes[plotNum-1] = game.GetTime();
    charge[plotNum-1] = 1;
    PT[plotNum-1] = cropNum == 1 ? 6.0 : cropNum == 2 ? 8.0 : cropNum == 3 ? 5.0 : cropNum == 4 ? 12.0 : cropNum == 5 ? 3.0 : 9.0;
    game.WaterCrop(plotNum);
    cropValue[cropNum-1] = cropValue[cropNum-1] * 0.75;
    if (cropNum == bonus) {
        bonus = 0; 
    }
}
float selfAbs(float x) {
    if (x < 0) x *= -1;
    return x;
}
int bestCrop() {
    float bestValRatio = 0;
    int bestCrop = 0;
    for (int j = 0; j < 6; j++) {
        float valRatio = (cropValue[j]) / (PlantTime[j]);
        if (valRatio > bestValRatio) {
            bestValRatio = valRatio;
            bestCrop = j+1;
        }
    }
    return bestCrop;
}
float distanceVal(int plot) {
    return selfAbs(game.GetRobotPositionX()-plotCoordinates[plot-1][0]) + selfAbs(game.GetRobotPositionY()-plotCoordinates[plot-1][1]);
}
float timeVal(int plot, float pTime) {
    return pTime - waterTimes[plot-1] - PT[plot-1];
}
void forceWater() {
    while (game.GetWaterUnits() == 0) {
        game.MoveWatering(); 
        game.FillWateringCan();
        if (game.OpponentAtWatering()) {
            game.SetWait(2);
        }
    }
}
int realBestPlot() {
    float pTime            = game.GetTime();
    float bestPlotDistance = 50000;
    int   bestPlotByDist   = 0;
    float bestPlotTime     = 50000;
    int   bestPlotByTime   = 0;
    for (int i = 0; i < 6; i++) {
        float plotDistance = distanceVal(i+1);
        float plotTime     = timeVal(i+1, pTime);
        if (plotDistance < bestPlotDistance) {
            bestPlotByDist = i+1;
            bestPlotDistance = plotDistance;
        }
        if (plotTime < bestPlotTime) {
            bestPlotByTime = i+1;
            bestPlotTime = plotTime;
        }
    }
    if (bestPlotByTime == bestPlotByDist) return bestPlotByTime;
    else if (bestPlotTime * distanceVal(bestPlotByTime) > bestPlotDistance * timeVal(bestPlotByDist, pTime)) { 
        return bestPlotByDist;
    } return bestPlotByTime;
}
void loop(){
    if (game.GetTime() >= 240) game.EndGame();
    if (cycle == 0) {
        wCrop(1, bestCrop());
        wCrop(3, bestCrop());
        wCrop(4, bestCrop());
        wCrop(5, bestCrop());
        wCrop(6, bestCrop());
        wCrop(2, bestCrop());
        cycle = 1;
    } if (cycle == 1) {
        forceWater();
        targetPlot = realBestPlot();
        game.MovePlot(targetPlot);
        while (timeVal(targetPlot, game.GetTime()) > 0) {
            game.MoveTo(game.GetRobotPositionX() + 0.5, game.GetRobotPositionY() + 0.5, game.GetRobotPositionZ() + 0.5);
            game.MoveTo(game.GetRobotPositionX() - 0.5, game.GetRobotPositionY() - 0.5, game.GetRobotPositionZ() - 0.5);
        }
        if (charge[targetPlot-1] == 1) {
            game.WaterCrop(targetPlot);
            waterTimes[targetPlot-1] = game.GetTime();
            charge[targetPlot-1] = 2;
        } else if (charge[targetPlot-1] == 2) {
            game.HarvestCrop(targetPlot);
            wCrop(realBestPlot(), bestCrop());
        }
    }
}